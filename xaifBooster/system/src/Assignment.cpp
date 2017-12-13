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
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/AssignmentAlgFactory.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

namespace xaifBooster { 

  const std::string Assignment::our_myLHS_XAIFName("xaif:AssignmentLHS");
  const std::string Assignment::our_myRHS_XAIFName("xaif:AssignmentRHS");
  const std::string Assignment::ourXAIFName("xaif:Assignment");
  const std::string Assignment::our_myId_XAIFName("statement_id");
  const std::string Assignment::our_myDoMapKey_XAIFName("do_chain");

  Assignment::Assignment(bool makeAlgorithm) : 
    myRHS(makeAlgorithm),
    myNonInlinableFlag(false),
    myNonInlinableCheckedFlag(false) {
    if (makeAlgorithm)
      myBasicBlockElementAlgBase_p=AssignmentAlgFactory::instance()->makeNewAlg(*this); 
  } 

  Assignment::~Assignment() {
  } 

  AssignmentAlgBase& 
  Assignment::getAssignmentAlgBase() const { 
    if (!myBasicBlockElementAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Assignment::getAssignmentAlgBase: not set");
    return dynamic_cast<AssignmentAlgBase&>(*myBasicBlockElementAlgBase_p);
  }

  void
  Assignment::printXMLHierarchy(std::ostream& os) const { 
    if (myBasicBlockElementAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getAssignmentAlgBase().printXMLHierarchy(os);
    else 
      printXMLHierarchyImpl(os);
  }

  void
  Assignment::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str()
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\""
       << ">" 
       << std::endl;
    printXMLHierarchyLHS(os);
    printXMLHierarchyRHS(os);
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  }

  void 
  Assignment::printXMLHierarchyLHS(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << our_myLHS_XAIFName.c_str()
       << " ";
    myLHS.printVariableReferenceXMLAttributes(os);
    os << ">" 
       << std::endl;
    myLHS.printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << our_myLHS_XAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of Assignment::printXMLHierarchyLHS

  void 
  Assignment::printXMLHierarchyRHS(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << our_myRHS_XAIFName.c_str()
       << ">"
       << std::endl;
    myRHS.printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << our_myRHS_XAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of Assignment::printXMLHierarchyRHS

  Variable& 
  Assignment::getLHS() { 
    return myLHS;
  }

  Expression& 
  Assignment::getRHS() {
    return myRHS;
  }

  const Variable& 
  Assignment::getLHS() const { 
    return myLHS;
  }

  const Expression& 
  Assignment::getRHS() const { 
    return myRHS;
  }

  std::string Assignment::debug () const { 
    std::ostringstream out;
    out << "Assignment[" << BasicBlockElement::debug().c_str()
        << ",myLHS=" << myLHS.debug().c_str()
        << ",myRHS=" << myRHS.debug().c_str()
        << ",myNonInlinableFlag=" << myNonInlinableFlag
        << ",myNonInlinableCheckedFlag=" << myNonInlinableCheckedFlag
	<< "]" << std::ends;  
    return out.str();
  } // end of Assignment::debug

  void Assignment::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
    getAssignmentAlgBase().genericTraversal(anAction_c);
    myRHS.genericTraversal(anAction_c);
    myLHS.genericTraversal(anAction_c);
  } 

  StatementIdSetMapKey& Assignment::getDoMapKey() { 
    return myDoMapKey;
  } 

  const StatementIdSetMapKey& Assignment::getDoMapKey() const { 
    return myDoMapKey;
  }

  bool
  Assignment::hasExpression(const Expression& anExpression) const {
    // first check whether the assignment itself contains the expression
    if (getLHS().hasExpression(anExpression)
     || getRHS().hasExpression(anExpression))
      return true;
    // check the AssignmentAlg
    if (myBasicBlockElementAlgBase_p)
      return getAssignmentAlgBase().hasExpression(anExpression);
    else
      return false;
  } // end Assignment::hasExpression

  bool
  Assignment::isNonInlinable() const {
    if (myNonInlinableCheckedFlag)
      return myNonInlinableFlag;
    const ExpressionVertex& maxVertex(getRHS().getMaxVertex());
    if (maxVertex.isIntrinsic()) {
      if (!((dynamic_cast<const Intrinsic&>(maxVertex)).isInlinable())) {
        // check that it is the only one that is not an argument
        Expression::ConstInEdgeIteratorPair p(getRHS().getInEdgesOf(maxVertex));
        Expression::ConstInEdgeIterator ieIt(p.first), endIt(p.second);
        for (;ieIt!=endIt;++ieIt) {
           if (!(getRHS().getSourceOf(*ieIt).isArgument())) {
             THROW_LOGICEXCEPTION_MACRO("Assignment::isNonInlinable(): canonicalization should have extracted the non-inlinable intrinsic >"
                                        << (dynamic_cast<const Intrinsic&>(maxVertex)).getName()
                                        << "< on line number "<< getLineNumber());
           }

        }
        myNonInlinableCheckedFlag=true;
        return myNonInlinableFlag=true;
      }
      else { // none of the intrinsics should be non-inlinable
         Expression::ConstVertexIteratorPair p(getRHS().vertices());
         Expression::ConstVertexIterator vIt(p.first), endIt(p.second);
         for (;vIt!=endIt;++vIt) {
           if ((*vIt).isIntrinsic() && !(dynamic_cast<const Intrinsic&>(*vIt)).isInlinable()) {
             THROW_LOGICEXCEPTION_MACRO("Assignment::isNonInlinable(): canonicalization should have extracted the non-inlinable intrinsic >"
                                        << (dynamic_cast<const Intrinsic&>(*vIt)).getName()
                                        << "< on line number "<< getLineNumber());
           }
         }
      }
    }
    myNonInlinableCheckedFlag=true;
    return myNonInlinableFlag=false;
  }

} // end of namespace xaifBooster 
