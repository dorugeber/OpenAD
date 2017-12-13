// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/sexp2whirl/sexp2wn.i,v 1.3 2007-10-08 18:28:34 utke Exp $
#ifndef wn2sexp_i
#define wn2sexp_i
#include <iostream>
#include <sexp.h>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "sexp2whirl.i"


// WNXlationTable: Maps WHIRL OPERATORs to functions designed to
// handle the translation of that particular node.  Handler functions
// have the type 'Handler'.  In the event that no specific handler is
// registered for a particular OPERATOR, a default handler is returned.
class WNXlationTable {
public:
  
  typedef WN* (*Handler)(sexp_t*);

public:
  WNXlationTable();
  ~WNXlationTable();

  Handler operator[](OPERATOR opr) const { Find(opr); }
  Handler Find(OPERATOR opr) const { return table[opr]; }
  
  void Dump(std::ostream& os = std::cerr) const;
  void DDump() const;

private:
  // Should not be used 
  WNXlationTable(const WNXlationTable& x) { }
  WNXlationTable& operator=(const WNXlationTable& x) { return *this; }

  struct InitEntry {
    OPERATOR opr;
    Handler  fn;
  };
  
private:
  static bool initialized;
  
  // The OPERATOR -> Handler table
  static Handler table[];
  static unsigned int tableSz;
  
  // Initialization table
  static InitEntry initTable[];
  static unsigned int initTableSz;
};


//***************************************************************************
// Control Flow Statements
//***************************************************************************

namespace sexp2whirl {

  extern WN*
  xlate_INTERFACE(sexp_t* sx);

  // ---------------------------------------------------------
  // Structured Control Flow Statements
  // ---------------------------------------------------------

  extern WN*
  xlate_FUNC_ENTRY(sexp_t* sx);
  
  extern WN* 
  xlate_BLOCK(sexp_t* sx);
  
  extern WN* 
  xlate_REGION(sexp_t* sx);
  
  extern WN* 
  xlate_structured_cf(sexp_t* sx);

  // ---------------------------------------------------------
  // Unstructured Control Flow Statements
  // ---------------------------------------------------------

  extern WN*
  xlate_IMPLIED_DO(sexp_t* sx);
  
  extern WN* 
  xlate_GOTOx_LABEL(sexp_t* sx);
  
  extern WN*
  xlate_multiBR(sexp_t* sx);
  
  extern WN*
  xlate_CASEGOTO(sexp_t* sx);
  
  extern WN* 
  xlate_AGOTO(sexp_t* sx);
  
  extern WN*
  xlate_ALTENTRY(sexp_t* sx);
  
  extern WN* 
  xlate_condBR(sexp_t* sx);
  
  extern WN* 
  xlate_RETURNx(sexp_t* sx);
  
}; /* namespace sexp2whirl */


//***************************************************************************
// Calls and Other Statements
//***************************************************************************

namespace sexp2whirl {

  // ---------------------------------------------------------
  // Calls
  // ---------------------------------------------------------

  extern WN* 
  xlate_xCALL(sexp_t* sx);
  
  extern WN* 
  xlate_IO(sexp_t* sx);

  // ---------------------------------------------------------
  // Other Statements
  // ---------------------------------------------------------

  extern WN*
  xlate_misc_stmt(sexp_t* sx);  

  extern WN* 
  xlate_xPRAGMA(sexp_t* sx);  
  
}; /* namespace sexp2whirl */


//***************************************************************************
// Memory Access
//***************************************************************************

namespace sexp2whirl {

  // ---------------------------------------------------------
  // Memory Access (or assignment and variable references)
  // ---------------------------------------------------------

  // Loads (variable reference) and Stores (assignment)
  extern WN* 
  xlate_LDA_LDMA(sexp_t* sx);
  
  extern WN* 
  xlate_LDID_STID(sexp_t* sx);
  
  extern WN* 
  xlate_IDNAME(sexp_t* sx);

  extern WN* 
  xlate_xLOADx_xSTOREx(sexp_t* sx);

  extern WN*
  xlate_PSTID(sexp_t* sx);

  extern WN*
  xlate_PSTORE(sexp_t* sx);

  // ---------------------------------------------------------
  // Memory Access (user defined structures)
  // ---------------------------------------------------------

  extern WN*
  xlate_STRCTFLD(sexp_t* sx);

  // ---------------------------------------------------------
  // Array Operators (N-ary Operations)
  // ---------------------------------------------------------

  extern WN*
  xlate_ARRAYx(sexp_t* sx);
  
}; /* namespace sexp2whirl */


//***************************************************************************
// Expression Operators, Type conversion, Leaf (Other)
//***************************************************************************

namespace sexp2whirl {

  // ---------------------------------------------------------
  // Type conversion
  // ---------------------------------------------------------

  extern WN* 
  xlate_CVT_CVTL(sexp_t* sx);
  
  extern WN* 
  xlate_TAS(sexp_t* sx);
  
  // ---------------------------------------------------------
  // Leaf (Other)
  // ---------------------------------------------------------

  extern WN* 
  xlate_CONST(sexp_t* sx);
  
  extern WN* 
  xlate_INTCONST(sexp_t* sx);
  
  // ---------------------------------------------------------
  // Expression Operators: Unary Operations
  // ---------------------------------------------------------

  extern WN* 
  xlate_UnaryOp(sexp_t* sx);

  extern WN* 
  xlate_PARM(sexp_t* sx);

  extern WN* 
  xlate_ALLOCA(sexp_t* sx) ;

  // ---------------------------------------------------------
  // Expression Operators: Binary Operations
  // ---------------------------------------------------------
  
  extern WN* 
  xlate_BinaryOp(sexp_t* sx);

  // ---------------------------------------------------------
  // Expression Operators: Ternary Operations
  // ---------------------------------------------------------
  
  extern WN* 
  xlate_TernaryOp(sexp_t* sx);

  // ---------------------------------------------------------
  // Expression Operators: N-ary Operations
  // ---------------------------------------------------------
  
  // Array expressions included in memory operations
  
  extern WN*
  xlate_IO_ITEM(sexp_t* sx);

}; /* namespace sexp2whirl */

//***************************************************************************

namespace sexp2whirl {
  
  extern WN* 
  xlate_unknown(sexp_t* sx);
  
}; /* namespace sexp2whirl */


//***************************************************************************

#endif /* wn2sexp_i */
