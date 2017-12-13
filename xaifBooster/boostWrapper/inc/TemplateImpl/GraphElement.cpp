// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef  _GRAPHELEMENT_INCLUDE_
#include "xaifBooster/boostWrapper/inc/GraphElement.hpp"
#endif

#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  template <class VertexOrEdgeDescriptor>
  GraphElement<VertexOrEdgeDescriptor>::GraphElement() : 
    myInitFlag(false),
    myVisitedFlag(false) {
  }
  
  template <class VertexOrEdgeDescriptor>
  void 
  GraphElement<VertexOrEdgeDescriptor>::init(const VertexOrEdgeDescriptor& aDescriptor) { 
    if (myInitFlag) 
      THROW_LOGICEXCEPTION_MACRO("GraphElement<VertexOrEdgeDescriptor>::init already initialized for instance at " << this);
    myInitFlag=true;
    myBoostDescriptor=aDescriptor;
  } // end of GraphElement<VertexOrEdgeDescriptor>::init
  
  template <class VertexOrEdgeDescriptor>
  const VertexOrEdgeDescriptor& 
  GraphElement<VertexOrEdgeDescriptor>::getDescriptor() const { 
    if (!myInitFlag) 
      THROW_LOGICEXCEPTION_MACRO("GraphElement<VertexOrEdgeDescriptor>::getDescriptor not initialized for instance at " << this);
    return myBoostDescriptor;
  } // end of GraphElement<VertexOrEdgeDescriptor>::getDescriptor
  
  template <class VertexOrEdgeDescriptor>
  std::string GraphElement<VertexOrEdgeDescriptor>::debug() const { 
    std::ostringstream out;
    out << "GraphElement[" << this 
	<< ",myInitFlag=" << myInitFlag 
	<< ",myBoostDescriptor=" << myBoostDescriptor
	<< "]" << std::ends;  
    return out.str();
  } // end of GraphElement<VertexOrEdgeDescriptor>::debug

  template <class VertexOrEdgeDescriptor>
  void   
  GraphElement<VertexOrEdgeDescriptor>::setVisited() const { 
    myVisitedFlag=true;
  }

  template <class VertexOrEdgeDescriptor>
  bool   
  GraphElement<VertexOrEdgeDescriptor>::wasVisited() const { 
    return myVisitedFlag;
  }

  template <class VertexOrEdgeDescriptor>
  void   
  GraphElement<VertexOrEdgeDescriptor>::resetVisited() const { 
    myVisitedFlag=false;
  }

} // end of namespace
