// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/testers/tester.cxx,v 1.21 2007-10-08 18:28:34 utke Exp $

#include <stdlib.h> // ANSI: cstdlib // for strtol
#include <string.h> // ANSI: cstring // for strcmp, etc.
#include <iostream>

#include "OpenAnalysis/ExprTree/ExprTree.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"
#include "ir_reader.h"      // fdump_tree
#include "w2f_driver.h"     // W2F_*
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "Open64IRInterface/SymTab.h"

#include "ScalarizedRefTab.h"
#include "Diagnostics.h"

#include "tester.h"

using std::cerr;
using std::endl;

static int
DumpExprTree(std::ostream& os, WN* wn);

static int
DumpExprTree(std::ostream& os, OA::OA_ptr<OA::ExprTree> tree);

static int
TestForEachPU(std::ostream& os, PU_Info* pu_forest);

static int
TestForEachWNPU(std::ostream& os, WN* wn_pu);

static int
TestForEachWN(std::ostream& os, WN* wn);

static void
RecursiveFnWN(std::ostream& os, WN* wn);


int
whirltester::TestIR(std::ostream& os, PU_Info* pu_forest)
{
  Diag_Set_Phase("WHIRL tester: TestIR");
  
  if (!pu_forest) { return 0; }
  
  // Here is where we can do something
  TestForEachPU(os, pu_forest);

  return 0;
}

static int
TestForEachPU(std::ostream& os, PU_Info* pu_forest)
{
  Open64IRProcIterator procIt(pu_forest);
  for ( ; procIt.isValid(); ++procIt) { 
    
    // The PU_Info* for this PU
    PU_Info* pu = (PU_Info*)procIt.current().hval();

    // The root of the WHIRL tree
    WN* wn_pu = PU_Info_tree_ptr(pu);
    
    TestForEachWNPU(os, wn_pu);
  }
  return 0;
}

static int
TestForEachWNPU(std::ostream& os, WN* wn_pu)
{
  // We may want to do something special with the top level WN (FUNC_ENTRY)
  //...
  
  // Iterate over each WN and do something
  WN_TREE_CONTAINER<PRE_ORDER> wtree(wn_pu);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator it;
  
  for (it = wtree.begin(); it != wtree.end(); ++it) {
    WN* curWN = it.Wn();

    TestForEachWN(os, curWN);
  }

  // Alternatively we may want to recursively do something with the WHIRL tree
  RecursiveFnWN(os, wn_pu);
  
  return 0;
}

static int
TestForEachWN(std::ostream& os, WN* wn)
{
  if (wn) {
    DumpExprTree(os, wn);
    os << endl;
  }
  return 0;
}

static void
RecursiveFnWN(std::ostream& os, WN* wn)
{
  if (wn == NULL) {
    // Base case

  } 
  
  OPERATOR opr = WN_operator(wn);

  if ( /* some test */ false ) {
    // Base case
  } else if (!OPERATOR_is_leaf(opr)) {
    
    // General recursive case
    if (WN_operator(wn) == OPR_BLOCK) {
      WN *kid = WN_first(wn);
      while (kid) {
	RecursiveFnWN(os, kid);
	kid = WN_next(kid);
      }
    } else {
      for (INT kidno = 0; kidno < WN_kid_count(wn); kidno++) {
	WN* kid = WN_kid(wn, kidno);
	RecursiveFnWN(os, kid);
      }
    }
  }
}

//****************************************************************************

static int
DumpExprNode(std::ostream& os, OA::OA_ptr<OA::ExprTree::Node> node, 
             OA::OA_ptr<Open64IRInterface> ir);

static int
DumpExprTree(std::ostream& os, WN* wn)
{
  OA::OA_ptr<Open64IRInterface> ir;
  ir = new Open64IRInterface;

  OPERATOR opr = WN_operator(wn);
  if (OPERATOR_is_expression(opr)) {
      OA::OA_ptr<OA::ExprTree> tree = 
      ir->getExprTree(OA::ExprHandle((OA::irhandle_t)wn));
    DumpExprTree(os, tree);
  }
  
  return 0;
}

static int
DumpExprTree(std::ostream& os, OA::OA_ptr<OA::ExprTree> tree)
{
  OA::OA_ptr<Open64IRInterface> ir;
  ir = new Open64IRInterface;
  
  OA::Tree::PreOrderIterator nodes_iter(*tree);
  for ( ; nodes_iter.isValid(); ++nodes_iter) {
    OA::OA_ptr<OA::Tree::Node> ntmp = nodes_iter.current();
    OA::OA_ptr<OA::ExprTree::Node> node = ntmp.convert<OA::ExprTree::Node>();
    DumpExprNode(os, node, ir);
  }
  
  return 0;
}

static int
DumpExprNode(std::ostream& os, OA::OA_ptr<OA::ExprTree::Node> node, 
             OA::OA_ptr<Open64IRInterface> ir)
{
#if 0
  std::string& attr = node->getAttr();
  os << "{ " << attr;
  
  if (node->isSym()) {
    const char* nm = ir.toString(node->getSymHandle());
    os << " sym: " << nm; 
  } else if (node->isConst()) {
    os << " const"; 
  }
  os << " }";
#endif

  return 0;
}


//****************************************************************************
// TestIR_OA: 
//****************************************************************************

static int
TestIR_OA_ForEachWNPU(std::ostream& os, WN* wn_pu);

#if 0 
static void
TestIR_OA_ForEachVarRef(std::ostream& os, WN* wn, 
			Open64IRInterface& ir, UJNumbers& vnmap);
#endif

int
whirltester::TestIR_OA(std::ostream& os, PU_Info* pu_forest)
{
  Diag_Set_Phase("WHIRL tester: TestIR_OA");
  
  if (!pu_forest) { return 0; }

  Open64IRProcIterator procIt(pu_forest);
  for ( ; procIt.isValid(); ++procIt) { 
    
    // The PU_Info* for this PU
    PU_Info* pu = (PU_Info*)procIt.current().hval();

    // The root of the WHIRL tree
    WN* wn_pu = PU_Info_tree_ptr(pu);
    
    TestIR_OA_ForEachWNPU(os, wn_pu);
  }
  return 0;
}

static int
TestIR_OA_ForEachWNPU(std::ostream& os, WN* wn_pu)
{
  WN* fbody = WN_func_body(wn_pu);

#if 0  
  Open64IRInterface irInterface;
  Open64IRStmtIterator irStmtIter(fbody);
  CFG cfg(irInterface, &irStmtIter, (SymHandle)WN_st(wn_pu), true);

  // Accumulate the ST* for parameters
  std::set<SymHandle> params;
  INT nparam = WN_num_formals(wn_pu);
  for (int i = 0; i < nparam; ++i) {
    ST* st = WN_st(WN_formal(wn_pu, i));
    params.insert((SymHandle)st);
  }

  UJNumbers vnmap(cfg, params);
  TestIR_OA_ForEachVarRef(os, wn_pu, irInterface, vnmap);
#endif

  return 0;
}

#if 0
static void
TestIR_OA_ForEachVarRef(std::ostream& os, WN* wn, 
			Open64IRInterface& ir, UJNumbers& vnmap)
{
  if (wn == NULL) {
    // Base case
  } 

  OPERATOR opr = WN_operator(wn);
  // OPR_STID is a special case.  It is a base case in the sense the
  // it represents a LHS definition; but it is also a recursive case
  // because uses are embedded in its RHS subtree.
  bool varref = IsRefTranslatableToXAIF(wn);
  bool recur = false;

  // Base case
  if (varref || opr == OPR_STID) {
    VN vn = vnmap.Find((ExprHandle)wn);

    ExprTree* tree = ir.GetExprTreeForExprHandle((ExprHandle)wn);    
    os << "VN = " << vn << endl;
    os << "  ";
    DumpExprTree(os, tree);
    os << endl;
    delete tree;

    if (opr == OPR_STID) { recur = true; }
  } 

  // General recursive case
  if ( (!varref || recur) && !OPERATOR_is_leaf(opr) ) {
    
    if (WN_operator(wn) == OPR_BLOCK) {
      WN *kid = WN_first(wn);
      while (kid) {
	TestIR_OA_ForEachVarRef(os, kid, ir, vnmap);
	kid = WN_next(kid);
      }
    } else {
      for (INT kidno = 0; kidno < WN_kid_count(wn); ++kidno) {
	WN* kid = WN_kid(wn, kidno);
	TestIR_OA_ForEachVarRef(os, kid, ir, vnmap);
      }
    }
  }
}
#endif


//****************************************************************************
// TestIR_whirl2f: 
//****************************************************************************

static WN*
PleaseGetMeSomething(WN* wn_pu);

int
whirltester::TestIR_whirl2f(std::ostream& os, PU_Info* pu_forest)
{
  static const int bufSZ = 1024 * 1024;
  static char buf[bufSZ];
  
  Diag_Set_Phase("WHIRL tester: TestIR_whirl2f");
  if (!pu_forest) { return 0; }
  
  PU_AllocBEGlobalSymtab();
  W2F_Init();
  
  Open64IRProcIterator procIt(pu_forest);
  for ( ; procIt.isValid(); ++procIt) { 
    
    // The PU_Info* for this PU
    PU_Info* pu = (PU_Info*)procIt.current().hval();
    
    // The root of the WHIRL tree and a statement to translate
    WN* wn_pu = PU_Info_tree_ptr(pu);
    WN* wn = PleaseGetMeSomething(wn_pu);

    PU_AllocBELocalSymtab(pu);
    W2F_Push_PU(wn_pu, wn);
    
    // Translate and output
    os << "----> The tree:" << endl;
    fdump_tree(stdout, wn);
    os << endl;
    
    os << "----> The translation:" << endl;
    W2F_Translate_Wn_Str(buf, bufSZ-1, wn);
    os << buf << endl;
    
    W2F_Pop_PU();
    PU_FreeBELocalSymtab(pu);
    break;
  }
  
  W2F_Fini();
  PU_FreeBEGlobalSymtab();
  return 0;
}

static WN*
PleaseGetMeSomething(WN* wn_pu) 
{
  WN_TREE_CONTAINER<PRE_ORDER> wtree(wn_pu);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator it;

  // Find the first non-empty block
  WN* blk = NULL;
  for (it = wtree.begin(); it != wtree.end(); ++it) {
    WN* curWN = it.Wn();
    OPERATOR opr = WN_operator(curWN);
    if (opr == OPR_BLOCK && WN_first(curWN)) { // non-empty block
      blk = curWN;
      break;
    }
  }
  
  if (blk) {
    // Create a comment and insert it into the block
    WN* com1 = WN_CreateComment("$OpenAD BEGIN FRAGMENT(1)");
    WN_INSERT_BlockFirst(blk, com1);
    
    WN* com2 = WN_CreateComment("$OpenAD END FRAGMENT(1)");
    WN_INSERT_BlockLast(blk, com2);
  }
  
  // Return body of the wn_pu
  return WN_func_body(wn_pu); // blk;
}


//****************************************************************************
