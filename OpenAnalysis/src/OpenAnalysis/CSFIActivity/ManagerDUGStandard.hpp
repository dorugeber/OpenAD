/*! \file
  
  \brief Declarations of the AnnotationManager that generates a DUGStandard
         from the CFG standards for each procedure

  \authors Michelle Strout
  \version $Id: ManagerDUGStandard.hpp,v 1.2 2005/06/10 02:32:04 mstrout Exp $

  Copyright (c) 2002-2004, Rice University <br>
  Copyright (c) 2004, University of Chicago <br>  
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>

*/

#ifndef DUGManagerStandard_h
#define DUGManagerStandard_h

//--------------------------------------------------------------------
// OpenAnalysis headers
#include <OpenAnalysis/IRInterface/DUGIRInterface.hpp>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/ExprTree/ExprTree.hpp>
#include <OpenAnalysis/CFG/EachCFGInterface.hpp>
#include <OpenAnalysis/CFG/CFGInterface.hpp>
#include <OpenAnalysis/SideEffect/InterSideEffectStandard.hpp>
#include <OpenAnalysis/SideEffect/ManagerInterSideEffectStandard.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/Activity/DepDFSet.hpp>
#include <OpenAnalysis/CSFIActivity/DUGStandard.hpp>


/*
#include <queue>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
*/

namespace OA {
  namespace DUG {

/*! 
   The AnnotationManager for DUGStandard.
   This class can build an DUGStandard, 
   (eventually) read one in from a file, and write one out to a file.
*/
class ManagerDUGStandard { 
public:
  ManagerDUGStandard(OA_ptr<DUGIRInterface> _ir, 
                     OA_ptr<Activity::ActivityIRInterface> _air);
 ~ManagerDUGStandard () {}

  OA_ptr<DUGStandard> performAnalysis( OA_ptr<IRProcIterator>,
                                       OA_ptr<DataFlow::ParamBindings>,
                                       OA_ptr<OA::CallGraph::CallGraphInterface>);

  void transitiveClosureDepMatrix(OA_ptr<OA::CallGraph::CallGraphInterface>);

private: // helper functions

  bool stmt_has_call(StmtHandle stmt);

  void insertEdge(SymHandle, SymHandle, EdgeType, CallHandle, ProcHandle, ProcHandle, ProcHandle);
  void labelCallRetEdges(StmtHandle, ProcHandle);
  void labelUseDefEdges(StmtHandle, ProcHandle);
  void collectIndependentSyms( ProcHandle);
  void collectDependentSyms( ProcHandle);

  void setDepMatrix(ProcHandle, SymHandle, SymHandle);
  void transitiveClosure(ProcHandle proc);
  void edgesBetweenActuals(ProcHandle proc);

  void setDepMatrix4Globals(SymHandle, SymHandle, ProcHandle);
  bool isPathThruOtherProcs(SymHandle, SymHandle, ProcHandle);
  bool isOutgoingToOtherProcs(SymHandle, ProcHandle);
  bool isIncomingFromOtherProcs(SymHandle, ProcHandle);
  bool hasEdgesToOtherProc(SymHandle, ProcHandle);
  bool hasEdgesFromOtherProc(SymHandle, ProcHandle);
  bool isLocal(SymHandle, ProcHandle);
  SymHandle getQuickAndDirtySymHandle(OA_ptr<MemRefExpr>);

private: // member variables

  OA_ptr<DUGIRInterface> mIR;
  OA_ptr<Activity::ActivityIRInterface> mActIR;
  OA_ptr<DUGStandard> mDUG;     
  OA_ptr<DataFlow::ParamBindings> mParamBind;

  // dependence matrix
  std::map<ProcHandle, 
           std::map<SymHandle, 
                    std::map<SymHandle, bool> > > mProcToMatrix;
  std::map<ProcHandle, std::set<SymHandle> > mProcToSymSet;
  // just to prevent duplicate edges between the same nodes
  std::map<EdgeType, std::map<SymHandle, 
                              std::map<SymHandle, bool> > > mMatrix;

  // for making edges between actual parameters
  // proc -> call sites that call 'proc'
  std::map<ProcHandle, std::set<CallHandle> > mProcToCallsiteSet;
  // proc -> formal parameters of proc
  std::map<ProcHandle, std::set<SymHandle> > mProcToFormalSet;
  // call site -> (formal -> set of actuals)
  std::map<CallHandle, 
           std::map<SymHandle, 
                    std::set<SymHandle> > > mFormalToActualMap;
  // call site -> proc containing the call site, i.e. the caller
  std::map<CallHandle, ProcHandle> mCallsiteToProc;

  // procedures to be processed. 
  // all procedures reachable from the ones with DEP and INDEP vars
  std::set<ProcHandle> mProcsOfInterest;
};

//! Visitors added by PLM 09/22/06

class IndepLocVisitor : public virtual LocationVisitor {
  public:
      
    IndepLocVisitor(OA_ptr<DUGStandard> dug, 
                    ProcHandle proc, 
                    std::set<ProcHandle> ProcsOfInterest,
                    OA_ptr<DUGIRInterface> ir
                    ) 
                  : mDUG(dug), mProc(proc), mProcsOfInterest(ProcsOfInterest), mIR(ir) 
    { }
    ~IndepLocVisitor() { }

    void visitNamedLoc(NamedLoc& loc);
    void visitUnnamedLoc(UnnamedLoc& loc);
    void visitInvisibleLoc(InvisibleLoc& loc);
    void visitUnknownLoc(UnknownLoc& loc);
    void visitLocSubSet(LocSubSet& loc);
    
    std::set<ProcHandle> getmProcsOfInterest() 
    {
        return mProcsOfInterest;
    }
  private:
    OA_ptr<DUGStandard> mDUG;
    ProcHandle mProc;
    std::set<ProcHandle> mProcsOfInterest;
    OA_ptr<DUGIRInterface> mIR;

};

class depLocVisitor : public virtual LocationVisitor {
  public:
    depLocVisitor(OA_ptr<DUGStandard> dug, 
                  ProcHandle proc,
                  std::set<ProcHandle> ProcsOfInterest ,
                  OA_ptr<DUGIRInterface> ir
                  )
                 : mDUG(dug), mProc(proc), mProcsOfInterest(ProcsOfInterest), mIR(ir)
    { }
    ~depLocVisitor() {}

    void visitNamedLoc(NamedLoc& loc);
    void visitUnnamedLoc(UnnamedLoc& loc);
    void visitInvisibleLoc(InvisibleLoc& loc);
    void visitUnknownLoc(UnknownLoc& loc);
    void visitLocSubSet(LocSubSet& loc);
    std::set<ProcHandle> getmProcsOfInterest()
    {
        return mProcsOfInterest;
    }

  private: 
     OA_ptr<DUGStandard> mDUG;
     ProcHandle mProc; 
     std::set<ProcHandle> mProcsOfInterest;
     OA_ptr<DUGIRInterface> mIR;
};



  } // end of DUG namespace
} // end of OA namespace

#endif
