#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_ADJOINTASSEMBLY_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_ADJOINTASSEMBLY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BaseConstant.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"
#include "xaifBooster/system/inc/ForLoopReversalType.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/TemporariesHelper.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  /** 
   * helper class to implement adjoints
   */
  class AdjointAssembly  {
    
  public: 

    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall& addInlinableSubroutineCall(const std::string& aSubroutineName,
											       const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    SubroutineCall& addSubroutineCall(const Symbol& aSubroutineNameSymbol,
				      const Scope& aSubroutineNameScope,
				      ActiveUseType::ActiveUseType_E anActiveUse,
				      const ForLoopReversalType::ForLoopReversalType_E& aReversalType,
                                      unsigned short formalArgCount);

    Assignment& addAssignment(const ForLoopReversalType::ForLoopReversalType_E& aReversalType, bool withAlgorithm);

  protected:

    AdjointAssembly(){};

    virtual ~AdjointAssembly();

    const PlainBasicBlock::BasicBlockElementList& 
    getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType=ForLoopReversalType::ANONYMOUS) const; 
    
    PlainBasicBlock::BasicBlockElementList& 
    getBasicBlockElementList(const ForLoopReversalType::ForLoopReversalType_E& aReversalType=ForLoopReversalType::ANONYMOUS); 

    virtual Scope& getScope()=0;

    typedef std::pair<const Variable*, const Variable*> VariablePPair;
    typedef std::list<VariablePPair> VariablePPairList;

    /// This list maps propagation address variables with the corresponding variables that have been popped
    VariablePPairList myAddressVariableCorList;

    /// This list maps propagation factor variables with the corresponding variables that have been popped
    VariablePPairList myFactorVariableCorList;

    /** 
     * main part of logic turning forward into reverse propagation 
     */
    void reinterpretDerivativePropagatorEntry(const xaifBoosterDerivativePropagator::DerivativePropagatorEntry& aDerivativePropagatorEntry); 
  
    /** 
     * popping array index values if we pushed any
     * based on the reversal type (see also ControlFlowReversal)
     */
    void reinterpretArrayAccess(const Variable& theOriginalVariable,
				Variable& theActualVariable,
				const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add the call to the proper BasicBlockElementList based on aReversalType
     */
    void addZeroDeriv(const Variable& theTarget,
		      const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add the call to the proper BasicBlockElementList based on aReversalType
     */
    void addUnitFactor(const Variable& theSource,
		       const Variable& theTarget,
		       const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add the call to the proper BasicBlockElementList based on aReversalType
     */
    void addNegativeUnitFactor(Variable& theSource,
                               Variable& theTarget,
                               const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add the call to the proper BasicBlockElementList based on aReversalType
     */
    const Variable& addFactorPop(const Symbol& aTemporarySymbol,
				 const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add the call to the proper BasicBlockElementList based on \p aReversalType
     * Unlike addFactorPop, this method generates its own symbol for the variable to be popped into.
     */
    const Variable& addAddressPop(const ForLoopReversalType::ForLoopReversalType_E& aReversalType,
				  xaifBoosterTypeChange::TemporariesHelper& aTemporariesHelper);

    /** 
     * add the call to allocate the temporary symbol
     */
    void addAllocation(const Symbol& aTemporarySymbol,
		       const Scope& theScope,
		       xaifBoosterTypeChange::TemporariesHelper& aHelper,
		       const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add a Saxpy call with variable factor \p theFactor to the proper BasicBlockElementList based on aReversalType
     */
    void addSaxpy(const Variable& theSource,
		  const Variable& theTarget,
		  const Variable& theFactor,
		  const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

    /** 
     * add a Saxpy call with constant factor \p theConstantFactor to the proper BasicBlockElementList based on aReversalType
     */
    void addSaxpy(const Variable& theSource,
                  const Variable& theTarget,
                  const Constant& theFactor,
                  const ForLoopReversalType::ForLoopReversalType_E& aReversalType);

  private: 
    /// no def
    AdjointAssembly(const AdjointAssembly&);

    /// no def
    AdjointAssembly& operator=(const AdjointAssembly&);

    /** 
     * the elements that make up the adjoint code
     * which assumes an anonymous reversal (no knowledge about the original 
     * loop variables)
     */
    PlainBasicBlock::BasicBlockElementList myBasicBlockElementListAnonymousReversal;

    /** 
     * the elements that make up the adjoint code
     * under an explicit reversal (loop constructs are reversed explicitly
     * and we assume all index expressions can be recalculated explcitly 
     * at reversal time from explicitly reversed loops)
     */
    PlainBasicBlock::BasicBlockElementList myBasicBlockElementListExplicitReversal;


  };
 
} 
                                                                     
#endif
