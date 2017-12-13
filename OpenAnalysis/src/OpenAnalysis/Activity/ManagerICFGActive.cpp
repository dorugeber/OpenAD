/*! \file
  
  \brief The AnnotationManager that generates InterActive INTERprocedurally.

  \authors Michelle Strout
  \version $Id: ManagerICFGActive.cpp,v 1.4 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <iostream>
#include "ManagerICFGActive.hpp"
#include <Utils/Util.hpp>



namespace OA {
  namespace Activity {

static bool debug = false;

ManagerICFGActive::ManagerICFGActive(
    OA_ptr<Activity::ActivityIRInterface> _ir) : mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerICFGActive:ALL", debug);
    mSolver = new DataFlow::ICFGDFSolver(DataFlow::ICFGDFSolver::Backward,*this);
}

/*!
   First calls Manager's to get ICFGDep and ICFGUseful results.

   Then does Vary and Activity analysis at same time over the ICFG using
   an ICFGDFSolver.
*/
OA_ptr<InterActive> 
ManagerICFGActive::performAnalysis(
        OA_ptr<ICFG::ICFGInterface> icfg,
        OA_ptr<DataFlow::ParamBindings> paramBind,
        OA_ptr<Alias::InterAliasInterface> interAlias,
        OA_ptr<SideEffect::InterSideEffectInterface> interSE,
        DataFlow::DFPImplement algorithm)
{
  OA_ptr<InterActive> retval;
  retval = new InterActive;

  // create a Manager that generates dep information for each statement in
  // ICFG
  OA_ptr<ManagerICFGDep> depman;
  depman = new ManagerICFGDep(mIR);
  OA_ptr<ICFGDep> icfgDep = depman->performAnalysis(icfg, paramBind,
                                                    interAlias,
                                                    algorithm);

  if (debug) { icfgDep->output(*mIR); }

 
  if (debug) {
      std::cout << "Calling usefulman->performAnalysis() ...\n";
      std::cout.flush();
  }

  OA_ptr<ManagerICFGUseful> usefulman;
  usefulman = new ManagerICFGUseful(mIR);
  OA_ptr<InterUseful> 
    interUseful = usefulman->performAnalysis(icfg, paramBind, interAlias, 
                                             interSE, icfgDep,
                                             algorithm);

  retval->setNumIterUseful(interUseful->getNumIter());
  if (debug) { interUseful->output(*mIR); }
  
  // ManagerICFGVaryActive does vary analysis and determines which
  // locations are active coming Into a stmt and Outof a stmt
  if (debug) {
      std::cout << "Calling varyman->performAnalysis() ...\n";
      std::cout.flush();
  }

  OA_ptr<ManagerICFGVaryActive> varyman;
  varyman = new ManagerICFGVaryActive(mIR);
  OA_ptr<ActivePerStmt> active = varyman->performAnalysis(icfg, paramBind,
          interAlias, icfgDep, interUseful,algorithm);

  retval->setNumIterVary(active->getNumIter());
  if (debug) { active->output(*mIR); }
  
  
  //-------------------------------------------------------------
  // Do backward dataflow analysis to determine which def memrefs
  // and stmts are active
  
  // store results that will be needed in callbacks
  mICFG = icfg;
  mParamBind = paramBind;
  mICFGDep = icfgDep;
  mInterAlias = interAlias;
  mActive = active;

  // call iterative data-flow solver for ICFG
  mSolver->solve(icfg,algorithm);

  // then iterate over each statement to find active use memrefs

  /*! Merge Conflict PLM 08/24/06
  // For each ICFG node:
  OA_ptr<ICFG::ICFGStandard::NodesIterator> nodeIter = 
    icfg->getNodesIterator();
  for ( ; nodeIter->isValid(); (*nodeIter)++) {

    OA_ptr<ICFG::ICFGStandard::Node> icfgNode = nodeIter->current();

    // get alias and active results for current procedure
    ProcHandle proc = icfgNode->getProc();
    OA_ptr<Alias::Interface> alias = interAlias->getAliasResults(proc);
    OA_ptr<ActiveStandard> active = mActiveMap[proc];

    // For each stmt in the ICFG node:
    OA_ptr<CFG::Interface::NodeStatementsIterator> stmtIter =
      icfgNode->getNodeStatementsIterator();
    for ( ; stmtIter->isValid(); (*stmtIter)++) {
      StmtHandle stmt = stmtIter->current();

      // if this is an active statement, mark all inVaryMemRefs as active
      if (active->isActive(stmt)) {

        OA_ptr<MemRefHandleIterator> useIter = mIR->getUseMemRefs(stmt);
        for (; useIter->isValid(); (*useIter)++ ) {
          MemRefHandle use = useIter->current();
          if (debug) {
            std::cout << "\tuse = ";
            mIR->dump(use,std::cout);
          }
          
          // iterate over locations that are active anywhere
          // in procedure, flow-insensitive active locations
          OA_ptr<LocIterator> procActiveLocIter 
            = active->getActiveLocsIterator();
          for ( ; procActiveLocIter->isValid(); (*procActiveLocIter)++ ) {
            OA_ptr<Location> activeLoc = procActiveLocIter->current();
            if (debug) { 
              std::cout << "\t activeLoc = ";
              activeLoc->dump(std::cout,mIR);
            }
            
            // iterate over may locs for use
            OA_ptr<LocIterator> mayLocIter = alias->getMayLocs(use);
            for ( ; mayLocIter->isValid(); (*mayLocIter)++ ) {
              OA_ptr<Location> useLoc = mayLocIter->current();
              if (debug) { 
                std::cout << "\t\t may loc for use, useLoc = ";
                useLoc->dump(std::cout,mIR);
              }
              if (activeLoc->mayOverlap(*useLoc)) {
                active->insertMemRef(use);
                if (debug) {
                  std::cout << "\t\tinserting active memref use = ";
                  mIR->dump(use,std::cout);
                  std::cout << std::endl;
                }

              } // if active loc mayOverlap the mayloc of the use memref
                // then use memref is set active

            } // for each mayloc of the use memref

          } // for each active locations anywhere in the procedure

        } // iterate over use memrefs
        
      } // if active stmt
       
    } // for each stmt in the icfg node 

  } // for each icfg node

*/




  // For each ICFG node:
  OA_ptr<ICFG::NodesIteratorInterface> nodeIter = 
    icfg->getICFGNodesIterator();
  for ( ; nodeIter->isValid(); (*nodeIter)++) {

    OA_ptr<ICFG::NodeInterface> icfgNode = nodeIter->currentICFGNode();

    // get alias and active results for current procedure
    ProcHandle proc = icfgNode->getProc();
    OA_ptr<Alias::Interface> alias = interAlias->getAliasResults(proc);
    OA_ptr<ActiveStandard> active = mActiveMap[proc];

    // For each stmt in the ICFG node:
    OA_ptr<CFG::NodeStatementsIteratorInterface> stmtIter =
      icfgNode->getNodeStatementsIterator();
    for ( ; stmtIter->isValid(); (*stmtIter)++) {
      StmtHandle stmt = stmtIter->current();

      // if this is an active statement, mark all inVaryMemRefs as active
      if (active->isActive(stmt)) {

        OA_ptr<MemRefHandleIterator> useIter = mIR->getUseMemRefs(stmt);
        for (; useIter->isValid(); (*useIter)++ ) {
          MemRefHandle use = useIter->current();
          if (debug) {
            std::cout << "\tuse = ";
            mIR->dump(use,std::cout);
          }
          
          // iterate over locations that are active anywhere
          // in procedure, flow-insensitive active locations
          OA_ptr<LocIterator> procActiveLocIter 
            = active->getActiveLocsIterator();
          for ( ; procActiveLocIter->isValid(); (*procActiveLocIter)++ ) {
            OA_ptr<Location> activeLoc = procActiveLocIter->current();
            if (debug) { 
              std::cout << "\t activeLoc = ";
              activeLoc->dump(std::cout,mIR);
            }
            
            // iterate over may locs for use
            OA_ptr<LocIterator> mayLocIter = alias->getMayLocs(use);
            for ( ; mayLocIter->isValid(); (*mayLocIter)++ ) {
              OA_ptr<Location> useLoc = mayLocIter->current();
              if (debug) { 
                std::cout << "\t\t may loc for use, useLoc = ";
                useLoc->dump(std::cout,mIR);
              }
              if (activeLoc->mayOverlap(*useLoc)) {
                active->insertMemRef(use);
                if (debug) {
                  std::cout << "\t\tinserting active memref use = ";
                  mIR->dump(use,std::cout);
                  std::cout << std::endl;
                }

              } // if active loc mayOverlap the mayloc of the use memref
                // then use memref is set active

            } // for each mayloc of the use memref

          } // for each active locations anywhere in the procedure

        } // iterate over use memrefs
        
      } // if active stmt
       
    } // for each stmt in the icfg node 

  } // for each icfg node






  // assign activity results for each procedure into InterActive
  std::map<ProcHandle,OA_ptr<ActiveStandard> >::iterator mapIter;
  for (mapIter=mActiveMap.begin(); mapIter!=mActiveMap.end(); mapIter++) {
    retval->mapProcToActive(mapIter->first,mapIter->second);
  }

  // how many iterations did this take?
  if (debug) 
    {
      std::cout << "ICFGActive took " << mSolver->getNumIter()
                << " iterations.\n";
    }

  retval->setNumIterActive(mSolver->getNumIter());

  return retval;
}

//========================================================
// implementation of ICFGDFProblem interface
//========================================================
//--------------------------------------------------------
// initialization callbacks
//--------------------------------------------------------

/*!
   Data-flow set passed around on the icfg is a
   LocDFSet.  The top value for this is an empty set.
*/
OA_ptr<DataFlow::DataFlowSet> ManagerICFGActive::initializeTop()
{
  OA_ptr<DataFlow::LocDFSet> retval;
  retval = new DataFlow::LocDFSet;
  return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::initializeNodeIN(OA_ptr<ICFG::NodeInterface> n)
{
  OA_ptr<DataFlow::LocDFSet> retval;
  retval = new DataFlow::LocDFSet;
  return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::initializeNodeOUT(OA_ptr<ICFG::NodeInterface> n)
{
  OA_ptr<DataFlow::LocDFSet> retval;
  retval = new DataFlow::LocDFSet;
  return retval;
}


//--------------------------------------------------------
// solver callbacks 
//--------------------------------------------------------
  
//! OK to modify set1 and return it as result, because solver
//! only passes a tempSet in as set1
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::meet(OA_ptr<DataFlow::DataFlowSet> set1, 
                      OA_ptr<DataFlow::DataFlowSet> set2)
{
    // just return in set
    return set1;
}

//! OK to modify in set and return it again as result because
//! solver clones the BB in sets
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::transfer(ProcHandle proc,
                        OA_ptr<DataFlow::DataFlowSet> out, OA::StmtHandle stmt)
{    
  
  // ignoring data flow, using outActiveSet from previous analysis
    OA_ptr<DataFlow::LocDFSet> outRecast =  mActive->getOutActiveSet(stmt);

    if (debug) {
        std::cout << "In transfer, stmt(hval=" << stmt.hval() << ")= ";
        mIR->dump(stmt,std::cout);
        std::cout << "\toutRecast = ";
        outRecast->dump(std::cout,mIR);
    }

    // will be storing activity results for stmt and memrefs in
    // ActiveStandard for current procedure
    if (mActiveMap[proc].ptrEqual(0)) {
        mActiveMap[proc] = new ActiveStandard(proc);
    }
        
    // get alias and active results for current procedure
    OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(proc);
    OA_ptr<ActiveStandard> active = mActiveMap[proc];

    // iterate over def mem refs and if any of them access a location
    // that is in the out active set then the mem ref is active and so
    // is the stmt
    bool activeStmt = false;
    OA_ptr<MemRefHandleIterator> defIter = mIR->getDefMemRefs(stmt);
    for (; defIter->isValid(); (*defIter)++ ) {
        MemRefHandle def = defIter->current();
        if (debug) {
          std::cout << "\tdef = ";
          mIR->dump(def,std::cout);
        }

        bool activeMemRef = false;
        // loop over may locs for the memref
        OA_ptr<LocIterator> locIter = alias->getMayLocs(def);
        for ( ; locIter->isValid()&&!activeMemRef; (*locIter)++ ) {
          OA_ptr<Location> loc = locIter->current();
          if (debug) { 
            std::cout << "\t\t mayloc for def = ";
            loc->dump(std::cout,mIR);
          }

          // if at least one may loc of this def overlaps with
          // something in the active set ... then we know
          // that this def is active, and we know
          // that this stmt is active as well.
          if (outRecast->hasOverlapLoc(loc)) {
            active->insertMemRef(def);

            // don't need to look at any more mayLocs, since we
            // already know that this def is active ... set flag
            activeMemRef = true;

            if (!activeStmt) { 
              // now we know it is, insert and set flag
              active->insertStmt(stmt);
              activeStmt = true;
            }
          }
        }
    } 

    // since we are ignoring in/out data flow sets, only one iteration
    // iterate through all of the active locations that come into this
    // statement and insert those locations as active within the
    // procedure
    OA_ptr<DataFlow::LocDFSet> inActiveSet = mActive->getInActiveSet(stmt);
    OA_ptr<LocIterator> locIter = inActiveSet->getLocIterator();
    for ( ; locIter->isValid(); (*locIter)++ ) {
        active->insertLoc(locIter->current());
    } 

    // If we are ignoring the incoming set (see comments at top of transfer)
    // upshot:  cuts the iterations by one. (no impact on correct output)
    return out;
}

/*!
   Will get the indep vars if this procedure has any and add them
   to incoming LocDFSet
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::entryTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> in)
{
    return in;
}

/*!
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::exitTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> out)
{
    return out;
}


//! Propagate a data-flow set from caller to callee
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::callerToCallee(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee)
{
  // no data flow for this analysis ... returning in set
  return dfset;
}
  
//! Propagate a data-flow set from callee to caller
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGActive::calleeToCaller(ProcHandle callee,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle caller)
{
  // no data flow for this analysis ... returning in set
  return dfset;
}

//! Propagate a data-flow set from call node to return node
OA_ptr<DataFlow::DataFlowSet>
ManagerICFGActive::callToReturn(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee)
{
  // no data flow for this analysis ... returning in set
  return dfset;
}

  } // end of namespace Activity
} // end of namespace OA
