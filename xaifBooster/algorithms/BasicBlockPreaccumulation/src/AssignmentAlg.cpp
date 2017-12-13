// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionVertexAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AssignmentAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdgeAlgFactory.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertexAlgFactory.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation {  

  AssignmentAlg::AssignmentAlg(Assignment& theContainingAssignment) : 
    xaifBoosterLinearization::AssignmentAlg(theContainingAssignment),
    myDerivAction_p(0) {
  }

  AssignmentAlg::~AssignmentAlg() {
    if (myDerivAction_p)
      delete (myDerivAction_p);
  }

  void AssignmentAlg::printXMLHierarchy(std::ostream& os) const {
    xaifBoosterLinearization::AssignmentAlg::printXMLHierarchy(os);
    if (myDerivAction_p) {
      myDerivAction_p->printXMLHierarchyImpl(os);
    }
  }

  std::string 
  AssignmentAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::AssignmentAlg[" << xaifBoosterLinearization::AssignmentAlg::debug().c_str()
        << ",myDerivAction_p=" << myDerivAction_p
        << "]" << std::ends;  
    return out.str();
  }

  void 
  AssignmentAlg::algorithm_action_1() { 
    xaifBoosterLinearization::AssignmentAlg::algorithm_action_1();
  }

  // local writer definition: 
  class VertexLabelWriter {
  public:
    VertexLabelWriter(const Expression& e) : myE(e) {};
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      xaifBoosterLinearization::ExpressionVertexAlg& va(dynamic_cast<xaifBoosterLinearization::ExpressionVertexAlg&>((*(boost::get(boost::get(BoostVertexContentType(),
																	      myE.getInternalBoostGraph()),
																   v))).getExpressionVertexAlgBase()));
      out << "[label=\"" << va.isActive() << "\"]";
    }
    const Expression& myE;
  };

  void 
  AssignmentAlg::algorithm_action_3() {
    if (!getActiveFlag() || !getContainingAssignment().isNonInlinable())
      return; 
    DBG_MACRO(DbgGroup::CALLSTACK,
	      "xaifBoosterBasicBlockPreaccumulation::AssignmentAlg::algorithm_action_3(handle non-inlinable) called for: "
	      << debug().c_str());
    const Expression& theOrigRHS(getContainingAssignment().getRHS());
    const ExpressionVertex& theOrigIntrinsic(theOrigRHS.getMaxVertex());
    const NonInlinableIntrinsicsCatalogueItem& theNonInlinableIntrinsicsCatalogueItem(theOrigIntrinsic.getNonInlinableIntrinsicsCatalogueItem());
    if (theNonInlinableIntrinsicsCatalogueItem.isExplicitJacobian()) {
      THROW_LOGICEXCEPTION_MACRO("AssignmentAlg::algorithm_action_3: not implemented for explicit Jacobian")
    }
    else if (theNonInlinableIntrinsicsCatalogueItem.isDirectAction()) {
      myDerivAction_p=new Assignment(false);
      myDerivAction_p->setId(getContainingAssignment().getId());
      getContainingAssignment().getLHS().copyMyselfInto(myDerivAction_p->getLHS());
      myDerivAction_p->getLHS().setDerivFlag();
      Intrinsic* newIntrinsic_p=new Intrinsic(theNonInlinableIntrinsicsCatalogueItem.getDirectAction().getDerivAction(),false);
      myDerivAction_p->getRHS().supplyAndAddVertexInstance(*newIntrinsic_p);
      newIntrinsic_p->setId(theOrigIntrinsic.getId());
      Expression::ConstInEdgeIteratorPair p(getContainingAssignment().getRHS().getInEdgesOf(getContainingAssignment().getRHS().getMaxVertex()));
      Expression::ConstInEdgeIterator ieIt(p.first), endIt(p.second);
      for (; ieIt!=endIt; ++ieIt) {
        ExpressionVertex& newInput(theOrigRHS.getSourceOf(*ieIt).createCopyOfMyself(false));
        if (newInput.isArgument()) {
          Argument& theArg(dynamic_cast<Argument&>(newInput));
          if (theArg.getVariable().getActiveType())
            theArg.getVariable().setDerivFlag();
        }
        myDerivAction_p->getRHS().supplyAndAddVertexInstance(newInput);
        ExpressionEdge* newEdge_p=new ExpressionEdge(false);
        myDerivAction_p->getRHS().supplyAndAddEdgeInstance(*newEdge_p,newInput,*newIntrinsic_p);
        newEdge_p->setPosition((*ieIt).getPosition());
        newEdge_p->setId((*ieIt).getId());
      }
    }
  }

  void AssignmentAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

} // end namespace xaifBoosterBasicBlockPreaccumulation
