/*! \file
  
  \brief The AnnotationManager that generates InterUseful using ICFGDFSolver.

  \authors Michelle Strout
  \version $Id: ManagerICFGUseful.cpp,v 1.3 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerICFGUseful.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace Activity {

static bool debug = false;

/*!
*/
ManagerICFGUseful::ManagerICFGUseful( OA_ptr<ActivityIRInterface> _ir) : mIR(_ir)
{
  OA_DEBUG_CTRL_MACRO("DEBUG_ManagerICFGUseful:ALL", debug);
  mSolver = new DataFlow::ICFGDFSolver(DataFlow::ICFGDFSolver::Backward,*this);
}

OA_ptr<Activity::InterUseful> 
ManagerICFGUseful::performAnalysis(
        OA_ptr<ICFG::ICFGInterface> icfg,
        OA_ptr<DataFlow::ParamBindings> paramBind,
        OA_ptr<Alias::InterAliasInterface> interAlias,
        OA_ptr<OA::SideEffect::InterSideEffectInterface> interSE,
        OA_ptr<ICFGDep> icfgDep,
        DataFlow::DFPImplement algorithm)
{
  // store results that will be needed in callbacks
  mICFG = icfg;
  mParamBind = paramBind;
  mICFGDep = icfgDep;
  mInterAlias = interAlias;
  mInterSE = interSE;

  // create an empty InterUseful
  mInterUseful = new InterUseful();

  // call iterative data-flow solver for ICFG
  mSolver->solve(icfg,algorithm);

  if (debug) {
      std::cout << std::endl << "%+%+%+%+ Useful numIter = " 
                << mSolver->getNumIter() 
                << " +%+%+%+%" << std::endl << std::endl;
  }

  mInterUseful->setNumIter(mSolver->getNumIter());    

  // assign results for each procedure into InterUseful
  std::map<ProcHandle,OA_ptr<UsefulStandard> >::iterator mapIter;
  for (mapIter=mUsefulMap.begin(); mapIter!=mUsefulMap.end(); mapIter++) {
    mInterUseful->mapProcToUseful(mapIter->first,mapIter->second);
  }

  return mInterUseful;
}

//========================================================
// implementation of ICFGDFProblem interface
//========================================================
//--------------------------------------------------------
// initialization callbacks
//--------------------------------------------------------

/*!
   Data-flow set passed around on the call graph is an
   LocDFSet.  The top value for this is an empty set.
*/
OA_ptr<DataFlow::DataFlowSet> ManagerICFGUseful::initializeTop()
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::initializeNodeIN(OA_ptr<ICFG::NodeInterface> n)
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::initializeNodeOUT(OA_ptr<ICFG::NodeInterface> n)
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    return retval;
 }


//--------------------------------------------------------
// solver callbacks 
//--------------------------------------------------------
  
/*! Unions location sets
 */
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::meet(OA_ptr<DataFlow::DataFlowSet> set1, 
                        OA_ptr<DataFlow::DataFlowSet> set2)
{
    if (debug) {
        std::cout << "-------- ManagerICFGUseful::meet" << std::endl;
    }
    OA_ptr<DataFlow::LocDFSet> remapSet1 = set1.convert<DataFlow::LocDFSet>();
    OA_ptr<DataFlow::LocDFSet> remapSet2 = set2.convert<DataFlow::LocDFSet>();
    if (debug) {
        std::cout << "\tremapSet1 = ";
        remapSet1->dump(std::cout, mIR);
        std::cout << "\tremapSet2 = ";
        remapSet2->dump(std::cout, mIR);
    }

    // making sure we don't trounce set1 just in case
    OA_ptr<DataFlow::LocDFSet> retval;
    OA_ptr<DataFlow::DataFlowSet> temp = remapSet1->clone();
    retval = temp.convert<DataFlow::LocDFSet>();
    *retval = retval->setUnion(*remapSet2);
    if (debug) {
        std::cout << "\tretval = ";
        OA_ptr<DataFlow::LocDFSet> temp = retval.convert<DataFlow::LocDFSet>();
        temp->dump(std::cout, mIR);
    }
    return retval;
}

//! OK to modify in set and return it again as result because
//! solver clones the BB in sets
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::transfer(ProcHandle proc,
                        OA_ptr<DataFlow::DataFlowSet> out, OA::StmtHandle stmt)
{   
    // convert incoming set to more specific subclass
    OA_ptr<DataFlow::LocDFSet> outRecast = out.convert<DataFlow::LocDFSet>();
    OA_ptr<DataFlow::DataFlowSet> temp = outRecast->clone();
    OA_ptr<DataFlow::LocDFSet> outUseful = temp.convert<DataFlow::LocDFSet>();
    OA_ptr<DataFlow::LocDFSet> inUseful;
    inUseful = new DataFlow::LocDFSet;
    if (debug) {
        std::cout << "In transfer, stmt (" << mIR->toString(stmt) << ")";
        std::cout << "\n\toutRecast = ";
        outRecast->dump(std::cout,mIR);
    }

    // store outUseful set (below stmt) for stmt into procedure's Useful result
    if (mUsefulMap[proc].ptrEqual(0)) {
      mUsefulMap[proc] = new UsefulStandard(proc);
    }
    mUsefulMap[proc]->copyIntoOutUseful(stmt,outUseful);
    
    // use dep pairs to determine what locations should be in InUseful (above)
    DataFlow::LocDFSetIterator outLocIter(*outRecast);

    for ( ; outLocIter.isValid(); ++outLocIter ) {
      OA_ptr<Location> outLoc = outLocIter.current();
      if (debug) {
          std::cout << "\toutLoc = ";
          outLoc->dump(std::cout, mIR);
      }
      OA_ptr<LocIterator> locIterPtr;
      locIterPtr = mICFGDep->getDiffUseIterator(stmt,outLoc);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          inUseful->insert(locIterPtr->current());
      } 
    }
    
    // store inUseful (above stmt) for stmt into procedure's Useful results
    mUsefulMap[proc]->copyIntoInUseful(stmt,inUseful);

    if (debug) {
        std::cout << "\tinUseful = ";
        inUseful->dump(std::cout,mIR);
    }

    return inUseful;
}

/*!
   Will get the dep vars if this procedure has any and add them
   to incoming LocDFSet
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::exitTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> out)
{
  if (debug) {
    std::cout << "-------- exitTransfer\n";
  }
    // create retval as copy of out
    OA_ptr<DataFlow::LocDFSet> retval;
    OA_ptr<DataFlow::LocDFSet> outRecast = out.convert<DataFlow::LocDFSet>();
    retval = new DataFlow::LocDFSet(*outRecast);

   // get iterator over dep MemRefExpr for procedure
    OA_ptr<MemRefExprIterator> depIter = mIR->getDepMemRefExprIter(proc);
    // get alias results for this procedure
    OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(proc);

    if (mUsefulMap[proc].ptrEqual(0)) {
        mUsefulMap[proc] = new UsefulStandard(proc);
    }
    for ( depIter->reset(); depIter->isValid(); (*depIter)++ ) {
      OA_ptr<MemRefExpr> memref = depIter->current();

      if (debug) {
        std::cout << "    dep memref dump = '";
        memref->dump(std::cout, mIR);
        std::cout << "'\n";
        std::cout << "    dep memref output = '";
        memref->output(*mIR);
        std::cout << "'\n";
      }

      // get may locs for memref
      OA_ptr<LocIterator> locIter = alias->getMayLocs(*memref,proc);
      for (locIter->reset(); locIter->isValid(); (*locIter)++ ) {
        if (debug) {
          std::cout << "        found dep location dump: '";
          (locIter->current())->dump(std::cout,mIR);
          std::cout << "'\n";
          std::cout << "        found dep location output: '";
          (locIter->current())->output(*mIR);
          std::cout << "'\n";
        }
        retval->insert(locIter->current());
        mUsefulMap[proc]->insertDepLoc(locIter->current());
      }
    }
    if (debug) {
        std::cout << "\tManagerICFGUseful, Dep locations for proc "
                  << mIR->toString(proc) << ": " << std::endl;
        retval->dump(std::cout, mIR);
        std::cout << "----" << std::endl;
    }

    return retval;
}

/*!
   Just pass along out because this won't be called since we are a Backward
   analysis
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::entryTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> in)
{
    assert(0);
    return in;
}


//! Propagate a data-flow set from caller to callee
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::callerToCallee(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee)
{
  // backward flow: (must be a return edge) on the way in
  // only need to do regular callerToCallee 
  //   (callerToCalleeTransitive makes no sense here)
  // When we modify callerToCallee() we should make similar changes
  //   to CallToReturn(), as it is performing a parallel operation

    OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();
    if(debug) {
      std::cout << "======== ManagerICFGUseful::callerToCallee" << std::endl;
      std::cout << "    LocDFSet from Caller:\n";
      inRecast->dump(std::cout,mIR);
    }
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = inRecast->callerToCallee(caller,call,callee,mInterAlias,
                                      mParamBind,mIR);
    if (debug) {
      std::cout << "    LocDFSet in Callee from nonTrans:\n";
      retval->dump(std::cout,mIR);
    }

    OA_ptr<Location> loc;
    DataFlow::LocDFSetIterator inLocIter(*inRecast);
    for ( ; inLocIter.isValid(); ++inLocIter ) {
       loc = inLocIter.current();
       if (debug) {
           std::cout << "\t\tUseful loc = ";
           loc->dump(std::cout,mIR);
       }
       if( loc->isaSubSet() ) {
           OA_ptr<Location> innerLoc;
           OA_ptr<LocSubSet> sloc = loc.convert<LocSubSet>();
           innerLoc = sloc->getLoc();
           while(innerLoc->isaSubSet()) {
               sloc = innerLoc.convert<LocSubSet>();
               innerLoc = sloc->getLoc();
           }
           if(innerLoc->isaNamed()) {
              OA_ptr<NamedLoc> nloc = innerLoc.convert<NamedLoc>();
              if(!nloc->isLocal()) {
                  retval->insert(loc);
              }
           }
       } else {
         if( loc->isaNamed() ) {
             OA_ptr<NamedLoc> nloc = loc.convert<NamedLoc>();
             if(!nloc->isLocal() ) {
                retval->insert(loc);
             }
         }
       }
    }

    return retval;
}
  
//! Propagate a data-flow set from callee to caller
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGUseful::calleeToCaller(ProcHandle callee,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle caller)
{
    if (debug) {
        std::cout << "-------- ManagerICFGUseful::calleeToCaller" << std::endl;
    }
    OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();

    if (debug) {
      std::cout << "    LocDFSet from Callee:\n";
      inRecast->dump(std::cout,mIR);
    }
    // backward flow: must be a call edge (on the way out)
    // need to do both regular and transitive calleeToCaller conversions
    OA_ptr<DataFlow::LocDFSet> trans;
    OA_ptr<DataFlow::LocDFSet> non_trans;
    non_trans = inRecast->calleeToCaller(callee,call,caller,mInterAlias,
                                    mParamBind,mIR);
    if (debug) {
      std::cout << "    LocDFSet in Caller from NonTrans:\n";
      non_trans->dump(std::cout,mIR);
    }
    trans = inRecast->calleeToCallerTransitive(callee,call,caller,mInterAlias,
                                               mParamBind,mIR);
    if (debug) {
      std::cout << "    LocDFSet in Caller from Trans:\n";
      trans->dump(std::cout,mIR);
    }
    // return the union of the two
    *trans = trans->setUnion(*non_trans);
    if (debug) {
      std::cout << "    LocDFSet in Caller from Trans/NonTrans Union:\n";
      trans->dump(std::cout,mIR);
    }
    return trans;

    // MMA following code was what was used instead of the calleeToCaller
    //     routines above.  The above routine has been re-worked, and may
    //     be what we want. 
    //     Need to compare output from above with output from below.
    /**********

    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet();

    OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();
    if (debug) {
        std::cout << "\tinRecast = ";
        inRecast->dump(std::cout, mIR);
    }

    // iterate over all locations in callee that are useful and
    // find all locations in caller that are associated with them
    OA_ptr<LocIterator> locIter = inRecast->getLocIterator();
    for (; locIter->isValid(); (*locIter)++ ) {
        OA_ptr<Location> loc = locIter->current();

        CalleeToCallerMayTransVisitor locVisitor(call, caller, mInterAlias,
                                                 mInterSE, mParamBind);
        loc->acceptVisitor(locVisitor);
        OA_ptr<LocIterator> callerLocIter = locVisitor.getCallerLocIterator();
        for ( ; callerLocIter->isValid(); (*callerLocIter)++ ) {
            OA_ptr<Location> callerLoc = callerLocIter->current();
            retval->insert( callerLoc );
            if (debug) {
                std::cout << "\tinserting callerLoc = ";
                callerLoc->dump(std::cout, mIR);
            }
        }
    }
    if (debug) {
        std::cout << "\tretval = ";
        retval->dump(std::cout, mIR);
    }
    return retval;
    **********************/
    // end MMA
}



 /**********
   * new pseudocode for removing killed definitions inside
   * procedure along CallReturn Edge.
   * e.g
   * {local a, formal p, global g}
   *      |
   *      |           CALL_EDGE
   * p1. call foo(p) ------------>   entry foo(p)
   *      |                         {
   *      |Call_RETURN_EDGE
   *     {a}                           p = ...
   *      |                            g = ...
   *      |           RETURN_EDGE   }
   * p2. return foo  <------------   exit foo
   *
   * We should get {a} after program point {p2} as 'p' and 'g'
   * are killed along the path through foo.
   *
   * pseudocode:
   * Remove NonLocal Locations from the Useful Set
   * for each actualParameter at Callsite get the MemRefHandle
   *   for each mre for memref, mIR->getMemRefExprIterator
   *     found_locs = true
   *     deref_mre = mre clone
   *     while(found_locs)
   *       found_locs = false
   *       deref = new deref of null memref
   *       deref_mre = deref->composeWith(deref_mre)
   *       query the alias analysis of Caller for mustlocs of deref_mre(killSet)
   *       for each mustLocs
   *           found_locs = true
   *           if mustLoc is found in the useful set then
   *              remove it from the useful set
   ******************************************
   */

//! Propagate a data-flow set from call node to return node
OA_ptr<DataFlow::DataFlowSet>
ManagerICFGUseful::callToReturn(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee){

  // Original code
  // create retval as copy of dfset
  OA_ptr<DataFlow::LocDFSet> remapdfset = dfset.convert<DataFlow::LocDFSet>();
  OA_ptr<DataFlow::LocDFSet> retval;
  OA_ptr<DataFlow::DataFlowSet> temp = remapdfset->clone();
  retval = temp.convert<DataFlow::LocDFSet>();
  OA_ptr<LocIterator> usefulIter = remapdfset->getLocIterator();

  // Remove NonLocal Locations from the Useful Set
  usefulIter->reset();
  std::set<OA_ptr<Location> > removeLocs;
  for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
       OA_ptr<Location> loc = usefulIter->current();
       if (debug) {
           std::cout << "\t\tinVary loc = ";
           loc->dump(std::cout,mIR);
       }
       if( loc->isaNamed() ) {
           OA_ptr<NamedLoc> nloc = loc.convert<NamedLoc>();
           if(!nloc->isLocal()) {
               removeLocs.insert(loc);
           }
       }
  }
  std::set<OA_ptr<Location> >::iterator rIter;
  for( rIter = removeLocs.begin() ;
       rIter != removeLocs.end();
       ++rIter ) {
       OA_ptr<Location> varyLoc = *rIter;
       retval->remove(varyLoc);
  }

  OA_ptr<MemRefHandleIterator> mrefIter;
  mrefIter = mParamBind->getActualIterator(call);
  for( ;mrefIter->isValid(); ++(*mrefIter) ) {
      MemRefHandle memref;
      memref = mrefIter->current();
      OA_ptr<MemRefExprIterator> mreIter;
      mreIter = mIR->getMemRefExprIterator(memref);
      for (; mreIter->isValid(); (*mreIter)++) {
           OA_ptr<MemRefExpr> mre_actual = mreIter->current();
           OA_ptr<MemRefExpr> deref_mre = mre_actual->clone();
           bool found_locs = true;
           while (found_locs) {
                found_locs = false;
                // Deref mre
                OA_ptr<MemRefExpr> nullmre;
                OA_ptr<Deref> deref;
                deref = new Deref(MemRefExpr::USE,nullmre,1);
                deref_mre = deref->composeWith(deref_mre);
                OA_ptr<Alias::Interface> callerAlias
                   = mInterAlias->getAliasResults(caller);
                // query the alias analysis of Caller for maylocs of deref_mre
                OA_ptr<LocIterator> mayLocIter;
                mayLocIter = callerAlias->getMayLocs(*deref_mre,caller);
                for (; mayLocIter->isValid(); (*mayLocIter)++) {
                     found_locs = true;
                     OA_ptr<Location> loc = mayLocIter->current();
                     if (debug) {
                       std::cout << "\t\tmayloc for memref, loc = ";
                       loc->output(*mIR);
                     }
                     usefulIter->reset();
                     for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
                          OA_ptr<Location> usefulloc = usefulIter->current();
                           if (loc->mustOverlap(*usefulloc)) {
                               retval->remove(usefulloc);
                           }
                     }
                }
           }
      }
  }

  return retval;

  /*************************************
  // The problem with callToReturn: (FIXME sometime)
  //
  // want to logically split the dfset into two parts:  
  //   partCallee: the locations that caused CallerToCallee to add locations 
  //               to the callee set
  //   partNonCallee: the locations that did not cause CallerToCallee to add
  //               locations to the callee set
  // want to return the partNonCallee to travel up the CALL_RETURN edge.
  //
  // The problem is that there may be multiple CALL/RETURN edge pairs for 
  // each CallNode (due to enhanced alias analysis and possibly multiple 
  // callsites per call statement).  The CALL_RETURN edge does not even
  // contain a usable CallHandle (it is currently CallHandle(0)) never
  // mind a CallHandle for each callsite in the call statement
  //
  // So, while LocDFSet::CallerToCalleeNot(caller,call,callee,mInterAlias,
  // mParamBind,mIR) is designed to return the residual set of locations
  // not converted into a callee set equivalent, it cannot work without a
  // definite call/callee pair (the caller is no problem).
  //
  // Proposed fix:  get call stmt from callNode, iterate over callsites,
  // call LocDFSet::callerToCalleeNot() for each call/callee pair
  // and aggregate the results (setIntersection on the residual sets?
  //
  // Not Complete:  08/08/06 BK
  //

  OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();
  if(debug) {
    std::cout << "======== ManagerICFGUseful::callToReturn" << std::endl;
    std::cout << "    LocDFSet from Caller:\n";
    inRecast->dump(std::cout,mIR);
  }
  OA_ptr<DataFlow::LocDFSet> retval;
  retval = inRecast->callerToCalleeNot(caller,call,callee,mInterAlias,
                                       mParamBind,mIR);
  if (debug) {
    std::cout << "    residual LocDFSet in Caller from nonTransNot:\n";
    retval->dump(std::cout,mIR);
  }
  return retval;
  ***********************************/
}

  } // end of namespace Activity
} // end of namespace OA
