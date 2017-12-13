#ifndef _CONTROLFLOWGRAPHCOMMONATTRIBUTES_INCLUDE_
#define _CONTROLFLOWGRAPHCOMMONATTRIBUTES_INCLUDE_
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
#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"

namespace xaifBooster { 

  /**
   * a ControlFlowGraph describes 
   * the control flow for a subroutine
   * it is a member of a CallGraphVertex
   */
  class ControlFlowGraphCommonAttributes : public XMLPrintable,
					   public ObjectWithId,
					   public ObjectWithAnnotation {
  public:

    ControlFlowGraphCommonAttributes(const Symbol& theSymbol,
				     const Scope& theScope,
				     const Scope& theCFGScope);

    ~ControlFlowGraphCommonAttributes();

    /** 
     * pass in an alternative
     * so we don't have to replicate 
     * all the printing
     */
    void printAttributes(std::ostream& os,
			 const SymbolReference& anAlternativeSymbolReference) const;

    std::string debug() const ;
    
    /**
     * name for symbol id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for symbol id used for mySymbolReference member as represented in XAIF schema
     */
    static const std::string our_mySymbolReferenceSymbolId_XAIFName;

    /**
     * name for scope id used for mySymbolReference member as represented in XAIF schema
     */
    static const std::string our_mySymbolReferenceScopeId_XAIFName;

    /**
     * name for scope id used for myScope member as represented in XAIF schema
     */
    static const std::string our_myScope_XAIFName;

    /** 
     * this is the subroutine name
     */
    const SymbolReference& getSymbolReference() const;

    /** 
     * returns myScope
     */
    const Scope& getScope() const;

  protected: 
    
    /** 
     * no def
     */
    ControlFlowGraphCommonAttributes();

    /**
     * the reference to the subroutine symbol 
     */
    const SymbolReference mySymbolReference;

    /**
     * the reference to the scope for the 
     * subroutine body and the formal arguments
     */
    const Scope& myScope;

  }; // end of class ControlFlowGraphCommonAttributes

} // end of namespace xaifBooster
                                                                     
#endif
