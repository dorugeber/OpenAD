#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_ARGUMENTSYMBOLREFERENCEALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONREVERSE_ARGUMENTSYMBOLREFERENCEALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ArgumentSymbolReferenceAlgBase.hpp"
#include "xaifBooster/system/inc/IntentType.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulationReverse {  

  /** 
   * class to change the intent 
   * for the ArgumentSymbolReferences
   */
  class ArgumentSymbolReferenceAlg : public ArgumentSymbolReferenceAlgBase {

  public:
    
    ArgumentSymbolReferenceAlg(ArgumentSymbolReference& theContaining);

    virtual ~ArgumentSymbolReferenceAlg() {};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    virtual void algorithm_action_1();

  private:

    /** 
     * no def
     */
    ArgumentSymbolReferenceAlg();

    /** 
     * no def
     */
    ArgumentSymbolReferenceAlg(const ArgumentSymbolReferenceAlg&);

    /** 
     * no def
     */
    ArgumentSymbolReferenceAlg& operator=(const ArgumentSymbolReferenceAlg&);

    /** 
     * the intent to be changed
     */
    IntentType::IntentType_E myIntent;

  };
 
} // end of namespace 
                                                                     
#endif
