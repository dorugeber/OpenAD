#ifndef _VARIABLEVERTEX_INCLUDE_
#define _VARIABLEVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/system/inc/VertexTraversable.hpp"

namespace xaifBooster { 

  class VariableVertex : public VertexTraversable, 
			 public XMLPrintable,
			 public ObjectWithId {
  public:
    
    VariableVertex (){};
    
    ~VariableVertex(){};

    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const ;

    virtual std::string equivalenceSignature() const =0;

    virtual VariableVertex& createCopyOfMyself() const =0;

    virtual bool isSymbol() const;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
