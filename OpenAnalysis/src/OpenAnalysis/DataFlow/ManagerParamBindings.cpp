/*! \file
  
  \brief The AnnotationManager that generates ParamBindings.

  \authors Michelle Strout
  \version $Id: ManagerParamBindings.cpp,v 1.7 2008/05/21 15:16:05 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerParamBindings.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace DataFlow {

static bool debug = false;

ManagerParamBindings::ManagerParamBindings(
    OA_ptr<DataFlow::ParamBindingsIRInterface> _ir) : mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerParamBindings:ALL", debug);
}

/*!
*/
OA_ptr<ParamBindings> 
ManagerParamBindings::performAnalysis( OA_ptr<CallGraph::CallGraphInterface> callGraph )
{
  // empty set of parameter bindings that we are going to fill
  OA_ptr<ParamBindings> retval;
  retval = new ParamBindings;

  if (debug) {
      std::cout << "In ManagerParamBindings::performAnalysis" << std::endl;
  }

  // for each node in the call graph get the set of formal 
  // parameters (that are actually refefenced)

  
  /*! commented out by PLM 08/11/06
  OA_ptr<CallGraph::Interface::NodesIterator> nodeIter=callGraph->getNodesIterator();
  */
  
  
  OA_ptr<CallGraph::NodesIteratorInterface> nodeIter = callGraph->getCallGraphNodesIterator();
  for (;nodeIter->isValid();++(*nodeIter)) {
    OA_ptr<CallGraph::NodeInterface> aCGNode=nodeIter->currentCallGraphNode();
    // don't bother with things we don't have a definition for 
    if (!aCGNode->isDefined()) 
      continue;
    ProcHandle aProcHandle=aCGNode->getProc();

    // iterate over all statements in the procedure
    OA_ptr<IRStmtIterator> stmtIterPtr = mIR->getStmtIterator(aProcHandle);
    // Iterate over the statements of this block adding procedure references
    for ( ; stmtIterPtr->isValid(); ++(*stmtIterPtr)) {
        StmtHandle stmt = stmtIterPtr->current();

        // get all of the memory references for the statement
        OA_ptr<MemRefHandleIterator> mrIterPtr = mIR->getAllMemRefs(stmt);
        for (; mrIterPtr->isValid(); (*mrIterPtr)++ ) {
            MemRefHandle memref = mrIterPtr->current();

            // get the memory reference expressions for this handle
            OA_ptr<MemRefExprIterator> mreIterPtr 
                = mIR->getMemRefExprIterator(memref);
      
            // for each mem-ref-expr associated with this memref
            for (; mreIterPtr->isValid(); (*mreIterPtr)++) {
                OA_ptr<OA::MemRefExpr> mre = mreIterPtr->current();
                if (debug) {
                    std::cout << "\tmre = ";
                    mre->output(*mIR);
                    std::cout << std::endl;
                }

                // check all symbol handles within MRE to see if formal
                FormalFinderVisitor visitor(mIR, aProcHandle, retval);
                mre->acceptVisitor(visitor);
            }
        }
    }
  }

  OA_ptr<std::list<ExprHandle> > exprList;

  // for each edge in the call graph get parameter binding
  OA_ptr<CallGraph::EdgesIteratorInterface> 
      edgeIter = callGraph->getCallGraphEdgesIterator();
  for ( ; edgeIter->isValid(); ++(*edgeIter) ) {
    OA_ptr<CallGraph::EdgeInterface> edge = edgeIter->currentCallGraphEdge();
    // only do all this if the callee is defined
    /*! commented out by PLM 08/11/06
    OA_ptr<CallGraph::Interface::Node> callee = edge->sink();
    if (!callee->isDefined()) {
      continue;
    }
    */

    OA_ptr<CallGraph::NodeInterface> callee = edge->getCallGraphSink();
    if(!callee->isDefined()) {
       
        continue;
    }

    // get procedure handle for caller
    /*! commented out by PLM 08/11/06
    OA_ptr<CallGraph::Interface::Node> caller = edge->source();
    */
    OA_ptr<CallGraph::NodeInterface> caller = edge->getCallGraphSource();
    ProcHandle callerProc = caller->getProc();

    // get procedure handle for callee
    ProcHandle calleeProc = callee->getProc();

    // iterate over parameters at call site
    CallHandle call = edge->getCallHandle();
    OA_ptr<IRCallsiteParamIterator> paramIterPtr = mIR->getCallsiteParams(call);
    exprList = new std::list<ExprHandle>;
    for ( ; paramIterPtr->isValid(); (*paramIterPtr)++ ) {
        ExprHandle param = paramIterPtr->current();
        
        // compile list of ExprHandles of actual parameters in-order
        exprList->push_back(param);

        if (debug) {
          std::cout << "==== param = " << mIR->toString(param) << std::endl;
          std::cout << "callerProc = " << mIR->toString(callerProc) 
                    << ", hval = " << callerProc.hval() << std::endl;
          std::cout << "calleeProc = " << mIR->toString(calleeProc) 
                    << ", hval = " << calleeProc.hval() << std::endl;
        }

        OA_ptr<ExprTree> eTreePtr = mIR->getExprTree(param);

        // associate this ExprTree with this ExprHandle for future reference
        retval->mapExprToTree(param,eTreePtr);
        
        // get formal associated with callsite param
        SymHandle formal = mIR->getFormalForActual(callerProc, call, 
                                                   calleeProc, param);

        // map the formal SymHandle to the actual ExprHandle
        retval->mapFormalToExpr(call, formal, param);
     
        // associate formal parameter with  the procedure
        retval->mapFormalToProc(formal, calleeProc);

        // if the parameter is a memory reference then get the memory reference
        EvalToMemRefVisitor evalVisitor;
        eTreePtr->acceptVisitor(evalVisitor);
        if (debug) { eTreePtr->dump(std::cout,mIR); }
        MemRefHandle memref;
        if ( evalVisitor.isMemRef() ) {
          memref = evalVisitor.getMemRef();
        } else {
          continue;
        }

        // map the actual memref to the formal
        retval->mapMemRefToFormal(call, memref, calleeProc, formal);

    } // loop over parameters at callsite
    
    // map the callHandle to the list of actual ExprHandles
    retval->mapCallToExprList(call, exprList);

  }

  return retval;
}

//----------------------------------------------------------
void FormalFinderVisitor::visitUnnamedRef(UnnamedRef& ref) 
{ 
}

void FormalFinderVisitor::visitUnknownRef(UnknownRef& ref) 
{ 
}

void FormalFinderVisitor::visitAddressOf(AddressOf& ref)
{
}

void FormalFinderVisitor::visitNamedRef(NamedRef& ref) 
{
    if (mIR->isParam(ref.getSymHandle())) { 
        // associate formal parameter with  the procedure
        mParamBind->mapFormalToProc(ref.getSymHandle(), mProc);
    }
}

void FormalFinderVisitor::visitDeref(Deref& ref) 
{ 
  // first call recursively on what we are a dereference for
  OA_ptr<MemRefExpr> mre = ref.getMemRefExpr();
  if (!mre.ptrEqual(0)) { mre->acceptVisitor(*this); }
}   

void FormalFinderVisitor::visitSubSetRef(SubSetRef& ref) 
{
  // first call recursively on what we are a subset of
  OA_ptr<MemRefExpr> mre = ref.getMemRefExpr();
  if (!mre.ptrEqual(0)) { mre->acceptVisitor(*this); }
}



  } // end of namespace DataFlow
} // end of namespace OA
