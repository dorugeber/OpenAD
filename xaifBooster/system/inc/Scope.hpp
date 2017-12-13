#ifndef _SCOPE_INCLUDE_
#define _SCOPE_INCLUDE_
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
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/VertexTraversable.hpp"
#include "xaifBooster/system/inc/SymbolTable.hpp"

namespace xaifBooster { 

  /** 
   * A Scope essentially as understood
   * in programming languages
   * out edges lead to child scopes
   * the (single) in edge to the parent 
   * scope
   */
  class Scope : public VertexTraversable, 
		public ObjectWithId,
		public XMLPrintable,
		public ObjectWithAnnotation {
  public:

    Scope (){};

    ~Scope(){};

    /**
     * print XML hierarchy
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;

    /**
     * returns a reference to the ControlFlowGraph
     */
    SymbolTable& getSymbolTable();

    /**
     * returns a reference to the ControlFlowGraph
     */
    const SymbolTable& getSymbolTable() const;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited myId as specified in XAIF schema
     */
    static const std::string our_myId_XAIFName;

  private: 

    /** 
     * each scope has a SymbolTable
     * @see SymbolTable
     */
    SymbolTable mySymbolTable;

  }; // end of class Scope
 
} // end of namespace xaifBooster
                                                                     
#endif
