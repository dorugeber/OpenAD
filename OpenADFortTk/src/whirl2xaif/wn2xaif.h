// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/whirl2xaif/wn2xaif.h,v 1.34 2006/07/19 19:48:19 utke Exp $

#ifndef wn2xaif_h
#define wn2xaif_h

#include <vector> // STL
#include <list>   // STL


#include "Open64IRInterface/Open64BasicTypes.h"


#include <OpenAnalysis/CFG/CFG.hpp> // for DGraphStandard, CFG::Edge
#include <OpenAnalysis/Utils/DGraph/DGraphInterface.hpp>

#include "whirl2xaif.h"
#include "PUXlationContext.h"
#include <xmlostream.h>
#include <XAIFStrings.h>


namespace whirl2xaif {
    
  // TranslateWN: Given a translation context, translates the given
  // WHIRL node, emitting output to 'xos'.
  extern void 
  TranslateWN(xml::ostream& xos, WN *wn, PUXlationContext& ctxt);

  // xlate_PregRef: (essentially a special version of xlate_SymRef for PREGs)
  void
  xlate_PregRef(xml::ostream& xos, ST* st, TY_IDX preg_ty, 
		PREG_IDX preg_idx, PUXlationContext& ctxt);
  
  // xlate_SymRef: Given a base symbol 'base_st' and an offset 'offset'
  // within it, generate an XAIF expression to reference an object of
  // 'ref_ty' at this location.  The base symbol 'base_st' has an
  // address of type 'baseptr_ty'.
  extern void
  xlate_SymRef(xml::ostream& xos, ST* base_st, TY_IDX baseptr_ty, 
	       TY_IDX ref_ty, STAB_OFFSET offset, PUXlationContext& ctxt);
  
  // xlate_MemRef: 
  extern void
  xlate_MemRef(xml::ostream& xos, WN* addr, TY_IDX addr_ty, TY_IDX object_ty,
	       STAB_OFFSET addr_offset, PUXlationContext& ctxt);
  



  // REMOVE/FIXME
  // WN2F_Sum_Offsets:  Sums any ADD nodes encountered in an address tree
  // WN2F_Address_Of: Generates an expression to explicitly take the
  // address of the lvalue constituted by the tokens in the given
  // token-buffer.
  extern void WN2F_Address_Of(xml::ostream& xos);
  extern WN_OFFSET WN2F_Sum_Offsets(WN *addr);


  // ***************************************************************************
  // Graph utilities
  // ***************************************************************************

  //typedef std::vector<OA::DGraph::DGraphStandard::Node*> DGraphNodeVec;
  //typedef std::vector<OA::DGraph::DGraphStandard::Edge*> DGraphEdgeVec;
  //typedef std::vector<OA::CFG::CFGStandard::Edge*> CFGEdgeVec;
  //typedef std::vector<OA::DGraph::DGraphStandard::Node> DGraphNodeVec;
  //typedef std::vector<OA::DGraph::DGraphStandard::Edge> DGraphEdgeVec;
  typedef std::vector<OA::OA_ptr<OA::DGraph::NodeInterface> > DGraphNodeVec;
  typedef std::vector<OA::OA_ptr<OA::DGraph::EdgeInterface> > DGraphEdgeVec;
  //typedef std::vector<OA::CFG::CFGStandard::Edge> CFGEdgeVec;
  typedef std::vector<OA::OA_ptr<OA::CFG::EdgeInterface> > CFGEdgeVec;


  // SortDGraphNodes: Sorts DGraph nodes.  User must deallocate returned
  // object.
  extern DGraphNodeVec*
  SortDGraphNodes(OA::OA_ptr<OA::DGraph::DGraphInterface> g);
  //SortDGraphNodes(OA::OA_ptr<OA::DGraph::DGraphStandard> g);

  // SortDGraphEdges: Sorts DGraph edges.  User must deallocate returned
  // object.
  extern DGraphEdgeVec*
  SortDGraphEdges(OA::OA_ptr<OA::DGraph::DGraphInterface> g);
  //SortDGraphEdges(OA::OA_ptr<OA::DGraph::DGraphStandard> g);

  // SortCFGEdges: Sorts CFG edges.  User must deallocate returned
  // object.
  extern CFGEdgeVec*
  SortCFGEdges(OA::OA_ptr<OA::CFG::CFGInterface> g);
  //SortCFGEdges(OA::OA_ptr<OA::CFG::CFGStandard> g);


  // ***************************************************************************
  // XAIF xml::ostream utilities
  // ***************************************************************************

  // DumpGraphEdge: Generic edge dumper.  Given an edge name 'nm', id,
  // source id, target id and position, dumps the edge in XAIF.  If
  // 'pos' is 0 it will not be output.
  void 
  DumpGraphEdge(xml::ostream& xos, const char* nm, 
		UINT eid, UINT srcid, UINT targid, UINT pos);

  // DumpScopeGraphEdge: Dumps a xaif:ScopeEdge
  inline void 
  DumpScopeGraphEdge(xml::ostream& xos, UINT eid, UINT srcid, UINT targid)
  {
    DumpGraphEdge(xos, "xaif:ScopeEdge", eid, srcid, targid, 0);
  }

  // DumpCallGraphEdge: Dumps a xaif:CallGraphEdge
  inline void 
  DumpCallGraphEdge(xml::ostream& xos, UINT eid, UINT srcid, UINT targid)
  {
    DumpGraphEdge(xos, "xaif:CallGraphEdge", eid, srcid, targid, 0);
  }

  // DumpCFGraphEdge: Dumps a xaif:ControlFlowEdge (special requirements)

  // DumpExprGraphEdge: Dumps a xaif:ExpressionEdge
  inline void 
  DumpExprGraphEdge(xml::ostream& xos, UINT eid, UINT srcid, UINT targid,
		    UINT pos)
  {
    DumpGraphEdge(xos, "xaif:ExpressionEdge", eid, srcid, targid, pos);
  }


  // ***************************************************************************
  // XAIF xml::ostream operators
  // ***************************************************************************

  // ---------------------------------------------------------
  // AttrSymId: Generate an XAIF symbol id attribute
  // ---------------------------------------------------------

  struct AttrSymId_ {
    const ST* st;
  };

  inline ostream&
  operator<<(std::ostream& os, const AttrSymId_& x) 
  {
    xml::ostream& xos = dynamic_cast<xml::ostream&>(os); // FIXME

    const ST* st = x.st;

    // We cannot generate symbol_id references for CONSTs because they
    // are not in the symbol table. (Also, a ST_name(st) on a CONST is
    // not valid.)
    FORTTK_ASSERT(ST_class(st) != CLASS_CONST,
		  "Attempting to generate a symbol_id for a CONST. ST level/index = " << (fortTkSupport::SymTabId)ST_level(st) << ", " << (fortTkSupport::SymId)ST_index(st));

    const char* st_name = ST_name(st);
    fortTkSupport::SymId st_id = (fortTkSupport::SymId)ST_index(st);
  
    xos << xml::BegAttr(XAIFStrings.attr_symId())
	<< st_name << "_" << st_id
	<< xml::EndAttr;

    return xos;
  }

  // AttrSymId: Given a symtab symbol (ST*), generate a symbol id attribute
  inline AttrSymId_
  AttrSymId(ST* st_)
  {
    AttrSymId_ x;
    x.st = st_;
    return x;
  }


  // ---------------------------------------------------------
  // AttrAnnot, AttrAnnotVal: Generate an XAIF annotation attribute
  // ---------------------------------------------------------
  template<class T> 
  struct AttrAnnotInfo_ {
    bool completeAttr;
    const char* tag;
    const T* val;
  };

  template<class T> 
  ostream&
  operator<<(std::ostream& os, const AttrAnnotInfo_<T>& x) 
  {
    xml::ostream& xos = dynamic_cast<xml::ostream&>(os); // FIXME

    if (x.completeAttr) {
      xos << xml::BegAttr(XAIFStrings.attr_annot());
    }

    xos << x.tag << *x.val << XAIFStrings.tag_End();
  
    if (x.completeAttr) {
      xos << xml::EndAttr;
    }

    return xos;
  }

  // AttrAnnot: Given a tag and a value, generate a complete annotiation
  // attribute
  template<class T> 
  AttrAnnotInfo_<T> 
  AttrAnnot(const char* tag_, const T& val_)
  {
    AttrAnnotInfo_<T> x;
    x.completeAttr = true;
    x.tag = tag_;
    x.val = &val_;
    return x;
  }

  // AttrAnnotVal: Given a tag and a value, generate only the
  // annotiation attribute value
  template<class T> 
  AttrAnnotInfo_<T>
  AttrAnnotVal(const char* tag_, const T& val_)
  {
    AttrAnnotInfo_<T> x;
    x.completeAttr = false;
    x.val = &val_;
    x.tag = tag_;
    return x;
  }

  // *AttrAnnot: Given a value, generate a complete annotiation
  // attribute with appropriate tag
  template<class T> 
  AttrAnnotInfo_<T> 
  SymTabIdAnnot(const T& val_) 
  {
    return AttrAnnot(XAIFStrings.tag_SymTabId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  SymIdAnnot(const T& val_) 
  {
    return AttrAnnot(XAIFStrings.tag_SymId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  PUIdAnnot(const T& val_) 
  {
    return AttrAnnot(XAIFStrings.tag_PUId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  WhirlIdAnnot(const T& val_) 
  {
    return AttrAnnot(XAIFStrings.tag_WHIRLId(), val_);
  }

  inline AttrAnnotInfo_<const char*>
  IntrinsicKeyAnnot(const char* const & val_)
  {
    return AttrAnnot(XAIFStrings.tag_IntrinsicKey(), val_);
  }

  template<class T>
  AttrAnnotInfo_<T>
  PregIdAnnot(const T& val_)
  {
    return AttrAnnot(XAIFStrings.tag_PregId(), val_);
  }


  // *AttrAnnotVal: Given a tag and a value, generate only the
  // annotiation attribute value with the appropriate tag
  template<class T> 
  AttrAnnotInfo_<T>
  SymTabIdAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_SymTabId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  SymIdAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_SymId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  PUIdAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_PUId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  WhirlIdAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_WHIRLId(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  StmtGotoAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_StmtGoto(), val_);
  }

  template<class T> 
  AttrAnnotInfo_<T>
  StmtLabelAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_StmtLabel(), val_);
  }

  template<class T>
  AttrAnnotInfo_<T>
  StmtReturnAnnotVal(const T& val_)
  {
    return AttrAnnotVal(XAIFStrings.tag_StmtReturn(), val_);
  }

} /* namespace whirl2xaif */

#endif 
