#ifndef _XAIFBOOSTERTYPECHANGE_SYMBOLALG_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_SYMBOLALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/SymbolAlgBase.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SignaturePattern.hpp"

using namespace xaifBooster;

namespace xaifBooster { 
  class Symbol;
  class SymbolReference;
  class ArgumentSymbolReference;
}

namespace xaifBoosterTypeChange {  

  /** 
   * class to track external subroutine references
   * and their active calling patterns 
   */
  class SymbolAlg : public SymbolAlgBase{

  public:
    
    SymbolAlg(const Symbol& theContainingSymbol);

    virtual ~SymbolAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    bool isExternal() const; 

    /**
     * set the symbol to be 
     * representing an external subroutine
     */
    void setExternal(); 

    bool hasHandCodedWrapper() const;

    /** 
     * indicate that this symbol has 
     * a handcoded wrapper
     */
    void setHandCodedWrapper(const SymbolReference& theOriginalSymbolReference);

    /** 
     * rename subroutines that have a 
     * definition within the representation 
     * as opposed to external calls
     */
    void internalRename(const SymbolReference& theOriginalSymbolReference); 

    /**    
     * if this is not a hand-adjoined call 
     * an exception is thrown. 
     */
    const SignaturePattern& getActivityPattern() const; 

    /**    
     * if this is not a hand-adjoined call 
     * an exception is thrown. 
     */
    SignaturePattern& getActivityPattern(); 

    const SymbolReference& getReplacementSymbolReference() const; 

    bool hasReplacementSymbolReference() const; 

    bool needsAllocation() const;

  private: 

    /** 
     * no def
     */
    SymbolAlg();

    /** 
     * no def
     */
    SymbolAlg(const SymbolAlg&);

    /** 
     * no def
     */
    SymbolAlg& operator=(const SymbolAlg&);

    /** 
     * true if this symbol refers to an 
     * external subroutine
     */
    bool myIsExternalFlag;

    /** 
     * true if this symbol's name 
     * appears on the list of hand coded 
     * wrappers 
     * which for now is supplied on the command line
     */
    bool myHasHandCodedWrapperFlag;

    /** 
     * activity pattern of formal arguments by position
     */
    SignaturePattern myActivityPattern;

    SymbolReference* myHandCodeWrapperSymbolReference_p; 

    void makeReplacementSymbol(const SymbolReference& theOriginalSymbolReference); 
     
  }; // end of class SymbolAlg
 
} 
                                                                     
#endif
