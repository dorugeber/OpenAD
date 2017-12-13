// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2sexp/wn2sexp.i,v 1.6 2007-10-08 18:28:34 utke Exp $
#ifndef wn2sexp_i
#define wn2sexp_i

#include "Open64IRInterface/Open64BasicTypes.h"

#include "sexpostream.h"

#include "whirl2sexp.i"

//***************************************************************************
// WNXlationTable
//***************************************************************************

// WNXlationTable: Maps WHIRL OPERATORs to functions designed to
// handle the translation of that particular node.  Handler functions
// have the type 'Handler'.  In the event that no specific handler is
// registered for a particular OPERATOR, a default handler is returned.
class WNXlationTable {
public:
  
  typedef whirl2sexp::status (*Handler)(sexp::ostream&, WN*);

public:
  WNXlationTable();
  ~WNXlationTable();

  Handler operator[](OPERATOR opr) const { return Find(opr); }
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

namespace whirl2sexp {

  // ---------------------------------------------------------
  // Structured Control Flow Statements
  // ---------------------------------------------------------

  extern whirl2sexp::status
  xlate_FUNC_ENTRY(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_BLOCK(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_REGION(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_structured_cf(sexp::ostream& sos, WN* wn);

  // ---------------------------------------------------------
  // Unstructured Control Flow Statements
  // ---------------------------------------------------------

  extern whirl2sexp::status
  xlate_IMPLIED_DO(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_GOTOx_LABEL(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status
  xlate_multiBR(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status
  xlate_CASEGOTO(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_AGOTO(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status
  xlate_ALTENTRY(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_condBR(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_RETURNx(sexp::ostream& sos, WN* wn);
  
}; /* namespace whirl2sexp */


//***************************************************************************
// Calls and Other Statements
//***************************************************************************

namespace whirl2sexp {

  // ---------------------------------------------------------
  // Calls
  // ---------------------------------------------------------

  extern whirl2sexp::status 
  xlate_xCALL(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_IO(sexp::ostream& sos, WN* wn);

  // ---------------------------------------------------------
  // Other Statements
  // ---------------------------------------------------------

  extern whirl2sexp::status
  xlate_misc_stmt(sexp::ostream& sos, WN* wn);  

  extern whirl2sexp::status 
  xlate_xPRAGMA(sexp::ostream& sos, WN* wn);  
  
}; /* namespace whirl2sexp */


//***************************************************************************
// Memory Access
//***************************************************************************

namespace whirl2sexp {

  // ---------------------------------------------------------
  // Memory Access (or assignment and variable references)
  // ---------------------------------------------------------

  // Loads (variable reference) and Stores (assignment)
  extern whirl2sexp::status 
  xlate_LDA_LDMA(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_LDID_STID(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_IDNAME(sexp::ostream& sos, WN* wn);

  extern whirl2sexp::status 
  xlate_xLOADx_xSTOREx(sexp::ostream& sos, WN* wn);

  extern whirl2sexp::status
  xlate_PSTID(sexp::ostream& sos, WN* wn);

  extern whirl2sexp::status
  xlate_PSTORE(sexp::ostream& sos, WN* wn);
  
  // ---------------------------------------------------------
  // Memory Access (user defined structures)
  // ---------------------------------------------------------

  extern whirl2sexp::status 
  xlate_STRCTFLD(sexp::ostream& sos, WN* wn);

  // ---------------------------------------------------------
  // Array Operators (N-ary Operations)
  // ---------------------------------------------------------

  extern whirl2sexp::status
  xlate_ARRAYx(sexp::ostream& sos, WN* wn);
  
}; /* namespace whirl2sexp */


//***************************************************************************
// Expression Operators, Type conversion, Leaf (Other)
//***************************************************************************

namespace whirl2sexp {

  // ---------------------------------------------------------
  // Type conversion
  // ---------------------------------------------------------

  extern whirl2sexp::status 
  xlate_CVT_CVTL(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_TAS(sexp::ostream& sos, WN* wn);
  
  // ---------------------------------------------------------
  // Leaf (Other)
  // ---------------------------------------------------------

  extern whirl2sexp::status 
  xlate_CONST(sexp::ostream& sos, WN* wn);
  
  extern whirl2sexp::status 
  xlate_INTCONST(sexp::ostream& sos, WN* wn);
  
  // ---------------------------------------------------------
  // Expression Operators: Unary Operations
  // ---------------------------------------------------------

  extern whirl2sexp::status 
  xlate_UnaryOp(sexp::ostream& sos, WN* wn);

  extern whirl2sexp::status 
  xlate_PARM(sexp::ostream& sos, WN* wn);

  extern whirl2sexp::status 
  xlate_ALLOCA(sexp::ostream& sos, WN* wn) ;

  // ---------------------------------------------------------
  // Expression Operators: Binary Operations
  // ---------------------------------------------------------
  
  extern whirl2sexp::status 
  xlate_BinaryOp(sexp::ostream& sos, WN* wn);

  // ---------------------------------------------------------
  // Expression Operators: Ternary Operations
  // ---------------------------------------------------------
  
  extern whirl2sexp::status 
  xlate_TernaryOp(sexp::ostream& sos, WN* wn);

  // ---------------------------------------------------------
  // Expression Operators: N-ary Operations
  // ---------------------------------------------------------
  
  // Array expressions included in memory operations
  
  extern whirl2sexp::status
  xlate_IO_ITEM(sexp::ostream& sos, WN* wn);

}; /* namespace whirl2sexp */

//***************************************************************************

namespace whirl2sexp {
  
  extern whirl2sexp::status 
  xlate_unknown(sexp::ostream& sos, WN* wn);
  
}; /* namespace whirl2sexp */


//***************************************************************************

#endif /* wn2sexp_i */
