// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XAIF_DOMFilters.h,v 1.18 2006/05/12 16:12:23 utke Exp $


#ifndef XAIF_DOMFilters_INCLUDED_h
#define XAIF_DOMFilters_INCLUDED_h

#include <iostream>

#include "xercesc/dom/DOMNodeFilter.hpp"

namespace xaif2whirl { 

  // Dumps only this node
  void 
  XercesPrintNode(std::ostream& os, const xercesc::DOMNode* n);

  // Dumps the tree rooted at 'n'
  void
  XercesPrintTree(std::ostream& os, const xercesc::DOMNode* n);

  std::ostream& 
  operator<<(std::ostream& os, const xercesc::DOMElement& elem);

  // Shortcut for XercesPrintNode
  void 
  XercesDumpNode(const xercesc::DOMNode* n);

  // Shortcut for XercesPrintTree
  void 
  XercesDumpTree(const xercesc::DOMNode* n);

  void 
  XercesDumpNode(void* n); // For *^#% debuggers

  void 
  XercesDumpTree(void* n); // For *^#% debuggers

  xercesc::DOMElement*
  GetFirstChildElement(const xercesc::DOMNode* n);

  xercesc::DOMElement*
  GetLastChildElement(const xercesc::DOMNode* n);

  xercesc::DOMElement*
  GetChildElement(const xercesc::DOMNode* n, const XMLCh* name);

  xercesc::DOMElement*
  GetChildElement(const xercesc::DOMNode* n, const xercesc::DOMNodeFilter* filter);

  unsigned int
  GetChildElementCount(const xercesc::DOMNode* n);

  xercesc::DOMElement*
  GetPrevSiblingElement(const xercesc::DOMNode* n);

  xercesc::DOMElement*
  GetNextSiblingElement(const xercesc::DOMNode* n);

  xercesc::DOMElement*
  GetNextSiblingElement(const xercesc::DOMNode* n, const XMLCh* name);

  xercesc::DOMElement*
  GetNextSiblingElement(const xercesc::DOMNode* n, const xercesc::DOMNodeFilter* filter);

  class XAIF_ElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_ElemFilter(const XMLCh* name) : mName(name) { }
    ~XAIF_ElemFilter() { }
    short acceptNode(const xercesc::DOMNode *node) const;
  private:
    const XMLCh* mName;
  };

  class XAIF_ScopeElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_ScopeElemFilter() { }
    ~XAIF_ScopeElemFilter() { }
    short acceptNode(const xercesc::DOMNode *node) const;
  };

  class XAIF_SymbolElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_SymbolElemFilter() { }
    ~XAIF_SymbolElemFilter() { }
    short acceptNode(const xercesc::DOMNode *node) const;
  };

  class XAIF_DimensionBoundsElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_DimensionBoundsElemFilter() { }
    ~XAIF_DimensionBoundsElemFilter() { }
    short acceptNode(const xercesc::DOMNode *node) const;
  };

  class XAIF_CFGElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_CFGElemFilter(bool cfgOrReplaceList_ = true) 
      : cfgOrReplaceList(cfgOrReplaceList_) { }
    ~XAIF_CFGElemFilter() { }

    short acceptNode(const xercesc::DOMNode *node) const;
  
    static bool IsCFG(const xercesc::DOMNode *node);
    static bool IsReplaceList(const xercesc::DOMNode *node);
    static bool IsReplacement(const xercesc::DOMNode *node);
  private:
    bool cfgOrReplaceList;
  };


  class XAIF_BBElemFilter : public xercesc::DOMNodeFilter {
  public:
    // if 'onlyBasicBlock' is true, only xaif:BasicBlocks are in the iteration
    XAIF_BBElemFilter(bool edges = true)
      : includeEdges(edges) { }
    ~XAIF_BBElemFilter() { }

    short acceptNode(const xercesc::DOMNode *node) const;
  
    static bool IsAnyBB(const xercesc::DOMNode *node);
    static bool IsBBEntry(const xercesc::DOMNode *node);
    static bool IsBBExit(const xercesc::DOMNode *node);
    static bool IsBB(const xercesc::DOMNode *node);
    static bool IsBBBranch(const xercesc::DOMNode *node);
    static bool IsBBEndBr(const xercesc::DOMNode *node);
    static bool IsBBForLoop(const xercesc::DOMNode *node);
    static bool IsBBPreLoop(const xercesc::DOMNode *node);
    static bool IsBBPostLoop(const xercesc::DOMNode *node);
    static bool IsBBEndLoop(const xercesc::DOMNode *node);
    static bool IsEdge(const xercesc::DOMNode *node);
  private:
    bool includeEdges;
  };


  // Accepts: basic block statements and DerivativeAccumulator
  class XAIF_BBStmtElemFilter : public xercesc::DOMNodeFilter {
  public:
    XAIF_BBStmtElemFilter() { }
    ~XAIF_BBStmtElemFilter() { }

    short acceptNode(const xercesc::DOMNode *node) const;

    static bool IsAnyStmt(const xercesc::DOMNode *node);
    static bool IsAssign(const xercesc::DOMNode *node);
    static bool IsSubCall(const xercesc::DOMNode *node);
    static bool IsInlinableSubCall(const xercesc::DOMNode *node);
    static bool IsMarker(const xercesc::DOMNode *node);
    static bool IsDerivProp(const xercesc::DOMNode *node);
  };


  class XAIF_DerivPropStmt : public xercesc::DOMNodeFilter {
  public:
    XAIF_DerivPropStmt() { }
    ~XAIF_DerivPropStmt() { }
  
    short acceptNode(const xercesc::DOMNode *node) const;

    static bool IsSetDeriv(const xercesc::DOMNode *node);
    static bool IsSetNegDeriv(const xercesc::DOMNode *node);
    static bool IsIncDeriv(const xercesc::DOMNode *node);
    static bool IsDecDeriv(const xercesc::DOMNode *node);
    static bool IsSax(const xercesc::DOMNode *node);
    static bool IsSaxpy(const xercesc::DOMNode *node);
    static bool IsZeroDeriv(const xercesc::DOMNode *node);  
  };

}

#endif 
