#include <alloca.h>
#include <stdlib.h> 
#include <string>   
#include <set> 
#include <vector> 

#include "OpenAnalysis/CFG/ManagerCFG.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/SymTab.h"
#include "Open64IRInterface/Open64IRInterface.hpp"

#include "wn2xaif.i"
#include "wn2xaif.h"
#include "wn2xaif_stmt.h"
#include "wn2xaif_expr.h"
#include "wn2xaif_mem.h"
#include "wn2xaif_io.h"
#include "st2xaif.h"
#include "ty2xaif.h"
#include "Args.h"

    
namespace whirl2xaif { 

  static void
  xlate_EntryPoint(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  static set<OA::SymHandle>* 
  GetParamSymHandleSet(WN* wn_pu);

  static const char*
  xlate_intent(WN* parm);

  static void
  xlate_BBStmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  static void 
  xlate_CFCondition(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  static void 
  xlate_LoopInitialization(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  static void 
  xlate_LoopUpdate(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  static void
  DumpCFGraphEdge(xml::ostream& xos, UINT eid, 
		  OA::OA_ptr<OA::CFG::EdgeInterface> edge);

  static const char*
  GetLoopReversalType(OA::OA_ptr<OA::CFG::CFGInterface> cfg, 
		      OA::OA_ptr<OA::CFG::NodeInterface> n);

  static std::string
  GetIDsForStmtsInBB(OA::OA_ptr<OA::CFG::NodeInterface> node, 
		     PUXlationContext& ctxt);

  // NOTE: removed static for Sun's compiler (supposed to be in unnamed
  // namespace now)
  pair<bool, INT64>
  GetCFGEdgeCondVal(const OA::OA_ptr<OA::CFG::EdgeInterface> edge);

  // lt_CFGEdge: Used to sort CFG::Edges by src, sink and condition value.
  struct lt_CFGEdge
  {
    // return true if e1 < e2; false otherwise
    bool operator()(const OA::OA_ptr<OA::CFG::EdgeInterface> e1, 
		    const OA::OA_ptr<OA::CFG::EdgeInterface> e2) const
    {
      unsigned int src1 = e1->getSource()->getId();
      unsigned int src2 = e2->getSource()->getId();
      if (src1 == src2) { 
	unsigned int sink1 = e1->getSink()->getId();
	unsigned int sink2 = e2->getSink()->getId();
	if (sink1 == sink2) {
	  pair<bool, INT64> ret1 = GetCFGEdgeCondVal(e1);
	  bool hasCondVal1 = ret1.first;
	  INT64 condVal1 = ret1.second;
    
	  pair<bool, INT64> ret2 = GetCFGEdgeCondVal(e2);
	  bool hasCondVal2 = ret2.first;
	  INT64 condVal2 = ret2.second;
	
	  if (hasCondVal1 && hasCondVal2) {
	    return (condVal1 < condVal2);
	  } 
	  else if (hasCondVal1 /* && !hasCondVal2 */) {
	    return false;  // e1 > e2
	  }
	  else if (hasCondVal2 /* && !hasCondVal1 */) {
	    return true; // e1 < e2
	  }
	  else /* !hasCondVal1 && !hasCondVal2 */ {
	    return false; // e1 == e2
	  }
	} 
	else { 
	  return (sink1 < sink2); 
	}
      } 
      else {
	return (src1 < src2);
      }
    }

  };

  // the handlers
  typedef void (*Handler)(xml::ostream&, WN*, PUXlationContext&);
  static std::map<int,Handler> ourHandlerTable;
  static bool ourHandlerTableInit=false;
  void initOurHandlerTable();

  // TranslateWN: see header. The task of translation is dispatched to
  // the appropriate handler registered in the handler table.
  void 
  TranslateWN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {   

    if (!wn) { return; }

    OPERATOR opr = WN_operator(wn);
  
#if 0
    //xos << BegComment << "Translating " << OPERATOR_name(opr) << EndComment;
    fortTkSupport::WNId id = ctxt.findWNId(wn);
#endif
  
    // Determine whether we are in a context where we expect this
    // expression to have logically valued arguments, or whether we are
    // entering a context where we expect this expression to be a
    // logically valued argument.
    OPCODE opc = WN_opcode(wn);
    if (OPCODE_is_boolean(opc) && WN2F_expr_has_boolean_arg(opc)) { 
      // We expect logical operands to this operator.  Note that this
      // may also be a logical argument, so
      // ctxt.currentXlationContext().isFlag(XlationContext::IS_LOGICAL_ARG) may also be true.
      ctxt.currentXlationContext().setFlag(XlationContext::HAS_LOGICAL_ARG);
    } 
    else if (ctxt.currentXlationContext().isFlag(XlationContext::HAS_LOGICAL_ARG)) { 
      // This is a logical argument.  This is the only place where we
      // should need to check whether this is expected to be a logical
      // valued expression. I.e. the only place where we apply
      // PUXlationContext_XlationContext::HAS_LOGICAL_ARG(context).  However, it may be
      // set at other places (e.g. in wn2f_stmt.c).
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
      ctxt.currentXlationContext().setFlag(XlationContext::IS_LOGICAL_ARG);
    }
    else {
      ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_LOGICAL_ARG);
      ctxt.currentXlationContext().unsetFlag(XlationContext::IS_LOGICAL_ARG);
    }
  
    // Dispatch to the appropriate handler for this construct.
    if (!ourHandlerTableInit)
      initOurHandlerTable();
    return ourHandlerTable[opr](xos, wn, ctxt);
  }


  // Function entry handlers
      

  // xlate_FUNC_ENTRY: Given the root of a WHIRL tree, and an
  // appropriate context 'ctxt', emits XAIF for the tree to the 'xos'
  // stream.  Assumes that Open64 symbol table globals are already set.
  void
  xlate_FUNC_ENTRY(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    using namespace OA::CFG;
    using namespace OA::DGraph;

    FORTTK_ASSERT(WN_operator(wn) == OPR_FUNC_ENTRY, fortTkSupport::Diagnostics::UnexpectedInput); 
  
    WN* fbody = WN_func_body(wn);

    // -------------------------------------------------------
    // Collect auxillary data
    // -------------------------------------------------------
  
    // 0. WHIRL parent map (FIXME: compute at callgraph)
    fortTkSupport::WhirlParentMap wnParentMap(wn);
    ctxt.setWNParentMap(&wnParentMap);

    // 1. OpenAnalysis info
    OA::ProcHandle proc((OA::irhandle_t)Current_PU_Info);

    fortTkSupport::IntraOAInfo* oaAnal = Whirl2Xaif::getOAAnalMap().Find(Current_PU_Info);
    OA::OA_ptr<OA::CFG::CFGInterface> cfg = 
      Whirl2Xaif::getOAAnalMap().getCFGEach()->getCFGResults(proc);
    ctxt.setUDDUChains(oaAnal->getUDDUChainsXAIF());
    ctxt.setAliasMapXAIF(oaAnal->getAliasXAIF());
    ctxt.setDoChains(oaAnal->getReachDefsOverwriteXAIF());
  
    // 2. Non-scalar symbol table
    fortTkSupport::ScalarizedRefTab_W2X* tab = Whirl2Xaif::getScalarizedRefTableMap().Find(Current_PU_Info);
    ctxt.setScalarizedRefTab(tab);
  
    // 3. WHIRL<->ID maps
    fortTkSupport::WNToWNIdMap* wnmap = Whirl2Xaif::getWNToWNIdTableMap().Find(Current_PU_Info);
    ctxt.setWNToIdMap(wnmap);
  
    // -------------------------------------------------------
    // Translate the function header
    // -------------------------------------------------------
    xlate_EntryPoint(xos, wn, ctxt); 
    xos << std::endl;

    // -------------------------------------------------------
    // Translate CFG (et al.) to XAIF
    // -------------------------------------------------------
    ctxt.createXlationContext(XlationContext::NOFLAG, wn);
  
    // Dump CFG vertices (basic blocks) in sorted order ('normalized')
    // Note: It might seem that instead of sorting, we could simply use
    // DGraphStandard::DFSIterator.  However, procedures can have
    // unreachable code that will not be found with a DFS.  A simple
    // example of this is that WHIRL often has two OPR_RETURNs at the
    // end of a procedure.
#if 0 // FIXME
    //     DGraphNodeVec* nodes = SortDGraphNodes(&cfg);
    //     for (DGraphNodeVec::iterator nodeIt = nodes->begin(); 
    // 	 nodeIt != nodes->end(); ++nodeIt) {
#endif
  
    // try a BFS iterator.  too bad for dead code. (actually DFS-- BFS
    // not yet implmented) -- toposort FIXME
    std::set<OA::OA_ptr<OA::CFG::NodeInterface> > usedNodes;
    
    OA::OA_ptr<OA::CFG::NodeInterface> entry = cfg->getEntry();
    OA::OA_ptr<OA::DGraph::NodesIteratorInterface> nodeItPtr = cfg->getDFSIterator(entry);
    for (; nodeItPtr->isValid(); ++(*nodeItPtr)) {
      OA::OA_ptr<OA::DGraph::NodeInterface> dn = nodeItPtr->current();
      OA::OA_ptr<OA::CFG::Node> n = dn.convert<OA::CFG::Node>();
      usedNodes.insert(n);
      // std::cout << "visiting " << n->getId() << std::endl;
      const char* vtype = fortTkSupport::GetCFGVertexType(cfg, n);
      fortTkSupport::SymTabId scopeId = ctxt.findSymTabId(Scope_tab[CURRENT_SYMTAB].st_tab);
      std::string ids = GetIDsForStmtsInBB(n, ctxt);
      // 1. BB element begin tag
      xos << xml::BegElem(vtype) << xml::Attr("vertex_id", n->getId());
      if (vtype == XAIFStrings.elem_BB()) {
	xos << xml::Attr("scope_id", scopeId);
      }
      else if (vtype == XAIFStrings.elem_BBForLoop()) {
	xos << xml::Attr("reversal", GetLoopReversalType(cfg,n));
      }
      if (vtype == XAIFStrings.elem_BBForLoop() 
	  || 
	  vtype == XAIFStrings.elem_BBPreLoop()
	  || 
	  vtype == XAIFStrings.elem_BBPostLoop()
	  || 
	  vtype == XAIFStrings.elem_BBBranch()) { 
	// to get the line number we need to get  
	// the whirl node which appears to be quite a chore
	OA::OA_ptr<OA::CFG::NodeStatementsIteratorInterface> stmtIt = n->getNodeStatementsIterator();
	bool found=false;
	for (; stmtIt->isValid() && !found; ++(*stmtIt)) {
	  OA::StmtHandle st = stmtIt->current();
	  WN* wstmt = (WN*)st.hval(); 
	  // now we need to figure out which one of these it actually is: 
	  OPERATOR opr = WN_operator(wstmt);
	  switch (opr) {
	  case OPR_DO_LOOP: 
	  case OPR_DO_WHILE: 
	  case OPR_WHILE_DO:
	  case OPR_IF:
	  case OPR_SWITCH: { 
	    USRCPOS srcpos;
	    USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wstmt);
	    xos << xml::Attr("lineNumber",USRCPOS_linenum(srcpos));
	    found=true;
	    break;
	  }
	  default: 
	    break; // fall through
	  }
	}
      }
      // 2. BB element contents
      ctxt.createXlationContext();
      OA::OA_ptr<OA::CFG::NodeStatementsIteratorInterface> stmtItPtr
	= n->getNodeStatementsIterator();
      for (; stmtItPtr->isValid(); ++(*stmtItPtr)) {
	WN* wstmt = (WN *)stmtItPtr->current().hval();
	xlate_BBStmt(xos, wstmt, ctxt);
      }
      ctxt.deleteXlationContext();
      // 3. BB element end tag
      xos << xml::EndElem << std::endl;
    }
#if 0
    delete nodes;
#endif

    // Dump CFG edges (only those within the XAIF graph)
    CFGEdgeVec* edges = SortCFGEdges(cfg);
    for (CFGEdgeVec::iterator edgeIt = edges->begin(); 
	 edgeIt != edges->end(); ++edgeIt) {
      OA::OA_ptr<OA::CFG::EdgeInterface> e = (*edgeIt);
      
      OA::OA_ptr<OA::DGraph::NodeInterface> dsrc = e->getSource();
      OA::OA_ptr<OA::CFG::Node> src = dsrc.convert<OA::CFG::Node>();
      OA::OA_ptr<OA::DGraph::NodeInterface> dsnk = e->getSink();
      OA::OA_ptr<OA::CFG::Node> snk = dsnk.convert<OA::CFG::Node>();
      if (usedNodes.find(src) != usedNodes.end() && 
	  usedNodes.find(snk) != usedNodes.end()) {
	DumpCFGraphEdge(xos, ctxt.currentXlationContext().getNewEdgeId(), e);
      }
    }

    delete edges;
  
    // -------------------------------------------------------
    // Cleanup
    // -------------------------------------------------------
    ctxt.deleteXlationContext();
  
    
  }

  // xlate_ALTENTRY:
  void
  xlate_ALTENTRY(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    // Similar to a FUNC_ENTRY, but without the function body.
    FORTTK_ASSERT(WN_operator(wn) == OPR_ALTENTRY, fortTkSupport::Diagnostics::UnexpectedInput); 
  
    // Translate the function entry point (FIXME)
    xlate_EntryPoint(xos, wn, ctxt);
  
    
  }


  void
  xlate_ignore(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    
  }

  void
  xlate_STRCTFLD(xml::ostream& xos, 
		 WN *wn, 
		 PUXlationContext& ctxt) {
    xlate_MemRef(xos, 
		 wn, 
		 WN_Tree_Type(wn),  
		 WN_Tree_Type(wn),  
		 0, 
		 ctxt);
  }

  void
  xlate_unknown(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) {
    OPERATOR opr = WN_operator(wn);
    FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
  }


  // FIXME: MOVE elsewhere
  bool
  IsActiveStmt(WN* wn, PUXlationContext& ctxt)
  {
    bool active = false;
  
    OPERATOR opr = WN_operator(wn);

    // if (OPERATOR_is_store(opr) || OPERATOR_is_call(opr)) { }

    if (OPERATOR_is_call(opr)) {
      // FIXME: For now we punt on calls and assume they are active.
      active = true;
    }
    else if (OPERATOR_is_store(opr)) {
      TY_IDX ty = WN_Tree_Type(wn);
      const char* ty_str = TranslateTYToSymType(ty); // FIXME
      active = (strcmp(ty_str, "real") == 0 || strcmp(ty_str, "complex") == 0);
    }
  
    return active;
  }


  // Variable references: xlate_SymRef, xlate_MemRef

  // Helper for xlate_MemRef
  /* just used to maintain the state of the recursions when */
  /* marking FLDs in nested addresses                       */
  class LOC_INFO {
  private:
    FLD_PATH_INFO * _flds_left;   /* points to tail of fld_path */
    STAB_OFFSET _off;             /* offset of last FLD used in fld_path */
    BOOL   _base_is_array;        /* was ST of address an array? */
  
  public:
    WN * _nested_addr;
  
    LOC_INFO(FLD_PATH_INFO * path)
      : _flds_left(path), _off(0), _base_is_array(FALSE), _nested_addr(NULL) { }
  
    void WN2F_Find_And_Mark_Nested_Address(WN * addr);
  };


  // xlate_PregRef: [FIXME: can we abstract witih xlate_SymRef]
  void
  xlate_PregRef(xml::ostream& xos, ST* st, TY_IDX preg_ty, 
		PREG_IDX preg_idx, PUXlationContext& ctxt)
  {
    bool closeVarRef = false;
    if (!ctxt.currentXlationContext().isFlag(XlationContext::VARREF)) {
      xos << xml::BegElem(XAIFStrings.elem_VarRef())
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << xml::Attr("du_ud", ctxt.findUDDUChainId(ctxt.getMostRecentWN()))
	  << xml::Attr("alias", ctxt.getAliasMapKey(ctxt.getMostRecentWN()));
      closeVarRef = true; 
    }

    ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
    fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);

    xos << xml::BegElem("xaif:SymbolReference") 
	<< xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	<< xml::Attr("scope_id", scopeid) << AttrSymId(st) << PregIdAnnot(preg_idx)
	<< xml::EndElem;
  
    if (closeVarRef) {
      xos << xml::EndElem /* elem_VarRef() */;
    }
  
    
  }


  // xlate_SymRef: see header.
  void
  xlate_SymRef(xml::ostream& xos, 
	       ST* base_st,        // base symbol
	       TY_IDX baseptr_ty,  // type of base symbol ptr
	       TY_IDX ref_ty,      // type of referenced object
	       STAB_OFFSET offset, // offset within 'base_st'
	       PUXlationContext& ctxt)
  {
    // FIXME: ugly, ugly, ugly
    /* The base symbol 'base_st' will be treated as having a lvalue
     * (address) type of 'baseptr_ty', except when "deref" is TRUE, when
     * the rvalue of 'base_st' is assumed to have the 'base_ty'
     * and must either explicitly (for POINTER variables) or implicitly
     * (for pass by reference arguments) be dereferenced.

     * Note that a compatible 'base_ty' and 'ref_ty' simply translates
     * into a reference to the given 'base_st'.  In all other cases we
     * expect 'ref_ty' to be a field or offset within the 'base_ty'
     * (structure or array).
     *
     * Note that we must have special handling for common-blocks and
     * equivalences.  Note that "base_ty" may be different from
     * "Stab_Pointer_To(ST_type(base_st))", both for "deref" cases and 
     * ptr_as_array variables.  */

    WN* ref_wn = ctxt.getMostRecentWN();
    TY_IDX base_ty = TY_pointed(baseptr_ty); 

    // -------------------------------------------------------
    // If we are not already within xaif:VariableReference... (FIXME: abstract)
    // -------------------------------------------------------
    bool constant = (ST_class(base_st) == CLASS_CONST);
    bool newContext = false;
    if (!constant && !ctxt.currentXlationContext().isFlag(XlationContext::VARREF)) {
      xos << xml::BegElem(XAIFStrings.elem_VarRef())
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << xml::Attr("du_ud", ctxt.findUDDUChainId(ref_wn))
	  << xml::Attr("alias", ctxt.getAliasMapKey(ref_wn));
      ctxt.createXlationContext(XlationContext::VARREF);
      newContext = true; 
    }

    /* Select variable-reference translation function */
    const BOOL deref_val = ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR);
    void (*translate_var_ref)(xml::ostream&, ST*, PUXlationContext&);

    if (deref_val && (ST_sclass(base_st) != SCLASS_FORMAL) 
	&& TY_Is_Pointer(ST_type(base_st)) 
	&& !TY_is_f90_pointer(ST_type(base_st))) {
      /* An explicit dereference */
      translate_var_ref = &ST2F_deref_translate;
    } 
    else {
      /* A direct reference or an implicit dereference */
      translate_var_ref = &TranslateSTUse;
    }
  
  
    // FIXME: for now, make sure this is only used for data refs 
    if (ST_class(base_st) == CLASS_FUNC) {
      std::cerr << "xlate_SymRef: translating function ref\n";
    } 
    else if (ST_class(base_st) == CLASS_BLOCK) { // FIXME
      TranslateSTUse(xos, base_st, ctxt);
      xos << "+ " << Num2Str(offset, "%lld");
    }
    
    // Note: top-var-refs will can be classified according to
    // IsRefSimple*() functions.  Things are a little more complicated
    // with sub-var-refs; hence the need for two tests in each 'if'.

    fortTkSupport::ScalarizedRef* sym = ctxt.findScalarizedRef(ref_wn);
    if (sym) { 
      // 1. A scalarized symbol
      ST_TAB* sttab = Scope_tab[CURRENT_SYMTAB].st_tab;
      fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
    
      xos << xml::BegElem("xaif:SymbolReference") 
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << xml::Attr("scope_id", scopeid) 
	  << xml::Attr("symbol_id", sym->getName()) << xml::EndElem;
    } 
    else if (fortTkSupport::ScalarizedRef::isRefScalar(base_ty, ref_ty) 
	     || 
	     fortTkSupport::ScalarizedRef::isRefSimpleScalar(ref_wn)) {
      // 2. Reference to a scalar symbol (==> offset into 'base_st' is zero)
      translate_var_ref(xos, base_st, ctxt);
    } 
    else if (TY_Is_Array(ref_ty) 
	     || 
	     fortTkSupport::ScalarizedRef::isRefSimpleArray(ref_wn)) {
      // 3. Reference to an array of scalars
      translate_var_ref(xos, base_st, ctxt);
    }
    else if (TY_Is_Array(base_ty) 
	     || 
	     fortTkSupport::ScalarizedRef::isRefSimpleArrayElem(ref_wn)) {
      // 4. Array element reference to a scalar
      translate_var_ref(xos, base_st, ctxt);
      if (!ctxt.currentXlationContext().isFlag(XlationContext::HAS_NO_ARR_ELMT)) { // FIXME: we expect arr elmt!
	TY2F_Translate_ArrayElt(xos, base_ty, offset);
	ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
      }
    }
    else {
      // 5. 
      //FORTTK_DIE("Unknown ref type.");
      translate_var_ref(xos, base_st, ctxt);

#if 0 // FIXME:REMOVE
      /* We only dereference a field when the base need not be 
       * dereferenced.  We never need to have both dereferenced, 
       * since pointers cannot occur in RECORDS and common/
       * equivalence blocks cannot be referenced through pointer 
       * identifiers. */
      FLD_PATH_INFO *fld_path = NULL;
      BOOL deref_fld;
      if ( !(TY_IsRecord(ref_ty) /* && FIXME*/) ) {
	deref_fld = (deref_val && !TY_Is_Pointer(ST_type(base_st))) ? TRUE:FALSE;
	if (deref_fld) { ref_ty = Stab_Pointer_To(ref_ty); }
	fld_path = TY2F_Get_Fld_Path(base_ty, ref_ty, offset);
      }
    
      if (fld_path == NULL) {
	translate_var_ref(xos, base_st, ctxt);
      } 
      else if (Stab_Is_Common_Block(base_st)) {
	// Common block reference (do not translate as field ref)
	// FIXME: make sure the fld_path is length 1 
	ST_IDX st_idx = fld_path->fld.Entry()->st;
	ST* st = (st_idx != 0) ? ST_ptr(st_idx) : NULL;
	if (st) {
	  translate_var_ref(xos, st, ctxt);
	} 
	else { // FIXME
	  TY2F_Translate_Fld_Path(xos, fld_path, deref_fld, 
				  // (Stab_Is_Common_Block(base_st) || 
				  //  Stab_Is_Equivalence_Block(base_st)),
				  TRUE, FALSE/*as_is*/, ctxt);
	}

      } else {
      
	// Structure: 
	/* Base the path at the 'base_st' object, and separate it from
	 * the remainder of the path with the field selection operator. */
	translate_var_ref(xos, base_st, ctxt);
	TY2F_Fld_Separator(xos);
	TY2F_Translate_Fld_Path(xos, fld_path, deref_fld, 
				// (Stab_Is_Common_Block(base_st) || 
				//  Stab_Is_Equivalence_Block(base_st)),
				FALSE, FALSE/*as_is*/, ctxt);
      }
      if (fld_path) { TY2F_Free_Fld_Path(fld_path); }
#endif

    }

    if (newContext) {
      ctxt.deleteXlationContext();
      xos << xml::EndElem /* elem_VarRef() */;
    }
  
    
  } /* xlate_SymRef */


  // xlate_MemRef: 
  void
  xlate_MemRef(xml::ostream& xos, 
	       WN*    addr,        // base-address expr
	       TY_IDX addr_ty,     // type of base-address
	       TY_IDX ref_ty,      // type of referenced object
	       STAB_OFFSET offset, // offset from base-address
	       PUXlationContext& ctxt)
  {
    /* Given an address expression and an offset from this address,
     * append a Fortran expression to "xos" to reference an object of
     * type 'ref_ty' at this offset address.  In effect, we dereference
     * the address to 'ref_ty'.
    
     FIXME
     * is a dereferencing operation on the base-address. The resultant
     * value (e.g. after a struct-field access) may be further
     * dereferenced.
     *
     * The address expression is unconditionally treated as an expression
     * of the addr_ty.
     *
     * For non-zero offsets, or when "!WN2F_Can_Assign_Types(ref_ty,
     * TY_pointed(addr_ty))", we expect the base-address to denote the
     * address of a structure or an array, where an object of the given 
     * ref_ty can be found at the given offset.
     *
     * Since Fortran does not have an explicit (only implicit) dereference
     * operation we cannot first calculate the address and then 
     * dereference. This constrains the kind of expression we may handle
     * here.  Note that equivalences and common-blocks always should be 
     * accessed through an LDID or an LDA(?) node.  */
  
    WN* ref_wn = ctxt.getMostRecentWN();
    TY_IDX base_ty = TY_pointed(addr_ty);
    const BOOL deref_fld = ctxt.currentXlationContext().isFlag(XlationContext::DEREF_ADDR);
  
    // -------------------------------------------------------
    //
    // -------------------------------------------------------
    bool constant = (WN_operator(addr) == OPR_LDA 
		     && ST_class(WN_st(addr)) == CLASS_CONST);
    bool newContext = false; 
    if (!constant && !ctxt.currentXlationContext().isFlag(XlationContext::VARREF)) {
      // FIXME: for du_ud numbers; ARRAY, not ILOAD, is top of ref
      WN* wn = ref_wn;
      if (WN_operator(wn) == OPR_ILOAD 
	  && WN_operator(WN_kid0(wn)) == OPR_ARRAY) {
	wn = WN_kid0(wn);
      }
    
      xos << xml::BegElem(XAIFStrings.elem_VarRef())
	  << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	  << xml::Attr("du_ud", ctxt.findUDDUChainId(wn))
	  << xml::Attr("alias", ctxt.getAliasMapKey(wn));
      ctxt.createXlationContext(XlationContext::VARREF); // FIXME: do we need wn?
      newContext = true; 
    }
  
  
    // -------------------------------------------------------
    //
    // -------------------------------------------------------
  
    // FIXME: for now, make sure this is only used for data refs 
    if (TY_kind(base_ty) == KIND_FUNCTION) {
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "memref FIXME");
    }


    /* Prepare to dereference the base-address expression */
    ctxt.currentXlationContext().setFlag(XlationContext::DEREF_ADDR);
  
    if (WN2F_Is_Address_Preg(addr, addr_ty)) { // FIXME
      /* Optimizer may put address PREGS into ARRAYs */
      /* and high level type is more or less useless */
      /* just go with WN tree ADDs etc.              */
      TranslateWN(xos, addr, ctxt);    
      if (offset != 0) {
	xos << '+' << offset /* "%lld" */;
      }
    } 
    else {
      fortTkSupport::ScalarizedRef* sym = ctxt.findScalarizedRef(ref_wn);
      if (!sym && WN_operator(addr) == OPR_STRCTFLD) {
	sym = ctxt.findScalarizedRef(addr);
      }
      if (sym) { 
	// 1. A scalarized symbol
	ST_TAB* sttab = Scope_tab[CURRENT_SYMTAB].st_tab;
	fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
      
	xos << xml::BegElem("xaif:SymbolReference") 
	    << xml::Attr("vertex_id", ctxt.currentXlationContext().getNewVertexId())
	    << xml::Attr("scope_id", scopeid) 
	    << xml::Attr("symbol_id", sym->getName().c_str()) << xml::EndElem;
      } 
      else if (fortTkSupport::ScalarizedRef::isRefScalar(base_ty, ref_ty) 
	       || 
	       fortTkSupport::ScalarizedRef::isRefSimpleScalar(ref_wn)) {
	// 2. Reference to a scalar symbol (==> offset into 'base_st' is zero)
	TranslateWN(xos, addr, ctxt);
      } 
      else if (TY_Is_Array(ref_ty) || TY_Is_Array(base_ty)) { // FIXME
	// 3. Array reference (non-scalar)
	bool hasArrayElmt = !ctxt.currentXlationContext().isFlag(XlationContext::HAS_NO_ARR_ELMT);
	if (TY_Is_Character_String(base_ty)) {
	  TranslateWN(xos, addr, ctxt); /* String lvalue */
	  if (hasArrayElmt) {
	    TY2F_Translate_ArrayElt(xos, base_ty, offset);
	  }
	} 
	else {
	  TranslateWN(xos, addr, ctxt); /* Array */
	  if (hasArrayElmt) {
	    TY2F_Translate_ArrayElt(xos, base_ty, offset);
	  }
	  else {
	    ctxt.currentXlationContext().unsetFlag(XlationContext::HAS_NO_ARR_ELMT);
	  }
	}    
      } 
      else if ((WN_operator(addr) == OPR_LDA || WN_operator(addr) == OPR_LDID) 
	       && (TY_kind(base_ty) != KIND_STRUCT)
	       && (Stab_Is_Common_Block(WN_st(addr)) 
		   || Stab_Is_Equivalence_Block(WN_st(addr)))) {
      
	// 4. A common-block or equivalence-block, both of which we
	// handle only in xlate_SymRef().
	FORTTK_ASSERT_WARN(WN2F_Can_Assign_Types(ST_type(WN_st(addr)), base_ty),
			   "Incompatible types");
      
	if (WN_operator(addr) == OPR_LDA) {
	  ctxt.currentXlationContext().unsetFlag(XlationContext::DEREF_ADDR);
	}
	xlate_SymRef(xos, WN_st(addr), addr_ty, ref_ty,
		     offset + WN_lda_offset(addr) /*offset*/, ctxt);
      }
      else {       
	// 5. Field access (Neither common-block nor equivalence)
	// Find the path to the field we wish to access and append
	// this path to the base-object reference.
      
	/* Get any offset given by an address ADDition node.  The type
	 * of the addition, as given by WN_Tree_Type(), is the type
	 * of base-object within which we are accessing, so the addr_ty
	 * is already set up correctly to handle the combined offsets.
	 */
	TranslateWN(xos, addr, ctxt);

#if 0 // FIXME:REMOVE
	WN_OFFSET tmp = WN2F_Sum_Offsets(addr);
	if (tmp < TY_size(TY_pointed(addr_ty)))
	  offset += tmp;
	else 
	  offset = tmp;
      
	FLD_PATH_INFO* fld_path = TY2F_Get_Fld_Path(base_ty, ref_ty, offset);
	FORTTK_ASSERT_WARN(fld_path != NULL, "Non-existent FLD path");
      
	/* May have ARRAY(ADD(ARRAY(LDA),CONST)) or some such. */
	/* The deepest ARRAY (with the address) is handled     */
	/* by the xlate_ARRAY processing, but the others        */
	/* are field references with array components.         */
	LOC_INFO det(fld_path);
	det.WN2F_Find_And_Mark_Nested_Address(addr);
	addr = det._nested_addr;
      
	/* Get the base expression to precede the path */
	TranslateWN(xos, addr, ctxt);
	TY2F_Fld_Separator(xos);
      
	/* Append the path-name, perhaps w/o array subscripts. */
	if (fld_path != NULL) {
	  TY2F_Translate_Fld_Path(xos, fld_path, deref_fld, 
				  FALSE/*common*/, FALSE/*as_is*/, ctxt);
	  TY2F_Free_Fld_Path(fld_path);
	} 
	else {
	  xos << "field-at-offset=" << offset /* %lld */;
	}
#endif
      }    
    }

    if (newContext) {
      ctxt.deleteXlationContext();
      xos << xml::EndElem /* elem_VarRef() */;
    }

    
  } /* xlate_MemRef */


  void 
  LOC_INFO::WN2F_Find_And_Mark_Nested_Address(WN * addr)
  {
    /* If this address expression contains nested ARRAY nodes */
    /* (and isn't a character expression), the ARRAYs refer   */
    /* to structure components, eg: aaa(1).kkk(3) yields      */
    /* ARRAY(ADD(const,ARRAY(LDA)). Add a pointer to the      */
    /* array elements of the fld path, associating each with  */
    /* corresponding OPC_ARRAY. TY2F_Translate_Fld_Path will  */
    /* write the subscript list.                              */

    /* In general, just the lowest LDID/LDA remains to be     */
    /* processed, however if the lowest ARRAY node is not a   */
    /* fld, and belongs to the address ST, then return that   */
    /* ARRAY.                                                 */
    OPERATOR opr = WN_operator(addr);
    switch (opr)
      {
      case OPR_ARRAY: 
      case OPR_ARRAYEXP: 
      case OPR_ARRSECTION:
	{
	  WN * kid;     

	  if (WN_operator(addr)==OPR_ARRAYEXP)
	    addr = WN_kid0(addr);

	  kid = WN_kid0(addr);
	  WN2F_Find_And_Mark_Nested_Address(kid);

	  if ((_flds_left && _flds_left->arr_elt) &&
	      (!(_base_is_array)))
	    {
	      _flds_left-> arr_wn = addr;
	      _flds_left = TY2F_Point_At_Path(_flds_left,_off);
	    } 
	  else 
	    _nested_addr = addr;

	  _base_is_array = FALSE;
	}
	break;


      case OPR_ADD:
	{
	  WN * cnst = WN_kid0(addr);
	  WN * othr = WN_kid1(addr);

	  if (WN_operator(cnst) != OPR_INTCONST) 
	    {
	      cnst = WN_kid1(addr);
	      othr = WN_kid0(addr);
	    }
	  WN2F_Find_And_Mark_Nested_Address(othr);
	  _off = WN_const_val(cnst);
	  _flds_left = TY2F_Point_At_Path(_flds_left,_off);
	  _base_is_array = FALSE;
	}
	break;

      case OPR_LDID:
	_off = 0;
	_nested_addr = addr;
	_flds_left = TY2F_Point_At_Path(_flds_left,_off);
	_base_is_array = ((TY_kind(WN_ty(addr)) == KIND_POINTER) && 
			  (TY_kind(TY_pointed(WN_ty(addr))) == KIND_ARRAY));
	break;

      case OPR_LDA:
	_off = WN_lda_offset(addr);
	_nested_addr = addr;
	_flds_left = TY2F_Point_At_Path(_flds_left,_off);
	_base_is_array = ((TY_kind(WN_ty(addr)) == KIND_POINTER) && 
			  (TY_kind(TY_pointed(WN_ty(addr))) == KIND_ARRAY));
	break;

      case OPR_ILOAD:
	_off = 0;
	_nested_addr = addr;
	_flds_left = TY2F_Point_At_Path(_flds_left,0);
	_base_is_array = ((TY_kind(WN_ty(addr)) == KIND_POINTER) && 
			  (TY_kind(TY_pointed(WN_ty(addr))) == KIND_ARRAY));
	break;

      default:
	FORTTK_DIE(fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
	break;
      }
    return;
  }


  WN_OFFSET
  WN2F_Sum_Offsets(WN *addr)
  {
    /* Accumulate any offsets (ADDs) in this address   */
    /* tree. Used for computing Fld paths              */
    BOOL sum = 0;

    switch (WN_operator(addr)) {
    case OPR_ARRAY: 
    case OPR_ARRAYEXP:
    case OPR_ARRSECTION:
      if (WN_operator(addr)==OPR_ARRAYEXP)
	addr = WN_kid0(addr);
    
      sum += WN2F_Sum_Offsets(WN_kid0(addr));
      break;
    
    case OPR_ADD:
      sum += WN2F_Sum_Offsets(WN_kid0(addr));
      sum += WN2F_Sum_Offsets(WN_kid1(addr));
      break;
    
    case OPR_INTCONST:
      sum = WN_const_val(addr);
      break;

    default: 
      break; // fall through
    }
    return sum;
  }


  void 
  WN2F_Address_Of(xml::ostream& xos)
  {
    //REMOVE Prepend_Token_Special(xos, '(');
    //REMOVE Prepend_Token_String(xos, "loc%");
    xos << "loc%()"; // FIXME
  }

  DGraphNodeVec*
  SortDGraphNodes(OA::OA_ptr<OA::DGraph::DGraphInterface> g)
  {
    DGraphNodeVec* vec = new DGraphNodeVec(g->getNumNodes());

    OA::OA_ptr<OA::DGraph::NodesIteratorInterface> it = g->getNodesIterator();
    for (int i = 0; it->isValid(); ++(*it), ++i) {
      (*vec)[i] = it->current();
    }
  
    // Sort by id (ascending)
    //std::sort(vec->begin(), vec->end(), (*(g->getNodeCompare())));
    std::sort(vec->begin(), vec->end(), OA::DGraph::lt_Node());
  
    return vec;
  }

  DGraphEdgeVec*
  SortDGraphEdges(OA::OA_ptr<OA::DGraph::DGraphInterface> g)
  {
    DGraphEdgeVec* vec = new DGraphEdgeVec(g->getNumEdges());

    OA::OA_ptr<OA::DGraph::EdgesIteratorInterface> it = g->getEdgesIterator();
    for (int i = 0; it->isValid(); ++(*it), ++i) {
      (*vec)[i] = it->current();
    }
  
    // Sort by source/target node ids (ascending)
    std::sort(vec->begin(), vec->end(), OA::DGraph::lt_Edge()); 
  
    return vec;
  }


  CFGEdgeVec*
  SortCFGEdges(OA::OA_ptr<OA::CFG::CFGInterface> g)
  {
    CFGEdgeVec* vec = new CFGEdgeVec(g->getNumEdges());

    OA::OA_ptr<OA::DGraph::EdgesIteratorInterface> it = g->getEdgesIterator();
    for (int i = 0; it->isValid(); ++(*it), ++i) {
      (*vec)[i] = it->current().convert<OA::CFG::Edge>();
    }
  
    // Sort by source/target node ids (ascending)
    std::sort(vec->begin(), vec->end(), lt_CFGEdge()); 
  
    return vec;
  }


  // DumpGraphEdge: see header.
  void 
  DumpGraphEdge(xml::ostream& xos, const char* nm, 
		UINT eid, UINT srcid, UINT targid, UINT pos)
  {
    xos << xml::BegElem(nm) 
	<< xml::Attr("edge_id", eid) 
	<< xml::Attr("source", srcid) << xml::Attr("target", targid);
    if (pos >= 1) {
      xos << xml::Attr("position", pos);
    }
    xos << xml::EndElem;
  }


  // DumpCFGraphEdge: Dump a CFG edge
  static void
  DumpCFGraphEdge(xml::ostream& xos, UINT eid, 
		  OA::OA_ptr<OA::CFG::EdgeInterface> edge)
  {
    using namespace OA::CFG;

    OA::OA_ptr<OA::DGraph::NodeInterface> dn1 = edge->getSource();
    OA::OA_ptr<OA::CFG::Node> n1 = dn1.convert<OA::CFG::Node>();
    OA::OA_ptr<OA::DGraph::NodeInterface> dn2 = edge->getSink();
    OA::OA_ptr<OA::CFG::Node> n2 = dn2.convert<OA::CFG::Node>();
  
    pair<bool, INT64> ret = GetCFGEdgeCondVal(edge);
    bool hasCondVal = ret.first;
    INT64 condVal = ret.second;

    xos << xml::BegElem("xaif:ControlFlowEdge") 
	<< xml::Attr("edge_id", eid) 
	<< xml::Attr("source", n1->getId()) << xml::Attr("target", n2->getId());
    if (hasCondVal) {
      xos << xml::Attr("has_condition_value", "true")
	  << xml::Attr("condition_value", condVal);
    }
    xos << xml::EndElem;
  }

  typedef std::set<ST *> SymbolPointerSet;

  void 
  xlate_SideEffectLocationPrint(ST* st, 
				SymbolPointerSet& coveredSymbols,
				fortTkSupport::SymTabId scopeid,
				xml::ostream& xos) {
    if (coveredSymbols.find(st)==coveredSymbols.end())
      coveredSymbols.insert(st);
    else { 
      const char* nm=ST_name(st);
      FORTTK_MSG(2, "xlate_SideEffectLocationPrint: ignoring duplicate symbol " << nm);
      return; 
    }
    // the wrapper for the VariableReference: 
    xos << xml::BegElem("xaif:SideEffectReference")
	<< xml::Attr("vertex_id", "1");
  
    // the contents, i.e. the SymbolReference
    xos << xml::BegElem("xaif:SymbolReference")
	<< xml::Attr("vertex_id", "1")
	<< xml::Attr("scope_id", scopeid) 
	<< AttrSymId(st)
	<< xml::EndElem;
    xos << xml::EndElem;
  } 


  void 
  xlate_SideEffectNamedLocation(OA::OA_ptr<OA::NamedLoc> theNamedLoc,
				SymbolPointerSet& coveredSymbols,
				xml::ostream& xos, 
				WN *wn, 
				PUXlationContext& ctxt,
				OA::OA_ptr<OA::SymHandleIterator> formalArgSymHandleI) 
  { 
    // OA may include constants in the location lists.  We filter them
    // out because they are not in the XAIF symbol table.
    ST* st = (ST*)theNamedLoc->getSymHandle().hval();
    ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
    fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
    // this is a temporary fix to a problem in the analysis 
    // for the following case: 
    // a symbol SY occurs in the sideffect list of routine R
    // as a consequence of R calling FOO, SY is declared within 
    // FOO (i.e. ST_level is 2) and referenced in BAR that is 
    // contained in FOO.  Because SY is from the enclosing scope
    // it is considered "not" local to BAR 
    // (see also comments in Open64IRInterface.cpp:3715 ff.)
    // Presumably somewhere in OA then the assumption is made 
    // that anything not local is global and it ends up 
    // in the R's side effect list. To be global, however,
    // it would have to have ST_level 1 or at least 
    // a level higher than the current PU. If it is not 
    // we will assume the above case and just return.
    // A similar problem also occurs when sideeffects are 
    // transferred from a module procedure to the place where the 
    // module is used. 
    UINT32 anIndex=ST_index(st);
    // we may not even have that index because the symbol comes from a large symbol table:
    if (anIndex>=sttab->Size()){ 
      ST* puStP = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
      const char* puName = ST_name(puStP);
      FORTTK_WMSG("xlate_SideEffectNamedLocation: ignoring symbol " << ST_name(st) << " (symbol index " << anIndex << " is out of bounds of current symbol table indicating it is invisible in " << puName << ")"); 
      return;
    }
    // if the symbol is visible we should find it in sttab via level/index:
    ST& anotherST=St_Table[ST_st_idx(*st)];
    if (&anotherST != st) { 
      ST* puStP = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
      const char* puName = ST_name(puStP);
      FORTTK_WMSG("xlate_SideEffectNamedLocation: ignoring symbol " << ST_name(st) << " (resolves in the current symbol table to " << ST_name(&anotherST) << " indicating it is invisible in " << puName << ")"); 
      return;
    }
    if (ST_class(st) == CLASS_CONST) {
      return;
    }

    if (theNamedLoc->isLocal()) { 
      // we don't want local variables except 
      // if it is a formal parameter
      bool foundAsFormalArgument=false;
      formalArgSymHandleI->reset();
      while (formalArgSymHandleI->isValid()) { 
	//       // JU: debugging prepare begin
	//       const char* nm=ST_name(st);
	//       ST* st1=(ST*)formalArgSymHandleI->current().hval();
	//       const char* nm1=ST_name(st1);
	//       // JU: debugging prepare end
	if (formalArgSymHandleI->current()==theNamedLoc->getSymHandle()) { 
	  // is a formal paramter
	  foundAsFormalArgument=true;
	  // 	// JU: debugging begin
	  // 	std::cout << "xlate_SideEffectNamedLocation: matched " << nm << " with " << nm1 << std::endl;  
	  // 	// JU: debugging end
	  break;
	}
	//       // JU: debugging begin
	//       else { 
	// 	std::cout << "xlate_SideEffectNamedLocation: did not match " << nm << " with " << nm1 << std::endl;  
	//       }
	//       // JU: debugging end
	++(*formalArgSymHandleI);
      }
      if (!foundAsFormalArgument) { 
	return;
      }
    }
    else { 
      ST_IDX stLevel=ST_level(st), puStLevel=PU_lexical_level(ST_ptr(PU_Info_proc_sym(Current_PU_Info)));
      bool inCurrentSt=false;
      // third spot of weeding out invisible symbols: 
      if (stLevel==puStLevel) { 
	// see if we can find it...
	for (INT i = 1; 
	     i < ST_Table_Size(stLevel) ; 
	     ++i) { 
	  // get the symbol from the table
	  ST* an_ST_p=&(St_Table(stLevel,i));
	  if (an_ST_p==st) {// same (not just equal) instance
	    inCurrentSt=true; 
	    break; 
	  }
	}
      }
      if((stLevel==puStLevel && !inCurrentSt) || stLevel>puStLevel) { 
	ST* puStP = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
	const char* puName = ST_name(puStP);
	FORTTK_WMSG("xlate_SideEffectNamedLocation: ignoring symbol " << ST_name(st) << " (nesting level " << (unsigned short)stLevel << ") which is invisible in " << puName << " (nesting level " << (unsigned short)puStLevel << ")"); 
	return; 
      }
    } 
    xlate_SideEffectLocationPrint(st,coveredSymbols,scopeid, xos);
  }

  void 
  xlate_SideEffectEntry(OA::OA_ptr<OA::Location> theTopLocation,
			OA::OA_ptr<OA::Location> theLocation,
			SymbolPointerSet& coveredSymbols,
			xml::ostream& xos, 
			WN *wn, 
			PUXlationContext& ctxt,
			OA::OA_ptr<OA::SymHandleIterator> formalArgSymHandleI) { 
    if (theLocation->isaNamed()) { 
      // get the named location
      OA::OA_ptr<OA::NamedLoc> namedLoc=
	theLocation.convert<OA::NamedLoc>();
      xlate_SideEffectNamedLocation(namedLoc,
				    coveredSymbols,
				    xos, 
				    wn, 
				    ctxt,
				    formalArgSymHandleI);
    }
    else if (theLocation->isaInvisible()) { 
      // get the invisible location's symbol
      OA::OA_ptr<OA::InvisibleLoc> theInvisibleLoc=
	theLocation.convert<OA::InvisibleLoc>();
      ST* st = (ST*)theInvisibleLoc->getBaseSym().hval();
      ST_TAB* sttab = Scope_tab[ST_level(st)].st_tab;
      fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
      xlate_SideEffectLocationPrint(st,coveredSymbols,scopeid, xos);
    }
    else if (theLocation->isaSubSet()) { 
      OA::OA_ptr<OA::LocSubSet> subSetLoc=
	theLocation.convert<OA::LocSubSet>();
      xlate_SideEffectEntry(theTopLocation,
			    subSetLoc->getLoc(),
			    coveredSymbols,
			    xos, 
			    wn, 
			    ctxt,
			    formalArgSymHandleI);
    }
    else if (theLocation->isaUnnamed()) { 
      OA::OA_ptr<OA::UnnamedLoc> theUnnamedLoc=
	theLocation.convert<OA::UnnamedLoc>();
      // save the context because "toString" may change it
      PU_Info* thisPU=Current_PU_Info;
      WN* wn_p((WN*)theUnnamedLoc->getExprHandle().hval());
      if (wn_p 
	  && 
	  (WN_operator(wn_p)==OPR_CONST
	   ||
	   WN_operator(wn_p)==OPR_INTCONST
	   ||
	   (WN_has_sym(wn_p) 
	    &&
	    ST_class(WN_st(wn_p))==CLASS_CONST))) { 
	FORTTK_MSG(2, "xlate_SideEffectEntry: side effect list contains an unnamed location for: " << ctxt.getIrInterface().toString(theUnnamedLoc->getExprHandle()));
      }
      else { 
	FORTTK_MSG(1, "xlate_SideEffectEntry: side effect list contains an unnamed location for: " << ctxt.getIrInterface().toString(theUnnamedLoc->getExprHandle()));
      }
      if (Current_PU_Info!=thisPU) 
        PU_SetGlobalState(thisPU);
    } 
    else if (theLocation->isaUnknown()) { 
      ST* pu_st = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
      const char* pu_nm = ST_name(pu_st);
      FORTTK_MSG(2,"xlate_SideEffectEntry: side effect list for " << pu_nm << " contains an unknown location.");
    } 
    else { 
      ST* pu_st = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
      const char* pu_nm = ST_name(pu_st);
      FORTTK_MSG(0,"xlate_SideEffectEntry: descended from top location:");
      theTopLocation->dump(std::cerr);
      FORTTK_MSG(0,"xlate_SideEffectEntry: down to locaton:");
      theLocation->dump(std::cerr);
      FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "side effect list for " << pu_nm << " contains an unexpected location (see location dumps above)");
    }
  } 

  /**
   * for a give wn descend recursively down the tree and 
   * collect all the variable symbols that are referenced
   */
  static void findVarSymbolsInTree(WN *wn, 
				   SymbolPointerSet& requiredSymbols) {
    if (WN_has_sym(wn)) { 
      ST* st=WN_st(wn); 
      if ( ST_class(st)==CLASS_VAR && !ST_is_temp_var(st))
	// we need to avoid picking up things like integer conversion calls
	requiredSymbols.insert(st);
    }
    for (INT kid = 0; kid < WN_kid_count(wn); kid++) { 
      WN *kidWN_p=WN_kid(wn,kid);
      if (kidWN_p)
	findVarSymbolsInTree(kidWN_p,
			     requiredSymbols);
    }
  } 

  /**
   * look for the definitions of the temp symbols 
   * in the program and collect the symbols 
   * the respective RHSs are referring to
   */
  static void findRequiredProgramSymbols(WN* wn,
					 PUXlationContext& ctxt,
					 SymbolPointerSet& requiredTempSymbols,
					 SymbolPointerSet& requiredProgramSymbols) { 
    FORTTK_ASSERT(WN_operator(wn) == OPR_FUNC_ENTRY, fortTkSupport::Diagnostics::UnexpectedInput);
    // go to the body
    WN* theLastBlock=WN_kid(wn,WN_kid_count(wn)-1);
    FORTTK_ASSERT(WN_operator(theLastBlock)==OPR_BLOCK, fortTkSupport::Diagnostics::UnexpectedInput); 
    if (WN_first(theLastBlock)==0) 
      return;
    WN* childWN=WN_first(theLastBlock);
    while (childWN!=0) { 
      OPERATOR childOpr=WN_operator(childWN);
      if (childOpr==OPR_STID) { 
	ST* st=WN_st(childWN); 
	if (requiredTempSymbols.find(st)!=requiredTempSymbols.end()) { 
	  findVarSymbolsInTree(childWN,
			    requiredProgramSymbols);
	  requiredTempSymbols.erase(st);
	}
      } 
      childWN=WN_next(childWN);
    }
  } 


  /**
   * look at a symbol table entry and
   * find symbols used for dimensions used 
   * in array declarations
   */
  class SearchSymbolTableEntry {
  public:
    SearchSymbolTableEntry(PUXlationContext& ctxt,
			   SymbolPointerSet& requiredSymbols) :
      myCtxt(ctxt),
      myRequiredSymbols(requiredSymbols){ 
    } 

    /** 
     * the operator referenced by For_all
     * in findRequiredSymbolsInSymbolTable
     */
    void operator()(UINT32 idx, ST* st) const {
      // must be formal parameter of local variable declaration
      if (ST_class(st)!=CLASS_VAR)
	return;
      TY_IDX tyIdx=ST_type(st); 
      TY_KIND tyKind=TY_kind(tyIdx);
      // must be a local array or a pointer to an array (for formal parameters)
      // except for strings
      if (tyKind!=KIND_ARRAY && tyKind!=KIND_POINTER
	  ||
	  TY_Is_Character_String(tyIdx)) 
	return; 
      while (tyKind==KIND_POINTER) { 
	tyIdx=TY_pointed(tyIdx);
	tyKind=TY_kind(tyIdx);
	if (tyKind!=KIND_POINTER) { 
	  if (tyKind==KIND_ARRAY
	      && 
	      !TY_Is_Character_String(tyIdx))
	    break; 
	  else
	    return; 
	}
      }
      // now we are looking at an array and can search the array bounds
      TY& ty = Ty_Table[tyIdx];
      ARB_HANDLE arbBaseHandle = TY_arb(ty);
      INT32 dim = ARB_dimension(arbBaseHandle) ;
      INT32 coDim = ARB_co_dimension(arbBaseHandle);
      FORTTK_ASSERT(coDim == 0,
		    fortTkSupport::Diagnostics::UnexpectedInput);      
      for (int i=0; i<dim; i++) {
	// lbound: 
	if (!ARB_const_lbnd(arbBaseHandle[i])
	    && 
	    !ARB_empty_lbnd(arbBaseHandle[i])) {
	  // get the array bounds entry
	  ST_IDX stIdx=ARB_lbnd_var(arbBaseHandle[i]);
	  if (stIdx!=0) { 
	    myRequiredSymbols.insert(&(St_Table[stIdx]));
	  } 
	}
	// ubound: 
	if (!ARB_const_ubnd(arbBaseHandle[i])
	    && 
	    !ARB_empty_ubnd(arbBaseHandle[i])) {
	  // get the array bounds entry
	  ST_IDX stIdx=ARB_ubnd_var(arbBaseHandle[i]);
	  if (stIdx!=0) { 
	    myRequiredSymbols.insert(&(St_Table[stIdx]));
	  }
	}
	// stride: 
	if (!ARB_const_stride(arbBaseHandle[i])
	    && 
	    !ARB_empty_stride(arbBaseHandle[i])) { 
	  // get the array bounds entry
	  ST_IDX stIdx=ARB_stride_var(arbBaseHandle[i]);
	  if (stIdx!=0) { 
	    myRequiredSymbols.insert(&(St_Table[stIdx]));
	  }
	}
      }
    }
  
  private:
    SYMTAB_IDX      mySymtab;
    PUXlationContext& myCtxt;  
    SymbolPointerSet& myRequiredSymbols;
  };

  /**
   * search the symbol table 
   * and find symbols used for dimensions used 
   * in array declarations
   */
  static void findRequiredSymbolsInSymbolTable(SYMTAB_IDX symtab_lvl, 
					       PUXlationContext& ctxt,
					       SymbolPointerSet& requiredSymbols) {
    For_all(St_Table, symtab_lvl, SearchSymbolTableEntry(ctxt,
							 requiredSymbols));
  }


  // xlate_EntryPoint: Translates a function entry or alternate entry
  // point, with parameter declarations.  
  // FIXME: XAIF doesn't support alt-entry.
  static void
  xlate_EntryPoint(xml::ostream& xos, WN *wn, PUXlationContext& ctxt) {
    OPERATOR opr = WN_operator(wn);
    FORTTK_ASSERT(opr == OPR_ALTENTRY || opr == OPR_FUNC_ENTRY,
		  fortTkSupport::Diagnostics::UnexpectedInput);
  
    ST* func_st = &St_Table[WN_entry_name(wn)];
    TY_IDX func_ty = ST_pu_type(func_st);
    TY_IDX return_ty = Func_Return_Type(func_ty);
  
    // Accumulate the parameter ST entries  // FIXME: GetParamSymHandleSet
    INT nparam = (opr == OPR_ALTENTRY) ? WN_kid_count(wn) : WN_num_formals(wn);
    ST** params_st = (ST **)alloca((nparam + 1) * sizeof(ST *));  
    for (INT parm = 0; parm < nparam; parm++) {
      params_st[parm] = WN_st(WN_formal(wn, parm));
    }
    params_st[nparam] = NULL; // terminate the list

    // Parameter name-list. Skip any implicit "length" parameters
    // associated with character strings.  Such implicit parameters
    // should be at the end of the parameter list. FIXME
    xos << xml::BegElem("xaif:ArgumentList");
  
    INT first_parm = ST2F_FIRST_PARAM_IDX(func_ty);
    INT implicit_parms = 0;
    UINT position = 1;
    for (INT parm = first_parm; parm < (nparam - implicit_parms); parm++) {

      WN* parm_wn = WN_formal(wn, parm);    
      ST* parm_st = params_st[parm]; //WN_st(parm_wn);
    
      if (!ST_is_return_var(parm_st)) {
	ST_TAB* sttab = Scope_tab[ST_level(parm_st)].st_tab;
	fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
	xos << xml::BegElem("xaif:ArgumentSymbolReference")
	    << xml::Attr("position", position) 
	    << xml::Attr("scope_id", scopeid) 
	    << AttrSymId(parm_st)
	    << xml::Attr("intent", xlate_intent(parm_wn))
	    << WhirlIdAnnot(ctxt.findWNId(parm_wn))
	    << xml::EndElem;
	position++;
      }
    
      if (STAB_PARAM_HAS_IMPLICIT_LENGTH(parm_st)) {
	implicit_parms++;
      
	/* FIXME: is function returning character_TY? if length follows */
	/* address - skip over it, but account for ',' in arg list */
	if ( ((parm == first_parm) && (params_st[parm+1] != NULL)) 
	     && (ST_is_value_parm(parm_st) 
		 && ST_is_value_parm(params_st[parm+1]))
	     && (return_ty != (TY_IDX)0 && TY_kind(return_ty) == KIND_VOID) ) {
	  parm++;
	  params_st[parm] = NULL; 
	  implicit_parms--;
	}
      }
    }
  
    xos << xml::EndElem /* xaif:ArgumentList */;

    // add the side effect lists here: 
    // the analysis result: 
    OA::OA_ptr<OA::SideEffect::InterSideEffectStandard> interSideEffects=
      Whirl2Xaif::getOAAnalMap().getInterSideEffect();

    // an iterator over locations: 
    OA::OA_ptr<OA::LocIterator> anOALocIterOAPtr;
    OA::ProcHandle proc((OA::irhandle_t)Current_PU_Info);

    // symbol handle iterator from parameter bindings to distinguish the formal parameters 
    // from the strictly local variables 
    OA::OA_ptr<OA::SymHandleIterator> symHandleI=Whirl2Xaif::getOAAnalMap().
      getParamBindings()->getFormalIterator(proc);

    //   // begin debugging stuff
    //   ST* st = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
    //   const char* nm = ST_name(st);
    //   if (!symHandleI->isValid())
    //     std::cout << "Note: in xlate_EntryPoint empty symHandleI for " << nm << std::endl;
    //   else 
    //     std::cout << "Note: in xlate_EntryPoint non-empty symHandleI for " << nm << std::endl;
    //   // end debugging stuff

    symHandleI->reset();
    xos << xml::BegElem("xaif:ModLocal");
    anOALocIterOAPtr = interSideEffects->getLMODIterator(proc);
    SymbolPointerSet coveredSymbols;
    for ( ; anOALocIterOAPtr->isValid(); ++(*anOALocIterOAPtr) ) {
      xlate_SideEffectEntry(anOALocIterOAPtr->current(),
			    anOALocIterOAPtr->current(), 
			    coveredSymbols,
			    xos, 
			    wn, 
			    ctxt, 
			    symHandleI);
    }
    xos << xml::EndElem; // xaif:ModLocal
    coveredSymbols.clear();
    symHandleI->reset();
    xos << xml::BegElem("xaif:Mod");
    anOALocIterOAPtr = interSideEffects->getMODIterator(proc);
    for ( ; anOALocIterOAPtr->isValid(); ++(*anOALocIterOAPtr) ) {
      xlate_SideEffectEntry(anOALocIterOAPtr->current(),
			    anOALocIterOAPtr->current(), 
			    coveredSymbols,
			    xos, 
			    wn, 
			    ctxt, 
			    symHandleI);
    }
    xos << xml::EndElem; // xaif:ModLocal
    coveredSymbols.clear();
    symHandleI->reset();
    xos << xml::BegElem("xaif:ReadLocal");
    anOALocIterOAPtr = interSideEffects->getLUSEIterator(proc);
    for ( ; anOALocIterOAPtr->isValid(); ++(*anOALocIterOAPtr)) {
      xlate_SideEffectEntry(anOALocIterOAPtr->current(),
			    anOALocIterOAPtr->current(), 
			    coveredSymbols,
			    xos, 
			    wn, 
			    ctxt, 
			    symHandleI);
    }
    xos << xml::EndElem; // xaif:ModLocal
    coveredSymbols.clear();
    symHandleI->reset();
    xos << xml::BegElem("xaif:Read");
    anOALocIterOAPtr = interSideEffects->getUSEIterator(proc);
    for ( ; anOALocIterOAPtr->isValid(); ++(*anOALocIterOAPtr)) {
      xlate_SideEffectEntry(anOALocIterOAPtr->current(),
			    anOALocIterOAPtr->current(), 
			    coveredSymbols,
			    xos, 
			    wn, 
			    ctxt, 
			    symHandleI);
    }
    xos << xml::EndElem; // xaif:ModLocal
    // populate the onEntry list: 
    SymbolPointerSet requiredTempSymbols; 
    findRequiredSymbolsInSymbolTable(CURRENT_SYMTAB, 
				     ctxt,
				     requiredTempSymbols);
    // now look at all the variables we found which 
    // presumably are all temporaries like t__1, t__2, ...
    // which are defined somewhere in the beginning of 
    // the body
    SymbolPointerSet requiredProgramSymbols;
    findRequiredProgramSymbols(wn,
			       ctxt,
			       requiredTempSymbols,
			       requiredProgramSymbols);
    if (!requiredTempSymbols.empty()) {
      ST* st = ST_ptr(PU_Info_proc_sym(Current_PU_Info));
      const char* puName = ST_name(st);
      const char* symbolName = ST_name(*(requiredTempSymbols.begin()));
      FORTTK_MSG(1,"cannot find a definition for temporary symbol \"" << symbolName << "\" in " << puName);
    }
    FORTTK_ASSERT(requiredTempSymbols.empty(),fortTkSupport::Diagnostics::UnexpectedInput << " not all symbols required in local declarations have a definition"); 
    if (!requiredProgramSymbols.empty()) { 
      coveredSymbols.clear();
      xos << xml::BegElem("xaif:OnEntry");
      for (SymbolPointerSet::iterator si=requiredProgramSymbols.begin();
	   si!=requiredProgramSymbols.end();
	   ++si) { 
	ST_TAB* sttab = Scope_tab[ST_level(*si)].st_tab;
	fortTkSupport::SymTabId scopeid = ctxt.findSymTabId(sttab);
	xlate_SideEffectLocationPrint(*si,
				      coveredSymbols,
				      scopeid,
				      xos);
      }
      xos << xml::EndElem; // xaif:OnEntry
    }
  }

  // GetParamSymHandleSet: Return a set of SymHandles representing the
  // parameters of the OPR_FUNC_ENTRY.
  static set<OA::SymHandle>* 
  GetParamSymHandleSet(WN* wn_pu)
  {
    // Accumulate the ST* for parameters
    set<OA::SymHandle>* params = new set<OA::SymHandle>;
    INT nparam = WN_num_formals(wn_pu);
    for (int i = 0; i < nparam; ++i) {
      ST* st = WN_st(WN_formal(wn_pu, i));
      params->insert(OA::SymHandle((OA::irhandle_t)st));
    }
    return params;
  }

  static const char*
  xlate_intent(WN* parm)
  {
    // Note: WN_parm flags are typically not set 
    // WN_parm_flag(parm), WN_Parm_Dummy(parm)
    ST* st = WN_st(parm);
    if (ST_is_intent_in_argument(st) || WN_Parm_In(parm)) {
      return "in";
    } 
    else if (ST_is_intent_out_argument(st) || WN_Parm_Out(parm)) {
      return "out";
    }
    else { // WN_Parm_By_Reference(parm)
      return "inout"; 
    }
  }

  // xlate_BBStmt: Given a statement within an XAIF basic block,
  // properly translate it.
  static void
  xlate_BBStmt(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    if (!wn) { return; }
  
    // If a structured statement, it must be translated specially.
    // Otherwise simply dispatch to TranslateWN(...).
    const char* vty = fortTkSupport::GetCFGControlFlowVertexType(wn);
    OPERATOR opr = WN_operator(wn);
    const char* opr_str = OPERATOR_name(opr);
  
    if (vty == XAIFStrings.elem_BBForLoop()) {
      if (WN_operator(WN_end(wn))==OPR_NE) { 
	FORTTK_WMSG("xlate_BBStmt: loop with variable stride");
      }
      xlate_LoopInitialization(xos, WN_start(wn), ctxt);
      xlate_CFCondition(xos, WN_end(wn), ctxt);
      xlate_LoopUpdate(xos, WN_step(wn), ctxt);
    } 
    else if (vty == XAIFStrings.elem_BBPostLoop() ||
	     vty == XAIFStrings.elem_BBPreLoop()) {
      xlate_CFCondition(xos, WN_while_test(wn), ctxt);
    }
    else if (vty == XAIFStrings.elem_BBBranch()) {
      WN* condWN = NULL;
      if (opr == OPR_IF || opr == OPR_TRUEBR || opr == OPR_FALSEBR) {
	condWN = WN_if_test(wn);
      } 
      else if (opr == OPR_SWITCH || opr == OPR_COMPGOTO) {
	condWN = WN_switch_test(wn);
      }
      FORTTK_ASSERT(condWN, fortTkSupport::Diagnostics::UnexpectedOpr << OPERATOR_name(opr));
      xlate_CFCondition(xos, condWN, ctxt);
    } 
    else if (vty == XAIFStrings.elem_BBEndBranch() ||
	     vty == XAIFStrings.elem_BBEndLoop()) {
      // skip bogus comment statement
      //xos << Comment(vty);
    }
    else {
      if (IsActiveStmt(wn, ctxt) || (opr==OPR_STID && ! ST_is_temp_var(WN_st(wn)))) {
	TranslateWN(xos, wn, ctxt);
      } 
      else {
	xlate_PassiveStmt(xos, wn, ctxt);
      }
    }
  }


  // xlate_CFCondition: Translate the BB's control flow condition (Loops, Ifs)
  static void 
  xlate_CFCondition(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    xos << xml::BegElem("xaif:Condition");
    ctxt.createXlationContext();
    TranslateWN(xos, wn, ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
  }


  // xlate_LoopInitialization: 
  static void 
  xlate_LoopInitialization(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    xos << xml::BegElem("xaif:Initialization")
	<< xml::Attr("statement_id", ctxt.findWNId(wn))
        << xml::Attr("do_chain", ctxt.findDoChainId(wn));
    ctxt.createXlationContext(XlationContext::ASSIGN); // implicit for this element
    TranslateWN(xos, wn, ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
  }

  // xlate_LoopUpdate: 
  static void 
  xlate_LoopUpdate(xml::ostream& xos, WN *wn, PUXlationContext& ctxt)
  {
    xos << xml::BegElem("xaif:Update")
	<< xml::Attr("statement_id", ctxt.findWNId(wn))
        << xml::Attr("do_chain", ctxt.findDoChainId(wn));
    ctxt.createXlationContext(XlationContext::ASSIGN); // implicit for this element
    TranslateWN(xos, wn, ctxt);
    ctxt.deleteXlationContext();
    xos << xml::EndElem;
  }

  // GetLoopReversalType:
  static const char*
  GetLoopReversalType(OA::OA_ptr<OA::CFG::CFGInterface> cfg, 
		      OA::OA_ptr<OA::CFG::NodeInterface> n)
  {
    const char* loopTy = "anonymous";

    // Find the WN corresponding to xaif:ForLoop
    WN* loopWN = NULL;
    OA::OA_ptr<OA::CFG::NodeStatementsIteratorInterface> stmtIt
      = n->getNodeStatementsIterator();
    for (; stmtIt->isValid(); ++(*stmtIt)) {
      OA::StmtHandle st = stmtIt->current();
      WN* wstmt = (WN*)st.hval();
      const char* vty = fortTkSupport::GetCFGControlFlowVertexType(wstmt);
      if (vty == XAIFStrings.elem_BBForLoop()) { 
	loopWN = wstmt;
	break;
      }
    }
  
    FORTTK_ASSERT(loopWN, "Could not find WN corresponding to xaif:ForLoop");

    // Check for a PRAGMA  right before the loop node
    // but skip possible STID nodes assigning temporaries
    // that the front-end may have inserted between the 
    // loop node and the pragma: 
    WN* prevWN_p=WN_prev(loopWN); 
    while (prevWN_p 
	   && 
	   WN_operator(prevWN_p) == OPR_STID
	   && 
	   ST_is_temp_var(WN_st(prevWN_p))) 
      prevWN_p=WN_prev(prevWN_p);
    if (prevWN_p && WN_operator(prevWN_p) == OPR_PRAGMA) {
      WN_PRAGMA_ID prag = (WN_PRAGMA_ID)WN_pragma(prevWN_p);
      if (prag == WN_PRAGMA_OPENAD_XXX) {
	static const char* TXT = "\"simple loop";
	const char* txt = Targ_Print(NULL, WN_val(prevWN_p)); // CLASS_CONST
	if (strncasecmp(txt, TXT, strlen(TXT)) == 0) {
	  loopTy = "explicit";
	}
      }
    }

    if (Args::ourSimpleLoopFlag)
      loopTy = "explicit";

    return loopTy;
  }

  // GetIDsForStmtsInBB: Returns a colon separated list for ids of
  // statements within the basic block.  In the event that a statement
  // id maps to zero, it is *not* included in the list.
  static std::string
  GetIDsForStmtsInBB(OA::OA_ptr<OA::CFG::NodeInterface> node, 
		     PUXlationContext& ctxt)
  {
    using namespace OA::CFG;
  
    std::string idstr;
    bool emptystr = true;
  
    OA::OA_ptr<OA::CFG::NodeStatementsIteratorInterface> stmtItPtr
      = node->getNodeStatementsIterator();
    for (; stmtItPtr->isValid(); ++(*stmtItPtr)) {
      WN* wstmt = (WN *)stmtItPtr->current().hval();
      fortTkSupport::WNId id = ctxt.findWNId(wstmt);
    
      // Skip statements without a valid id
      if (id == 0) { continue; }

      const char* str = Num2Str(id, "%lld");
      //std::cout << id << " --> " << str << " // ";
    
      if (!emptystr) {
	idstr += ":";
      }
      idstr += str;
      emptystr = false;
    }
 
    return idstr;
  }


  // GetCFGEdgeCondVal: Given a CFG edge, returns a pair indicating
  // whether the edge has a condition value, and if so, its value.
  // (There is no reserved NULL value for the condition value; it should
  // only be used when the first part of the pair is true!)
  pair<bool, INT64>
  GetCFGEdgeCondVal(const OA::OA_ptr<OA::CFG::EdgeInterface> edge)
  {
    using namespace OA::CFG;
  
    EdgeType ety = edge->getType();
    WN* eexpr = (WN*)edge->getExpr().hval();
  
    bool hasCondVal = false;
    INT64 condVal = 0;
    if (ety == TRUE_EDGE) {
      hasCondVal = true;
      condVal = 1;
    } 
    else if (ety == MULTIWAY_EDGE && eexpr) {
      hasCondVal = true;
      OPERATOR opr = WN_operator(eexpr);
      if (opr == OPR_CASEGOTO) { // from an OPR_SWITCH
	condVal = WN_const_val(eexpr);
      } 
      else if (opr == OPR_GOTO) { // from an OPR_COMPGOTO
	// to find condVal, must find parent COMPGOTO and then find the
	// index of this GOTO in the jumptable.
	FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "Conditions for COMPGOTO");
      } 
      else {
	FORTTK_DIE(fortTkSupport::Diagnostics::Unimplemented << "Unknown multiway branch");
      }
    }
    return pair<bool, INT64>(hasCondVal, condVal);
  }

  void initOurHandlerTable() { 

    // Note: Organization generally corresponds to that in
    // Open64/documentation/whirl.tex
  
    // Structured control flow
    ourHandlerTable[OPR_FUNC_ENTRY]=           &xlate_FUNC_ENTRY ;
    ourHandlerTable[OPR_BLOCK]=                &xlate_BLOCK ;
    ourHandlerTable[OPR_REGION]=               &WN2F_region ;
    ourHandlerTable[OPR_REGION_EXIT]=          &xlate_GOTO ;
    ourHandlerTable[OPR_DO_LOOP]=              &xlate_DO_LOOP ;
    ourHandlerTable[OPR_DO_WHILE]=             &xlate_DO_WHILE ;
    ourHandlerTable[OPR_WHILE_DO]=             &xlate_WHILE_DO ;
    ourHandlerTable[OPR_IF]=                   &xlate_IF ;
  
    // Other control flow
    ourHandlerTable[OPR_IMPLIED_DO]=           &WN2F_noio_implied_do ;
    ourHandlerTable[OPR_GOTO]=                 &xlate_GOTO ;
    ourHandlerTable[OPR_SWITCH]=               &xlate_SWITCH ;
    ourHandlerTable[OPR_CASEGOTO]=             &xlate_unknown ;
    ourHandlerTable[OPR_COMPGOTO]=             &xlate_unknown ;
    ourHandlerTable[OPR_AGOTO]=                &xlate_unknown ;
    ourHandlerTable[OPR_ALTENTRY]=             &xlate_ALTENTRY ;
    ourHandlerTable[OPR_TRUEBR]=               &xlate_condBR ;
    ourHandlerTable[OPR_FALSEBR]=              &xlate_condBR ;
    ourHandlerTable[OPR_RETURN]=               &xlate_RETURN ;
    ourHandlerTable[OPR_RETURN_VAL]=           &xlate_RETURN_VAL ;
    ourHandlerTable[OPR_LABEL]=                &xlate_LABEL ;

    // Statements: Calls
    ourHandlerTable[OPR_CALL]=                 &xlate_CALL ;
    ourHandlerTable[OPR_ICALL]=                &xlate_CALL ;
    ourHandlerTable[OPR_PICCALL]=              &xlate_CALL ;
    ourHandlerTable[OPR_INTRINSIC_CALL]=       &xlate_INTRINSIC_CALL ;
    ourHandlerTable[OPR_IO]=                   &xlate_IO ;

    // Statements: Other
    ourHandlerTable[OPR_EVAL]=                 &WN2F_eval ;
    ourHandlerTable[OPR_PRAGMA]=               &xlate_PRAGMA ;
    ourHandlerTable[OPR_XPRAGMA]=              &xlate_PRAGMA ;
    ourHandlerTable[OPR_PREFETCH]=             &xlate_PREFETCH ;
    ourHandlerTable[OPR_PREFETCHX]=            &xlate_PREFETCH ;
    ourHandlerTable[OPR_COMMENT]=              &xlate_COMMENT ;
    ourHandlerTable[OPR_TRAP]=                 &xlate_ignore ; // FIXME
    ourHandlerTable[OPR_ASSERT]=               &xlate_ignore ; // FIXME
    ourHandlerTable[OPR_FORWARD_BARRIER]=      &xlate_ignore ; // FIXME
    ourHandlerTable[OPR_BACKWARD_BARRIER]=     &xlate_ignore ; // FIXME
    ourHandlerTable[OPR_DEALLOCA]=             &WN2F_dealloca ;

    ourHandlerTable[OPR_USE]=                  &xlate_USE ;
    ourHandlerTable[OPR_NAMELIST]=             &WN2F_namelist_stmt ;
    ourHandlerTable[OPR_IMPLICIT_BND]=         &WN2F_implicit_bnd ;  
    ourHandlerTable[OPR_NULLIFY]=              &WN2F_nullify_stmt ;
    ourHandlerTable[OPR_INTERFACE]=            &WN2F_interface_blk ;
    ourHandlerTable[OPR_ARRAY_CONSTRUCT]=      &WN2F_ar_construct ;
  
    // Memory Access (or assignment and variable references)
    ourHandlerTable[OPR_LDA]=                  &xlate_LDA ;    // Leaf
    ourHandlerTable[OPR_LDID]=                 &xlate_LDID ;
    ourHandlerTable[OPR_STID]=                 &xlate_STID ;
    ourHandlerTable[OPR_ILOAD]=                &xlate_ILOAD ;
    ourHandlerTable[OPR_ILOADX]=               &xlate_ILOADX ;
    ourHandlerTable[OPR_MLOAD]=                &WN2F_mload ;
    ourHandlerTable[OPR_ISTORE]=               &xlate_ISTORE ;
    ourHandlerTable[OPR_ISTOREX]=              &xlate_ISTOREX ;
    ourHandlerTable[OPR_MSTORE]=               &WN2F_mstore ;
    ourHandlerTable[OPR_STRCTFLD]=             &xlate_STRCTFLD ;
    ourHandlerTable[OPR_PSTID]=                &WN2F_pstid ;  // pointer STID 
    ourHandlerTable[OPR_PSTORE]=               &WN2F_pstore ; // pointer STORE

    // Type conversion
    ourHandlerTable[OPR_CVT]=                  &WN2F_cvt ;
    ourHandlerTable[OPR_CVTL]=                 &WN2F_cvtl ;
    ourHandlerTable[OPR_TAS]=                  &WN2F_tas ;
  
    // Expressions: Unary operations
    ourHandlerTable[OPR_INTCONST]=             &xlate_INTCONST ; // Leaf
    ourHandlerTable[OPR_CONST]=                &xlate_CONST ;    // Leaf

    ourHandlerTable[OPR_NEG]=                  &xlate_UnaryOp ;
    ourHandlerTable[OPR_ABS]=                  &xlate_UnaryOp ;
    ourHandlerTable[OPR_SQRT]=                 &xlate_UnaryOp ;
    ourHandlerTable[OPR_RSQRT]=                &WN2F_rsqrt ;
    ourHandlerTable[OPR_RECIP]=                &xlate_RECIP ;
    ourHandlerTable[OPR_REALPART]=             &WN2F_realpart ; // OPR_FIRSTPART
    ourHandlerTable[OPR_IMAGPART]=             &WN2F_imagpart ; // OPR_SECONDPART
    ourHandlerTable[OPR_PAREN]=                &xlate_PAREN ;
    ourHandlerTable[OPR_RND]=                  &xlate_UnaryOp ;
    ourHandlerTable[OPR_TRUNC]=                &xlate_UnaryOp ;
    ourHandlerTable[OPR_CEIL]=                 &xlate_UnaryOp ;
    ourHandlerTable[OPR_FLOOR]=                &xlate_UnaryOp ;
    ourHandlerTable[OPR_BNOT]=                 &xlate_UnaryOp ;
    ourHandlerTable[OPR_LNOT]=                 &xlate_UnaryOp ;
    ourHandlerTable[OPR_PARM]=                 &WN2F_parm ;
    ourHandlerTable[OPR_ALLOCA]=               &WN2F_alloca ;

    // Expressions: Binary operations
    ourHandlerTable[OPR_COMPLEX]=              &xlate_BinaryOp ; // OPR_PAIR
    ourHandlerTable[OPR_ADD]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_SUB]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_MPY]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_DIV]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_MOD]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_REM]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_MAX]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_MIN]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_EQ]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_NE]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_GE]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_GT]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_LE]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_LT]=                   &xlate_BinaryOp ;
    ourHandlerTable[OPR_BAND]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_BIOR]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_BNOR]=                 &WN2F_bnor ;
    ourHandlerTable[OPR_BXOR]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_LAND]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_LIOR]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_CAND]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_CIOR]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_SHL]=                  &xlate_BinaryOp ;
    ourHandlerTable[OPR_ASHR]=                 &xlate_BinaryOp ;
    ourHandlerTable[OPR_LSHR]=                 &WN2F_lshr ;
  
    // Expressions: Ternary operations
    ourHandlerTable[OPR_SELECT]=               &WN2F_select ;
    ourHandlerTable[OPR_MADD]=                 &WN2F_madd ;
    ourHandlerTable[OPR_MSUB]=                 &WN2F_msub ;
    ourHandlerTable[OPR_NMADD]=                &WN2F_nmadd ;
    ourHandlerTable[OPR_NMSUB]=                &WN2F_nmsub ;

    // Expressions: N-ary operations
    ourHandlerTable[OPR_ARRAY]=                &xlate_ARRAY ;
    ourHandlerTable[OPR_INTRINSIC_OP]=         &xlate_INTRINSIC_OP ;
    ourHandlerTable[OPR_TRIPLET]=              &WN2F_triplet ;
    ourHandlerTable[OPR_SRCTRIPLET]=           &WN2F_src_triplet ;
    ourHandlerTable[OPR_ARRAYEXP]=             &WN2F_arrayexp ;
    ourHandlerTable[OPR_ARRSECTION]=           &WN2F_arrsection ;
    ourHandlerTable[OPR_WHERE]=                &WN2F_where ;

    // just for convenience
    ourHandlerTable[OPERATOR_UNKNOWN]=         &xlate_unknown; 
  } 
}
