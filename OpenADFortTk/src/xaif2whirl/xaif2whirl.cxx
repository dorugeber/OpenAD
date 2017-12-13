// -*-Mode: C++;-*-

#include <stdlib.h> // ANSI: cstdlib // for strtol
#include <string.h> // ANSI: cstring // for strcmp, etc.
#include <iostream>
#include <vector>
#include <set>
#include <list> // FIXME: for TopologicalSort
#include <map>  // FIXME: for TopologicalSort

#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/dom/DOMElement.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "Open64IRInterface/SymTab.h"
#include "Open64IRInterface/IFDiagnostics.h"
#include "Open64IRInterface/wn_attr.h"
#include "Open64IRInterface/stab_attr.h"
#include "OpenAnalysis/Utils/DGraph/DGraphInterface.hpp"
#include "OpenAnalysis/Utils/DGraph/DGraphImplement.hpp"


#include "WhirlIDMaps.h"
#include "WhirlParentize.h"
#include "XAIFStrings.h"
#include "Diagnostics.h"

#include "xaif2whirl.h"
#include "Args.h"
#include "XlateExpression.h"
#include "XlateStmt.h"
#include "XAIF_DOMFilters.h"
#include "XercesStrX.h"
#include "InterfaceData.h"

// *************************** Forward Declarations ***************************

namespace xaif2whirl { 

  fortTkSupport::IntrinsicXlationTable   
  IntrinsicTable(fortTkSupport::IntrinsicXlationTable::X2W);
  fortTkSupport::WNIdToWNTabMap          WNIdToWNTableMap;

  // FIXME
  extern AlgorithmType opt_algorithm;

  // FIXME
  extern TY_IDX ActiveTypeTyIdx;            // OpenAD active pseudo type
  extern TY_IDX ActiveTypeInitializedTyIdx; // OpenAD active pseudo type
  TY_IDX ActiveTypeTyIdx;            
  TY_IDX ActiveTypeInitializedTyIdx;

  // *************************** Forward Declarations ***************************
  // ControlFlowGraph

  static void
  TranslateCFG(WN *wn_pu, const xercesc::DOMElement* cfgElem, PUXlationContext& ctxt);

  static WN*
  xlate_CFG(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
	    OA::OA_ptr<MyDGNode> root, PUXlationContext& ctxt,
	    unsigned& startLabel_r, 
	    bool structuredCF);

  static WN*
  TranslateBasicBlock(WN *wn_pu, 
		      const xercesc::DOMElement* bbElem, 
		      PUXlationContext& ctxt,
		      bool skipMarkeredGotoAndLabels,
		      unsigned endLabel);

  // *************************** Forward Declarations ***************************

  // ControlFlowGraph -- basic block patching algorithm

  static void
  TranslateBB_OLD(WN *wn_pu, const xercesc::DOMElement* bbElem, PUXlationContext& ctxt);

  static void
  xlate_BasicBlock_OLD(WN *wn_pu, const xercesc::DOMElement* bbElem, 
		       PUXlationContext& ctxt);
  static void
  xlate_BBCond_OLD(WN* wn_pu, const xercesc::DOMElement* bbElem, PUXlationContext& ctxt);

  static bool
  FindNextStmtInterval(const xercesc::DOMElement* bbElem, fortTkSupport::IdList<fortTkSupport::WNId>* bbIdList, 
		       fortTkSupport::WNIdToWNMap* wnmap, WN* blkWN,
		       xercesc::DOMElement* &begXAIF, xercesc::DOMElement* &endXAIF,
		       WN* &begWN, WN* &endWN);

  static WN*
  FindIntervalBoundary(const xercesc::DOMElement* elem, fortTkSupport::IdList<fortTkSupport::WNId>* bbIdList, 
		       fortTkSupport::WNIdToWNMap* wnmap, WN* blkWN, int boundary);

  static WN* 
  FindWNBlock(const xercesc::DOMElement* bbElem, fortTkSupport::IdList<fortTkSupport::WNId>* idlist, 
	      PUXlationContext& ctxt);

  static WN* 
  FindSafeInsertionPoint(WN* blckWN, WN* stmtWN);

  static void
  RemoveFromWhirlIdMaps(WN* wn, fortTkSupport::WNToWNIdMap* wn2idmap, fortTkSupport::WNIdToWNMap* id2wnmap);

  // *************************** Forward Declarations ***************************
  // Scopes and Symbols

  static void
  xlate_SymbolTable(const xercesc::DOMElement* elem,
		    const char* scopeId, PU_Info* pu, 
		    PUXlationContext& ctxt);

  static void
  xlate_Symbol(const xercesc::DOMElement* elem, 
	       const char* scopeId, 
	       PU_Info* pu, 
	       PUXlationContext& ctxt,
	       bool doTempSymbols);

  // *************************** Forward Declarations ***************************

  // WHIRL Creation functions

  static WN*
  CreateOpenADReplacementBeg(const char* placeholder);

  static WN*
  CreateOpenADReplacementEnd();

  static WN* 
  CreateIfCondition(WN* condWN);

  static ST* 
  CreateST(const xercesc::DOMElement* elem, 
	   SYMTAB_IDX level, 
	   const char* nm,
	   fortTkSupport::XAIFSymToSymbolMap& symMap,
	   const char* scopeId);

  static ST* 
  ConvertIntoGlobalST(ST* st);

  static void 
  ConvertToActiveType(ST* st);

  static void 
  ConvertStructMemberToActiveType(TY_IDX base_ty, TY_IDX ref_ty,
				  UINT field_id);

  static void 
  ConvertScalarizedRefToActiveType(WN* wn);

  static FLD_HANDLE
  TY_Lookup_FLD(TY_IDX struct_ty, TY_IDX ref_ty, UINT64 ref_ofst, unsigned short eqInst=1);


  // FIXME (Note: TYPE_ID and TY_IDX are typedef'd to the same type, so
  // overloading is not possible!)
  // static TY_IDX MY_Make_Array_Type1 (TYPE_ID elem_ty, INT32 ndim, INT64 len);
  static TY_IDX MY_Make_Array_Type (TY_IDX elem_ty, 
				    INT32 ndim, 
				    bool fixed,  
				    const INT64* lower, 
				    const INT64* upper);

  static TY_IDX
  XAIFTyToWHIRLTy(const char* type, const TYPE_ID mtype); 

  // *************************** Forward Declarations ***************************

  class ConvertModuleTypeFctr {
  public:
    ConvertModuleTypeFctr(TY_IDX struct_ty_, TY_IDX ref_ty_, UINT field_id_)
      : struct_ty(struct_ty_), ref_ty(ref_ty_), field_id(field_id_)
    { 
      ty_name  = TY_name(struct_ty);
      ty_mtype = TY_mtype(struct_ty);
      ty_size  = TY_size(struct_ty);
    }
    ~ConvertModuleTypeFctr() { }
  
    bool operator()(UINT32 idx, const TY* entry) const { 
      // If this is the non-external version of the type we seek, change it
      if (!TY_is_external(*entry) 
	  && TY_mtype(*entry) == ty_mtype
	  && TY_size(*entry) == ty_size
	  && strcmp(TY_name(*entry), ty_name) == 0) {
	TY_IDX ty = make_TY_IDX(idx);
	ConvertStructMemberToActiveType(ty, ref_ty, field_id);
	return true; // early exit
      }
      return false; // continue
    }
  
  private:
    TY_IDX struct_ty;
    TY_IDX ref_ty;
    UINT field_id;

    // cached values
    const char* ty_name;
    MTYPE       ty_mtype;
    UINT64      ty_size;
  };

  // *************************** Forward Declarations ***************************

  // MyDGNode routines

  unsigned int MyDGNode::nextId = 1;

  // sort_CondVal: Used to sort operands of (arguments to) an expression
  // by the "condition_value" attribute
  struct sort_CondVal
  {
    sort_CondVal(bool ascending_ = true) : ascending(ascending_) { }
  
    // return true if e1 < e2; false otherwise
    bool operator()(const OA::OA_ptr<MyDGEdge> e1, 
		    const OA::OA_ptr<MyDGEdge> e2) const
    {
      unsigned int cond1 = GetCondAttr(e1->GetElem());
      unsigned int cond2 = GetCondAttr(e2->GetElem());
      return (ascending) ? (cond1 < cond2) : (cond1 > cond2);
    }

  private:
    bool ascending;
  };

  static OA::OA_ptr<OA::DGraph::DGraphInterface> 
  CreateCFGraph(const xercesc::DOMElement* elem);

  //static list<OA::OA_ptr<OA::DGraph::Interface::Node> >*
  //TopologicalSort(OA::OA_ptr<OA::DGraph::Interface> graph);


  // ****************************************************************************
  // Top level translation routines
  // ****************************************************************************

  // TranslateCFG: Translate XAIF CFG or XAIF Replacement to WHIRL
  void
  TranslateCFG(PU_Info* pu_forest, const xercesc::DOMElement* cfgElem,
	       PUXlationContext& ctxt)
  {
    // -------------------------------------------------------
    // Find original PU and set globals
    // -------------------------------------------------------
    fortTkSupport::PUId puid = GetPUId(cfgElem);
    PU_Info* pu = ctxt.findPU(puid);
    if (!pu) { return; }

    // the PU_info is the original one. 
    // but we may have changed the name
    // so we should compare if the name 
    // matches and replace the symbol reference 
    // to the proper name.  UNLESS this is 
    // a module
    fortTkSupport::Symbol* symd = GetSymbol(cfgElem, ctxt);
    FORTTK_ASSERT(symd, "Could not find symbol for CFG element " << *cfgElem);
    ST* std = symd->GetST();
    bool isModuleName=(ST_is_in_module(*std) 
		       && 
		       (PU_lexical_level(Pu_Table[ST_pu(*std)])<3));
    if (ST_is_in_module(*std) 
	&& 
	PU_lexical_level(Pu_Table[ST_pu(*std)])==3
	&& 
	PU_Info_proc_sym(pu)!=ST_st_idx (*std)) { 
      // need to see if there is a corresponding interface and adjust it
      InterfaceData::findAndAdjustInterface(InterfaceData::getParentOf(pu),
					    PU_Info_tree_ptr(pu),
					    std);
    } 
    // compare this by comparing the symbol table index
    if (!isModuleName && PU_Info_proc_sym(pu)!=ST_st_idx (*std))
      PU_Info_proc_sym(pu)=ST_st_idx (*std);

    FORTTK_MSG(1,"TranslateCFG: starting on " << XercesStrX(cfgElem->getNodeName()) << ": " << ST_name(std));
  
    // Set globals
    WN *wn_pu = PU_Info_tree_ptr(pu);
    // set up the name in the FUNC_ENTRY too
    // compare this by comparing the symbol table index UNLESS this is 
    // a module
    if (!isModuleName && WN_st_idx(wn_pu)!=ST_st_idx (*std))
      WN_st_idx(wn_pu)=ST_st_idx (*std);
    PU_SetGlobalState(pu);


    // -------------------------------------------------------
    // Translate, modifying 'wn_pu'
    // -------------------------------------------------------

    ST* st = ST_ptr(PU_Info_proc_sym(pu));
    if (IsActivePU(st)) {
      TranslateCFG(wn_pu, cfgElem, ctxt);
    }
  
#if 0
    fprintf(stderr, "\n----------------------------------------------------\n");
    fdump_tree(stderr, wn_pu);
#endif
  }


  // ****************************************************************************
  // ControlFlowGraph
  // ****************************************************************************

  // Structured and Unstructured CFG Translation, with special reference
  // to WHIRL SWITCHes.
  //
  // WHIRL does not have a structured multiway branch and so Fortran
  // selects are translated into a jump table and several
  // label/case-specific-code/goto-select-end blocks.
  // 
  // Given a 'structured' WHIRL PU -- a PU without any GOTOs except for
  // CASE-block-related ones -- we can translate the SWITCH into an XAIF
  // structured multiway branch, complete with an EndBranch node.  (The
  // modified OpenAnalysis CFG provides the needed information.)
  // Assuming the XAIF retains the structured property, then the XAIF
  // can be translated back into a WHIRL switch node by removing all
  // original WHIRL LABELs/GOTOs and adding new LABELs/GOTOs to the
  // SWITCH case blocks.  (The original code must have had only 1)
  // SWITCH-related GOTOs/LABELs and 2) possibly some non-target LABELs
  // generated by mfef90.  It is safe to remove (2).  It is safe to
  // remove (2) because new LABELs/GOTOs will be generated.)
  // 
  // Given a non-structured WHIRL PU, we can translate multiway branches
  // into WHIRL SWITCHes in the same way as above.  A general and simple
  // solution is to replace all original LABELs (always at the beginning
  // of a basic block) and GOTOs (always at the end of a basic block)
  // with new ones.  This eliminates the need to develop a more complex
  // mechanism (such as label remapping) to ensure newly generated
  // labels to not conflict with original labels.


  static WN*
  xlate_CFG_BasicBlock(WN *wn_pu, OA::OA_ptr<MyDGNode> curBB, 
		       PUXlationContext& ctxt, 
		       bool skipMarkeredGotoAndLabels, 
		       unsigned newCurBBLbl, 
		       unsigned newNextBBLbl,
		       unsigned endLabel);

  static WN*
  xlate_CFG_BranchMulti(OA::OA_ptr<MyDGNode> curNode, WN* condWN, 
			unsigned lastLbl,
			vector<OA::OA_ptr<MyDGEdge> >& outedges,
			map<OA::OA_ptr<MyDGNode>, unsigned>& nodeToLblMap);


  // TranslateCFG: Given an XAIF CFG or XAIF Replacement rooted at
  // 'cfgElem' and its corresponding WHIRL tree 'wn_pu', modify the
  // WHIRL to reflect the XAIF.
  static void
  TranslateCFG(WN *wn_pu, const xercesc::DOMElement* cfgElem, PUXlationContext& ctxt)
  {
    // -------------------------------------------------------
    // 1. Create auxiliary data structures
    // -------------------------------------------------------
  
    // 0. WHIRL parent map
    fortTkSupport::WhirlParentMap wnParentMap(wn_pu);
    ctxt.setWNParentMap(&wnParentMap);
  
    // 1. WHIRL<->ID maps
    fortTkSupport::WNToWNIdMap* wnmapx = new fortTkSupport::WNToWNIdMap();
    CreateWhirlIdMaps(wn_pu, wnmapx, NULL);
    ctxt.setWNToWNIdMap(wnmapx);

    fortTkSupport::WNIdToWNMap* wnmapy = WNIdToWNTableMap.Find(Current_PU_Info);
    ctxt.setWNIdToWNMap(wnmapy);
  
    // -------------------------------------------------------
    // 2. Update passing style for arguments (especially used in reverse
    // mode to change active arguments to pass-by-reference)
    // -------------------------------------------------------
    xercesc::DOMElement* arglst = GetChildElement(cfgElem, XAIFStrings.elem_ArgList_x());
    xercesc::DOMElement* arg = (arglst) ? 
      GetChildElement(arglst, XAIFStrings.elem_ArgSymRef_x()) : NULL;
    for ( ; arg; arg = GetNextSiblingElement(arg)) {
      // find corresponding WN and symbol
      fortTkSupport::WNId id = GetWNId(arg);
      WN* parmWN = ctxt.findWN(id, true /* mustFind */);
      fortTkSupport::Symbol* sym = GetSymbol(arg, ctxt);
      ST* parmST = sym->GetST();
    
      //bool active = GetActiveAttr(arg); 
      const XMLCh* intentX = arg->getAttribute(XAIFStrings.attr_intent_x());
      XercesStrX intent = XercesStrX(intentX);
    
      if (strcmp(intent.c_str(), "in") == 0) {
	WN_Set_Parm_In(parmWN);
	Set_ST_is_intent_in_argument(parmST);
      }
      else if (strcmp(intent.c_str(), "out") == 0) {
	WN_Set_Parm_Out(parmWN);
	Set_ST_is_intent_out_argument(parmST);		
      }
      else if (strcmp(intent.c_str(), "inout") == 0) {
	WN_Set_Parm_By_Reference(parmWN); // unnecessary for 'whirl2f'
	Clear_ST_is_intent_in_argument(parmST);
	Clear_ST_is_intent_out_argument(parmST);
      }
      else {
	FORTTK_DIE("Unknown intent to argument:\n" << *arg);
      }
    }
  
    // -------------------------------------------------------
    // 3. Translate each XAIF CFG into WHIRL
    // -------------------------------------------------------
  
    // Collect the list of CFGs we need to translate.  
    list<xercesc::DOMElement*> cfglist;
    if (XAIF_CFGElemFilter::IsReplaceList(cfgElem)) {
      XAIF_ElemFilter filter(XAIFStrings.elem_Replacement_x());
      for (xercesc::DOMElement* e = GetChildElement(cfgElem, &filter); 
	   (e); e = GetNextSiblingElement(e, &filter)) {
	cfglist.push_back(e);
      }
    }
    else {
      cfglist.push_back(const_cast<xercesc::DOMElement*>(cfgElem));
    }

    // Translate
    WN* newstmtblkWN = WN_CreateBlock();
    unsigned startLabel=1;
    for (list<xercesc::DOMElement*>::iterator it = cfglist.begin(); 
	 it != cfglist.end(); ++it) {
      xercesc::DOMElement* cfgelm = (*it);
      OA::OA_ptr<OA::DGraph::DGraphInterface> cfg = CreateCFGraph(cfgelm);
    
      if (opt_algorithm == ALG_BB_PATCHING) { 
	XAIF_BBElemFilter filt(false /* edges */);
	for (xercesc::DOMElement* elem = GetChildElement(cfgelm, &filt);
	     (elem); elem = GetNextSiblingElement(elem, &filt)) {
	  TranslateBB_OLD(wn_pu, elem, ctxt);
	}
      } 
      else {
	OA::OA_ptr<OA::DGraph::NodesIteratorInterface> enodeIter
	  = cfg->getEntryNodesIterator();
	assert(enodeIter->isValid());
	OA::OA_ptr<OA::DGraph::NodeInterface> temp = enodeIter->current();
	OA::OA_ptr<MyDGNode> root = temp.convert<MyDGNode>();
	(*enodeIter)++; assert(!enodeIter->isValid());
	WN* cfgblkWN = xlate_CFG(wn_pu, 
				 cfg, 
				 root, 
				 ctxt, 
				 startLabel,
				 GetBoolAttr(cfgelm, 
					     XAIFStrings.attr_structured_x(),
					     true/*default if not specified*/));
	if (XAIF_CFGElemFilter::IsReplacement(cfgelm)) {
	  const XMLCh* pX = 
	    cfgelm->getAttribute(XAIFStrings.attr_placeholder_x());
	  XercesStrX p = XercesStrX(pX);
	  WN* begWN = CreateOpenADReplacementBeg(p.c_str());
	  WN* endWN = CreateOpenADReplacementEnd();
	  WN_INSERT_BlockFirst(cfgblkWN, begWN);
	  WN_INSERT_BlockLast(cfgblkWN, endWN);
	}
	WN_INSERT_BlockLast(newstmtblkWN, cfgblkWN);
      }
    }
  
    // -------------------------------------------------------
    // 4. Replace old WHIRL code with newly translated WHIRL
    // -------------------------------------------------------
    if (opt_algorithm != ALG_BB_PATCHING) { 
      // Delete old WHIRL
      WN* funcblk = WN_func_body(wn_pu);
      for (WN* kid = WN_first(funcblk); (kid); /* */) {
	WN* nextkid = WN_next(kid); // must find next 'kid' now!
	WN_DELETE_FromBlock(funcblk, kid);
	kid = nextkid;
      }
    
      // Splice in newly translated WHIRL 
      for (WN* kid = WN_first(newstmtblkWN); (kid); /* */) {
	WN* nextkid = WN_next(kid); // must find next 'kid' now!
	WN_EXTRACT_FromBlock(newstmtblkWN, kid);
	WN_INSERT_BlockLast(funcblk, kid);
	kid = nextkid;
      }
      WN_Delete(newstmtblkWN); // not recursive -- should be empty
    }
  
    // -------------------------------------------------------
    // 5. Cleanup
    // -------------------------------------------------------
    delete wnmapx;
  }


  // xlate_CFG: Given the original WHIRL tree, a CFG structure
  // representing the XAIF CFG, and the root CFG node, translate the CFG
  // into a block of WHIRL statements.  If the CFG contains only
  // structured control flow, 'structured' should be true; the WHIRL
  // will also contain goto-less nested and structured control flow.
  // Otherwise, 'structured' should be false and the WHIRL will contain
  // labels and gotos.
  //
  // During translation, the non-numerical WHIRL statements represented
  // by xaif:Marker will be copied from the original WHIRL tree and
  // placed in the returned block.  It is expected that the *caller*
  // will splice the returned block containing new statements back into
  // the WHIRL FUNC_ENTRY.
  //
  // Note: This routine will not translate any basic blocks in the CFG
  // that are unreachable from 'startNode' (i.e. dead code). 
  // [FIXME unstructured]

  static pair<WN*, OA::OA_ptr<MyDGNode> >
  xlate_CFGstruct(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
		  OA::OA_ptr<MyDGNode> startNode, set<xercesc::DOMElement*>& xlated, 
		  PUXlationContext& ctxt,
		  unsigned int& startLabel_r);

  static WN*
  xlate_CFGunstruct(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
		    OA::OA_ptr<MyDGNode> startNode, set<xercesc::DOMElement*>& xlated, 
		    PUXlationContext& ctxt,
		    unsigned int& startLabel_r);

  static WN*
  xlate_CFG(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
	    OA::OA_ptr<MyDGNode> root, PUXlationContext& ctxt, 
	    unsigned int& startLabel_r,
	    bool structuredCF)
  {
    WN* blkWN = NULL;
    set<xercesc::DOMElement*> xlated;
    if (structuredCF) {
      pair<WN*, OA::OA_ptr<MyDGNode> > ret = 
	xlate_CFGstruct(wn_pu, cfg, root, xlated, ctxt, startLabel_r);
      blkWN = ret.first;
    } else {
      blkWN = xlate_CFGunstruct(wn_pu, cfg, root, xlated, ctxt, startLabel_r);
    }
    return blkWN;
  }


  // xlate_CFGstruct: Helper for translating a structured CFG.  The
  // algorithm uses the structured CF and recursion to implicitly create
  // nested control flow.
  //
  // Return value: <new-WHIRL-stmt-block, ending-basic-block> (If the
  // latter is NULL, it means we saw the Exit basic block)
  static pair<WN*, OA::OA_ptr<MyDGNode> >
  xlate_CFGstruct(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
		  OA::OA_ptr<MyDGNode> startNode, set<xercesc::DOMElement*>& xlated, 
		  PUXlationContext& ctxt,
		  unsigned int& startLabel_r)
  {
    using namespace OA::CFG;  

    WN* blkWN = WN_CreateBlock();
  
    // ---------------------------------------------------
    // We must generate labels FIXME
    // ---------------------------------------------------
    map<OA::OA_ptr<MyDGNode>, unsigned> nodeToLblMap;
  
    // Initialize label maps
    OA::OA_ptr<OA::DGraph::NodesIteratorInterface> nodeIt 
      = cfg->getNodesIterator();
    for ( ; nodeIt->isValid(); ++(*nodeIt)) {
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = nodeIt->current();
      OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();

      nodeToLblMap[n] = startLabel_r++;
    }
  
    // ---------------------------------------------------
    // Translate, beginning with 'startNode'
    // ---------------------------------------------------
    bool continueIteration = true;
    bool generateLbl = false;
    OA::OA_ptr<MyDGNode> curNode = startNode;
    while (!curNode.ptrEqual(NULL) && continueIteration) {

      xercesc::DOMElement* bbElem = curNode->GetElem();
      unsigned curLbl = nodeToLblMap[curNode];
   
      if (XAIF_BBElemFilter::IsBBEntry(bbElem) ||
	  XAIF_BBElemFilter::IsBBExit(bbElem) ||
	  XAIF_BBElemFilter::IsBB(bbElem)) {
	// ---------------------------------------------------
	// A non-control-flow basic block
	// ---------------------------------------------------
	OA::OA_ptr<MyDGNode> nextNode = GetSuccessor(curNode); // at most one outgoing edge
	unsigned lbl = (generateLbl) ? curLbl : 0;
	WN* stmts = xlate_CFG_BasicBlock(wn_pu, 
					 curNode, 
					 ctxt, 
					 true, 
					 lbl,
					 0,
					 0);
	WN_INSERT_BlockLast(blkWN, stmts);
	generateLbl = false;
	curNode = nextNode;
      }
      else if (XAIF_BBElemFilter::IsBBBranch(bbElem)) {
	// ---------------------------------------------------
	// Begin a structured branch.  Note: in XAIF branches are
	// 'structured switches'.
	// ---------------------------------------------------
	unsigned int numOutEdges = curNode->num_outgoing();

	// 1. Translate condition expression. 
	xercesc::DOMElement* cond = 
	  GetChildElement(bbElem, XAIFStrings.elem_Condition_x());
	xercesc::DOMElement* condexpr = GetFirstChildElement(cond);      
	WN* condWN = XlateExpression::translateExpression(condexpr, 
							  ctxt);
	if (numOutEdges == 2) {
	  // Because branches are 'structured switches', ensure we have
	  // a boolean expression for an 'if'.
	  condWN = CreateIfCondition(condWN);
	}
      
	// 2. Gather all outgoing edges, sorted by condition (specially
	// sort two-way branches into true-false order.)
	OA::OA_ptr<MyDGEdge> tmp; tmp = NULL;
	vector<OA::OA_ptr<MyDGEdge> > outedges(numOutEdges, tmp);
	OA::OA_ptr<OA::DGraph::EdgesIteratorInterface> it = 
	  curNode->getOutgoingEdgesIterator();
	for (int i = 0; it->isValid(); ++(*it), ++i) {
	  OA::OA_ptr<OA::DGraph::EdgeInterface> etmp = it->current();
	  outedges[i] = etmp.convert<MyDGEdge>();
	}
	std::sort(outedges.begin(), outedges.end(), 
		  sort_CondVal((numOutEdges != 2)));

	// 3. Translate (recursively) each child block of this branch
	vector<WN*> childblksWN(numOutEdges, NULL);
	OA::OA_ptr<MyDGNode> endBrNode; endBrNode = NULL;
	for (unsigned i = 0; i < outedges.size(); ++i) {
	  OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = outedges[i]->getSink();
	  OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
	  pair<WN*, OA::OA_ptr<MyDGNode> > p 
	    = xlate_CFGstruct(wn_pu, cfg, n, xlated, ctxt, startLabel_r);
	  childblksWN[i] = p.first;
	  endBrNode = p.second; // will be EndBranch for structured-CF
	}
	OA::OA_ptr<MyDGNode> nextNode = GetSuccessor(endBrNode);
      
	// 4. Create branch control flow
	if (numOutEdges == 2) {
	  WN* ifWN = WN_CreateIf(condWN, childblksWN[0], childblksWN[1]);
	  WN_INSERT_BlockLast(blkWN, ifWN);
	} 
	else {
	  // Find the branch-around (or last) label
	  unsigned lastLbl = nodeToLblMap[nextNode];
	
	  // Add a LABEL/GOTO at the front/end of each successor block
	  for (unsigned i = 0; i < outedges.size(); ++i) {
	    OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = outedges[i]->getSink();
	    OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
	    WN* nblkWN = childblksWN[i];
	  
	    WN* lblWN = WN_CreateLabel(nodeToLblMap[n], 0 /*label_flag*/, NULL);
	    WN_INSERT_BlockFirst(nblkWN, lblWN);
	    WN* gotoWN = WN_CreateGoto(lastLbl);
	    WN_INSERT_BlockLast(nblkWN, gotoWN);
	  }
	  generateLbl = true; // add label to front of successor
	
	  // Create SWITCH with CASEGOTOs
	  WN* switchWN = xlate_CFG_BranchMulti(curNode, condWN, lastLbl,
					       outedges, nodeToLblMap);
	  WN_INSERT_BlockLast(blkWN, switchWN);
	
	  // Add switch blocks right after SWITCH
	  for (unsigned i = 0; i < childblksWN.size(); ++i) {
	    WN_INSERT_BlockLast(blkWN, childblksWN[i]);
	  }
	}
      
	curNode = nextNode;
      }
      else if (XAIF_BBElemFilter::IsBBEndBr(bbElem)) {
	// ---------------------------------------------------
	// End a structured branch
	// ---------------------------------------------------
	continueIteration = false;
      }
      else if (XAIF_BBElemFilter::IsBBForLoop(bbElem) ||
	       XAIF_BBElemFilter::IsBBPreLoop(bbElem) ||
	       XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
	// ---------------------------------------------------
	// Begin a structured loop
	// ---------------------------------------------------
      
	bool isDoLoop = (XAIF_BBElemFilter::IsBBForLoop(bbElem));

	// 1. Gather children23
	OA::OA_ptr<MyDGNode> body = GetSuccessorAlongEdge(curNode, 1);
	OA::OA_ptr<MyDGNode> fallthru = GetSuccessorAlongEdge(curNode, 0);
      
	// 2. Translate (recursively) loop body
	pair<WN*, OA::OA_ptr<MyDGNode> > p 
	  = xlate_CFGstruct(wn_pu, cfg, body, xlated, ctxt, startLabel_r);
	WN* bodyWN = p.first;
      
	// 3. Translate condition expression (and update/init statements)
	xercesc::DOMElement* cond = 
	  GetChildElement(bbElem, XAIFStrings.elem_Condition_x());
	xercesc::DOMElement* condexpr = GetFirstChildElement(cond);      
	WN* condWN = NULL;
	if (isDoLoop) {
	  condWN = XlateExpression::translateExpressionSimple(condexpr, ctxt);
	} else {
	  condWN = XlateExpression::translateExpression(condexpr, ctxt);
	}
      
	xercesc::DOMElement *init = NULL, *update = NULL;
	WN *initWN = NULL, *updateWN = NULL;
	if (XAIF_BBElemFilter::IsBBForLoop(bbElem)) {
	  // Note: initWN and updateWN are STIDs
	  init = GetChildElement(bbElem, XAIFStrings.elem_LpInit_x());
	  update = GetChildElement(bbElem, XAIFStrings.elem_LpUpdate_x());
	  initWN = XlateStmt::translateStmt(init, ctxt);
	  updateWN = XlateStmt::translateStmt(update, ctxt);
	}
      
	// 4. Create control flow statement
	WN* stmtWN = NULL;
	if (isDoLoop) {
	  WN* idxWN = WN_CreateIdname(WN_store_offset(initWN), WN_st(initWN));
	  stmtWN = WN_CreateDO(idxWN, initWN, condWN, updateWN, bodyWN, NULL);
	}
	else if (XAIF_BBElemFilter::IsBBPreLoop(bbElem)) {
	  stmtWN = WN_CreateWhileDo(condWN, bodyWN);
	}
	else if (XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
	  stmtWN = WN_CreateDoWhile(condWN, bodyWN);
	}
      
	WN_INSERT_BlockLast(blkWN, stmtWN);
	curNode = fallthru;
      }      
      else if (XAIF_BBElemFilter::IsBBEndLoop(bbElem)) {
	// ---------------------------------------------------
	// End a structured loop
	// ---------------------------------------------------
	continueIteration = false;
      }
      else {
	FORTTK_DIE("Unknown XAIF basic block:\n" << *bbElem);
      }
    }
  
    return make_pair(blkWN, curNode);
  }

  /** 
   * for the back translation of unstructured control flow graphs
   */ 
  OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > >
  getOrderedSinkNodesList(OA::OA_ptr<OA::DGraph::DGraphInterface> cfg,
			  OA::OA_ptr<OA::DGraph::NodeInterface> pNode) {
    OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > retval;
    retval = new std::list<OA::OA_ptr<OA::DGraph::NodeInterface> >;
    // put all sink nodes in a list
    OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > tempList;
    tempList = new std::list<OA::OA_ptr<OA::DGraph::NodeInterface> >;
    std::list<OA::OA_ptr<OA::DGraph::EdgeInterface> >::iterator iter;
    OA::OA_ptr<MyDGNode> cfgNode = pNode.convert<MyDGNode>();
    xercesc::DOMElement* bbElem = cfgNode->GetElem();
    OA::OA_ptr<OA::DGraph::EdgesIteratorInterface>it=pNode->getOutgoingEdgesIterator();
    for(; it->isValid(); ++(*it)) {
      OA::OA_ptr<OA::DGraph::EdgeInterface> e = it->current();
      OA::OA_ptr<MyDGEdge> cfgEdge = e.convert<MyDGEdge>();
      // std::cout << "getOrderedSinkNodesList edge has cond: " <<  GetHasConditionAttr(cfgEdge->GetElem()) << " val " << GetCondAttr(cfgEdge->GetElem()) << std::endl; 
      if ((XAIF_BBElemFilter::IsBBForLoop(bbElem) 
	   ||
	   XAIF_BBElemFilter::IsBBPostLoop(bbElem))
	  && 
	  ( 
	   (GetHasConditionAttr(cfgEdge->GetElem()) 
	    && 
	    GetCondAttr(cfgEdge->GetElem())==0 )
	   || 
	   ! GetHasConditionAttr(cfgEdge->GetElem()))) { 
	retval->push_front(e->getSink());
      }
      else { 
	retval->push_back(e->getSink());
      }
    }
    return retval;
  }
    
  /** 
   * for the back translation of unstructured control flow graphs
   */ 
  void getReversePostDFSListR(OA::OA_ptr<OA::DGraph::DGraphInterface> cfg,
			      OA::OA_ptr<OA::DGraph::NodeInterface> pNode,
			      std::map<OA::OA_ptr<OA::DGraph::NodeInterface>,bool>& visitMap,
			      OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > pList ) { 
    OA::OA_ptr<MyDGNode> cfgNode = pNode.convert<MyDGNode>();
    xercesc::DOMElement* bbElem = cfgNode->GetElem();
    // std::cout << " getReversePostDFSListR invoked for " << bbElem->getNodeName() << " " << bbElem->getAttribute(XAIFStrings.attr_Vid_x()) << std::endl;   

    // mark as visited so that we don't get in an infinite
    // loop on cycles in the graph
    visitMap[pNode] = true;
    // loop over the successors or predecessors based on orientation
    OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > nodeList = getOrderedSinkNodesList(cfg,pNode); 
    std::list<OA::OA_ptr<OA::DGraph::NodeInterface> >::iterator it=nodeList->begin();
    for (; it!=nodeList->end(); ++it) {
      OA::OA_ptr<OA::DGraph::NodeInterface> n = *it;
      // if the node hasn't been visited then call recursively
      if (!visitMap[n]) {
	getReversePostDFSListR(cfg, n, visitMap, pList);
      }
    }
    // add ourselves to the beginning of the list
    // std::cout << " getReversePostDFSListR pushing for " << bbElem->getNodeName() << " " << bbElem->getAttribute(XAIFStrings.attr_Vid_x()) << std::endl;   
    pList->push_front(pNode);
  } 

  /** 
   * for the back translation of unstructured control flow graphs
   * this behaves like normal reversePostDFS order 
   * (as it was implemented in OpenAnalysis)  
   * with the one exception that we the successor nodes of a given loop node 
   * are ordered such that the node to loop successor (not the loop body) 
   * comes first in the DFS search (see the logic in getOrderedSinkNodesList). 
   * This removes the scenario of an EXIT node being placed in the middle of the list of blocks 
   * that are being connected via gotos (which consequently rather than 
   * exiting leads to continued execution of the next block that 
   * happens to follow the EXIT node in the list). 
   */ 
  OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > >
  getReversePostDFSList(OA::OA_ptr<OA::DGraph::DGraphInterface> cfg) { 
    std::map<OA::OA_ptr<OA::DGraph::NodeInterface>,bool> visitMap;
    // loop over all nodes and set their visit field to false
    OA::OA_ptr<OA::DGraph::NodesIteratorInterface> nodeIter = cfg->getNodesIterator();
    for ( ; nodeIter->isValid(); (*nodeIter)++ ) {
      OA::OA_ptr<OA::DGraph::NodeInterface> node = nodeIter->current();
      visitMap[node] = false;
    }
    // generate a list of nodes in the requested ordering
    OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > retval;
    retval = new std::list<OA::OA_ptr<OA::DGraph::NodeInterface> >;
    nodeIter = cfg->getEntryNodesIterator();
    for ( ; nodeIter->isValid(); (*nodeIter)++ ) {
      OA::OA_ptr<OA::DGraph::NodeInterface> on = nodeIter->current();
      getReversePostDFSListR(cfg,
			     nodeIter->current(), 
			     visitMap, 
			     retval);
    }
    return retval;
  } 

  // xlate_CFGunstruct: Helper for translating an unstructured CFG.  
  // 
  // Note: The CFG node (MyDGNode) id forms an implicit label number for
  // each basic block.  We do not worry about interfering with original
  // labels because we do not keep them.
  static WN*
  xlate_CFGunstruct(WN* wn_pu, OA::OA_ptr<OA::DGraph::DGraphInterface> cfg, 
		    OA::OA_ptr<MyDGNode> startNode, set<xercesc::DOMElement*>& xlated, 
		    PUXlationContext& ctxt,
		    unsigned int& startLabel_r)
  {
    //    using namespace OA::DGraph;
    //    using namespace OA::CFG;


    WN* blkWN = WN_CreateBlock();

    // Topological sort to ensure that, e.g., the exit node is last

    // ---------------------------------------------------
    // We must generate labels that do not conflict with other labels in
    // the WHIRL code.  We use two maps to remember label values.
    // ---------------------------------------------------

    map<OA::OA_ptr<MyDGNode>, unsigned> nodeToLblMap;
    map<OA::OA_ptr<MyDGNode>, unsigned> nodeToLoopContLblMap;
  
    // Initialize label maps
    OA::OA_ptr<std::list<OA::OA_ptr<OA::DGraph::NodeInterface> > > nodeList;
    nodeList=getReversePostDFSList(cfg);
    std::list<OA::OA_ptr<OA::DGraph::NodeInterface> >::iterator it= nodeList->begin();
    
    // the final label for this CFG guaranteed to be at the end
    unsigned endLabel=startLabel_r++;
    
    for (; it!=nodeList->end(); ++it) {
            
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = *it;
      OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
      nodeToLblMap[n] = startLabel_r++;
    
      // See notes on translating loops below
      xercesc::DOMElement* bbElem = n->GetElem();
      if (XAIF_BBElemFilter::IsBBForLoop(bbElem) ||
	  XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
	nodeToLoopContLblMap[n] = startLabel_r++;
      } 
      else if (XAIF_BBElemFilter::IsBBPreLoop(bbElem)) {
	nodeToLoopContLblMap[n] = nodeToLblMap[n];
      }
    }
  
    // ---------------------------------------------------
    // Translate in topological order
    // ---------------------------------------------------
    for (it= nodeList->begin(); it!=nodeList->end();++it) {
        
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = *it;
      OA::OA_ptr<MyDGNode> curNode = ntmp.convert<MyDGNode>();
      xercesc::DOMElement* bbElem = curNode->GetElem();
      unsigned curLbl = nodeToLblMap[curNode];
      // std::cout << " looking at " << bbElem->getNodeName() << std::endl;   
      if (XAIF_BBElemFilter::IsBBEntry(bbElem) ||
	  XAIF_BBElemFilter::IsBBExit(bbElem) ||
	  XAIF_BBElemFilter::IsBB(bbElem)) {
	// ---------------------------------------------------
	// A non-control-flow basic block
	// ---------------------------------------------------
	OA::OA_ptr<MyDGNode> nextNode = GetSuccessor(curNode); // at most one outgoing edge
	unsigned nextLbl = (!nextNode.ptrEqual(NULL)) ? nodeToLblMap[nextNode] : 0;
	if (XAIF_BBElemFilter::IsBBExit(bbElem))
	  nextLbl=endLabel;
	WN* stmts = xlate_CFG_BasicBlock(wn_pu, 
					 curNode, 
					 ctxt, 
					 true, 
					 curLbl, 
					 nextLbl,
					 endLabel);
	WN_INSERT_BlockLast(blkWN, stmts);
      }
      else if (XAIF_BBElemFilter::IsBBBranch(bbElem)) {
	// ---------------------------------------------------
	// A branch with possibly unstructured control flow
	// ---------------------------------------------------
	unsigned int numOutEdges = curNode->num_outgoing();

	// 1. Translate condition expression
	xercesc::DOMElement* cond = 
	  GetChildElement(bbElem, XAIFStrings.elem_Condition_x());
	xercesc::DOMElement* condexpr = GetFirstChildElement(cond);      
	WN* condWN = XlateExpression::translateExpression(condexpr, ctxt);
	if (numOutEdges == 2) {
	  // Because branches are 'structured switches', ensure we have
	  // a boolean expression for an 'if'.
	  condWN = CreateIfCondition(condWN);
	}
      
	// 2. Gather all outgoing edges, sorted by condition (specially
	// sort two-way branches into true-false order.)
	OA::OA_ptr<MyDGEdge> tmp; tmp = NULL;
	vector<OA::OA_ptr<MyDGEdge> > outedges(numOutEdges, tmp);
	OA::OA_ptr<OA::DGraph::EdgesIteratorInterface> it
	  = curNode->getOutgoingEdgesIterator();
	for (int i = 0; it->isValid(); ++(*it), ++i) {
	  OA::OA_ptr<OA::DGraph::EdgeInterface> etmp = it->current();
	  outedges[i] = etmp.convert<MyDGEdge>();
	}
	std::sort(outedges.begin(), outedges.end(), 
		  sort_CondVal((numOutEdges != 2)));

	// 3. Create branch control flow
	WN* lblWN = WN_CreateLabel(curLbl, 0 /*label_flag*/, NULL);
	WN_INSERT_BlockLast(blkWN, lblWN);
	if (numOutEdges == 2) {
	  // Create GOTOs for each child block
	  vector<WN*> childblksWN(numOutEdges, NULL);
	  for (unsigned i = 0; i < outedges.size(); ++i) {
	    OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = outedges[i]->getSink();
	    OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
	    WN* gotoblkWN = WN_CreateBlock();
	    WN* gotoWN = WN_CreateGoto(nodeToLblMap[n]);
	    WN_INSERT_BlockFirst(gotoblkWN, gotoWN);
	    childblksWN[i] = gotoblkWN;
	  }
	
	  // Create IF with GOTOs
	  WN* ifWN = WN_CreateIf(condWN, childblksWN[0], childblksWN[1]);
	  WN_INSERT_BlockLast(blkWN, ifWN);
	} 
	else {
	  unsigned lastLbl = 0; // do not know last label
	
	  // Create SWITCH with CASEGOTOs
	  WN* switchWN = xlate_CFG_BranchMulti(curNode, condWN, lastLbl,
					       outedges, nodeToLblMap);
	  WN_INSERT_BlockLast(blkWN, switchWN);
	}
      }
      else if (XAIF_BBElemFilter::IsBBEndBr(bbElem)) {
	// ---------------------------------------------------
	// EndBranch: a dummy basic block
	// ---------------------------------------------------
	OA::OA_ptr<MyDGNode> nextNode = GetSuccessor(curNode); // at most one outgoing edge
	unsigned nextLbl = (!nextNode.ptrEqual(NULL)) ? nodeToLblMap[nextNode] : 0;
	WN* stmts = xlate_CFG_BasicBlock(wn_pu, 
					 curNode, 
					 ctxt, 
					 true, 
					 curLbl, 
					 nextLbl,
					 endLabel);
	WN_INSERT_BlockLast(blkWN, stmts);
      }
      else if (XAIF_BBElemFilter::IsBBForLoop(bbElem) ||
	       XAIF_BBElemFilter::IsBBPreLoop(bbElem) ||
	       XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
	// ---------------------------------------------------
	// A loop with possibly unstructured control flow
	// ---------------------------------------------------
      
	// XAIF Loop sub-graphs
	// --------------------
	//   ForLoop ---> [loopbody nodes...] ---> EndLoop--| 
	//     Init  \                                   <--| (backedge)
	//     Cond   \
	  //     Update  \------------------------------------> fallthru block
	//             
	//   PreLoop and PostLoop are the same, but without special Init
	//   and Update statements.
	// 
	// Translation into unstructured WHIRL:
	// ForLoop               PreLoop               PostLoop
	// ----------------------------------------------------------------
	// label for_loop
	// Init
	// goto for_test                               
	// label for_cntnue                            label post_loop
	// Update                                      goto loop_body
	// label for_test        label pre_loop        label post_cntnue
	// if (Cond)             if (Cond)             if (Cond)
	//   goto loop_body        goto loop_body        goto loop_body
	// else                  else                  else
	//   goto fallthru_blk     goto fallthru_blk     goto fallthru_blk
	// 
	// ----------------------------------------------------------------
	//  [fallthru subgraph]  label loop_body
	//                       ... 
	//                       goto end_loop
	//
	//            [EndLoop]  label end_loop
	//                       goto for_cntnue/pre_loop/post_cntnue
	//
	// Note: Moving Init and Update statments out of the 'loop
	// scope' is not a problem -- i.e. there won't be symbol clashes
	// -- because in WHIRL the whole procedure is actually in the same
	// lexical scope.
      
	bool isDoLoop = (XAIF_BBElemFilter::IsBBForLoop(bbElem));
      
	// 1. Gather children
	OA::OA_ptr<MyDGNode> bodyNode = GetSuccessorAlongEdge(curNode, 1);
	OA::OA_ptr<MyDGNode> fallthruNode = GetSuccessorAlongEdge(curNode, 0);
      
	// 2. Translate condition expression (and update/init statements)
	xercesc::DOMElement* cond = 
	  GetChildElement(bbElem, XAIFStrings.elem_Condition_x());
	xercesc::DOMElement* condexpr = GetFirstChildElement(cond);
	WN* condWN = NULL;
	if (isDoLoop) {
	  condWN = XlateExpression::translateExpressionSimple(condexpr, ctxt);
	} else {
	  condWN = XlateExpression::translateExpression(condexpr, ctxt);
	}
      
	xercesc::DOMElement *init = NULL, *update = NULL;
	WN *initWN = NULL, *updateWN = NULL;
	if (XAIF_BBElemFilter::IsBBForLoop(bbElem)) {
	  // Note: initWN and updateWN are STIDs
	  const XMLCh* lineNumberX = bbElem->getAttribute(XAIFStrings.attr_lineNumber_x());
	  XercesStrX lineNumber = XercesStrX(lineNumberX);
	  FORTTK_MSG(2, "doing loop with line number attribute: " << lineNumber.c_str())	 
	  init = GetChildElement(bbElem, XAIFStrings.elem_LpInit_x());
	  update = GetChildElement(bbElem, XAIFStrings.elem_LpUpdate_x());
	  initWN = XlateStmt::translateStmt(init, ctxt);
	  updateWN = XlateStmt::translateStmt(update, ctxt);
	}
      
	// 3. Create loop control flow
	// Create loop label
	WN* lblWN = WN_CreateLabel(curLbl, 0 /*label_flag*/, NULL);
	WN_INSERT_BlockLast(blkWN, lblWN);
      
	// Create other special pre-loop statements
	WN* stmtWN = NULL;
	if (isDoLoop) {
	  INT32 lbl_test = startLabel_r++;
	  INT32 lbl_cntnue = nodeToLoopContLblMap[curNode];
        
	  WN_INSERT_BlockLast(blkWN, initWN); // Init
	  WN* gotoWN = WN_CreateGoto(lbl_test);
	  WN_INSERT_BlockLast(blkWN, gotoWN);
	  WN* lbl1WN = WN_CreateLabel(lbl_cntnue, 0 /*label_flag*/, NULL);
	  WN_INSERT_BlockLast(blkWN, lbl1WN);
	  WN_INSERT_BlockLast(blkWN, updateWN); // Update
	  WN* lbl2WN = WN_CreateLabel(lbl_test, 0 /*label_flag*/, NULL);
	  WN_INSERT_BlockLast(blkWN, lbl2WN);
	}
	else if (XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
	  INT32 lbl_cntnue = nodeToLoopContLblMap[curNode];

	  WN* gotoWN = WN_CreateGoto(nodeToLblMap[bodyNode]);
	  WN_INSERT_BlockLast(blkWN, gotoWN);
	  WN* lblWN = WN_CreateLabel(lbl_cntnue, 0 /*label_flag*/, NULL);
	  WN_INSERT_BlockLast(blkWN, lblWN);
	}
      
	// Create 'if (Cond)'
	WN* thenblkWN = WN_CreateBlock();
	WN* elseblkWN = WN_CreateBlock();
	WN* thenWN = WN_CreateGoto(nodeToLblMap[bodyNode]);
	WN* elseWN = WN_CreateGoto(nodeToLblMap[fallthruNode]);
	WN_INSERT_BlockFirst(thenblkWN, thenWN);
	WN_INSERT_BlockFirst(elseblkWN, elseWN);
      
	WN* ifWN = WN_CreateIf(condWN, thenblkWN, elseblkWN);
	WN_INSERT_BlockLast(blkWN, ifWN);
      }
      else if (XAIF_BBElemFilter::IsBBEndLoop(bbElem)) {
	// ---------------------------------------------------
	// The loop back-branch: loop back to continue branch!
	// ---------------------------------------------------
	OA::OA_ptr<MyDGNode> nextNode = GetSuccessor(curNode); // at most one outgoing edge
	unsigned nextLbl = (!nextNode.ptrEqual(NULL)) ? nodeToLoopContLblMap[nextNode] : 0;
	WN* stmts = xlate_CFG_BasicBlock(wn_pu, 
					 curNode, 
					 ctxt, 
					 true, 
					 curLbl, 
					 nextLbl,
					 endLabel);
	WN_INSERT_BlockLast(blkWN, stmts);
      }
      else {
	FORTTK_DIE("Unknown XAIF basic block:\n" << *bbElem);
      }
    }
    WN* lblWN = WN_CreateLabel(endLabel, 0 /*label_flag*/, NULL);
    WN_INSERT_BlockLast(blkWN, lblWN);
    return blkWN;
  }  


  // xlate_CFG_BasicBlock: Translate a non-control-flow basic block.
  // Optionally skips GOTOs and LABELs within 'wn_pu'.
  // Optionally adds a label at the beginning of the block and a
  // 'fallthru-goto' at the end if non-zero labels are provided.
  static WN*
  xlate_CFG_BasicBlock(WN *wn_pu, OA::OA_ptr<MyDGNode> curBB, 
		       PUXlationContext& ctxt, 
		       bool skipMarkeredGotoAndLabels, 
		       unsigned newCurBBLbl, 
		       unsigned newNextBBLbl,
		       unsigned endLabel)
  {
    xercesc::DOMElement* bbElem = curBB->GetElem();

    // typically addNewGotoAndLabels is only true when
    // skipOldGotoAndLabels is also true
    bool skipOldGotoAndLabels = skipMarkeredGotoAndLabels;
    bool addNewGotoAndLabels = (newCurBBLbl != 0); 
  
    // 1. Translate (if we add our own goto's and labels, then we need
    // to throw away any original goto and label at the end and
    // beginning of the block)
    WN* stmtblk = TranslateBasicBlock(wn_pu, bbElem, ctxt, skipOldGotoAndLabels,endLabel);
  
    // 2. If necessary, add a label to front and goto at end
    if (addNewGotoAndLabels) {
      WN* lblWN = WN_CreateLabel(newCurBBLbl, 0 /*label_flag*/, NULL);
      WN_INSERT_BlockFirst(stmtblk, lblWN);
    
      if (newNextBBLbl != 0) {
	WN* gotoWN = WN_CreateGoto(newNextBBLbl);
	WN_INSERT_BlockLast(stmtblk, gotoWN); 
      }
    }
  
    return stmtblk;
  }


  // xlate_CFG_BranchMulti: abstract translation of multi-way branches
  static WN*
  xlate_CFG_BranchMulti(OA::OA_ptr<MyDGNode> curNode, WN* condWN, 
			unsigned lastLbl,
			vector<OA::OA_ptr<MyDGEdge> >& outedges,
			map<OA::OA_ptr<MyDGNode>, unsigned>& nodeToLblMap)
  {
    // Case values are in ascending order; the default case (if any)
    // will be at the beginning and have a false condition attribute
  
    // Create default goto if necessary
    WN* defltWN = NULL;
    int defltIdx = -1;
    if (!GetHasConditionAttr(outedges[0]->GetElem())) {
      defltIdx = 0;
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = outedges[0]->getSink();
      OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
      unsigned gotolbl = nodeToLblMap[n];
      defltWN = WN_CreateGoto(gotolbl);
    }
  
    // Create casegoto for each block
    WN* casegotoBlkWN = WN_CreateBlock();
    int numcases = outedges.size() - (defltIdx + 1);
    for (unsigned i = defltIdx + 1; i < outedges.size(); ++i) {
      xercesc::DOMElement* elemEdge = outedges[i]->GetElem();
      OA::OA_ptr<OA::DGraph::NodeInterface> ntmp = outedges[i]->getSink();
      OA::OA_ptr<MyDGNode> n = ntmp.convert<MyDGNode>();
    
      INT64 caseval = GetCondAttr(elemEdge);
      WN* wn = WN_CreateCasegoto(caseval, nodeToLblMap[n]);
      WN_INSERT_BlockLast(casegotoBlkWN, wn);
    }
  
    // Create switch
    WN* switchWN = WN_CreateSwitch(numcases, condWN, casegotoBlkWN,
				   defltWN, lastLbl);
  
    return switchWN;
  }


  // TranslateBasicBlock: Translate a non-control-flow basic block
  static WN*
  TranslateBasicBlock(WN *wn_pu, 
		      const xercesc::DOMElement* bbElem, 
		      PUXlationContext& ctxt,
		      bool skipMarkeredGotoAndLabels,
		      unsigned endLabel)
  {
    WN* blkWN = WN_CreateBlock();

    // -------------------------------------------------------
    // 1. Find some info now to prevent several recalculations
    // -------------------------------------------------------
    // FIXME: use parent map -- w2x does not need to generate this id list 
    fortTkSupport::IdList<fortTkSupport::WNId>* idlist = GetWNIdList(bbElem); // FIXME
    WN* origblkWN = FindWNBlock(bbElem, idlist, ctxt);
    if (idlist->size() > 0) { 
      FORTTK_ASSERT(origblkWN, "Could not find WHIRL block for:\n" << *bbElem);
    }

    // -------------------------------------------------------
    // 2. Translate statements
    // -------------------------------------------------------
    XAIF_BBStmtElemFilter filt;
    for (xercesc::DOMElement* stmt = GetChildElement(bbElem, &filt);
	 (stmt); stmt = GetNextSiblingElement(stmt, &filt)) {
      WN* wn = NULL;
      if (XAIF_BBStmtElemFilter::IsMarker(stmt)) {
	bool isGotoOrLabel = (IsTagPresent(stmt, XAIFStrings.tag_StmtGoto()) ||
			      IsTagPresent(stmt, XAIFStrings.tag_StmtLabel()));
	bool skip = (isGotoOrLabel && skipMarkeredGotoAndLabels);
	if (!skip) {
	  if (IsTagPresent(stmt, XAIFStrings.tag_StmtReturn())) { 
	    // replace return with goto endlabel
	    wn = WN_CreateGoto(endLabel);
	  }
	  else { 
	    fortTkSupport::WNId id = GetWNId(stmt);
	    WN* foundWN = ctxt.findWN(id, true /* mustFind */);
	    wn = WN_COPY_Tree(foundWN);
	    XlateStmt::patchWNStmt(wn, ctxt); // FIXME
	  }
	}
      }
      else {
	wn = XlateStmt::translateStmt(stmt, ctxt);
      }
      if (wn) {
	WN_INSERT_BlockLast(blkWN, wn);
      }
    }
    return blkWN;
  }


  // ****************************************************************************
  // ControlFlowGraph -- basic block patching algorithm
  // ****************************************************************************

  static void
  TranslateBB_OLD(WN *wn_pu, const xercesc::DOMElement* bbElem, PUXlationContext& ctxt)
  {
    if (XAIF_BBElemFilter::IsBB(bbElem)) {
      xlate_BasicBlock_OLD(wn_pu, bbElem, ctxt);
    } 
    else if (XAIF_BBElemFilter::IsBBBranch(bbElem)
	     || XAIF_BBElemFilter::IsBBPreLoop(bbElem)
	     || XAIF_BBElemFilter::IsBBPostLoop(bbElem)) {
      xlate_BBCond_OLD(wn_pu, bbElem, ctxt);
    } 
    else if (XAIF_BBElemFilter::IsBBForLoop(bbElem)) {
      // FIXME: what to do with ForLoops?
    } 
    else {
      // skip anything else for now
    }
  }


  static void
  xlate_BasicBlock_OLD(WN *wn_pu, const xercesc::DOMElement* bbElem, PUXlationContext& ctxt)
  {
    // -------------------------------------------------------
    // 1. Find some info now to prevent several recalculations
    // -------------------------------------------------------
    fortTkSupport::IdList<fortTkSupport::WNId>* idlist = GetWNIdList(bbElem);
    WN* blkWN = FindWNBlock(bbElem, idlist, ctxt);
    if (idlist->size() > 0) { 
      FORTTK_ASSERT(blkWN, "Could not find WHIRL block for:\n" << *bbElem);
    }
  
    // -------------------------------------------------------
    // 2. Translate statements
    // -------------------------------------------------------  
    xercesc::DOMElement* begXAIF = NULL, *endXAIF = NULL;
    WN* begWN = NULL, *endWN = NULL;
    while (FindNextStmtInterval(bbElem, idlist, ctxt.getWNIdToWNMap(), blkWN,
				begXAIF, endXAIF, begWN, endWN)) {
    
      // We now have two non-NULL intervals.  [begWN, endWN] represents
      // the WHIRL statements that will be replaced with the XAIF
      // statements [begXAIF, endXAIF]
    
      // 1. Find (or create) a statement just prior to the interval to
      // serve as an insertion point.
      WN* ipWN = FindSafeInsertionPoint(blkWN, begWN);

      // 2. Delete all WHIRL statements within [begWN, endWN]
      WN* it1End = WN_next(endWN); // result may be NULL
      for (WN* wn = begWN; (wn != it1End); wn = WN_next(wn)) {
	// Remove from persistent id maps (to assist debugging)
	RemoveFromWhirlIdMaps(wn, ctxt.getWNToWNIdMap(), ctxt.getWNIdToWNMap());
	WN_DELETE_FromBlock(blkWN, wn);
      }
    
      // 3. For each new XAIF statement within [begXAIF, endXAIF],
      // create a WHIRL node and insert it
      xercesc::DOMElement* it2End = GetNextSiblingElement(endXAIF); // result may be NULL
      for (xercesc::DOMElement* stmt = begXAIF; (stmt != it2End); 
	   stmt = GetNextSiblingElement(stmt)) {
      
	WN* wn = XlateStmt::translateStmt(stmt, ctxt);
	if (!wn) { continue; }

	// Find the soon-to-be new insertion point
	WN* newIP = (WN_operator(wn) == OPR_BLOCK) ? WN_last(wn) : wn;

	// If 'wn' is a OPR_BLOCK, the block is automatically deleted
	WN_INSERT_BlockAfter(blkWN, ipWN, wn); 
	ipWN = newIP; // update the new insertion point
      }
    }

    // -------------------------------------------------------
    // 3. Patch certain statements represented by xaif:Markers
    // -------------------------------------------------------
    for (xercesc::DOMElement* stmt = GetFirstChildElement(bbElem); (stmt); 
	 stmt = GetNextSiblingElement(stmt, XAIFStrings.elem_Marker_x())) {
      fortTkSupport::WNId id = GetWNId(stmt);
      if (id != 0) {
	WN* wn = ctxt.findWN(id, true /* mustFind */);
	XlateStmt::patchWNStmt(wn, ctxt);
      }
    }
  
    // -------------------------------------------------------
    // 4. Cleanup
    // -------------------------------------------------------
    delete idlist;
  }


  static void
  xlate_BBCond_OLD(WN* wn_pu, const xercesc::DOMElement* bbElem, PUXlationContext& ctxt)
  {
    // -------------------------------------------------------
    // 1. Find corresponding WHIRL condition node
    // -------------------------------------------------------
    // Conveniently, XAIF 'if' or 'loop' condition is represented by the
    // WHIRL structured control flow node, i.e. the corresponding WHIRL
    // 'if' or 'loop'.
    fortTkSupport::IdList<fortTkSupport::WNId>* idlist = GetWNIdList(bbElem);

    xercesc::DOMElement* cond = GetChildElement(bbElem, XAIFStrings.elem_Condition_x());
    if (cond) {
      FORTTK_ASSERT(idlist->size() == 1, "Invalid id list:\n" << *cond);
    }

    WN* wn = ctxt.findWN(idlist->front(), true /* mustFind */);

    INT condKid = 0;
    OPERATOR opr = WN_operator(wn);
    switch (opr) {
    
    case OPR_DO_WHILE:
    case OPR_WHILE_DO:
      condKid = 0; // WN_kid0(wn) == WN_while_test(wn)
      break;

    case OPR_IF:
    case OPR_TRUEBR:
    case OPR_FALSEBR:
      condKid = 0; // WN_kid0(wn) == WN_if_test(wn)
      break;

    case OPR_SWITCH:
      condKid = -1;
      break; // integer expression
    
    default: 
      FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
    }
  
    // -------------------------------------------------------
    // 2. Ensure the condition expression is patched
    // -------------------------------------------------------
    if (condKid >= 0) {
      XlateExpression::patchWNExpr(wn, condKid /* kidno */, ctxt);
    }
  }


  // FindNextStmtInterval: Finds the next translation interval within
  // the XAIF BB 'bbElem' given the current interval.  The current
  // interval's status is defined by [begXAIF, endXAIF] both of which
  // are NULL when no interval yet exists.  The function finds two new
  // intervals, the XAIF statements [begXAIF, endXAIF] and their
  // corresponding WHIRL statements [begWN, endWN].  Returns true if an
  // interval has been found and the interval boundaries appropriately
  // updated; otherwise, returns false.  Note that in the latter case,
  // original interval boundaries are not necessary preserved.
  //
  // Intervals within the BB are created by the presence of xaif:Marker
  // elements that contain a WhirlId annotation, but xaif:Marker's are
  // not actually within the interval.  If no explicit xaif:Marker
  // begins or ends the BB, its existence is assumed.  Consequently, for
  // non-NULL intervals, 'begXAIF' and 'endXAIF' will never point to an
  // xaif:Marker element with annotation attribute and will never be
  // NULL.
  static bool
  FindNextStmtInterval(const xercesc::DOMElement* bbElem, fortTkSupport::IdList<fortTkSupport::WNId>* bbIdList, 
		       fortTkSupport::WNIdToWNMap* wnmap, WN* blkWN,
		       xercesc::DOMElement* &begXAIF, xercesc::DOMElement* &endXAIF,
		       WN* &begWN, WN* &endWN)
  {
    // 1. Find beginning of the interval
    if (!begXAIF) {
      begXAIF = GetFirstChildElement(bbElem);   // first interval (tmp)
    } 
    else if (endXAIF) {
      begXAIF = GetNextSiblingElement(endXAIF); // successive intervals (tmp)
    } 
    else {
      begXAIF = NULL;                           // no more intervals exist
    }  
  
    // If 'begXAIF' is non-NULL, it points to a temporary beginning
    // point.  From this point, find the first non-xaif:Marker element.
    // This skips over consecutive sequences of xaif:Markers, a
    // necessary step to obtain a correct boundary begin point.
    while (begXAIF) {
      if (XAIF_BBStmtElemFilter::IsMarker(begXAIF) && GetWNId(begXAIF) != 0) {
	begXAIF = GetNextSiblingElement(begXAIF);
      } else {
	break; // not an xaif:Marker with WhirlId annotation!
      }
    } // Note: 'begXAIF' could be NULL now indicating a NULL interval  
    begWN = FindIntervalBoundary(begXAIF, bbIdList, wnmap, blkWN, 0 /* beg */);
  
    // 2. Find ending of the interval
    if (begXAIF) {

      // See if another xaif:Marker exists containing a WhirlId
      // annotation; if not, 'endXAIF' will be NULL.  (Note that we may
      // encounter an xaif:Marker without the annotation.)
      endXAIF = begXAIF; // of course, we start from the beginning!
      while ( (endXAIF = 
	       GetNextSiblingElement(endXAIF, XAIFStrings.elem_Marker_x())) ) {
	if (GetWNId(endXAIF) != 0) {
	  break; // found!
	}
      }
    
      // If 'endXAIF' is non-NULL, it points to the first xaif:Marker
      // after 'begXAIF'.  If it is NULL, we use the very last element
      // (which must be a non-xaif:Marker).
      if (endXAIF) {
	endXAIF = GetPrevSiblingElement(endXAIF);
      } else {
	endXAIF = GetLastChildElement(bbElem);
      }
      endWN = FindIntervalBoundary(endXAIF, bbIdList, wnmap, blkWN, 1 /* end */);
    
    } else {
      endXAIF = NULL;
      endWN = NULL;
    }  
  
    return (begXAIF && begWN);
  }


  // FindIntervalBoundary: Finds the appropriate WN* for the given
  // interval boundary statement 'elem' and boundary type (begin/end).
  // The boundary is assumed to be of the form [beg, end], where beg and
  // end are never xaif:Marker statements.  N.B.: It is assumed that
  // this function is called for the begin interval *before* being
  // called for the end interval.
  //
  // boundary: 0 (begin), 1 (end)
  //
  // For begin and end boundaries: If 'elem' is non-NULL the
  // corrresponding WN* should never be NULL.  If 'elem' is NULL, the
  // interval is NULL.
  static WN*
  FindIntervalBoundary(const xercesc::DOMElement* elem, fortTkSupport::IdList<fortTkSupport::WNId>* bbIdList, 
		       fortTkSupport::WNIdToWNMap* wnmap, WN* blkWN, int boundary)
  {
    if (!elem) {
      return NULL;
    }

    WN* wn = NULL;
    if (boundary == 0) {
      // For begin boundaries: If the previous element is an xaif:Marker
      // with WhirlId annotation, use it to find the WN*; otherwise try
      // to use 'bbIdList' to return the first WN* in the list.
      xercesc::DOMElement* adj = GetPrevSiblingElement(elem);
      if (adj && XAIF_BBStmtElemFilter::IsMarker(adj)) {
	fortTkSupport::WNId id = GetWNId(adj);
	if (id != 0) {
	  wn = wnmap->Find(id, true /* mustFind */);

	  // We used 'adj' (instead of 'elem') to find 'wn'.  Correct
	  // the interval boundary by moving in the opposite direction.
	  WN* nextWN = WN_next(wn); // Result may be NULL! (see above)

	  if (nextWN) {
	    wn = nextWN;
	  } else {
	    // The interval corresponding to 'elem' is the NULL interval
	    // after 'wn'.  We must create a dummy WN* to represent it
	    // with [beg, end) notation.
	    WN* newWN = WN_CreateAssert(0, WN_CreateIntconst(OPC_I4INTCONST, 
							     (INT64)1));
	    WN_INSERT_BlockAfter(blkWN, wn, newWN);
	    wn = newWN; // set 'wn' to the new node
	  }
	}
      }
      if (!wn && bbIdList->size() > 0) {
	wn = wnmap->Find(bbIdList->front(), true /* mustFind */);
      }
    } 
    else if (boundary == 1) {
      // For end boundaries: If the next element is an xaif:Marker
      // with WhirlId annotation, use it to find the WN*; otherwise try
      // to use 'bbIdList' to return the last WN* in the list.
      xercesc::DOMElement* adj = GetNextSiblingElement(elem);
      if (adj && XAIF_BBStmtElemFilter::IsMarker(adj)) {
	fortTkSupport::WNId id = GetWNId(adj);
	if (id != 0) {
	  wn = wnmap->Find(id, true /* mustFind */);

	  // We used 'adj' (instead of 'elem') to find 'wn'.  Correct
	  // the interval boundary by moving in the opposite direction.
	  WN* prevWN = WN_prev(wn); // never NULL b/c of insertion above!
	  FORTTK_ASSERT(prevWN, "Internal error");

	  wn = prevWN;
	}
      }
      if (!wn && bbIdList->size() > 0) {
	wn = wnmap->Find(bbIdList->back(), true /* mustFind */);
      }
    } 
    else {
      FORTTK_DIE("Internal error.");
    }
  
    return wn;
  }


  // FindWNBlock: Given an XAIF basic block element, find the
  // corresponding WHIRL block.
  static WN* 
  FindWNBlock(const xercesc::DOMElement* bbElem, fortTkSupport::IdList<fortTkSupport::WNId>* idlist, 
	      PUXlationContext& ctxt)
  {
    // We pass 'idlist' to avoid continual reparsing
    WN* wn = NULL;
    if (idlist->size() > 0) {
      fortTkSupport::WNId id = idlist->front();
      wn = ctxt.findWN(id, true /* mustFind */);
    }
  
    WN* blk = NULL;
    if (wn) {
      blk = ctxt.findParentBlockWN(wn);
    }
    return blk;
  }


  // FindSafeInsertionPoint: Given a WHIRL statement node 'stmtWN' and
  // its containing block 'blckWN', find (or create) the statement just
  // prior to 'stmtWN'.
  static WN* 
  FindSafeInsertionPoint(WN* blckWN, WN* stmtWN)
  {
    WN* ipWN = NULL;

    // 1. Just return the previous statement, if available
    if ( (ipWN = WN_prev(stmtWN)) != NULL ) {
      return ipWN;
    }

    // 2. There is no previous statement so we insert a dummy stmt to
    // serve as a handle.  whirl2f should ignore this.  (If not, a
    // compiler will be able to optimize this away.)
    ipWN = WN_CreateAssert(0, WN_CreateIntconst(OPC_I4INTCONST, (INT64)1));
    WN_INSERT_BlockBefore(blckWN, stmtWN, ipWN);
    return ipWN;
  }


  // RemoveFromWhirlIdMaps: Remove 'wn' and all of its descendents from
  // the WhirlId maps.
  static void
  RemoveFromWhirlIdMaps(WN* wn, fortTkSupport::WNToWNIdMap* wn2idmap, fortTkSupport::WNIdToWNMap* id2wnmap)
  {
    WN_TREE_CONTAINER<PRE_ORDER> wtree(wn);
    WN_TREE_CONTAINER<PRE_ORDER>::iterator it;
    for (it = wtree.begin(); it != wtree.end(); ++it) {
      WN* curWN = it.Wn();

      fortTkSupport::WNId curId = 0;
      fortTkSupport::WNToWNIdMap::iterator it1 = wn2idmap->find(curWN);
      if (it1 != wn2idmap->end()) {
	curId = (*it1).second;
	wn2idmap->erase(it1);
      }
      id2wnmap->erase(curId);
    }
  }


  // ****************************************************************************
  // Scopes and Symbols
  // ****************************************************************************

  fortTkSupport::Symbol*
  GetSymbol(const xercesc::DOMElement* elem, PUXlationContext& ctxt)
  {
    const XMLCh* scopeIdX = elem->getAttribute(XAIFStrings.attr_scopeId_x());
    const XMLCh* symIdX = elem->getAttribute(XAIFStrings.attr_symId_x());

    XercesStrX scopeId = XercesStrX(scopeIdX);
    XercesStrX symId = XercesStrX(symIdX);
  
    FORTTK_ASSERT(strcmp(scopeId.c_str(), "") != 0 && 
		  strcmp(symId.c_str(), "") != 0,
		  "Invalid id attribute:\n" << *elem);
  
    return ctxt.findSym(scopeId.c_str(), symId.c_str());
  }


  fortTkSupport::Symbol*
  GetOrCreateSymbol(const char* sname, PUXlationContext& ctxt)
  {
    // FIXME: make more general
    bool active = false;
  
    // FIXME: need to associate current PU with a scope id...
    const char* scopeId = "1"; // assume global for now
  
    fortTkSupport::Symbol* sym = ctxt.getXAIFSymToSymbolMap().Find(scopeId, sname);
    if (!sym) {
      // FIXME: use CreateST...
      TY_IDX ty = MTYPE_To_TY(MTYPE_F8);
      SYMTAB_IDX level = GLOBAL_SYMTAB; // FIXME: coordinate with scopeId
      ST* st = New_ST(level);
      ST_Init(st, Save_Str(sname), CLASS_VAR, SCLASS_AUTO, EXPORT_LOCAL, ty);
    
      sym = new fortTkSupport::Symbol(st, 0, active);
      ctxt.getXAIFSymToSymbolMap().Insert(scopeId, sname, sym);
    }
    return sym;
  }


  fortTkSupport::Symbol*
  GetOrCreateBogusTmpSymbol(PUXlationContext& ctxt)
  {
    static const char* sname = "OpenAD_bogus";
    return GetOrCreateSymbol(sname, ctxt);
  }


  // ****************************************************************************

  void
  xlate_Scope(const xercesc::DOMElement* elem,
	      PUXlationContext& ctxt) {
    // Find the corresponding WHIRL symbol table (ST_TAB)
    fortTkSupport::SymTabId symtabId = GetSymTabId(elem);
    pair<ST_TAB*, PU_Info*> stab = ctxt.findSymTab(symtabId);
  
    PU_Info* pu = stab.second;
    if (pu) { // This is a local symbol table; restore it's global state.
      PU_SetGlobalState(pu);
    
      // Need WHIRL<->ID maps for translating ScalarizedRefs
      fortTkSupport::WNIdToWNMap* wnmap = WNIdToWNTableMap.Find(pu);
      ctxt.setWNIdToWNMap(wnmap);
    }
  
    // Find the scope id
    const XMLCh* scopeIdX = elem->getAttribute(XAIFStrings.attr_Vid_x());
    XercesStrX scopeId = XercesStrX(scopeIdX);

    // Translate the xaif:SymbolTable (the only child)
    xercesc::DOMElement* symtabElem = GetFirstChildElement(elem);
    xlate_SymbolTable(symtabElem, scopeId.c_str(), pu, ctxt);
  }  


  static void
  xlate_SymbolTable(const xercesc::DOMElement* elem,
		    const char* scopeId, PU_Info* pu,
		    PUXlationContext& ctxt) {
    // For all xaif:fortTkSupport::Symbol in the xaif:SymbolTable
    XAIF_SymbolElemFilter filt;
    for (xercesc::DOMElement* e = GetChildElement(elem, &filt);
	 (e); e = GetNextSiblingElement(e, &filt)) {
      // do the non-temporary ones first
      xlate_Symbol(e, scopeId, pu, ctxt, false);
    }
    for (xercesc::DOMElement* e = GetChildElement(elem, &filt);
	 (e); e = GetNextSiblingElement(e, &filt)) {
      // now do the temporary ones since in the 
      // subroutine ones we refer to the original 
      // subroutine symbols so we had to translate those
      // first.
      xlate_Symbol(e, scopeId, pu, ctxt, true);
    }
  }


  // xlate_Symbol: Note that symbols can only be in a global or PU
  // scope; IOW, there are no block scopes.
  static void
  xlate_Symbol(const xercesc::DOMElement* elem, 
	       const char* scopeId, 
	       PU_Info* pu, 
	       PUXlationContext& ctxt, 
	       bool doTempSymbols) {
    // at this time do we do temporaries or not?
    if (doTempSymbols != GetBoolAttr(elem, XAIFStrings.attr_temp_x(), false /* default */)) { 
      return;
    }
    // 1. Initialize
    SYMTAB_IDX level = (pu) ? CURRENT_SYMTAB : GLOBAL_SYMTAB;
  
    // For symbols not introduced by xaifBooster, *one* of the following applies
    fortTkSupport::SymId symId = GetSymId(elem); // non-zero for a normal symbol
    fortTkSupport::WNId wnId = GetWNId(elem);    // non-zero for a scalarized symbol
  
    bool normalSym = (wnId == 0); // true if a non-scalarized symbol
    bool active = GetActiveAttr(elem);
  
    const XMLCh* symNmX = elem->getAttribute(XAIFStrings.attr_symId_x());
    XercesStrX symNm = XercesStrX(symNmX);
  
    // 2. Find or Create WHIRL symbol; change type if necessary
    ST* st = NULL;
    if (normalSym) {
      if (symId == 0) {
	// Create the symbol
	st = CreateST(elem, 
		      level, 
		      symNm.c_str(), 
		      ctxt.getXAIFSymToSymbolMap(),
		      scopeId);
	FORTTK_ASSERT(st != 0,
		      "CreateST returned a null pointer!");
      } 
      else {
	// Find the symbol and change type if necessary.  N.B. we skip
	// variables of structured type because they will be handled
	// through the scalarized references.
	st = &(Scope_tab[level].st_tab->Entry(symId));
	if (active && ST_class(st) == CLASS_VAR
	    && (TY_kind(ST_type(st)) != KIND_STRUCT)) {
	  ConvertToActiveType(st);
	}
      }
    } 
    else {
      // scalarized symbol
      FORTTK_ASSERT(level == CURRENT_SYMTAB,
		    "Scalarized symbols must be in a PU-scoped symbol table!");
      if (active) {
	WN* pathVorlage = ctxt.findWN(wnId, true /* mustFind */);
	ConvertScalarizedRefToActiveType(pathVorlage);
      }
    }
  
    // 3. Create our own symbol structure and add to the map
    fortTkSupport::Symbol* sym = new fortTkSupport::Symbol(st, wnId, active);
    ctxt.getXAIFSymToSymbolMap().Insert(scopeId, symNm.c_str(), sym);
  } 


  // ****************************************************************************
  // Attribute retrieval and 'annotation' attribute functions
  // ****************************************************************************

  bool
  GetBoolAttr(const xercesc::DOMElement* elem, XMLCh* attr, bool default_val)
  {
    const XMLCh* aX = elem->getAttribute(attr);
    XercesStrX a = XercesStrX(aX);
  
    // boolean values can be true/false or 1/0
    bool a_bool = default_val;
    if (strlen(a.c_str()) > 0) { // if attribute exists
      if (a.c_str()[0] == '0' || (strcmp(a.c_str(), "false") == 0)) {
	a_bool = false;
      } else {
	a_bool = true;
      }
    }
    return a_bool;
  }


  int
  GetIntAttr(const xercesc::DOMElement* elem, XMLCh* attr, int default_val)
  {
    const XMLCh* aX = elem->getAttribute(attr);
    XercesStrX a = XercesStrX(aX);

    int a_int = default_val;
    if (strlen(a.c_str()) > 0) { // if attribute exists
      a_int = strtol(a.c_str(), (char **)NULL, 10);
    }
    return a_int;
  }


  bool
  GetHasConditionAttr(const xercesc::DOMElement* elem)
  {
    return GetBoolAttr(elem, XAIFStrings.attr_hasCondval_x(), false /*default*/);
  }


  unsigned int
  GetCondAttr(const xercesc::DOMElement* elem)
  {
    unsigned int val = 0;
    if (GetHasConditionAttr(elem)) {
      val = GetIntAttr(elem, XAIFStrings.attr_condval_x(), 0 /* default */);
    }
    return val;
  }


  bool
  GetActiveAttr(const xercesc::DOMElement* elem)
  {
    return GetBoolAttr(elem, XAIFStrings.attr_active_x(), true /* default */);
  }


  bool
  GetDerivAttr(const xercesc::DOMElement* elem)
  {
    return GetBoolAttr(elem, XAIFStrings.attr_deriv_x(), false /* default */);
  }


  unsigned int
  GetPositionAttr(const xercesc::DOMElement* elem)
  {
    return GetIntAttr(elem, XAIFStrings.attr_position_x(), 0 /* default */);
  }


  bool
  IsTagPresent(const xercesc::DOMElement* elem, const char* tag)
  {
    const XMLCh* annot = (elem) ? elem->getAttribute(XAIFStrings.attr_annot_x())
      : NULL;
    XercesStrX annotStr = XercesStrX(annot);
    return IsTagPresent(annotStr.c_str(), tag);
  }


  bool
  IsTagPresent(const char* annotstr, const char* tag)
  {
    return (strstr(annotstr, tag) != NULL);
  }


  fortTkSupport::SymTabId GetSymTabId(const xercesc::DOMElement* elem) {
    return GetId<fortTkSupport::SymTabId>(elem, XAIFStrings.tag_SymTabId());
  }


  fortTkSupport::SymId GetSymId(const xercesc::DOMElement* elem) {
    return GetId<fortTkSupport::SymId>(elem, XAIFStrings.tag_SymId());
  }


  fortTkSupport::PUId GetPUId(const xercesc::DOMElement* elem) {
    return GetId<fortTkSupport::PUId>(elem, XAIFStrings.tag_PUId());
  }


  fortTkSupport::WNId GetWNId(const xercesc::DOMElement* elem) {
    return GetId<fortTkSupport::WNId>(elem, XAIFStrings.tag_WHIRLId());
  }


  fortTkSupport::IdList<fortTkSupport::WNId>*
  GetWNIdList(const xercesc::DOMElement* elem)
  {
    return GetIdList<fortTkSupport::WNId>(elem, XAIFStrings.tag_WHIRLId());
  }


  std::string
  GetIntrinsicKey(const xercesc::DOMElement* elem)
  {
    const XMLCh* annot = (elem) ? elem->getAttribute(XAIFStrings.attr_annot_x()) : NULL;
    XercesStrX annotStr_x = XercesStrX(annot);
    const char* annotStr = annotStr_x.c_str();
    std::string key;
    char *start = NULL, *end = NULL;
    start = strstr(const_cast<char*>(annotStr), XAIFStrings.tag_IntrinsicKey());
    if (start) {
      start = start + strlen(XAIFStrings.tag_IntrinsicKey());
      end = strstr(start, XAIFStrings.tag_End());
    }
    if (start && end) {
      for (char* p = start; p < end; ++p) { key += *p; }
    }
      return key;
  }


  PREG_IDX
  GetPregId(const xercesc::DOMElement* elem)
  {
    return GetId<PREG_IDX>(elem, XAIFStrings.tag_PregId());
  }


  // GetId, GetIdList: <see header>
  template <class T>
  T
  GetId(const xercesc::DOMElement* elem, const char* tag)
  {
    const XMLCh* annot = (elem) ? elem->getAttribute(XAIFStrings.attr_annot_x())
      : NULL;
    XercesStrX annotStr = XercesStrX(annot);
    T id = GetId<T>(annotStr.c_str(), tag);
    return id;
  }


  template <class T>
  fortTkSupport::IdList<T>*
  GetIdList(const xercesc::DOMElement* elem, const char* tag)
  {
    const XMLCh* annot = (elem) ? elem->getAttribute(XAIFStrings.attr_annot_x())
      : NULL;
    XercesStrX annotStr = XercesStrX(annot);
    fortTkSupport::IdList<T>* idlist = GetIdList<T>(annotStr.c_str(), tag);
    return idlist;
  }


  // GetId, GetIdList: <see header>
  template <class T>
  T
  GetId(const char* idstr, const char* tag)
  {
    T id = 0;
    if (!idstr) { return id; }

    // Find the tag indicating presence of id
    const char* start = strstr(idstr, tag);
    if (!start) { return id; }
    start += strlen(tag); // move pointer past tag
  
    char* endptr = NULL;
    id = strtol(start, &endptr, 10);

    unsigned int len = strlen(XAIFStrings.tag_End());
    FORTTK_ASSERT(endptr && strncmp(endptr, XAIFStrings.tag_End(), len) == 0,
		  "Could not find '" << tag << "' within " << idstr);
    return id;
  }


  template <class T>
  fortTkSupport::IdList<T>*
  GetIdList(const char* idstr, const char* tag)
  {
    fortTkSupport::IdList<T>* idlist = new fortTkSupport::IdList<T>;

    if (!idstr) { return idlist; }
  
    // Find the tag indicating presence of list
    const char* start = strstr(idstr, tag);
    if (!start) { return idlist; }
    start += strlen(tag); // move pointer past tag
  
    // Parse the colon separated id list.  The list is ended by
    // XAIFStrings.tag_End()
    char* tok = strtok(const_cast<char*>(start), ":");
    while (tok != NULL) {
    
      char* endptr = NULL;
      T id = strtol(tok, &endptr, 10);
      if (endptr != tok) { 
	FORTTK_ASSERT(id != 0, "Found invalid " << tag << " id " << id 
		      << " within " << idstr);
	idlist->push_back(id); // we found some digits to convert
      }

      tok = strtok((char*)NULL, ":");
      if (endptr && strcmp(endptr, XAIFStrings.tag_End()) == 0) {
	// we should be done with iteration now
	FORTTK_ASSERT(tok == NULL, "Could not find end of " << tag 
		      << " within " << idstr);
      }
    }

    return idlist;
  }


  // ****************************************************************************
  // WHIRL Creation functions
  // ****************************************************************************

  WN*
  CreateCallToIntrin(TYPE_ID rtype, const char* fname, unsigned int argc)
  {
    // cf. WN* cwh_intrin_build(...)
    // cf. WN* Gen_Call_Shell(...) in be/com/wn_instrument.cxx
  
    TY_IDX ty = Make_Function_Type(MTYPE_To_TY(rtype));
    ST* st = Gen_Intrinsic_Function(ty, fname); // create if non-existant
  
    WN* callWN = WN_Call(rtype, MTYPE_V, argc, st);
    WN_Set_Call_Default_Flags(callWN); // set conservative assumptions
  
    return callWN;
  }


  WN*
  CreateCallToIntrin(TYPE_ID rtype, const char* fname, std::vector<WN*>& args)
  {
    unsigned int numiArgs = 0; // implicit args if any
    for (unsigned int i = 0; i < args.size(); ++i) {
      if (args[i]) { 
	TY_IDX ty = WN_Tree_Type(args[i]);
	if (TY_Is_Character_Reference(ty) || TY_Is_Chararray_Reference(ty)) {
	  numiArgs++;
	}
      }
    }
    WN* callWN = CreateCallToIntrin(rtype, fname, args.size()+numiArgs);
    for (unsigned int i = 0; i < args.size(); ++i) {
      if (args[i]) { 
	// conservatively assume pass by reference
	WN_actual(callWN, i) = CreateParm(args[i], WN_PARM_BY_REFERENCE);
	TY_IDX ty = WN_Tree_Type(args[i]);
	if (TY_Is_Character_Reference(ty) || TY_Is_Chararray_Reference(ty)) {
	  numiArgs++;
	}
      }
    }
    if (WN_intrinsic(callWN)==INTRN_SCAN) { 
      for (unsigned i = args.size(); i < args.size()+numiArgs; ++i) {
	// Create bogus values, knowing that we only want to unparse the WHIRL
	WN_actual(callWN, i) = CreateParm(WN_CreateIntconst(OPC_I4INTCONST, 0),WN_PARM_BY_VALUE); // a white lie
      }
    }
    return callWN;
  }


  WN*
  CreateIntrinsicCall(OPERATOR opr, INTRINSIC intrn, 
		      TYPE_ID rtype, TYPE_ID dtype, std::vector<WN*>& args)
  {
    // Collect arguments into a temporary array for WN_Create_Intrinsic().
    WN** kids = new WN*[args.size()];
    for (unsigned int i = 0; i < args.size(); ++i) {
      kids[i] = args[i];
    }
  
    WN* wn = WN_Create_Intrinsic(opr, rtype, dtype, intrn, args.size(), kids);
  
    delete[] kids;
    return wn;
  }


  WN* 
  CreateBoolConst(unsigned int val)
  {
    // We use OPR_CONST instead of OPR_INTCONST so that we can set the
    // boolean flag for a TY.  Note, however, that an OPC_??CONST cannot
    // have the boolean rtype.
  
    // Use a boolean mtype for the new ST so that it is safe to set the
    // associated TY's 'is_logical' flag.
    TCON tcon = Host_To_Targ(MTYPE_B, val); // use boolean mtype here
    ST* st = New_Const_Sym(Enter_tcon(tcon), MTYPE_To_TY(TCON_ty(tcon)));
    Set_TY_is_logical(ST_type(st));
    WN* wn = WN_CreateConst(OPC_I4CONST, st);
    return wn;
  }


  static WN*
  CreateOpenADReplacementBeg(const char* placeholder)
  {
    std::string com = "$OpenAD$ BEGIN REPLACEMENT ";
    com += placeholder;
    WN* comWN = WN_CreateComment((char*)com.c_str());
    return comWN;
  }


  static WN*
  CreateOpenADReplacementEnd()
  {
    WN* comWN = WN_CreateComment((char*)"$OpenAD$ END REPLACEMENT");
    return comWN;
  }


  // CreateIfCondition: Convert an expression that is a var-reference to
  // a comparison.  E.g.
  //   if (OpenAD_Symbol_2303) --> if (OpenAD_Symbol_2303 .ne. 0)
  static WN* 
  CreateIfCondition(WN* condWN)
  {
    WN* newcondWN = condWN;
  
    TY_IDX ty = WN_Tree_Type(condWN);
    if (OPERATOR_is_load(WN_operator(condWN)) && !TY_is_logical(ty)) {
      WN* zeroWN = WN_Zerocon(Boolean_type); // CreateBoolConst(0);
      newcondWN = WN_NE(Boolean_type, condWN, zeroWN);
    }
  
    return newcondWN;
  }


  // CreateST: Creates and returns a WHIRL ST* at level 'level' with
  // name 'nm' using 'elem' to gather ST shape and storage class info.
  static ST* 
  CreateST(const xercesc::DOMElement* elem, 
	   SYMTAB_IDX level, 
	   const char* nm,
	   fortTkSupport::XAIFSymToSymbolMap& symMap,
	   const char* scopeId)
  {
    const XMLCh* kindX = elem->getAttribute(XAIFStrings.attr_kind_x());
    const XMLCh* typeX = elem->getAttribute(XAIFStrings.attr_type_x());
    const XMLCh* fetypeX = elem->getAttribute(XAIFStrings.attr_feType_x());
    const XMLCh* shapeX = elem->getAttribute(XAIFStrings.attr_shape_x());    

    XercesStrX kind = XercesStrX(kindX);
    XercesStrX type = XercesStrX(typeX);
    XercesStrX fetype = XercesStrX(fetypeX);
    XercesStrX shape = XercesStrX(shapeX);
  
    bool active = GetActiveAttr(elem);

    bool hasToBeAllocatable=false; // set to true for temp arrays without dimension bounds
  
    // FIXME: assume only
    FORTTK_ASSERT(strcmp(kind.c_str(), "variable") == 0 
		  || 
		  strcmp(kind.c_str(), "subroutine") == 0,
		  fortTkSupport::Diagnostics::Unimplemented << "Can create only symbols that are temporary variables or subroutine names derived from a given subroutine that has the specified prefix prepended");
    TY_IDX ty;

    ST_CLASS symbolClass;

    if (strcmp(kind.c_str(), "variable") == 0) { 
      symbolClass=CLASS_VAR;
      // 1. Find basic type according to 'type' and 'active'
      TY_IDX basicTy = XAIFTyToWHIRLTy(type.c_str(),
				       XAIFFETypeToWHIRLMTy(fetype.c_str()));
      if (active) {
	basicTy = ActiveTypeTyIdx;
      } 
  
      // 2. Modify basic type according to the (non-scalar) shapes
      TY_IDX ty;
      if (strcmp(shape.c_str(), "scalar") == 0) {
	ty = basicTy;
      } 
      else {
	// Note: cf. be/com/wn_instrument.cxx:1253 for example creating vector
	INT32 ndim = 0;
	if (strcmp(shape.c_str(), "vector") == 0) {
	  ndim = 1;
	} 
	else if (strcmp(shape.c_str(), "matrix") == 0) {
	  ndim = 2;
	} 
	else if (strcmp(shape.c_str(), "three_tensor") == 0) {
	  ndim = 3;
	} 
	else if (strcmp(shape.c_str(), "four_tensor") == 0) {
	  ndim = 4;
	} 
	else if (strcmp(shape.c_str(), "five_tensor") == 0) {
	  ndim = 5;
	} 
	else if (strcmp(shape.c_str(), "six_tensor") == 0) {
	  ndim = 6;
	} 
	else {
	  // FIXME: add other tensors
	  FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "Cannot translate variables of shape " << shape.c_str() );
	}
    
	INT64 *lower, *upper;
	lower = new INT64[ndim];
	upper = new INT64[ndim];

	INT32 ndimIndex = 0;
	XAIF_DimensionBoundsElemFilter dbFilt;
	for (xercesc::DOMElement* dbElem = GetChildElement(elem, &dbFilt);
	     (dbElem); 
	     ++ndimIndex,
	       dbElem = GetNextSiblingElement(dbElem, &dbFilt)) {
	  if (ndimIndex==ndim) { 
	    FORTTK_DIE("Cannot have more DimensionBounds than data type allows");
	  }
	  const XMLCh* lowerX = dbElem->getAttribute(XAIFStrings.attr_lower_x());
	  XercesStrX lowerS = XercesStrX(lowerX);
	  lower[ndimIndex]=strtol(lowerS.c_str(), (char **)NULL, 10);
	  const XMLCh* upperX = dbElem->getAttribute(XAIFStrings.attr_upper_x());
	  XercesStrX upperS = XercesStrX(upperX);
	  upper[ndimIndex]=strtol(upperS.c_str(), (char **)NULL, 10);
	}
    
	if (ndimIndex>0 && ndimIndex!=ndim) { 
	  FORTTK_DIE("Variable " << nm << " needs to have all or no DimensionBounds specified (have only " << ndimIndex << " for " << shape.c_str() << ")" );
	}
    
	bool haveDimensionBounds = false;
	if (ndimIndex) {
	  haveDimensionBounds = true;
	}
	else {
	  // if we don't know the dimension somebody has to allocate this 
	  // since assumed shape arrays can otherwise only be formal parameters
	  hasToBeAllocatable = true; 
	}
	ty = MY_Make_Array_Type(basicTy, ndim, haveDimensionBounds,lower,upper);

	delete[] lower;
	delete[] upper;
      }
      // 3. Find storage class and export scope 
      ST_SCLASS sclass = SCLASS_AUTO; // default: auto implies local storage
      ST_EXPORT escope = EXPORT_LOCAL_INTERNAL;
      if (level == GLOBAL_SYMTAB) {
	sclass = SCLASS_COMMON;
	escope = EXPORT_LOCAL;
      }
    
      // 4. Create the new symbol
      ST* st = New_ST(level);
      ST_Init(st, Save_Str(nm), symbolClass, sclass, escope, ty);
      if (hasToBeAllocatable)
	Set_ST_is_allocatable(*st);
    
      // 5. For global symbols, modify and add to a global/common block
      if (level == GLOBAL_SYMTAB) {
	//FIXME ConvertIntoGlobalST(st);
      }
      return st;
    }
    if (strcmp(kind.c_str(), "subroutine") == 0) { 
      // the prefix must be in front of the original name
      // remove the prefix 
      std::string newXAIFName(nm);
      if (newXAIFName.find(PUXlationContext::getPrefix())!=0) {
	FORTTK_DIE("Cannot only copy existing subroutine calls: "
		   << nm 
		   << " does not begin with the required prefix "
		   << PUXlationContext::getPrefix().c_str());
      }
      std::string origXAIFName(newXAIFName.substr(PUXlationContext::getPrefix().size()));
      fortTkSupport::Symbol* origNameSymbol_p = symMap.Find(scopeId, origXAIFName.c_str());
      if (!origNameSymbol_p) {
	FORTTK_DIE("Cannot find "
		   << origXAIFName.c_str()
		   << " in the temporary symbol map");
      }
      // the XAIFName has some _index number appended that we need to loose
      ST* origNameST_p = origNameSymbol_p->GetST();
      if (!origNameST_p) {
	FORTTK_DIE("Cannot find whirl symbol table entry. Can only copy existing subroutine names: "
		   << origXAIFName.c_str()
		   << " does not exist in the internal symbol table");
      }
      std::string origName(ST_name(*origNameST_p));
      ST* newNameST_p=Copy_ST(origNameST_p); // make a copy in the same scope
      // reset the name to the newName
      Set_ST_name_idx (*newNameST_p,Save_Str((PUXlationContext::getPrefix()+origName).c_str()));
      return newNameST_p;
    }
  }


  static ST* 
  ConvertIntoGlobalST(ST* st)
  {
    static ST* OpenADCommonBlockST = NULL;
    static TY_IDX OpenADCommonBlockTY = 0;
    static UINT64 OpenADCommonBlockOffset = 0;
    static FLD_HANDLE OpenADCommonBlockLastField = FLD_HANDLE();
  
    // Create common block ST if necessary
    bool isFirst = false;
    if (!OpenADCommonBlockST) {
      // cf. cwh_stab_common_ST()
      isFirst = true;

      INT64 sz = 0;
      TY& ty = New_TY(OpenADCommonBlockTY); // sets 'OpenADCommonBlockTY'
      TY_Init(ty, sz, KIND_STRUCT, MTYPE_M, Save_Str(".openad.common."));
      // Note: Common block fields are created below
    
      OpenADCommonBlockST = New_ST(GLOBAL_SYMTAB);
      ST_Init(OpenADCommonBlockST, Save_Str("OpenADGlobals"), CLASS_VAR, 
	      SCLASS_COMMON, EXPORT_LOCAL, OpenADCommonBlockTY);
    
      //Set_ST_base(ST& s, *OpenADCommonBlock); // base symbol at the procedure?
      //Set_ST_ofst(ST& s, UINT64 offset);
    }
  
    // Create a new field for common block type
    FLD_HANDLE fld = New_FLD();
    TY_IDX fldTy = ST_type(st);
    FLD_Init(fld, Save_Str(ST_name(st)), fldTy, OpenADCommonBlockOffset);
    if (isFirst) {
      Set_TY_fld(OpenADCommonBlockTY, fld);
    } else {
      Clear_FLD_last_field(OpenADCommonBlockLastField); // fld is now the last
    }
    Set_FLD_last_field(fld);
    OpenADCommonBlockLastField = fld;
  
    // Increase size of common block
    UINT64 sz = TY_size(ST_type(st));
    OpenADCommonBlockOffset += sz;
    Set_TY_size(OpenADCommonBlockTY, sz);
  
    // Modify/Add 'st' to common block
    Set_ST_base(*st, *OpenADCommonBlockST);
    Set_ST_ofst(*st, OpenADCommonBlockOffset);
  
    return st;
  }


  void 
  DeclareActiveTypes()
  {
    // We create pseudo active types aliased to F8
    static char 
      activeTypeName[Args::ourActiveTypeNmLength], 
      activeInitializedTypeName[Args::ourActiveTypeNmLength+5];
    std::string activeInitializedTypeNameStr=Args::ourActiveTypeNm+std::string("_init");
    strncpy(activeTypeName,
	    Args::ourActiveTypeNm.c_str(),
	    Args::ourActiveTypeNmLength);
    strncpy(activeInitializedTypeName,
	    activeInitializedTypeNameStr.c_str(),
	    Args::ourActiveTypeNmLength+4);
    static const char* psTypeNames[] = 
      { activeTypeName, activeInitializedTypeName};
    static unsigned psTypeNamesSZ = sizeof(psTypeNames) / sizeof(char*);
  
    static TY_IDX* psTyIdx[] = 
      { &ActiveTypeTyIdx, &ActiveTypeInitializedTyIdx };
    static unsigned psTyIdxSZ = psTypeNamesSZ;
  
    for (unsigned i = 0; i < psTypeNamesSZ; ++i) {
      TY_IDX ty_idx;
      TY& ty = New_TY(ty_idx); // sets 'ty_idx'
      TY_Init(ty, 8, KIND_SCALAR, MTYPE_F8, Save_Str(psTypeNames[i]));
      Set_TY_align(ty_idx, 8);
      Set_TY_is_external(ty);
      *(psTyIdx[i]) = ty_idx;
    }
  }

  // ConvertToActiveType: Given a symbol, convert it to active type
  static void 
  ConvertToActiveType(ST* st) {
    static std::set<std::string> cbSymbolSet, eqSymbolSet;
    // Find the type that will be replaced
    TY_IDX typeIndex = ST_type(st);
    // -------------------------------------------------------
    // issue warnings
    // -------------------------------------------------------
    if ((TY_kind(typeIndex) == KIND_SCALAR 
	 || 
	 TY_kind(typeIndex) == KIND_ARRAY) 
	&& Stab_Is_Valid_Base(st)) { 
      if (ST_is_equivalenced(st)) {
	if (eqSymbolSet.find(ST_name(st))==eqSymbolSet.end()) { 
	  FORTTK_WMSG("EQUIVALENCE construct detected for " << ST_name(st) << " conflicts with default initialization within the active type (required for adjoint mode)");
	  eqSymbolSet.insert(ST_name(st));
	}
      }
      if (Stab_Is_Equivalence_Block(ST_base(st))) {
	if (eqSymbolSet.find(ST_name(st))==eqSymbolSet.end()) { 
	  FORTTK_WMSG("EQUIVALENCE construct detected for " << ST_name(st) << " conflicts with default initialization within the active type (required for adjoint mode)");
	  eqSymbolSet.insert(ST_name(st));
	}
      }
      if (Stab_Is_Common_Block(ST_base(st))) {
	if (cbSymbolSet.find(ST_name(st))==cbSymbolSet.end()) { 
	  FORTTK_WMSG("COMMON construct detected for " << ST_name(st) << " conflicts with default initialization within the active type (required for adjoint mode)");
	  cbSymbolSet.insert(ST_name(st));
	}
      }
    }
    // -------------------------------------------------------
    // 1. Setup
    // -------------------------------------------------------
    if (TY_kind(typeIndex) == KIND_POINTER) { // only have one level of indirection
      typeIndex = TY_pointed(typeIndex);
    }

    // Get the replacement type
    TY_IDX newBaseTypeIndex = ActiveTypeTyIdx;
    if (ST_is_initialized(st)) {
      INITO_IDX inito = Find_INITO_For_Symbol(st);
      if (inito != (INITO_IDX)0) {
	newBaseTypeIndex = ActiveTypeInitializedTyIdx;
      }
    }

    // -------------------------------------------------------
    // 2. Change the type of this symbol
    // -------------------------------------------------------
    if (TY_kind(typeIndex) == KIND_SCALAR) {
      Set_ST_type(*st, newBaseTypeIndex);
      if (Stab_Is_Valid_Base(st)
	  && 
	  (Stab_Is_Equivalence_Block(ST_base(st))
	   ||
	   ST_is_equivalenced(st)
	   ||
	   Stab_Is_Common_Block(ST_base(st)))) { 
	TY_IDX baseTypeIndex = ST_type(ST_base(st));
	mUINT64 offset = ST_ofst(st); // offset into base symbol
	// find field with correct offset or symbol
	FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset);
	Set_FLD_type(fld, newBaseTypeIndex);
	if (ST_is_equivalenced(st)) {
	  // retrieve fields with the same offset
	  unsigned short eqInst=2;
	  FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,eqInst);
	  while (!fld.Is_Null()) { 
	    Set_FLD_type(fld, newBaseTypeIndex);
	    fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,++eqInst);
	  }
	}
      }
    }
    else if (TY_kind(typeIndex) == KIND_ARRAY) {
      // get the element type index 
      TY_IDX elementTypeIndex = TY_etype(typeIndex);
      if (TY_kind(elementTypeIndex) == KIND_SCALAR) { 
	// we do this only for scalars because structures 
	// are supposed to activated element by element in the 
	// structure definition and arrays are supposed to be flat 
	// (i.e. no nesting arrays in arrays without a structure definition)
	// Note: because types may be shared, we cannot simply change the
	// element type.  For now we create a new type for each active
	// symbol.
	TY_IDX newArrayTypeIndex = Copy_TY(typeIndex); 
	Set_TY_etype(newArrayTypeIndex, newBaseTypeIndex); // alignment, etc. should be ok
      
	// Now find the appropriate type for the symbol
	TY_IDX newArraySymbolTypeIndex = newArrayTypeIndex;
	if (TY_kind(ST_type(st)) == KIND_POINTER) {
	  newArraySymbolTypeIndex = Make_Pointer_Type(newArrayTypeIndex);
	}
	Set_ST_type(st,newArraySymbolTypeIndex);
	if (Stab_Is_Valid_Base(st)
	    && 
	    (Stab_Is_Equivalence_Block(ST_base(st))
	     ||
	     ST_is_equivalenced(st)
	     ||
	     Stab_Is_Common_Block(ST_base(st)))) { 
	  TY_IDX baseTypeIndex = ST_type(ST_base(st));
	  mUINT64 offset = ST_ofst(st); // offset into base symbol
	  // find field with correct offset or symbol
	  FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset);
	  Set_FLD_type(fld, newArraySymbolTypeIndex);
	  if (ST_is_equivalenced(st)) {
	    // retrieve fields with the same offset
	    unsigned short eqInst=2;
	    FLD_HANDLE fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,eqInst);
	    while (!fld.Is_Null()) { 
	      Set_FLD_type(fld, newArraySymbolTypeIndex);
	      fld = TY_Lookup_FLD(baseTypeIndex, 0, offset,++eqInst);
	    }
	  }
	}
      }
    } 
    else {
      // Note: We should never see a KIND_STRUCT; this is handled
      // through scalarization.
      FORTTK_DIE("Unexpected type kind: " << TY_kind(typeIndex));
    }
  }

  // ConvertStructMemberTouActiveType: Given a base structure type, a
  // referenced object type and the offset of the referenced object,
  // change the type of the referenced field.
  static void 
  ConvertStructMemberToActiveType(TY_IDX base_ty, TY_IDX ref_ty, 
				  UINT field_id)
  {
    UINT cur_field_id=0;
    FLD_HANDLE fld = FLD_get_to_field (base_ty, field_id, cur_field_id);
    FORTTK_ASSERT(fld.Entry(), "Could not find field in " << TY_name(base_ty));
    TY_IDX fldTy=fld.Entry()->type;
    if (TY_kind(fldTy) == KIND_POINTER) {
      // replicate the pointer type but let it point to the active type
      TY_IDX fieldPointed = TY_pointed(fldTy);
      if (TY_kind(fieldPointed) == KIND_ARRAY) {
        TY_IDX newArrayTypeIndex = Copy_TY(fieldPointed); 
        Set_TY_etype(newArrayTypeIndex, ActiveTypeTyIdx);
        TY_IDX newArraySymbolTypeIndex = Make_Pointer_Type(newArrayTypeIndex);
        Set_FLD_type(fld, newArraySymbolTypeIndex);
      }
      else {
        TY_IDX newFldType=Copy_TY(fldTy);
        Set_TY_pointed(newFldType, ActiveTypeTyIdx);
        Set_FLD_type(fld, newFldType);
      }
    } 
    else if (TY_kind(fldTy) == KIND_ARRAY) {
      // replicate the pointer type but let it point to the active type 
      //      TY_IDX typeIndex=TY_pointed(fldTy);
      TY_IDX newArrayTypeIndex = Copy_TY(fldTy); 
      Set_TY_etype(newArrayTypeIndex, ActiveTypeTyIdx);
      TY_IDX newArraySymbolTypeIndex = Make_Pointer_Type(newArrayTypeIndex);
      Set_FLD_type(fld, newArraySymbolTypeIndex);
    } 
    else { 
      Set_FLD_type(fld, ActiveTypeTyIdx);
    }
  }


  // ConvertScalarizedRefToActiveType: Change type of the last component
  // of the scalarized path.  That is, for "a%b%c%d", change the type of
  // 'd'.  (This means we can safely ignore internal path components.)
  // 
  // Note that types from modules will be duplicated in the type table
  // for each 'use', with the duplicates receiving a 'TY_IS_EXTERNAL'
  // flag.  Because the duplicates are igored by whirl2f, the the
  // non-external version of the type needs to be changed so that the
  // module definition is changed.
  static void 
  ConvertScalarizedRefToActiveType(WN* wn)
  {
    TY_IDX baseobj_ty = WN_GetBaseObjType(wn);
    TY_IDX refobj_ty  = WN_GetRefObjType(wn);
    if (TY_Is_Array(baseobj_ty)) {
      // array reference, such as "s%b(i)"
      // must change type of ref-obj.
      // Note that we assume the WHIRL includes offsets instead of field ids
      WN* kid0;
      if (WN_operator(wn)==OPR_ISTORE) {
	wn=WN_kid1(wn);
      }
      // descend until the OPR_STRCTFLD
      while (WN_operator(wn)!=OPR_STRCTFLD && (NULL!=(kid0=WN_kid0(wn)))) { 
	wn=kid0;
      }
      // in case we descended
      baseobj_ty = WN_GetBaseObjType(wn);
      FORTTK_ASSERT(OPERATOR_has_field_id(WN_operator(wn)), "Uh-oh!");
      UINT field_id = WN_field_id(wn);
      ConvertStructMemberToActiveType(baseobj_ty, refobj_ty, field_id);
      if (TY_is_external(baseobj_ty)) {
	For_all_until(Ty_Table,
		      ConvertModuleTypeFctr(baseobj_ty, refobj_ty, field_id));
      }
    }
    else {
      // structure member reference, such as "s%a" or "b(i)%a"
      // must change type of ref-obj.
      FORTTK_ASSERT(OPERATOR_has_field_id(WN_operator(wn)), "Uh-oh!");
      UINT field_id = WN_field_id(wn);
      ConvertStructMemberToActiveType(baseobj_ty, refobj_ty, field_id);
      if (TY_is_external(baseobj_ty)) {
	For_all_until(Ty_Table,
		      ConvertModuleTypeFctr(baseobj_ty, refobj_ty, field_id));
      }
    }
  }


  // TY_Lookup_FLD: Given a base structure type, a referenced object type
  // and the offset of the referenced object, return the field entry.
  // The referenced object type may be 0.
  //
  // This is not an overly efficient method, but WHIRL doesn't make this
  // query easy.
  //
  // cf. FLD_get_to_field
  static FLD_HANDLE 
  TY_Lookup_FLD(TY_IDX struct_ty, TY_IDX ref_ty, UINT64 ref_ofst,unsigned short eqInst)
  {
    FLD_ITER fld_iter = Make_fld_iter(TY_fld(struct_ty));
    unsigned short foundInst=0;
    do {
      FLD_HANDLE fld(fld_iter);
      UINT64 ofst = FLD_ofst(fld);
      TY_IDX ty   = FLD_type(fld);
      if (ofst == ref_ofst) {
	++foundInst;
	if (ref_ty == 0) {
	  if (eqInst==foundInst)
	    return fld;
	}
	else {
	  if (Stab_Identical_Types(ref_ty, ty, FALSE /* check_quals */,
				   FALSE /* check_scalars */, TRUE)) {
	    return fld;
	  }
	}
      }
    } while (!FLD_last_field(fld_iter++));
  
    return FLD_HANDLE(); // null field
  }

  // FIXME: Available in symtab_utils.h / symtab.cxx
  static TY_IDX
  MY_Make_Array_Type (TY_IDX elem_ty, 
		      INT32 ndim, 
		      bool fixed,  
		      const INT64* lower, 
		      const INT64* upper) {
    INT64 elem_sz = TY_size (elem_ty);
    UINT elem_align = TY_align(elem_ty);
    FORTTK_ASSERT(elem_sz > 0 && elem_align > 0,
		  "Cannot make an array of " 
		  << TY_name(elem_ty));
    ARB_HANDLE arb_h,arb_h_first;
    INT64 ty_size=0; // for variable length arrays this should stay 0
    for (INT i = 0; i < ndim; ++i) {
      arb_h = New_ARB ();
      if (i==0) {
	arb_h_first = arb_h;
      }
      if (!fixed) { 
	ARB * arb = arb_h.Entry();
	arb->flags = ARB_EMPTY_LBND | ARB_EMPTY_UBND | ARB_EMPTY_STRIDE;
	arb->dimension = 1;
	arb->co_dimension = 0;
	arb->unused = 0;
	arb->u1.lbnd_val = 0;
	arb->u2.ubnd_val = 0;
	arb->u3.stride_val = 0;
      }
      else { 
	ARB_Init (arb_h, lower[i], upper[i], elem_sz);
	ty_size+=(upper[i]-lower[i])*elem_sz;
      }
      Set_ARB_dimension (arb_h, ndim-i);
    }
    Set_ARB_last_dimen (arb_h);
    Set_ARB_first_dimen (arb_h_first);
    TY_IDX ty_idx;
    TY& ty = New_TY (ty_idx);
    TY_Init (ty, ty_size, KIND_ARRAY, MTYPE_UNKNOWN, 0);
    Set_TY_align (ty_idx, elem_align);
    Set_TY_etype (ty, elem_ty);
    Set_TY_arb (ty, arb_h_first);
    return ty_idx;
  } // Make_Array_Type

  static TY_IDX
  XAIFTyToWHIRLTy(const char* type, const TYPE_ID mtype)
  {
    TY_IDX ty = 0;
    if (mtype!=MTYPE_UNKNOWN)
      ty = MTYPE_To_TY(mtype);
    else if (strcmp(type, "real") == 0) {
      ty = MTYPE_To_TY(Args::ourDefaultMTypeReal);
    } 
    else if (strcmp(type, "integer") == 0) {
      ty = MTYPE_To_TY(Args::ourDefaultMTypeInt);
    } 
    else {
      // FIXME: don't know about anything else yet
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "Unknown XAIF type: " << type);
    }
    return ty;
  }

  TYPE_ID
  XAIFFETypeToWHIRLMTy(const char* anFETypeName) {
    if (strcmp(anFETypeName,"")==0) { 
      return MTYPE_UNKNOWN;
    }
    return Name_To_Mtype(anFETypeName);
  }

  // ****************************************************************************
  // MyDGNode routines
  // ****************************************************************************

  OA::OA_ptr<MyDGNode>
  GetSuccessor(OA::OA_ptr<MyDGNode> node, bool succIsOutEdge)
  {
    using namespace OA::DGraph;
    
    int numSucc = (succIsOutEdge) ? node->num_outgoing() : node->num_incoming();
    if (numSucc == 0) {
      OA::OA_ptr<MyDGNode> retval;  retval = NULL;
      return retval;
    }
    else if (numSucc > 1) {
      xercesc::DOMElement* elem = node->GetElem();
      FORTTK_DIE("Cannot find unique successor to graph node; found " << numSucc
		 << ":\n" << *elem);
    }
  
    // We know there is one successor
    OA::OA_ptr<MyDGNode> succ; succ = NULL;
    if (succIsOutEdge) {
      OA::OA_ptr<NodesIteratorInterface> it;
      it = node->getSinkNodesIterator();
      OA::OA_ptr<NodeInterface> ntmp = it->current();
      succ = ntmp.convert<MyDGNode>();
    }
    else {
      OA::OA_ptr<NodesIteratorInterface> it;
      it = node->getSourceNodesIterator();
      OA::OA_ptr<NodeInterface> ntmp = it->current();
      succ = ntmp.convert<MyDGNode>();
    }
    return succ;
  }


  OA::OA_ptr<MyDGNode>
  GetSuccessorAlongEdge(OA::OA_ptr<MyDGNode> node, unsigned int condition, 
			bool succIsOutEdge)
  {
    using namespace OA::DGraph;

    OA::OA_ptr<MyDGNode> succ; succ = NULL;
    int numSucc = (succIsOutEdge) ? node->num_outgoing() : node->num_incoming();
  
    if (succIsOutEdge) {
      OA::OA_ptr<EdgesIteratorInterface> it;
      it = node->getOutgoingEdgesIterator();
      for ( ; it->isValid(); ++(*it)) {
	OA::OA_ptr<EdgeInterface> etmp = it->current();
	OA::OA_ptr<MyDGEdge> edge = etmp.convert<MyDGEdge>();
	xercesc::DOMElement* e = edge->GetElem();
      
	unsigned int cond = GetCondAttr(e);
	if (condition == cond) {
	  OA::OA_ptr<NodeInterface> ntmp = edge->getSink();
	  succ = ntmp.convert<MyDGNode>();
	  break;
	}
      }
    }
    else {
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "Transform into a template.");
    }
    return succ;
  }


  // CreateCFGraph: Given an XAIF control flow graph, create and
  // return a CFG where CFG nodes point to XAIF CVG vertices.
  static OA::OA_ptr<OA::DGraph::DGraphInterface> 
  CreateCFGraph(const xercesc::DOMElement* cfgElem)
  {
    using namespace OA::DGraph;
  
    MyDGNode::resetIds();
    OA::OA_ptr<OA::DGraph::DGraphImplement> g; 
    g = new DGraphImplement();
    VertexIdToMyDGNodeMap m;
  
    // -------------------------------------------------------
    // Create the graph
    // -------------------------------------------------------
    XAIF_BBElemFilter filt;
    for (xercesc::DOMElement* elem = GetChildElement(cfgElem, &filt);
	 (elem); elem = GetNextSiblingElement(elem, &filt)) {
      if (XAIF_BBElemFilter::IsEdge(elem)) {
	// Add an edge to the graph. 
      
	// Find src and target (sink) nodes. 
	const XMLCh* srcX = elem->getAttribute(XAIFStrings.attr_source_x());
	const XMLCh* targX = elem->getAttribute(XAIFStrings.attr_target_x());
	XercesStrX src = XercesStrX(srcX);
	XercesStrX targ = XercesStrX(targX);

	OA::OA_ptr<MyDGNode> gn1; gn1 = m[std::string(src.c_str())];  // source
	OA::OA_ptr<MyDGNode> gn2; gn2 = m[std::string(targ.c_str())]; // target
	FORTTK_ASSERT(!gn1.ptrEqual(NULL) && !gn2.ptrEqual(NULL), 
		      "Invalid edge in CFG:\n" << *elem);

	OA::OA_ptr<MyDGEdge> ge; ge = new MyDGEdge(gn1, gn2, elem); // src, targ
	g->addEdge(ge);
      } 
      else {
	// Add a vertex to the graph
	const XMLCh* vidX = elem->getAttribute(XAIFStrings.attr_Vid_x());
	XercesStrX vid = XercesStrX(vidX);
	FORTTK_ASSERT(strlen(vid.c_str()) > 0, 
		      "Invalid vertex in CFG:\n" << *elem);

	OA::OA_ptr<MyDGNode> gn; gn = new MyDGNode(elem);
	g->addNode(gn);
	m[std::string(vid.c_str())] = gn;
      } 
    }
  
    return g;
  }


  // DumpDotGraph:

  static std::string
  DumpDotGraph_GetNodeName(OA::OA_ptr<MyDGNode> n);

  void
  DDumpDotGraph(OA::OA_ptr<OA::DGraph::DGraphInterface> graph)
  {
    DumpDotGraph(std::cerr, graph);
  }

  void
  DumpDotGraph(std::ostream& os, OA::OA_ptr<OA::DGraph::DGraphInterface> graph)
  {
    using namespace OA::DGraph;
    
    os << "digraph MyGraph {\n";
    os << "  graph [ ];\n"
       << "  node [ fontsize = \"10\" ];\n"
       << "  edge [ ];\n"
       << std::endl;
  
    OA::OA_ptr<EdgesIteratorInterface> edgesItPtr;
    edgesItPtr = graph->getEdgesIterator();
    for (; edgesItPtr->isValid(); ++(*edgesItPtr)) {
      OA::OA_ptr<OA::DGraph::EdgeInterface> e = edgesItPtr->current();
      OA::OA_ptr<OA::DGraph::NodeInterface> srctmp = e->getSource();
      OA::OA_ptr<OA::DGraph::NodeInterface> snktmp = e->getSink();
      OA::OA_ptr<MyDGNode> src = srctmp.convert<MyDGNode>();
      OA::OA_ptr<MyDGNode> snk = snktmp.convert<MyDGNode>();
      std::string srcNm = DumpDotGraph_GetNodeName(src);
      std::string snkNm = DumpDotGraph_GetNodeName(snk);
      os << "  \"" << srcNm.c_str() << "\" -> \"" << snkNm.c_str() << "\";\n";
    }
    os << "}" << std::endl;
  }

  static std::string
  DumpDotGraph_GetNodeName(OA::OA_ptr<MyDGNode> n) 
  {
    std::string name;
  
    // MyDGNode portion
    const char* nodeIdStr = Num2Str(n->getId(), "%u");
    name += nodeIdStr;

    // XAIF portion
    xercesc::DOMElement* elem = n->GetElem();
    const XMLCh* xaifNameX = elem->getNodeName();
    XercesStrX   xaifName = XercesStrX(xaifNameX);
    const XMLCh* vidX = elem->getAttribute(XAIFStrings.attr_Vid_x());
    XercesStrX   vid = XercesStrX(vidX);  
    name += ", (";
    name += vid.c_str();
    name += ", ";
    name += xaifName.c_str();
    name += ")";
  
    return name;
  }

}
