/*! \file
  
  \brief One-to-one mapping between formal parameter symbols in callee and
         memory reference handles in caller.

  \authors Michelle Strout 
  \version $Id: ParamBindings.hpp,v 1.5 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ParamBindings_h
#define ParamBindings_h

#include <iostream>
#include <map>

#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Utils/GenOutputTool.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>
#include <OpenAnalysis/ExprTree/ExprTree.hpp>

namespace OA {
  namespace DataFlow {

class ParamBindings : public virtual Annotation {
public:
  ParamBindings() { }
  ~ParamBindings() {}

  //! get caller mem ref associated with callee formal
  MemRefHandle getCallerMemRef(CallHandle call, SymHandle calleeFormal);

  //! returns an iterator over the memory reference parameters for a call
  OA_ptr<MemRefHandleIterator> getActualIterator(CallHandle call);

  //! get callee formal associated with caller mem ref
  SymHandle getCalleeFormal(CallHandle call, MemRefHandle callerRef,
                            ProcHandle callee);

  //! returns an iterator over the formal symbols for a procedure
  OA_ptr<SymHandleIterator> getFormalIterator(ProcHandle proc);

  //! returns true if given formal is a reference parameter
  //bool isRefParam(SymHandle formal);

  //! returns an iterator over the actual ExprHandles for a call in order
  OA_ptr<ExprHandleIterator> getActualExprHandleIterator(CallHandle call);

  //! get ExprTree for actual ExprHandle
  OA_ptr<ExprTree> getActualExprTree(ExprHandle expr);

  //! get actual ExprHandle in CallHandle for formal SymHandle
  ExprHandle getActualExprHandle(CallHandle call, SymHandle formal);

  //*****************************************************************
  // Output
  //*****************************************************************
  void output(IRHandlesIRInterface& ir);

  //! debugging output
  void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);
  
  //*****************************************************************
  // Construction methods 
  //*****************************************************************
  void mapMemRefToFormal(CallHandle call, MemRefHandle ref, 
                         ProcHandle callee, SymHandle sym);
  //void setRefParam(SymHandle sym);
  void mapFormalToProc(SymHandle formal, ProcHandle proc);
  //void mapActualToCall(MemRefHandle actual, CallHandle call);

  void mapCallToExprList(CallHandle call,OA_ptr<std::list<ExprHandle> > elist);
  void mapExprToTree(ExprHandle act_expr, OA_ptr<ExprTree> etree);
  void mapFormalToExpr(CallHandle call, SymHandle formal, ExprHandle act_expr);

private:
  OUTPUT

  GENOUT std::map<CallHandle,std::map<ProcHandle,std::map<MemRefHandle,SymHandle> > > 
      mCallerToCalleeMap;
  GENOUT std::map<CallHandle,std::map<SymHandle,MemRefHandle> > mCalleeToCallerMap;
  std::map<ProcHandle,OA_ptr<std::set<SymHandle> > > mProcToFormalSetMap;
  std::map<CallHandle,OA_ptr<std::set<MemRefHandle> > > mCallToActualSetMap;
  //std::map<SymHandle,bool> mIsRefParam;

  GENOUT std::map<CallHandle,OA_ptr<std::list<ExprHandle> > > mCallToExprListMap;
  GENOUT std::map<ExprHandle,OA_ptr<ExprTree> > mExprToTreeMap;
  GENOUT std::map<CallHandle,std::map<SymHandle,ExprHandle> > mCallToFormalToActualMap;
  

  //! An iterator over Symbols
  class SymIterator : public virtual OA::SymHandleIterator,
                      public OA::IRHandleSetIterator<OA::SymHandle>
  {
  public:
    SymIterator(OA::OA_ptr<std::set<OA::SymHandle> > pSet) 
      : OA::IRHandleSetIterator<OA::SymHandle>(pSet) {}
    ~SymIterator() {}

    void operator++() { OA::IRHandleSetIterator<OA::SymHandle>::operator++(); }
    bool isValid() const
      { return OA::IRHandleSetIterator<OA::SymHandle>::isValid(); }
    OA::SymHandle current() const
      { return OA::IRHandleSetIterator<OA::SymHandle>::current(); }
    void reset() { OA::IRHandleSetIterator<OA::SymHandle>::reset(); }
  };
  
  //! An iterator over MemRefHandle's
  class MemRefIterator : public virtual OA::MemRefHandleIterator,
                         public OA::IRHandleSetIterator<OA::MemRefHandle>
  {
  public:
    MemRefIterator(OA::OA_ptr<std::set<OA::MemRefHandle> > pList) 
      : OA::IRHandleSetIterator<OA::MemRefHandle>(pList) {}
    ~MemRefIterator() {}

    void operator++() { OA::IRHandleSetIterator<OA::MemRefHandle>::operator++(); }
    bool isValid() const
      { return OA::IRHandleSetIterator<OA::MemRefHandle>::isValid(); }
    OA::MemRefHandle current() const
      { return OA::IRHandleSetIterator<OA::MemRefHandle>::current(); }
    void reset() { OA::IRHandleSetIterator<OA::MemRefHandle>::reset(); }
  };

  //! An iterator over ExprHandles that is initialized with a list
  class ExprIterator : public virtual OA::ExprHandleIterator,
                      public OA::IRHandleListIterator<OA::ExprHandle>
  {
  public:
    ExprIterator(OA::OA_ptr<std::list<OA::ExprHandle> > eList) 
      : OA::IRHandleListIterator<OA::ExprHandle>(eList) {}
    ~ExprIterator() {}

    void operator++() 
      { OA::IRHandleListIterator<OA::ExprHandle>::operator++(); }
    bool isValid() const
      { return OA::IRHandleListIterator<OA::ExprHandle>::isValid(); }
    OA::ExprHandle current() const
      { return OA::IRHandleListIterator<OA::ExprHandle>::current(); }
    void reset() { OA::IRHandleListIterator<OA::ExprHandle>::reset(); }
  };
  


};

  } // end of DataFlow namespace
} // end of OA namespace

#endif
