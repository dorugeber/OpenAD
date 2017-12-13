/*! \file
  
  \brief Concrete ExprTreeVisitor that determines which locations are
         differentiable in an expression.  Basically all non-constants that
         are not index expressions.
  
  \authors Michelle Strout
  \version $Id: DifferentiableLocsVisitor.hpp,v 1.4 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef DifferentiableLocsVisitor_H
#define DifferentiableLocsVisitor_H

#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class DifferentiableLocsVisitor : public ExprTreeVisitor {
public:
  DifferentiableLocsVisitor(OA_ptr<Alias::Interface> alias);
  ~DifferentiableLocsVisitor() {}

  void visitExprTreeBefore(ExprTree&) 
    { mDiffLocs = new LocSet; }
  void visitExprTreeAfter(ExprTree&) { }

  //---------------------------------------
  // method for each ExprTree::Node subclass
  //---------------------------------------
  // default base class so that visitors can handle unknown
  // node sub-classes in a generic fashion
  void visitNode(ExprTree::Node&);

  void visitOpNode(ExprTree::OpNode& n);
  void visitCallNode(ExprTree::CallNode& n);
  void visitMemRefNode(ExprTree::MemRefNode& n);
  void visitConstSymNode(ExprTree::ConstSymNode& n);
  void visitConstValNode(ExprTree::ConstValNode& n);

  //! interface for results of visiting the expression tree
  OA_ptr<LocIterator> getDiffLocsIterator()
  { OA_ptr<LocIterator> retval;
    retval = new LocSetIterator(mDiffLocs);
    return retval;
  }

private:
  OA_ptr<LocSet> mDiffLocs;
  OA_ptr<Alias::Interface> mAlias;
};


} // end of OA namespace

#endif
