#ifndef _XAIFBOOSTERTYPECHANGE_SUBROUTINECALLALG_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_SUBROUTINECALLALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string> 
#include <list> 

#include "xaifBooster/system/inc/SubroutineCallAlgBase.hpp"
#include "xaifBooster/system/inc/ArgumentSymbolReference.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/ConcreteArgument.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/SaveValuesAcross/inc/SaveValuesAcross.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterTypeChange {  

  /** 
   * class to implement conversion to avoid 
   * type mismatches and potential renames 
   * if indicated by wrappernames or 
   * by the forced renames
   */
  class SubroutineCallAlg : public SubroutineCallAlgBase{

  public:
    
    SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall);

    virtual ~SubroutineCallAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    void printXMLHierarchyImpl(std::ostream& os) const;

    void printXMLHierarchyImplWithAdjustments(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /** 
     * see replaceArguments
     */
    virtual void algorithm_action_1();

    /** 
     * adjust for active/passive type mismatches 
     * \todo fix handling for external calls which should all be passive!
     * rename calls for all subroutines that are external and have 
     * wrappers or all internally defined subroutines if this is 
     * enforced, see CallGraphVertexAlg
     */
    void replaceArguments(bool withCopy); 

    static void addWrapperNames(const std::string& theSpaceSeparatedNames);

    /**
     * check mySaveValuesAcrossForTypeChange, myPriorAdjustmentsList, and myPostAdjustmentsList for \p anExpression
     */
    virtual bool hasExpression(const Expression& theExpression) const;

  protected:

    /** 
     * Class for saving values across the subroutine call, in order to facilitate correct type conversion after the call.
     * (Imagine, for example, that an array index is changed as a side-effect of the call.  We would need it's pre-call
     * value in order to do the type change BACK to the earlier type.)
     */
    xaifBoosterSaveValuesAcross::SaveValuesAcross mySaveValuesAcrossForTypeChange;

  private:
    
    /** 
     * no def
     */
    SubroutineCallAlg();

    /** 
     * no def
     */
    SubroutineCallAlg(const SubroutineCallAlg&);

    /** 
     * no def
     */
    SubroutineCallAlg& operator=(const SubroutineCallAlg&);

    /** 
     * prior call argument adjustments
     */
    PlainBasicBlock::BasicBlockElementList myPriorAdjustmentsList;

    /** 
     * post call argument adjustments
     */
    PlainBasicBlock::BasicBlockElementList myPostAdjustmentsList;

    /** 
     * makes a temporary variable for a given formal argument symbol
     */
    const Variable& makeTempVariable(const Symbol& formalArgument);

    /** 
     * the bit that replaces the argument 
     * and creates the inlinable calls
     * if withCopy is true
     */
    void addConversion(const ConcreteArgument& theConcreteArgument,
		       const ArgumentSymbolReference& aFormalArgumentSymbolReference,
		       const BasicBlock& theBasicBlock,
		       bool withCopy);

    /**
     * if forcePassive then we create a passive type, 
     * otherwise we create the type specified in the formalArgumentSymbol
     * \todo FIXME (AML): Most of this should be moved to SymbolTable, where it will replace the deprecated (and unused) method "SymbolTable::addUniqueAuxSymbolMatchingVariable", which was removed 3/11/09
     * returns true if the temporary needs to be allocated 
     *     */
    bool makeTempSymbol(const ConcreteArgument& theConcreteArgument,
			const Symbol& formalArgumentSymbol,
			const Scope& formalArgumentScope,
			Variable& aVariable,
			SymbolShape::SymbolShape_E tmpSymbolShape,
			bool forcePassive); 

    /** 
     * the bit that creates the inlinable calls for such 
     * external calls that don't have hand written adjoints
     * i.e. we don't have a formal parameter list for these calls
     */
    void addExternalConversion(const ConcreteArgument& theConcreteArgument,
			       const BasicBlock& theBasicBlock);

    /** 
     * the list of external subroutines that have hand
     * written adjoints 
     */
    static std::list<std::string> ourWrapperSubRoutineNameList;

    void initExternalCall(SymbolAlg& aSymbolAlg);

    void handleExternalCall(const BasicBlock& theBasicBlock);

    /** 
     * inserts assignments for index values
     */
    void handleArrayAccessIndices(const ConcreteArgument& theConcreteArgument,
				  const BasicBlock& theBasicBlock);

    /** 
     * add allocation calls for type change temps without fixed bounds
     */
    void addAllocation(const Variable& toBeAllocated,
		       const ConcreteArgument& argumentToMatch,
                       bool allocateSliceOfargumentToMatch);

    /** 
     * add shape test for type change temps without fixed bounds
     */
    void addShapeTest(const Variable& toBeAllocated,
		      const ConcreteArgument& argumentToMatch,
                      bool testSliceOfargumentToMatch);

    static  const std::string ourConversionRoutineName;

  }; // end of class SubroutineCallAlg
 
} 
                                                                     
#endif
