#ifndef _XAIFBOOSTERCODEREPLACEMENT_REPLACEMENT_INCLUDE_
#define _XAIFBOOSTERCODEREPLACEMENT_REPLACEMENT_INCLUDE_
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
#include "xaifBooster/system/inc/ControlFlowGraphBase.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/PrintVersion.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterCodeReplacement { 

  class Replacement : public XMLPrintable {
  public:

    Replacement (unsigned int aPlaceHolder);

    ~Replacement ();

    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for myPlaceHolder member as represented in XAIF schema
     */
    static const std::string our_myPlaceHolder_XAIFName;

    const unsigned int getPlaceHolder() const; 

    /** 
     * use this if we are referring to another ControlFlowGraphBase 
     * we don't own it
     */
    void setControlFlowGraphBase(const ControlFlowGraphBase& theOtherControlFlowGraphBase); 

    const ControlFlowGraphBase& getControlFlowGraphBase() const; 

    /** 
     * use this if we are referring to another ReversibleControlFlowGraph 
     * we don't own it
     */
    void setReversibleControlFlowGraph(const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& theOtherReversibleControlFlowGraph); 

    const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& getReversibleControlFlowGraph() const; 

    void setPrintVersion(PrintVersion::PrintVersion_E aPrintVersion);

    PrintVersion::PrintVersion_E getPrintVersion() const;

  private: 

    /** 
     * no def
     */ 
    Replacement();
    
    const unsigned int myPlaceHolder;

    /** 
     * reference to external ControlFlowGraph
     * we don't own this and it is exclusively 
     * either this reference or 
     * the reference in 
     * myReversibleControlFlowGraph_p
     */
    const ControlFlowGraphBase* myControlFlowGraphBase_p;

    /** 
     * reference to external ReversibleControlFlowGraph
     * we don't own this and it is exclusively 
     * either this reference or 
     * the reference in 
     * myControlFlowGraphBase_p 
     */
    const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph* 
    myReversibleControlFlowGraph_p;

    PrintVersion::PrintVersion_E myPrintVersion;

  }; // end of class Replacement
 
} // end of namespace xaifBooster
                                                                     
#endif
