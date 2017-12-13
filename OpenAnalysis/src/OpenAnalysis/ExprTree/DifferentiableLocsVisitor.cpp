/*! \file
  
  \brief Implementation of DifferentiableLocsVisitor class.
  
  \authors Michelle Strout
  \version $Id: DifferentiableLocsVisitor.cpp,v 1.4 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <OpenAnalysis/ExprTree/DifferentiableLocsVisitor.hpp>

namespace OA {

//static bool debug = true;

DifferentiableLocsVisitor::DifferentiableLocsVisitor(
          OA_ptr<Alias::Interface> alias) : mAlias(alias)
{
}

//! if we don't know how to handle a node then need to assert
//! because there is no default conservative action for this visitor
void DifferentiableLocsVisitor::visitNode(ExprTree::Node&)
{
    assert(0);
}

void DifferentiableLocsVisitor::visitOpNode(ExprTree::OpNode& n)
{
    
  // visit each child
  OA_ptr<ExprTree::Node> nodePtr;
  ExprTree::ChildNodesIterator cNodesIter(n);
  for ( ; cNodesIter.isValid(); ++cNodesIter) {
      nodePtr = cNodesIter.current();

      nodePtr->acceptVisitor(*this);
  } 
}

void DifferentiableLocsVisitor::visitCallNode(ExprTree::CallNode& n)
{
  // not doing anything because callsite parameter expressions
  // will be visited separately

}

void DifferentiableLocsVisitor::visitMemRefNode(ExprTree::MemRefNode& n)
{
    
  MemRefHandle memref = n.getHandle();

  //if (debug) {
  //  std::cout << "DiffLocsVisitor::visitMemRefNode():\n"
  //            << "\tmemRefHandle = (hval= " << memref.hval() << ")\n";
  //}

  // add all may locations of this reference to our set of differentiable
  // locations.  Notice that MemRefNode will only have top
  // memory reference handle.  For example, if memory reference is A[i] then
  // there is no way to get to memory reference handle for i through the
  // expression tree for A[i] + 3.  This is good in this case because array
  // index variables aren't considered differentiable.
  OA_ptr<LocIterator> locIterPtr = mAlias->getMayLocs(memref);

  for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
    //if (debug) {
    //  std::cout << "\t\tinserting loc into mDiffLocs\n";
    //}
      mDiffLocs->insert(locIterPtr->current());
  }
}

void DifferentiableLocsVisitor::visitConstSymNode(ExprTree::ConstSymNode& n)
{
    
  // constants aren't differentiable
}

void DifferentiableLocsVisitor::visitConstValNode(ExprTree::ConstValNode& n)
{

  // constants aren't differentiable
}


} // end of OA namespace

