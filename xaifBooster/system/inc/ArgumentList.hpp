#ifndef _ARGUMENTLIST_INCLUDE_
#define _ARGUMENTLIST_INCLUDE_
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

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"
#include "xaifBooster/system/inc/ArgumentSymbolReference.hpp"

namespace xaifBooster { 

  /**
   * a ControlFlowGraph describes 
   * the control flow for a subroutine
   * it is a member of a CallGraphVertex
   */
  class ArgumentList : public XMLPrintable,
		       public GenericTraverseInvoke {

  public:

    ArgumentList();

    ~ArgumentList();

    void printXMLHierarchy(std::ostream& os) const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    std::string debug() const ;
    
    /**
     * name for member myArgumentList as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for symbol id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_symbolId_XAIFName;

    /**
     * name for scope id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_scopeId_XAIFName;

    typedef std::list<ArgumentSymbolReference*> ArgumentSymbolReferencePList;

    ArgumentSymbolReferencePList& getArgumentSymbolReferencePList();

    const ArgumentSymbolReferencePList& getArgumentSymbolReferencePList() const;

    /**
     * \todo cleanup see const casts in the code
     * \see myScope_p
     */ 
    Scope& getScope() const;

    /* 
     * 
     */
    void copyMyselfInto(ArgumentList& theArgumentList,
			bool makeAlgorithm=false) const;

  private: 
    
    /**
     * a list of arguments 
     * these are owned by this instance and 
     * will be deleted in the dtor 
     */
    ArgumentSymbolReferencePList myArgumentSymbolReferencePList;

    /** 
     * this is pointer for setting the scope 
     * which we don't know at construction time
     * it is not explicitly given but taken 
     * from the first element in myArgumentSymbolReferencePList
     * in getScope or an exception is thrown.
     */
    mutable Scope* myScope_p;

  }; // end of class ArgumentList

} // end of namespace xaifBooster
                                                                     
#endif
