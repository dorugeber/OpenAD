// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################
#include <ctime>
#include <fstream>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "whirl2xaif.h"
#include "wn2xaif.h"
#include "st2xaif.h"
#include "Args.h"

namespace whirl2xaif { 

  fortTkSupport::IntrinsicXlationTable   
  Whirl2Xaif::ourIntrinsicTable(fortTkSupport::IntrinsicXlationTable::W2X);

  fortTkSupport::InterOAInfoMap       Whirl2Xaif::ourOAAnalMap;
  fortTkSupport::ScalarizedRefTabMap_W2X Whirl2Xaif::ourScalarizedRefTableMap;
  fortTkSupport::WNToWNIdTabMap          Whirl2Xaif::ourWNToWNIdTableMap;
  const std::string       Whirl2Xaif::ourDividerComment("********************************************************************");

  fortTkSupport::IntrinsicXlationTable& Whirl2Xaif::getIntrinsicXlationTable() { 
    return ourIntrinsicTable;
  }

  fortTkSupport::InterOAInfoMap& Whirl2Xaif::getOAAnalMap() { 
    return ourOAAnalMap;
  }

  fortTkSupport::ScalarizedRefTabMap_W2X& Whirl2Xaif::getScalarizedRefTableMap() { 
    return ourScalarizedRefTableMap;
  }

  fortTkSupport::WNToWNIdTabMap& Whirl2Xaif::getWNToWNIdTableMap() { 
    return ourWNToWNIdTableMap;
  }

  void Whirl2Xaif::translateIR(std::ostream& os, 
			       PU_Info* pu_forest,
			       const char* tmpVarPrefix) {
    using namespace OA::CallGraph;
    Diag_Set_Phase("WHIRL to XAIF: translate IR");
    if (!pu_forest) { return; }
    if (Args::ourDoNotFilterFlag) { 
      fortTkSupport::IntraOAInfo::setDoNotFilterFlag();
    }
    // -------------------------------------------------------
    // 1. Initialization (Much of this information must be collected
    // here because it is part of the CallGraph instead of a
    // ControlFlowGraph)
    // -------------------------------------------------------
    OA::OA_ptr<Open64IRInterface> irInterface; irInterface=new Open64IRInterface; 
    Open64IRInterface::initContextState(pu_forest);
    Open64IRInterface::setIgnoreBlackBoxRoutines();
    xml::ostream xos(os.rdbuf());
    PUXlationContext ctxt("whirl2xaif::translateIR",*irInterface);
    if (!Args::ourNoTimeStampFlag) 
	dumpTranslationHeaderComment(xos);
    // Initialize global id maps
    // NOTE: Do this first so that ids will match in back-translation
    fortTkSupport::SymTabToSymTabIdMap stabmap(pu_forest);
    ctxt.setSymTabToIdMap(&stabmap);
    fortTkSupport::PUToPUIdMap pumap(pu_forest);
    ctxt.setPUToIdMap(&pumap);
    ourWNToWNIdTableMap.Create(pu_forest); // Note: could make this local
    // Initialize and create inter/intra analysis information
    ourOAAnalMap.init(pu_forest,irInterface,Args::ourVariedOnlyFlag);
    ctxt.setActivity(ourOAAnalMap.getInterActiveFortran());
    ctxt.setAlias(ourOAAnalMap.getInterAlias());
    // Create scalarized var reference table
    ourScalarizedRefTableMap.Create(pu_forest);
    // -------------------------------------------------------
    // 2. Generate XAIF CallGraph
    // -------------------------------------------------------
    OA::OA_ptr<OA::CallGraph::CallGraph> cgraph = 
      ourOAAnalMap.getCallGraph();
    // CallGraph header info
    xos << xml::BegElem("xaif:CallGraph")
	<< xml::Attr("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance")
	<< xml::Attr("xmlns:xaif", "http://www.mcs.anl.gov/XAIF")
	<< xml::Attr("xsi:schemaLocation", "http://www.mcs.anl.gov/XAIF xaif.xsd")
	<< xml::Attr("program_name", "***myprog***")
	<< xml::Attr("prefix", tmpVarPrefix);
    // ScopeHierarchy
    ctxt.createXlationContext();
    translateScopeHierarchy(xos, pu_forest, ctxt);
    ctxt.deleteXlationContext();
    // Analysis Info Maps
    translateAnalMaps(xos, pu_forest, ctxt);
    // CallGraph vertices
    DGraphNodeVec* nodes = SortDGraphNodes(cgraph);
    for (DGraphNodeVec::iterator nodeIt = nodes->begin();
	 nodeIt != nodes->end(); ++nodeIt) {
      ctxt.createXlationContext();
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = *nodeIt;
      OA::OA_ptr<Node> n = 
	ntmp.convert<Node>();
      translatePU(xos, n, n->getId(), ctxt);
      ctxt.deleteXlationContext();
    }
    delete nodes;
    // CallGraph edges
    DGraphEdgeVec* edges = SortDGraphEdges(cgraph);
    for (DGraphEdgeVec::iterator edgeIt = edges->begin(); 
	 edgeIt != edges->end(); ++edgeIt) {
      OA::OA_ptr<OA::DGraph::EdgeInterface> e = (*edgeIt);
      OA::OA_ptr<OA::DGraph::NodeInterface> n1 = e->getSource();
      OA::OA_ptr<OA::DGraph::NodeInterface> n2 = e->getSink();
      DumpCallGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), n1->getId(), n2->getId());
    }
    delete edges;
    // Done!
    xos << xml::EndElem; /* xaif:CallGraph */
  }

  void Whirl2Xaif::translateScopeHierarchy(xml::ostream& xos, 
					   PU_Info* pu_forest, 
					   PUXlationContext& ctxt) {
    // We implicitly create the ScopeHierarchy/ScopeGraph using
    // DFS-style iteration of PUs.  In addition to the global scope,
    // there is one scope for each PU.
    xos << xml::BegElem("xaif:ScopeHierarchy");
    // translate global symbol table
   fortTkSupport::SymTabId scopeId = ctxt.findSymTabId(Scope_tab[GLOBAL_SYMTAB].st_tab);
    xos << xml::BegElem("xaif:Scope") << xml::Attr("vertex_id", scopeId)
	<< SymTabIdAnnot(scopeId) << xml::EndAttrs;
    xlate_SymbolTables(xos, GLOBAL_SYMTAB, NULL, ctxt);
    xos << xml::EndElem << std::endl;
    // translate each PU, descending into children first
    for (PU_Info *pu = pu_forest; pu != NULL; pu = PU_Info_next(pu)) {
      translateScopeHierarchyPU(xos, pu, scopeId, ctxt);
    }
    xos << xml::EndElem; /* xaif:ScopeHierarchy */
    xos << std::endl;
  }

  void Whirl2Xaif::translateScopeHierarchyPU(xml::ostream& xos, 
					     PU_Info* pu, 
					     UINT32 parentId, 
					     PUXlationContext& ctxt) {
    PU_SetGlobalState(pu);
    // Need WHIRL<->ID maps for translating ScalarizedRefs
    fortTkSupport::WNToWNIdMap* wnmap = ourWNToWNIdTableMap.Find(pu);
    ctxt.setWNToIdMap(wnmap);
    fortTkSupport::ScalarizedRefTab_W2X* tab = ourScalarizedRefTableMap.Find(pu);
   fortTkSupport::SymTabId scopeId = ctxt.findSymTabId(Scope_tab[CURRENT_SYMTAB].st_tab);  
    // translate symbol tables 
    xos << xml::BegElem("xaif:Scope") << xml::Attr("vertex_id", scopeId) 
	<< SymTabIdAnnot(scopeId) << xml::EndAttrs;
    xlate_SymbolTables(xos, CURRENT_SYMTAB, tab, ctxt);
    xos << xml::EndElem << std::endl;
    // Generate an edge to parent
    DumpScopeGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), scopeId, parentId);
    xos << std::endl;
    // Recursively translate all children
    for (PU_Info *child = PU_Info_child(pu); child != NULL;
	 child = PU_Info_next(child)) {
      translateScopeHierarchyPU(xos, child, scopeId, ctxt);
    }
  }

  void Whirl2Xaif::translateAnalMaps(xml::ostream& xos, 
				     PU_Info* pu_forest, 
				     PUXlationContext& ctxt) {
    // -------------------------------------------------------
    // AliasSetList: The first element has to be there
    // -------------------------------------------------------
    xos << xml::BegElem("xaif:AliasSetMap");

    // print info for set 0 (unknown) and set 1 (empty loc set)
    xos << xml::BegElem("xaif:AliasSet") << xml::Attr("key", 0) << xml::EndElem;
    xos << xml::BegElem("xaif:AliasSet") << xml::Attr("key", 1) << xml::EndElem;

    Open64IRProcIterator procIt(pu_forest);
    // iterate over processed units
    for (int procCnt = 1; procIt.isValid(); ++procIt, ++procCnt) {
      PU_Info* pu = (PU_Info*)procIt.current().hval();
      fortTkSupport::IntraOAInfo* oaAnal = ourOAAnalMap.Find(pu);
      fortTkSupport::WNToWNIdMap* wnmap = ourWNToWNIdTableMap.Find(pu);
      OA::OA_ptr<OA::XAIF::AliasMapXAIF> aliasSets = oaAnal->getAliasXAIF();
      // iterate over alias sets
      for (OA::OA_ptr<OA::XAIF::IdIterator> aliasSetIdsIter = aliasSets->getIdIterator(); aliasSetIdsIter->isValid(); ++(*aliasSetIdsIter)) {
	int setId = aliasSetIdsIter->current();
	//if ((setId == 0 || setId == 1) && procCnt != 1) continue;
	if (setId == 0 || setId == 1) continue;
	xos << xml::BegElem("xaif:AliasSet") << xml::Attr("key", setId);
	// iterate over alias ranges
	for (OA::OA_ptr<OA::XAIF::LocTupleIterator> aLocTupleIter = aliasSets->getLocIterator(setId); aLocTupleIter->isValid(); ++(*aLocTupleIter) ) {
	  xos << xml::BegElem("xaif:AliasRange");
	  xos << xml::Attr("from_virtual_address", aLocTupleIter->current().getLocRange().getStart());
	  xos << xml::Attr("to_virtual_address", aLocTupleIter->current().getLocRange().getEnd());
	  xos << xml::Attr("partial", !(aLocTupleIter->current().isFull()));
	  xos << xml::EndElem;
	}
	xos << xml::EndElem; // xaif:AliasSet
      }
    }
    xos << xml::EndElem; 
    xos << std::endl;
    // -------------------------------------------------------
    // DUUDSetMap: The first two elements are the *same* for each procedure.
    // -------------------------------------------------------
    xos << xml::BegElem("xaif:DUUDSetMap");
    procIt.reset();
    for (int procCnt = 1; procIt.isValid(); ++procIt, ++procCnt) {
      PU_Info* pu = (PU_Info*)procIt.current().hval();
      fortTkSupport::IntraOAInfo* oaAnal = ourOAAnalMap.Find(pu);
      fortTkSupport::WNToWNIdMap* wnmap = ourWNToWNIdTableMap.Find(pu);
      OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> udduchains = oaAnal->getUDDUChainsXAIF();
      for (OA::OA_ptr<OA::XAIF::UDDUChainsXAIF::ChainsIterator> chainsIter = udduchains->getChainsIterator(); chainsIter->isValid(); ++(*chainsIter)) {
	OA::OA_ptr<OA::XAIF::ChainsXAIF::ChainIterator> chainIter = chainsIter->currentChainIterator();
	int chainid = chainsIter->currentId(); // 0-2 are same for each proc
	if ((0 <= chainid && chainid <= 2) && procCnt != 1) { continue; }
	xos << xml::BegElem("xaif:StmtIdSet") << xml::Attr("key", chainid);
	for ( ; chainIter->isValid(); (*chainIter)++ ) {
	  OA::StmtHandle stmt = chainIter->current();
	  WN* stmtWN = (WN*)(stmt.hval());
	  fortTkSupport::WNId stmtid = wnmap->Find(stmtWN);
	  xos << xml::BegElem("xaif:StatementId");
	  if (stmtWN == NULL) {
	    xos << xml::Attr("idRef", "");
	  }
	  else {
	    xos << xml::Attr("idRef", stmtid);
	  }
	  xos << xml::EndElem;
	}
	xos << xml::EndElem; 
      }
    }
    xos << xml::EndElem; 
    xos << std::endl;
    // -------------------------------------------------------
    // DOSetMap: The first two elements are the *same* for each procedure.
    // -------------------------------------------------------
    xos << xml::BegElem("xaif:DOSetMap");
    procIt.reset();
    for (int procCnt = 1; procIt.isValid(); ++procIt, ++procCnt) {
      PU_Info* pu = (PU_Info*)procIt.current().hval();
      fortTkSupport::IntraOAInfo* oaAnal = ourOAAnalMap.Find(pu);
      fortTkSupport::WNToWNIdMap* wnmap = ourWNToWNIdTableMap.Find(pu);
      OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> rdo = oaAnal->getReachDefsOverwriteXAIF();
      OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF::ChainsIterator> chainsIter 
	= rdo->getChainsIterator();
      for ( ; chainsIter->isValid(); ++(*chainsIter)) {
	OA::OA_ptr<OA::XAIF::ChainsXAIF::ChainIterator> chainIter 
	  = chainsIter->currentChainIterator();
	int chainid = chainsIter->currentId(); // 0-2 are same for each proc
	if ((0 <= chainid && chainid <= 2) && procCnt != 1) { continue; }
	xos << xml::BegElem("xaif:StmtIdSet") << xml::Attr("key", chainid);
	for ( ; chainIter->isValid(); (*chainIter)++ ) {
	  OA::StmtHandle stmt = chainIter->current();
	  WN* stmtWN = (WN*)(stmt.hval());
	  fortTkSupport::WNId stmtid = wnmap->Find(stmtWN);
	  xos << xml::BegElem("xaif:StatementId");
	  if (stmtWN == NULL) {
	    xos << xml::Attr("idRef", "");
	  }
	  else {
	    xos << xml::Attr("idRef", stmtid);
	  }
	  xos << xml::EndElem;
	}
	xos << xml::EndElem; 
      }
    }
    xos << xml::EndElem; 
    xos << std::endl;
  }

  void Whirl2Xaif::translatePU(xml::ostream& xos, 
			       OA::OA_ptr<OA::CallGraph::Node> n, 
			       UINT32 vertexId, 
			       PUXlationContext& ctxt) {
    // FIXME: A more general test will be needed
    PU_Info* pu = (PU_Info*)n->getProc().hval();
    FORTTK_ASSERT(pu, fortTkSupport::Diagnostics::UnexpectedInput << "PU is NULL");
    translatePU(xos, pu, n->getId(), ctxt);
    xos << std::endl;
    xos.flush();
  }

  bool hasUnstructuredCF(WN* thePU_WN_p,
			 ST* puST_p) { 
    // quick test for offending nodes  
    WN_TREE_CONTAINER<PRE_ORDER> aWNPtree(thePU_WN_p);
    WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
    while (aWNPtreeIterator != aWNPtree.end()) { 
      WN* curWN_p = aWNPtreeIterator.Wn();
      OPERATOR opr = WN_operator(curWN_p);
      if (opr==OPR_RETURN) { 
	// does it have a tail of Return nodes...
	// see if this curWN_p is the last node
	WN* tailWN_p=WN_last(WN_kid(thePU_WN_p,WN_kid_count(thePU_WN_p)-1));
	while (tailWN_p) {
	  if (tailWN_p==curWN_p)
	    break;
	  else {
	    if (WN_operator(tailWN_p)!=OPR_RETURN) { 
	      FORTTK_MSG(1,"hasUnstructuredCF: found early return in " << ST_name(puST_p));
	      return true; 
	    }
	    tailWN_p=WN_prev(tailWN_p);
	  }
	}
      }
      if (opr==OPR_GOTO || opr==OPR_AGOTO) {
	LABEL_IDX lb=WN_label_number(curWN_p);
	if (Label_Table[lb].kind==LKIND_LOOP_GEN) { 
	  FORTTK_MSG(1,"hasUnstructuredCF: found unstructured loop control flow statements in "<< ST_name(puST_p));
	  return true; 
	}
	if (Label_Table[lb].kind!=LKIND_SELECT_GEN) { 
	  FORTTK_MSG(1,"hasUnstructuredCF: found goto statements in "<< ST_name(puST_p));
	  return true; 
	}
      }
      if (opr==OPR_TRUEBR) { 
	LABEL_IDX lb=WN_label_number(curWN_p);
	if (Label_Table[lb].kind==LKIND_LOOP_GEN) { 
	  FORTTK_MSG(1,"hasUnstructuredCF: found unstructured loop control flow statements in "<< ST_name(puST_p));
	  return true; 
	}
      } 
      ++aWNPtreeIterator;
    } 
    return false; 
  }

  void Whirl2Xaif::translatePU(xml::ostream& xos, 
			       PU_Info *pu, 
			       UINT32 vertexId,
			       PUXlationContext& ctxt) {
    if (!pu) { return; }
    PU_SetGlobalState(pu);
    bool isCompilerGen = ((Language == LANG_F90) && 
			  (CURRENT_SYMTAB == GLOBAL_SYMTAB + 2) && 
			  Is_Set_PU_Info_flags(pu, PU_IS_COMPILER_GENERATED));
    Is_True(!isCompilerGen, ("Compiler generated PU!")); // FIXME
    // -------------------------------------------------------
    // translate the PU
    // -------------------------------------------------------
    // FIXME: how is PU_f90_lang() different from (Language == LANG_F90)?
    PU& real_pu = PU_Info_pu(pu); 
    ctxt.setF90(PU_f90_lang(real_pu) != 0); // FIXME: set F90 flag
    bool isProgram = PU_is_mainpu(real_pu);
    ST* st = ST_ptr(PU_Info_proc_sym(pu));
    WN *wn_pu = PU_Info_tree_ptr(pu);
    fortTkSupport::PUId puId = ctxt.findPUId(pu);
    ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
   fortTkSupport::SymTabId scopeId = ctxt.findSymTabId(sttab);
   fortTkSupport::SymTabId puScopeId = ctxt.findSymTabId(Scope_tab[CURRENT_SYMTAB].st_tab);
    // Generate the CFG
    xos << xml::Comment(ourDividerComment.c_str());
    if (isProgram) { xos << xml::Comment("*** This is the PROGRAM routine ***"); }
    xos << xml::BegElem("xaif:ControlFlowGraph") 
	<< xml::Attr("vertex_id", vertexId) << xml::Attr("scope_id", scopeId)
	<< AttrSymId(st) << PUIdAnnot(puId)
	<< xml::Attr("controlflowgraph_scope_id", puScopeId);
    if (hasUnstructuredCF(wn_pu,st)) { 
      FORTTK_WMSG("translatePU: found unstructured control flow in "<< ST_name(st) << " while simple loop annotations (-s) were requested");
      xos << xml::Attr("structured", false);
    }
    xos << xml::EndAttrs;
    if (IsActivePU(st)) {
      translateWNPU(xos, wn_pu, ctxt);
    }
    xos << xml::EndElem; // xaif:ControlFlowGraph
  }

  void Whirl2Xaif::translateWNPU(xml::ostream& xos, 
				 WN *wn_pu, 
				 PUXlationContext& ctxt) {
    const char* const caller_err_phase = Get_Error_Phase();
    Diag_Set_Phase("WHIRL to XAIF: translate PU");
    Is_True(WN_opcode(wn_pu) == OPC_FUNC_ENTRY, ("Invalid opcode"));
    // fdump_tree(stderr, wn_pu);
    TranslateWN(xos, wn_pu, ctxt);
    Stab_Free_Tmpvars();  // FIXME: it would be nice to eventually 
    Stab_Free_Namebufs(); // remove this stuff
    Diag_Set_Phase(caller_err_phase);
  }

  void Whirl2Xaif::dumpTranslationHeaderComment(xml::ostream& xos) {
    // 1. Get a string representation of the time
    char tmStr[30] = "unknown time\n";
    time_t tm = time(NULL);
    if (tm != (time_t)-1) { strcpy(tmStr, ctime(&tm)); }
    tmStr[strlen(tmStr)-1] = '\0'; // remove the trailing '\n'
    // 2. Generate header comment
    xos << xml::Comment(ourDividerComment.c_str())
	<< xml::BegComment << "XAIF file translated from WHIRL at " << tmStr 
	<< xml::EndComment
	<< xml::Comment(ourDividerComment.c_str()) << std::endl;
  }

}
