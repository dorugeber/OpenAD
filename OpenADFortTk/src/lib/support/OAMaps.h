// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/lib/support/OAMaps.h,v 1.4 2005/08/15 20:17:25 utke Exp $

#ifndef OAMaps_INCLUDED_h
#define OAMaps_INCLUDED_h

#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"

//************************ OpenAnalysis Include Files ***********************

#include "OpenAnalysis/CallGraph/ManagerCallGraph.hpp"
#include "OpenAnalysis/CallGraph/CallGraph.hpp"
#include "OpenAnalysis/CallGraph/CallGraphInterface.hpp"
#include "OpenAnalysis/CFG/ManagerCFG.hpp"
#include "OpenAnalysis/CFG/EachCFGStandard.hpp"
#include "OpenAnalysis/Alias/ManagerAliasMapBasic.hpp"
#include "OpenAnalysis/Alias/ManagerInterAliasMapBasic.hpp"
#include "OpenAnalysis/Alias/ManagerFIAliasAliasMap.hpp"
#include "OpenAnalysis/ReachDefsOverwrite/ManagerReachDefsOverwriteStandard.hpp"
#include "OpenAnalysis/SideEffect/ManagerSideEffectStandard.hpp"
#include "OpenAnalysis/SideEffect/InterSideEffectStandard.hpp"
#include "OpenAnalysis/SideEffect/ManagerInterSideEffectStandard.hpp"
#include "OpenAnalysis/UDDUChains/ManagerUDDUChainsStandard.hpp"
#include "OpenAnalysis/CSFIActivity/ManagerDUActive.hpp"
#include "OpenAnalysis/XAIF/UDDUChainsXAIF.hpp"
#include "OpenAnalysis/XAIF/ManagerUDDUChainsXAIF.hpp"
#include "OpenAnalysis/XAIF/ReachDefsOverwriteXAIF.hpp"
#include "OpenAnalysis/XAIF/ManagerReachDefsOverwriteXAIF.hpp"
#include "OpenAnalysis/XAIF/ManagerAliasMapXAIF.hpp"
#include "OpenAnalysis/DataFlow/ManagerParamBindings.hpp"


//*************************** User Include Files ****************************

#include "Open64IRInterface/Open64IRInterface.hpp"

#include "BaseMap.h"
#include "Diagnostics.h"

namespace fortTkSupport {


  typedef std::list<OA::OA_ptr<OA::CFG::NodeInterface> > CFGNodeList;
  typedef std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > DGraphNodeList;
  
  const char*
    GetCFGVertexType(OA::OA_ptr<OA::CFG::CFGInterface> cfg, 
		     OA::OA_ptr<OA::CFG::NodeInterface> n);

  const char*
    GetCFGControlFlowVertexType(WN* wstmt);
  
  class IntraOAInfo;
  
  /** 
   * PU to OAInfo maps (global/interprocedural)
   */
  class InterOAInfoMap : public fortTkSupport::BaseMap<PU_Info*, IntraOAInfo*> {

  public:  
  
    InterOAInfoMap() { }
    virtual ~InterOAInfoMap() { }

    void init(PU_Info* pu_forest, OA::OA_ptr<Open64IRInterface> x,
	      bool activeWithVariedOnly=false);

    OA::OA_ptr<Open64IRInterface> getIRInterface() { return myIRIF; }
    void setIRInterface(OA::OA_ptr<Open64IRInterface> x) { myIRIF = x; }

    OA::OA_ptr<OA::CallGraph::CallGraph> getCallGraph() { return myCGraph; }
    void setCallGraph(OA::OA_ptr<OA::CallGraph::CallGraph> x) { myCGraph = x;}

    OA::OA_ptr<OA::CFG::EachCFGInterface> getCFGEach() { return myCFGEach; }
    void setCFGEach(OA::OA_ptr<OA::CFG::EachCFGInterface> x) { myCFGEach = x; }

    OA::OA_ptr<OA::DataFlow::ParamBindings> getParamBindings() {return myParamBindings;}
    void setParamBind(OA::OA_ptr<OA::DataFlow::ParamBindings> x) {myParamBindings = x;}
  
    OA::OA_ptr<OA::Alias::InterAliasMap> getInterAlias() { return myInterAliasMap; }
    void setInterAlias(OA::OA_ptr<OA::Alias::InterAliasMap> x) { myInterAliasMap = x; }

    OA::OA_ptr<OA::SideEffect::InterSideEffectStandard> getInterSideEffect() { return myInterSideEffect; }
    void setInterSideEffect(OA::OA_ptr<OA::SideEffect::InterSideEffectStandard> x) {myInterSideEffect = x; }

    OA::OA_ptr<OA::Activity::InterActiveFortran> getInterActiveFortran() { return myInterActiveFortran; }
    void setInterActiveFortran(OA::OA_ptr<OA::Activity::InterActiveFortran> x) {myInterActiveFortran = x; }

  private:
    OA::OA_ptr<Open64IRInterface> myIRIF;
    OA::OA_ptr<OA::CallGraph::CallGraph> myCGraph;
    OA::OA_ptr<OA::CFG::EachCFGInterface> myCFGEach;
    OA::OA_ptr<OA::Alias::InterAliasMap> myInterAliasMap;
    OA::OA_ptr<OA::DataFlow::ParamBindings> myParamBindings;
    OA::OA_ptr<OA::SideEffect::InterSideEffectStandard> myInterSideEffect;
    OA::OA_ptr<OA::Activity::InterActiveFortran> myInterActiveFortran;  
  };

  /** 
   * information per PU
   */
  class IntraOAInfo {
  public:

    IntraOAInfo() {}

    IntraOAInfo(PU_Info* pu, InterOAInfoMap* interInfo);

    ~IntraOAInfo() {}
  
    OA::OA_ptr<OA::Alias::Interface> getAlias() { return myIntraAlias; }
    void setAlias(OA::OA_ptr<OA::Alias::Interface> x) { myIntraAlias = x; }

    OA::OA_ptr<OA::ReachDefsOverwrite::ReachDefsOverwriteStandard> getReachDefsOverwrite() { return myReachDefsOverwrite; }
    void setReachDefsOverwrite(OA::OA_ptr<OA::ReachDefsOverwrite::ReachDefsOverwriteStandard> x) { myReachDefsOverwrite = x; }
  
    OA::OA_ptr<OA::UDDUChains::UDDUChainsStandard> getUDDUChains() { return myUDDUChains; }
    void setUDDUChains(OA::OA_ptr<OA::UDDUChains::UDDUChainsStandard> x) { myUDDUChains = x; }
      
    OA::OA_ptr<OA::XAIF::AliasMapXAIF> getAliasXAIF() { return myAliasXaif; }
    void setAliasXAIF(OA::OA_ptr<OA::XAIF::AliasMapXAIF> x) { myAliasXaif = x; }
  
    OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> getUDDUChainsXAIF() { return myUDDUChainsXAIF; }
    void setUDDUChainsXAIF(OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> x) {myUDDUChainsXAIF = x; }
  
    OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> getReachDefsOverwriteXAIF() { return myReachDefsOverwriteXAIF; }
    void setReachDefsOverwriteXAIF(OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> x) {myReachDefsOverwriteXAIF = x; }
  
    static bool isGlobalSymbolActive(ST* anST_p);

    //   // this is only for context sensitive analysis
    //   static void collectGlobalSymbolActivityInfo(OA::OA_ptr<OA::Activity::InterActive> active,
    // 					      OA::OA_ptr<OA::Alias::InterAliasMap> interAlias,
    // 					      OA::OA_ptr<Open64IRInterface> irIF,
    // 					      PU_Info* pu_forest); 

    static void setDoNotFilterFlag();

    static bool getDoNotFilterFlag();

  private:

    OA::OA_ptr<OA::Alias::Interface> myIntraAlias;
    OA::OA_ptr<OA::ReachDefsOverwrite::ReachDefsOverwriteStandard> myReachDefsOverwrite;
    OA::OA_ptr<OA::UDDUChains::UDDUChainsStandard> myUDDUChains;
    OA::OA_ptr<OA::XAIF::AliasMapXAIF> myAliasXaif;
    OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> myUDDUChainsXAIF;
    OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> myReachDefsOverwriteXAIF;
    /** 
     * this is for context sensitive analysis, not in use right now 
     */
    static std::set<ST*> ourActiveGlobalSTPSet;

    /** 
     * a flag to pass on to the OA analysis 
     * whether or not to filter information 
     * per basic block
     */
    static bool ourDoNotFilterFlag;

  };
  
}

#endif 
