// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/xaif2whirl.h,v 1.24 2006/05/12 16:12:24 utke Exp $
#ifndef xaif2whirl_INCLUDED_h
#define xaif2whirl_INCLUDED_h


#include <iostream>
#include <vector>

#include "xercesc/dom/DOMDocument.hpp"

#include "OpenAnalysis/CFG/CFG.hpp" // for DGraphStandard, CFG::Edge
//#include "OpenAnalysis/Utils/DGraph/DGraphInterfaceIterators.hpp" 
#include "OpenAnalysis/Utils/DGraph/DGraphInterface.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"

#include "WhirlIDMaps.h"
#include "IntrinsicXlationTable.h"

#include "PUXlationContext.h"

namespace xaif2whirl {

  enum AlgorithmType {
    ALG_NULL,
    ALG_BB_PATCHING      // temporary
  };
  
  extern fortTkSupport::IntrinsicXlationTable IntrinsicTable;
  extern fortTkSupport::WNIdToWNTabMap        WNIdToWNTableMap;

  void
  TranslateIR(PU_Info* pu_forest, const xercesc::DOMDocument* doc);

  // ****************************************************************************
  // Functions that were static, but now aren't anymore (so that the SAX2 parser can call them)

  void
  xlate_Scope(const xercesc::DOMElement* elem,
	      PUXlationContext& ctxt);
 
  /**
   * Translate XAIF CFG or XAIF Replacement to WHIRL
   */
  void
  TranslateCFG(PU_Info* pu_forest,
	       const xercesc::DOMElement* cfgElem,
	       PUXlationContext& ctxt);
 
  void DeclareActiveTypes();

  // ****************************************************************************

  // FIXME: relocate...
  fortTkSupport::Symbol*
  GetSymbol(const xercesc::DOMElement* elem, PUXlationContext& ctxt);

  void
  DDumpDotGraph(OA::OA_ptr<OA::DGraph::DGraphInterface> graph);

  void
  DumpDotGraph(std::ostream& os, OA::OA_ptr<OA::DGraph::DGraphInterface> graph);

  TYPE_ID
  XAIFFETypeToWHIRLMTy(const char* anFETypeName);

  fortTkSupport::Symbol*
  GetOrCreateSymbol(const char* sname, PUXlationContext& ctxt);

  fortTkSupport::Symbol*
  GetOrCreateBogusTmpSymbol(PUXlationContext& ctxt);

  // ****************************************************************************

  // -------------------------------------------------------
  // Generic attribute retrieval funtions
  // -------------------------------------------------------

  bool
  GetBoolAttr(const xercesc::DOMElement* elem, XMLCh* attr, bool default_val);

  int
  GetIntAttr(const xercesc::DOMElement* elem, XMLCh* attr, int default_val);

  // Return the value of the respective attribute.  The default values
  // are given below if the attribute is not present.

  // Default: false.
  bool
  GetHasConditionAttr(const xercesc::DOMElement* elem);

  // Default: 0.
  unsigned int
  GetCondAttr(const xercesc::DOMElement* elem);
  
  // Default: true.
  bool
  GetActiveAttr(const xercesc::DOMElement* elem);
  
  // Default: false.
  bool
  GetDerivAttr(const xercesc::DOMElement* elem);
  
  // Default: 0. A valid position value is non-zero.
  unsigned int
  GetPositionAttr(const xercesc::DOMElement* elem);


  // -------------------------------------------------------
  // Get information within XAIF 'annotation' attribute
  // -------------------------------------------------------

  // Given an element or annotation string and a tag, returns whether
  // the tag is present in teh annotation attribute.
  bool
  IsTagPresent(const xercesc::DOMElement* elem, const char* tag);
  
  bool
  IsTagPresent(const char* annotstr, const char* tag);

  // Get the appropriate persistant id from the element 'elem'.  See
  // detailed descriptions for generic functions below.
  fortTkSupport::SymTabId GetSymTabId(const xercesc::DOMElement* elem);
  
  fortTkSupport::SymId GetSymId(const xercesc::DOMElement* elem);
  
  fortTkSupport::PUId GetPUId(const xercesc::DOMElement* elem);
  
  fortTkSupport::WNId GetWNId(const xercesc::DOMElement* elem);
  
  fortTkSupport::IdList<fortTkSupport::WNId>* GetWNIdList(const xercesc::DOMElement* elem);

  // Get the value of the tag IntrinsicKey
  std::string GetIntrinsicKey(const xercesc::DOMElement* elem);

  // Get the value of the tag PregId
  PREG_IDX GetPregId(const xercesc::DOMElement* elem);
  
  // GetId, GetIdList: Returns an id or list of ids from the given tag
  // within the annotation attribute.  For the non-list version, 0 is
  // returned if no id is found. For the list version, the returned list
  // may be empty; the caller is responsible for freeing returned
  // memory.
  template <class T>
  T GetId(const xercesc::DOMElement* elem, const char* tag);
  
  template <class T>
  fortTkSupport::IdList<T>* GetIdList(const xercesc::DOMElement* elem, const char* tag);
  
  
  // GetId, GetIdList: Returns an id or the list of ids from the given
  // tag within the string 'idstr'.  For the non-list version, 0 is
  // returned if no id is found.  For the list version, the returned
  // list may be empty; the caller is responsible for freeing returned
  // memory.
  template <class T>
  T GetId(const char* idstr, const char* tag);
  
  template <class T>
  fortTkSupport::IdList<T>* GetIdList(const char* idstr, const char* tag);
  

  // ***************************************************************************

  // FIXME: move to another file

  WN* CreateCallToIntrin(TYPE_ID rtype, const char* fname, unsigned int argc);

  WN* CreateCallToIntrin(TYPE_ID rtype, const char* fname, std::vector<WN*>& args);

  WN* CreateIntrinsicCall(OPERATOR opr, INTRINSIC intrn, 
			  TYPE_ID rtype, TYPE_ID dtype, std::vector<WN*>& args);

  inline WN* CreateParm(WN *arg, UINT32 flag)
  {
    TYPE_ID rtype = WN_rtype(arg);
    return WN_CreateParm(rtype, arg, MTYPE_To_TY(rtype), flag);
  }

  WN* CreateBoolConst(unsigned int val);

  // ***************************************************************************

  class MyDGNode;

  typedef std::map<std::string, OA::OA_ptr<MyDGNode> > VertexIdToMyDGNodeMap;

  // ---------------------------------------------------------
  // MyDGNode, MyDGEdge: Used to create graph structures from XAIF
  // graphs (lists of nodes and edges)
  // ---------------------------------------------------------
  class MyDGNode : public OA::DGraph::NodeImplement {
  public:
    MyDGNode(const xercesc::DOMElement* e_) : e(e_) { Ctor(); }
    virtual ~MyDGNode() { }
  
    xercesc::DOMElement* GetElem() const { return const_cast<xercesc::DOMElement*>(e); }
  
    // getId: An id unique within instances of this class
    virtual unsigned int getId() const { return id; }
    // resetIds: reset id numbering
    static void resetIds() { nextId = 1; }
  
  private:
    void Ctor() { id = nextId++; }
    static unsigned int nextId;
  
    const xercesc::DOMElement* e;
    unsigned int id; // 0 is reserved; first instance is 1
  };

  class MyDGEdge : public OA::DGraph::EdgeImplement {
  public:
    MyDGEdge(OA::OA_ptr<OA::DGraph::NodeImplement> source_, 
	     OA::OA_ptr<OA::DGraph::NodeImplement> sink_, 
	     const xercesc::DOMElement* e_) 
      : OA::DGraph::EdgeImplement(source_, sink_), e(e_) { }
    virtual ~MyDGEdge() { }
  
    xercesc::DOMElement* GetElem() const { return const_cast<xercesc::DOMElement*>(e); }
  
  private:
    const xercesc::DOMElement* e;
  };


  // GetSuccessor: Assuming node has 0 or 1 outgoing edges, return the
  // successor node.  In most graphs the successor will be along the
  // outgoing edge, and this parameter defaults to true.
  extern OA::OA_ptr<MyDGNode>
  GetSuccessor(OA::OA_ptr<MyDGNode> node, bool succIsOutEdge = true);

  extern OA::OA_ptr<MyDGNode>
  GetSuccessorAlongEdge(OA::OA_ptr<MyDGNode> node, unsigned int condition, 
			bool succIsOutEdge = true);

}

#endif 
