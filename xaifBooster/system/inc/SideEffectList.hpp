#ifndef _SIDEEFFECTLIST_INCLUDE_
#define _SIDEEFFECTLIST_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>
#include "xaifBooster/system/inc/Variable.hpp"

namespace xaifBooster { 

  /**
   * class SideEffectList contains  a list of unique Variables
   * as determined by sideeffect analysis in the context 
   * of a subroutine 
   */
  class SideEffectList {
  public: 

    SideEffectList();

    ~SideEffectList();

    void printXMLHierarchy(const std::string& ListName,
			   std::ostream& os) const;

    std::string debug () const;

    /** 
     * the XAIF name of the list elements 
     */
    static const std::string our_SideEffectReference_XAIFName;

    typedef std::list<Variable*> VariablePList;

    const VariablePList& getVariablePList() const;

    Variable& addSideEffectReference();

    bool hasElement(const Variable& aVariable) const;

  private:

    VariablePList myVariablePList; 

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
