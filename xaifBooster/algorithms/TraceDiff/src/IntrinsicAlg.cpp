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

#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/TraceDiff/inc/IntrinsicAlg.hpp"

namespace xaifBoosterTraceDiff { 

  IntrinsicAlg::IntrinsicAlg(Intrinsic& theContainingIntrinsic) : 
    ExpressionVertexAlgBase(theContainingIntrinsic),
    IntrinsicAlgBase(theContainingIntrinsic),
    myRenameFlag(false) {
  }

  IntrinsicAlg::~IntrinsicAlg() {}

  std::string IntrinsicAlg::debug () const { 
    std::ostringstream out;
    out << "xaifBoosterTraceDiff::IntrinsicAlg[" << this
 	<< "]" << std::ends;  
    return out.str();
  } // end of IntrinsicAlg::debug

  void
  IntrinsicAlg::printXMLHierarchy(std::ostream& os) const { 
    if (ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      IntrinsicAlgBase::getContaining().printXMLHierarchyImpl(os);
    else {
      const Intrinsic& theOriginal(dynamic_cast<const Intrinsic&>(getContaining()));
      PrintManager& pm=PrintManager::getInstance();
      os << pm.indent() 
	 << "<"
	 << Intrinsic::ourXAIFName.c_str() 
	 << " " 
	 << Intrinsic::our_myId_XAIFName.c_str() 
	 << "=\"" 
	 << theOriginal.getId().c_str()
	 << "\" ";
      if (theOriginal.annotationIsSet())
	os << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str()
	   << "=\""
	   << theOriginal.getAnnotation().c_str()
	   << "\" "; 
      os << Intrinsic::our_myName_XAIFName.c_str()
	 << "=\"";
      if (myRenameFlag)
	os << "oad_" ;
      os << theOriginal.getName().c_str()
	 << "\"/>" 
	 << std::endl; 
      pm.releaseInstance();
    }
  }

  void IntrinsicAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void IntrinsicAlg::algorithm_action_2() {
    DBG_MACRO(DbgGroup::CALLSTACK,
              "xaifBoosterTraceDiff::IntrinsicAlg::algorithm_action_2(tracing) called for: "
              << debug().c_str());
    if (dynamic_cast<const Intrinsic&>(getContaining()).isNonSmooth())
      myRenameFlag=true;
  }

}
