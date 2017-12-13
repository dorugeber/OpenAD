/*! \file
  
  \brief Implementation of ParamBindings

  \authors Michelle Strout 
  \version $Id: ParamBindings.cpp,v 1.3 2007/10/08 19:19:11 utke Exp $


  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ParamBindings.hpp"

namespace OA {
  namespace DataFlow {

//! get caller mem ref associated with callee formal
MemRefHandle 
ParamBindings::getCallerMemRef(CallHandle call, SymHandle calleeFormal)
{
    return mCalleeToCallerMap[call][calleeFormal];
}

//! returns an iterator over the memory reference parameters for a call
OA_ptr<MemRefHandleIterator> ParamBindings::getActualIterator(CallHandle call)
{
    OA_ptr<MemRefIterator> retval;
    OA_ptr<std::set<OA::MemRefHandle> > retSet;
    OA_ptr<std::set<OA::MemRefHandle> > memrefSet = mCallToActualSetMap[call];
    if (memrefSet.ptrEqual(0)) {
        retSet = new std::set<OA::MemRefHandle>;
    } else {
        retSet = mCallToActualSetMap[call];
    }
    retval = new MemRefIterator(retSet);
    return retval;
}

//! get callee formal associated with caller mem ref
SymHandle 
ParamBindings::getCalleeFormal(CallHandle call, MemRefHandle callerRef,
                               ProcHandle callee)
{
    return mCallerToCalleeMap[call][callee][callerRef];
}

//! returns an iterator over the formal symbols for a procedure
OA_ptr<SymHandleIterator> ParamBindings::getFormalIterator(ProcHandle proc)
{
    OA_ptr<SymIterator> retval;
    if (mProcToFormalSetMap[proc].ptrEqual(0)) {
        OA_ptr<std::set<SymHandle> > emptySet;
        emptySet = new std::set<SymHandle>;
        retval = new SymIterator(emptySet);
    } else {
        retval = new SymIterator(mProcToFormalSetMap[proc]);
    }
    return retval;
}

  //! returns an iterator over the actual ExprHandles for a call in order
OA_ptr<ExprHandleIterator> 
ParamBindings::getActualExprHandleIterator(CallHandle call) {

  OA_ptr<ExprHandleIterator> retval;
  if (mCallToExprListMap[call].ptrEqual(0)) {
    OA_ptr<std::list<ExprHandle> > emptyList;
    emptyList = new std::list<ExprHandle>;
    retval = new ExprIterator(emptyList);
  } else {
    retval = new ExprIterator(mCallToExprListMap[call]);
  }
  return retval;
}

//! get ExprTree for actual ExprHandle
OA_ptr<ExprTree> ParamBindings::getActualExprTree(ExprHandle expr){
  return mExprToTreeMap[expr];
}

//! get actual ExprHandle in CallHandle for formal SymHandle
ExprHandle ParamBindings::getActualExprHandle(CallHandle call, SymHandle formal) {
  return mCallToFormalToActualMap[call][formal];
}




//*****************************************************************
// Construction methods 
//*****************************************************************
void ParamBindings::mapMemRefToFormal(CallHandle call, MemRefHandle actual, 
                                      ProcHandle callee, SymHandle sym)
{
    mCallerToCalleeMap[call][callee][actual] = sym;
    mCalleeToCallerMap[call][sym] = actual;
    if (mCallToActualSetMap[call].ptrEqual(0)) {
        mCallToActualSetMap[call] = new std::set<MemRefHandle>;
    }
    mCallToActualSetMap[call]->insert(actual);
}


void ParamBindings::mapFormalToProc(SymHandle formal, ProcHandle proc) 
{
    if (mProcToFormalSetMap[proc].ptrEqual(0)) {
        mProcToFormalSetMap[proc] = new std::set<SymHandle>;
    }
    mProcToFormalSetMap[proc]->insert(formal);
}

/*
void ParamBindings::mapActualToCall(MemRefHandle actual, ExprHandle call) 
{
    if (mCallToActualSetMap[call].ptrEqual(0)) {
        mCallToActualSetMap[call] = new std::set<MemRefHandle>;
    }
    mCallToActualSetMap[call]->insert(actual);
}
*/


void ParamBindings::mapCallToExprList(CallHandle call,
                                      OA_ptr<std::list<ExprHandle> > elist)
{
  mCallToExprListMap[call] = elist;
}

void ParamBindings::mapExprToTree(ExprHandle expr, OA_ptr<ExprTree> etree)
{
  mExprToTreeMap[expr] = etree;
}

void ParamBindings::mapFormalToExpr(CallHandle call, SymHandle formal, 
                                    ExprHandle act_expr)
{
  mCallToFormalToActualMap[call][formal] = act_expr;
}


void ParamBindings::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    os << "====================== ParamBindings" << std::endl;

    // iterate over all the calls we have information about
    std::map<CallHandle,std::map<ProcHandle,std::map<MemRefHandle,SymHandle> > >::iterator callIter;
    for (callIter=mCallerToCalleeMap.begin();
         callIter!=mCallerToCalleeMap.end(); callIter++) 
    {
        /*
        os << "Call = " << ir->toString(callIter->first) << std::endl;
        std::map<MemRefHandle,SymHandle>::iterator memRefIter;
        for (memRefIter=callIter->second.begin();
             memRefIter!=callIter->second.end(); memRefIter++)
        {
            MemRefHandle ref = memRefIter->first;
            SymHandle sym = memRefIter->second;
            os << "\tCaller MemRef = " << ir->toString(ref);
            os << ", Callee Formal = " << ir->toString(sym);
            os << std::endl;
        }
        */
    }

}

  } // end of DataFlow namespace
} // end of OA namespace

