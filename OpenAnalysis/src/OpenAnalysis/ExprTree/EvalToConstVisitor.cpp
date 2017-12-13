/*! \file
  
  \brief Implementation of EvalToConstVisitor class.
  
  \authors Barbara Kreaseck, Michelle Strout
  \version $Id: EvalToConstVisitor.cpp,v 1.6 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <OpenAnalysis/ExprTree/EvalToConstVisitor.hpp>

static bool debug = false;

namespace OA {


EvalToConstVisitor::EvalToConstVisitor(OA_ptr<EvalToConstVisitorIRInterface> ir,
                                       OA_ptr<ReachConsts::Interface> rc)
    : mIR(ir), mReachConsts(rc)
{
    mEvalResult = NULL;
}

//! if we don't know how to handle a node then the default action
//! is to change the eval result to NULL
void EvalToConstVisitor::visitNode(ExprTree::Node&)
{
    mEvalResult = NULL;
}

void EvalToConstVisitor::visitOpNode(ExprTree::OpNode& n)
{
  OpHandle opH = n.getHandle();
  OA_ptr<ConstValBasicInterface> ch1; ch1 = NULL;
  OA_ptr<ConstValBasicInterface> ch2; ch2 = NULL;

  if (debug){
      std::cout << "In EvalToConstVisitor::visitOpNode---> OP="
                << mIR->returnOpEnumValInt(opH) 
                << " (" << n.num_children() << " children), ";
  }

  // check that there are at most 2 children
  if ( n.num_children() <= 2 ) {

    // if so then visit each child
    OA_ptr<ExprTree::Node> cetNodePtr;
    ExprTree::ChildNodesIterator cNodesIter(n);

    // child1
    if (cNodesIter.isValid()) {
      cetNodePtr = cNodesIter.current();

      cetNodePtr->acceptVisitor(*this);
      ch1 = getConstVal();
    }
    ++cNodesIter;

    // child2
    if (cNodesIter.isValid()) {
      cetNodePtr = cNodesIter.current();

      cetNodePtr->acceptVisitor(*this);
      ch2 = getConstVal();
    }
  
    if (debug){
      std::cout << " with children { (";
      if (ch1.ptrEqual(NULL)) {
        std::cout << "NULL";
      } else {
        std::cout << ch1->toString();
      }
      std::cout << ") (";
      if (ch2.ptrEqual(NULL)) {
        std::cout << "NULL";
      } else {
        std::cout << ch2->toString();
      }
      std::cout << ")" << std::endl;
      std::cout.flush();
    }

    mEvalResult = mIR->evalOp(opH,ch1,ch2);

  // there are more than 2 children
  } else {
    mEvalResult = NULL;
  }

  if (debug) {
      if (mEvalResult.ptrEqual(NULL)) {
        std::cout << " } evalResult (NULL)" << std::endl;
      } else {
        std::cout << " } evalResult (" << mEvalResult->toString() 
                  << ")" << std::endl;
      }
  }

}

void EvalToConstVisitor::visitCallNode(ExprTree::CallNode& n)
{
  mEvalResult = NULL;
}

void EvalToConstVisitor::visitMemRefNode(ExprTree::MemRefNode& n)
{
  MemRefHandle memref = n.getHandle();

  //ConstValBasicInterface *cvbiPtr = cpPtr->getReachConst(memref);
  //std::cout << "In MemRefNode::eval---> got memref (" << mIR->toString(memref)
  //          << ") with ReachConst (" << cvbiPtr->toString() << ")"
  //          << std::endl;
  
  if (mReachConsts.ptrEqual(NULL)) {
    mEvalResult = NULL;
  } else {
    // if this memref has its address taken, then this should result 
    // in no constant value.  need to check this ... BK
    mEvalResult = mReachConsts->getReachConst(memref);
  }

}

void EvalToConstVisitor::visitConstSymNode(ExprTree::ConstSymNode& n)
{
  mEvalResult = mIR->getConstValBasic(n.getHandle());
}

void EvalToConstVisitor::visitConstValNode(ExprTree::ConstValNode& n)
{
  mEvalResult = mIR->getConstValBasic(n.getHandle());
}


} // end of OA namespace

