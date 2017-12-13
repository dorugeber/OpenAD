/*! \file
  
  \brief Implementation of LinearityLocsVisitor class.
  
  \authors Luis Ramos
  \version $Id: LinearityLocsVisitor.cpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <OpenAnalysis/ExprTree/LinearityLocsVisitor.hpp>

static bool debug = false;

namespace OA {


LinearityLocsVisitor::LinearityLocsVisitor(OA_ptr<Linearity::LinearityIRInterface> ir,
                                           OA_ptr<Linearity::Interface> rc,
                                           OA_ptr<Alias::Interface> alias,
                                           OA_ptr<DataFlow::ParamBindings> paramBind)
    : mIR(ir), mLM(rc), mAlias(alias), mParamBind(paramBind)
{
    mIsConst = false;
    mDeps = new Linearity::LinearityDepsSet;
}

//! if we don't know how to handle a node then the default action
//! is to change the eval result to NULL
void LinearityLocsVisitor::visitNode(ExprTree::Node&)
{
    //std::cout << "\n--visitNode\n";
    mDeps = new Linearity::LinearityDepsSet;
    mIsConst = false;
}

void LinearityLocsVisitor::visitOpNode(ExprTree::OpNode& n)
{
  //std::cout << "\n--visitOpNode\n";
  mDeps = new Linearity::LinearityDepsSet;
  //Want to Call GetOpType, get the Enum Type, then Switch
  //between all the optypes...call appropiate Deps.
  //For Each Children, I have to call 
  //Need to get Hande
  //Deps = new LinearityDepsSet(child,mLM->getDepsSet(child);
  OpHandle opH = n.getHandle();
  OA_ptr<Linearity::LinearityDepsSet> ch1; ch1 = NULL;
  OA_ptr<Linearity::LinearityDepsSet> ch2; ch2 = NULL;
  bool ch1isConst = false;
  bool ch2isConst = false;

  // Get Operator Type
  Linearity::LinOpType opType = mIR->getLinearityOpType(opH);

  if (debug) {
  std::cout << "\nOperator OPR is:\n" << mIR->toString(opH);
  }
  // check that there are at most 2 children
  // Unary and Binary Operations
  if ( n.num_children() <= 2 ) {

    // if so then visit each child
    OA_ptr<ExprTree::Node> cetNodePtr;
    ExprTree::ChildNodesIterator cNodesIter(n);

    // child1
    if (cNodesIter.isValid()) {
      cetNodePtr = cNodesIter.current();

      cetNodePtr->acceptVisitor(*this);
      if (mIsConst == true)
          ch1isConst = true;
      ch1 = getDepsSet();
      //ch1 = new LinearityDepsSet(ch1
    }
    ++cNodesIter;

    // child2
    if (cNodesIter.isValid()) {
      cetNodePtr = cNodesIter.current();

      cetNodePtr->acceptVisitor(*this);
      if (mIsConst == true)
          ch1isConst = true;
      ch2 = getDepsSet();
    }

    switch (opType) {
        
        case Linearity::OPR_ADDSUB:
            {
                //std::cout << "\n|--OPR_ADDSUB--|\n";
                mDeps = ch1->addsub(ch2);
            }
            break;
        case Linearity::OPR_MPYDIV:
            {
                //std::cout << "\n|--OPR_MPYDIV--|\n";
                if (ch1isConst == true && ch2isConst == true)
                    mIsConst = true;
                else if (ch2isConst == true)
                    mDeps = ch1->linear();
                else if (ch1isConst == true)
                    mDeps = ch2->linear();
                else
                    mDeps = ch1->multdiv(ch2);
            }
            break;
        case Linearity::OPR_POWER:
            {
                //std::cout << "\n|--OPR_POWER--|\n";
                mDeps = ch1->powerk(ch2);
            }
            break;
        case Linearity::OPR_LINEAR:
            {
                //std::cout << "\n|--OPR_LINEAR--|\n";
                if (ch1.ptrEqual(NULL))
                    mDeps = ch2->linear();
                else
                    mDeps = ch1->linear();                
            }
            break;
        case Linearity::OPR_NONLINEAR:
                //std::cout << "\n|--OPR_NONLINEAR--|\n";
                if (ch1.ptrEqual(NULL))
                    mDeps = ch2->nonlinear();
                else
                    mDeps = ch1->nonlinear();    
            break;
        case Linearity::OPR_NONE:
            break;
    }
  

//    mEvalResult = mIR->evalOp(opH,ch1,ch2);

  // there are more than 2 children
  } else {
    std::cout << "\n--More than 2 Children\n";

  }

//  if (debug) {
//      if (mEvalResult.ptrEqual(NULL)) {
//        std::cout << " } evalResult (NULL)" << std::endl;
//      } else {
//        std::cout << " } evalResult (" << mEvalResult->toString() 
//                  << ")" << std::endl;
//      }
//  }

}

void LinearityLocsVisitor::visitCallNode(ExprTree::CallNode& n)
{
  //std::cout << "\n--visitCallNode\n";
  /*PseudoCode
   * Get ExprHandleIterator from param->getActualExprHandleIterator(n.getHandle());
   * for each ExprHandle in Iterator
   *    Get ExprTree from param->getActualExprTree(ExprHandleIter);
   *    Use MemRefVisitor to visit the ExprTree, ExprTree->acceptVisitor(MemRefVisitor);
   *    If MemRefVisitor has a MemRefHandles then
   *       Get MemRefIterator from MemRefVisitor->getMemRefsIterators();
   *       for each MemRefIter in Iterator
   *          Get MemRefHandle Iterator->current()
   *          Get Iterator for all the MayLocs from Alias->getMayLocs(MemRefHandle);
   *          for each Location in MayLocsIterator
   *            Add Location to locldSet = new LinearityDepsSet(loc,mLM->getDepsSet(loc));
   *            ldSet meet locldSet;
   *          end for
   *       end for
   *    end if
   * end for
   * mDeps = ldset->func(ldset);
   *              
   */
  mDeps = new Linearity::LinearityDepsSet;

  OA_ptr<Linearity::LinearityDepsSet> ldSet;
  ldSet = new Linearity::LinearityDepsSet();
   
  CallHandle call = n.getHandle();
  if (call == CallHandle(0)) 
      std::cout << "Call Handle is 0\n";
  else
      std::cout << "Call Handle String:\n" << mIR->toString(call);
 
  //std::cout.flush();
  OA_ptr<ExprHandleIterator> ExprHandleIter;
     ExprHandleIter = mParamBind->getActualExprHandleIterator(call);



  for ( ; ExprHandleIter->isValid(); (*ExprHandleIter)++ ) {
      ExprHandle expr = ExprHandleIter->current();
      // get ExprTrees of the actual parameters for the call
      // transitive:  so step into ExprTrees to get all MemRefs
      OA_ptr<ExprTree> eTree = mParamBind->getActualExprTree(expr);

      //std::cout << "\n  ++Valid Expression Handle\n";

      // get all the memrefs in the ExprTree
      MemRefsVisitor eTreeVisitor;
      eTree->acceptVisitor(eTreeVisitor);
      if (debug) { eTree->dump(std::cout,mIR); }
      if ( eTreeVisitor.hasMemRef() ) {
          
          //std::cout << "    ++Expression Tree has MemRef\n";
          
          // get memrefhandles
          OA_ptr<MemRefHandleIterator> memrefIter;
          memrefIter = eTreeVisitor.getMemRefsIterator();
          for (; memrefIter->isValid(); (*memrefIter)++ ) {

              //std::cout << "      ++Valid MemRef in ExprTree\n";
              
              MemRefHandle memref = memrefIter->current();

              // get all the may locs for the memrefs
              OA_ptr<LocIterator> locIter = mAlias->getMayLocs(memref);
              for ( ; locIter->isValid(); (*locIter)++ ) {

                  //std::cout << "        ++Valid Location\n";
                  
                  OA_ptr<Location> loc = locIter->current();
                  OA_ptr<Linearity::LinearityDepsSet> locldSet;
                    locldSet = new Linearity::LinearityDepsSet(loc,mLM->getDepsSet(loc));
                    ldSet = ldSet->meet(locldSet);
              }
          }
      }
  }
  mDeps = ldSet->func(ldSet);
  mIsConst = false;
}

void LinearityLocsVisitor::visitMemRefNode(ExprTree::MemRefNode& n)
{
  //std::cout << "\n--visitMemRefNode\n";
  mDeps = new Linearity::LinearityDepsSet;
  MemRefHandle memref = n.getHandle();

//  if (mLM.ptrEqual(NULL)) {
//    mDeps = NULL;
//  } else {

// stmt: a = b;
//
// if this was preceeded by:
//   x is independent
//   if (c == 3) {
//     b = &p    {<*b,<*b,p>,linear>}
//   } else {
//     b = &q    {<*b,<*b,q>,linear>}
//   }
//   {*b -> p,q}
//   p = 2* x    {<*b,p,linear>,<*b,q,linear>,<p,x,linear>}
//   
//   q = x*x     {                                        ,<q,x,nonlinear>}
//   
//   a = *b      depsa = {<p,lin>, <q,lin>, <x,nonlin>, <*b,lin>}
  
    // get alias results for this procedure
    //OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(mProc);
    
    // maylocs need to be used for Dep pairs so that we get
    // conservative results
    OA_ptr<Linearity::LinearityDepsSet> ldSet;
    ldSet = new Linearity::LinearityDepsSet();
   
    OA_ptr<LocIterator> locIterPtr = mAlias->getMayLocs(memref);
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
        OA_ptr<Location> loc = locIterPtr->current();
        if (debug) {
          std::cout << "MemRef::Loc: ";
          loc->output(*mIR);
        }
        //OA_ptr<LinearityDepsSet> locldSet = mLM->getDepsSet(loc);
        OA_ptr<Linearity::LinearityDepsSet> locldSet;
          locldSet = new Linearity::LinearityDepsSet(loc,mLM->getDepsSet(loc));
          //locldSet->output(*mIR);
        ldSet = ldSet->meet(locldSet);
    }
    mDeps = ldSet;
    mIsConst = false;
//  }

}

void LinearityLocsVisitor::visitConstSymNode(ExprTree::ConstSymNode& n)
{
  //std::cout << "\n--visitConstSymNode\n";
  mDeps = new Linearity::LinearityDepsSet(0.0);
  mIsConst = true;
}

void LinearityLocsVisitor::visitConstValNode(ExprTree::ConstValNode& n)
{
  //std::cout << "\n--visitConstValNode\n";
  mDeps = new Linearity::LinearityDepsSet(0.0);
  mIsConst = true;
}


} // end of OA namespace

