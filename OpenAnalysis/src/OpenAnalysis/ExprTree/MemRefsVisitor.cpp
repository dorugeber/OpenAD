/*! \file
  
  \brief Implementation of MemRefsVisitor class.
  
  \authors Barbara Kreaseck
  \version $Id: MemRefsVisitor.cpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <OpenAnalysis/ExprTree/MemRefsVisitor.hpp>

namespace OA {

//static bool debug = false;

//! if we don't know how to handle a node then need to assert
//! because there is no default conservative action for this visitor
void MemRefsVisitor::visitNode(ExprTree::Node&)
{
    assert(0);
}

void MemRefsVisitor::visitOpNode(ExprTree::OpNode& n)
{
  // visit each child
  OA_ptr<ExprTree::Node> nodePtr;
  ExprTree::ChildNodesIterator cNodesIter(n);
  for ( ; cNodesIter.isValid(); ++cNodesIter) {
      nodePtr = cNodesIter.current();

      nodePtr->acceptVisitor(*this);
  } 
}

void MemRefsVisitor::visitCallNode(ExprTree::CallNode& n)
{
  // not doing anything because callsite parameter expressions
  // will be visited separately

}

void MemRefsVisitor::visitMemRefNode(ExprTree::MemRefNode& n)
{
    mHasMemRef = true;
    mMemRefSet->insert(n.getHandle());
}

void MemRefsVisitor::visitConstSymNode(ExprTree::ConstSymNode& n)
{
  // constants aren't memrefs (BK: Is this right??)
}

void MemRefsVisitor::visitConstValNode(ExprTree::ConstValNode& n)
{
  // constants aren't memrefs
}


} // end of OA namespace

