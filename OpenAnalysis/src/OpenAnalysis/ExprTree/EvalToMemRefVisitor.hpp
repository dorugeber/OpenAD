/*! \file
  
  \brief Concrete ExprTreeVisitor that determines if the only  node in the
         tree is a memory reference.
  
  \authors Michelle Strout
  \version $Id: EvalToMemRefVisitor.hpp,v 1.4 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef EvalToMemRefVisitor_H
#define EvalToMemRefVisitor_H

#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class EvalToMemRefVisitor : public ExprTreeVisitor {
public:
  EvalToMemRefVisitor() : mIsMemRef(false) {}
  ~EvalToMemRefVisitor() {}

  void visitExprTreeBefore(ExprTree&) { }
  void visitExprTreeAfter(ExprTree&) { }

  //---------------------------------------
  // method for each ExprTree::Node subclass
  //---------------------------------------
  // default base class so that visitors can handle unknown
  // node sub-classes in a generic fashion
  void visitNode(ExprTree::Node&) { mIsMemRef = false; }

  void visitOpNode(ExprTree::OpNode& n) { mIsMemRef = false; }
  void visitCallNode(ExprTree::CallNode& n)  { mIsMemRef = false; }
  void visitMemRefNode(ExprTree::MemRefNode& n) 
    { mIsMemRef = true; mMemRef = n.getHandle(); }
  void visitConstSymNode(ExprTree::ConstSymNode& n) { mIsMemRef = false; }
  void visitConstValNode(ExprTree::ConstValNode& n) { mIsMemRef = false; }

  //! interface for results of visiting the expression tree
  bool isMemRef() { return mIsMemRef; }
  MemRefHandle getMemRef() { return mMemRef; }

private:
  bool mIsMemRef;
  MemRefHandle mMemRef;
};


} // end of OA namespace

#endif
