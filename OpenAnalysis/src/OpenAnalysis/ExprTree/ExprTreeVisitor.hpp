/*! \file
  
  \brief Abstract visitor for ExprTree's
  
  \authors Michelle Strout
  \version $Id: ExprTreeVisitor.hpp,v 1.6 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ExprTreeVisitor_H
#define ExprTreeVisitor_H

#include <OpenAnalysis/ExprTree/ExprTree.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! ExprTreeVisitor is an abstract base class used to implement the 
    Visitor pattern on ExprTree Nodes.

    The ExprTree::acceptVisitor method will call visitExprTreeBefore,
    visitExprTreeAfter, and acceptVisitor on the root node.  If you
    want to traverse other nodes then you must call acceptVisitor on
    children nodes when appropriate.  See EvalToConstVisitor::visitOpNode
    for an example.

    Memory Management:  It is assumed that the concrete visitors
    will NOT keep references to expression tree nodes and that
    the expression tree will not be deallocated while an ExprTreeVisitor
    is visiting an ExprTree.
    Concrete visitors should also never take the address of a Node
    or ExprTree and pass it to a method that requires an OA_ptr<Node>.
*/
class ExprTreeVisitor {
protected:
  ExprTreeVisitor() {}
public:
  virtual ~ExprTreeVisitor() {}

  //! called before root noded in the expression tree is visited
  virtual void visitExprTreeBefore(ExprTree&) = 0;

  //! called after root noded in the expression tree is visited
  virtual void visitExprTreeAfter(ExprTree&) = 0;

  //---------------------------------------
  // method for each ExprTree::Node subclass
  //---------------------------------------

  // default base class so that visitors can handle unknown
  // node sub-classes in a generic fashion
  virtual void visitNode(ExprTree::Node&) = 0;

  virtual void visitOpNode(ExprTree::OpNode& n) { visitNode(n); }
  virtual void visitCallNode(ExprTree::CallNode& n) { visitNode(n); } 
  virtual void visitMemRefNode(ExprTree::MemRefNode& n)  { visitNode(n); }
  virtual void visitConstSymNode(ExprTree::ConstSymNode& n)  { visitNode(n); }
  virtual void visitConstValNode(ExprTree::ConstValNode& n)  { visitNode(n); }

};


} // end of OA namespace

#endif
