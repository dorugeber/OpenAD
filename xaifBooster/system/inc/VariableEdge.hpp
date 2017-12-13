#ifndef _VARIABLEEDGE_INCLUDE_
#define _VARIABLEEDGE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/EdgeTraversable.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include <sstream>

namespace xaifBooster { 

  class Variable;

  class VariableEdge : public EdgeTraversable, 
		       public ObjectWithId {
  public:
    VariableEdge (){};
    ~VariableEdge(){};

    static const std::string ourXAIFName;
    static const std::string our_myId_XAIFName;
    static const std::string our_source_XAIFName;
    static const std::string our_target_XAIFName;

    /**
     * \todo JU implement this
     */
    void printXMLHierarchy(std::ostream& os,
			   const Variable& theVariable) const;

    std::string debug() const;

  };
 
} // end of namespace xaifBooster

#endif
