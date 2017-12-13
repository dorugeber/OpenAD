#ifndef _SUBROUTINECALL_INCLUDE_
#define _SUBROUTINECALL_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/SymbolReference.hpp"
#include "xaifBooster/system/inc/ConcreteArgument.hpp"
#include "xaifBooster/system/inc/SubroutineCallAlgBase.hpp"
#include "xaifBooster/system/inc/ActiveUseType.hpp"
#include "xaifBooster/system/inc/ObjectWithLineNumber.hpp"

namespace xaifBooster { 

  /**
   * class SubroutineCall describes a SubroutineCall within a 
   * ControlFlowGraph
   */
  class SubroutineCall : public BasicBlockElement,
			 public ObjectWithLineNumber {
  public:

    /** 
     * \param theSymbol the name of the subroutine
     * \param theScope  the corresponding scope of theSymbol
     * \param activeUseType should be true if this call is active, i.e. has active arguments
     * \param formalArgCount the number of formal arguments
     * \param makeAlgorithm  news up an algorithm object if required which then  is also carried through for the respective members
     */
    SubroutineCall (const Symbol& theSymbol,
		    const Scope& theScope,
		    const ActiveUseType::ActiveUseType_E activeUseType,
		    unsigned short formalArgCount,
		    bool makeAlgorithm=true);

    ~SubroutineCall();

    /** 
     * algorithm access where the SubroutineCall may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    SubroutineCallAlgBase& getSubroutineCallAlgBase()const;

    void printXMLHierarchy(std::ostream& os) const;

    /**
     * actual implementation for printing xaif
     */
    void printXMLHierarchyImpl(std::ostream& os) const;

    std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for symbol id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_symbolId_XAIFName;

    /**
     * name for scope id used for SymbolReference member as represented in XAIF schema
     */
    static const std::string our_scopeId_XAIFName;

    /**
     * name for member myFormalArgCount as represented in XAIF schema
     */
    static const std::string our_myFormalArgCount_XAIFName;

    typedef std::list<ConcreteArgument*> ConcreteArgumentPList;

    ConcreteArgumentPList& getConcreteArgumentPList();

    const ConcreteArgumentPList& getConcreteArgumentPList() const;

    const ConcreteArgument& getConcreteArgument(unsigned int aPosition) const;

    const SymbolReference& getSymbolReference() const;

    /**
     * get the data type active flag from the SymbolReference
     */
    bool getActiveType() const;

    /** 
     * get the active use of this Subroutine
     * if UNDEFINED we go by the type specified in 
     * the SymbolTable for this subroutine
     */
    bool getActiveFlag() const;
    
    /** 
     * set myActiveUse once
     */
    void setActiveUse(ActiveUseType::ActiveUseType_E anActiveUse); 

    /** 
     * returns the active use as currently set including 
     * an UNDEFINEDUSE setting.
     * for determining activity prefer getActiveFlag
     */
    ActiveUseType::ActiveUseType_E getActiveUse() const; 

    unsigned short getFormalArgCount() const; 

    /** 
     * the implementation is incomplete
     * \todo refer to alias results
     */
    bool overwrites(const Variable& aVariable) const; 

    static bool getBlackBoxOptimism(); 

    /** 
     * set ourBlackBoxOptimism to false
     */
    static void noBlackBoxOptimism(); 

    /**
     * Checks each of the arguments for \p anExpression.
     * If it is not found, the respective algorithm (if any) is subsequently checked.
     */
    virtual bool hasExpression(const Expression& anExpression) const;

  private: 
    
    /**
     * no def
     */
    SubroutineCall();

    /**
     * the reference to the subroutine symbol 
     */
    const SymbolReference mySymbolReference;

    /**
     * indicating that this call makes active or passive
     * use of a subroutine
     */
    mutable ActiveUseType::ActiveUseType_E myActiveUse;

    mutable bool myActiveUseSetFlag;

    /** 
     * the list of concrete arguments for this call
     * these are owned by this instance and 
     * will be deleted in the dtor 
     * of SubroutineCall
     */
    ConcreteArgumentPList myConcreteArgumentPList;

    /** 
     * if true (default) we are optimistic 
     * in regard to black box routines, 
     * e.g. they don't have side effects
     */
    static bool ourBlackBoxOptimism; 

    /** 
     * the total count of formal arguments
     * for this subroutine, i.e. 
     * the upper bound for ConcreteArgument::myPosition 
     */
    unsigned short myFormalArgCount; 

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
