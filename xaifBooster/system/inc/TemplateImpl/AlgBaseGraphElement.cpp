// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef  _ALGBASEGRAPHELEMENT_INCLUDE_
#include "xaifBooster/system/inc/AlgBaseGraphElement.hpp"
#endif

namespace xaifBooster {  

  template <class AlgContaining, class GraphType>
  AlgBaseGraphElement<AlgContaining,GraphType>::AlgBaseGraphElement(const AlgContaining& theContaining) : 
    myContaining(theContaining) { 
  }

  template <class AlgContaining, class GraphType>
  AlgBaseGraphElement<AlgContaining,GraphType>::~AlgBaseGraphElement(){
  }

  template <class AlgContaining, class GraphType>
  const AlgContaining& 
  AlgBaseGraphElement<AlgContaining,GraphType>::getContaining() const { 
    return myContaining;
  }

  template <class AlgContaining, class GraphType>
  void AlgBaseGraphElement<AlgContaining,GraphType>::printXMLHierarchy(std::ostream& os,
		  const GraphType& theGraph) const { 
    myContaining.printXMLHierarchyImpl(os, theGraph);
  }

  template <class AlgContaining, class GraphType>
  void AlgBaseGraphElement<AlgContaining,GraphType>::printXMLHierarchy(std::ostream& os) const { 
    myContaining.printXMLHierarchyImpl(os);
  }

  template <class AlgContaining, class GraphType>
  std::string 
  AlgBaseGraphElement<AlgContaining,GraphType>::debug() const { 
    return std::string("AlgBaseGraphElement");
  }

  template <class AlgContaining, class GraphType>
  void 
  AlgBaseGraphElement<AlgContaining,GraphType>::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

}
