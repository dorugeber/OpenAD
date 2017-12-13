/*! \file
  
  \brief The AnnotationManager that generates ActivePerStmt.

  \authors Michelle Strout
  \version $Id: ManagerICFGVaryActive.cpp,v 1.1 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerICFGVaryActive.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace Activity {

static bool debug = false;

/*!
*/
ManagerICFGVaryActive::ManagerICFGVaryActive( 
        OA_ptr<ActivityIRInterface> _ir) : mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerICFGVaryActive:ALL", debug);
    mSolver = new DataFlow::ICFGDFSolver(DataFlow::ICFGDFSolver::Forward,*this);
}

OA_ptr<Activity::ActivePerStmt> 
ManagerICFGVaryActive::performAnalysis(
        OA_ptr<ICFG::ICFGInterface> icfg,
        OA_ptr<DataFlow::ParamBindings> paramBind,
        OA_ptr<Alias::InterAliasInterface> interAlias,
        OA_ptr<ICFGDep> icfgDep,
        OA_ptr<InterUseful> interUseful,
        DataFlow::DFPImplement algorithm)
{
  // store results that will be needed in callbacks
  mICFG = icfg;
  mParamBind = paramBind;
  mICFGDep = icfgDep;
  mInterAlias = interAlias;
  mInterUseful = interUseful;

  // create an empty ActivePerStmt
  mActive = new ActivePerStmt();

  // call iterative data-flow solver for ICFG
  mSolver->solve(icfg,algorithm);

  OA_ptr<DataFlow::LocDFSet> retval;
  OA_ptr<LocIterator> usefulIter;
  OA_ptr<DataFlow::LocDFSet> varySet;

  // For each ICFG node:
  OA_ptr<ICFG::NodesIteratorInterface> nodeIter =
    icfg->getICFGNodesIterator();
  for ( ; nodeIter->isValid(); (*nodeIter)++) {
    OA_ptr<ICFG::NodeInterface> icfgNode = nodeIter->currentICFGNode();
    // get alias and active results for current procedure
    ProcHandle proc = icfgNode->getProc();
    OA_ptr<Alias::Interface> alias = interAlias->getAliasResults(proc);
    OA_ptr<UsefulStandard> useful = mInterUseful->getUsefulResults(proc);

    // For each stmt in the ICFG node:
    OA_ptr<CFG::NodeStatementsIteratorInterface> stmtIter =
      icfgNode->getNodeStatementsIterator();

    // for the first statement InActive and OutActive are
    // manual calculations.

    if( stmtIter->isValid() ) {

        StmtHandle stmt = stmtIter->current();

        // calculate InActive
        retval = new DataFlow::LocDFSet;

        usefulIter = useful->getInUsefulIterator(stmt);
        varySet = mActive->getInActiveSet(stmt);
        DataFlow::LocDFSetIterator varyIter(*varySet);
        for ( ; varyIter.isValid(); ++varyIter ) {
             OA_ptr<Location> loc = varyIter.current();
             usefulIter->reset();
             for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
                if (loc->mayOverlap(*(usefulIter->current())) ) {
                    retval->insert(loc);
                    retval->insert(usefulIter->current());
                }
             }
        }
        mActive->copyIntoInActive(stmt, retval);

        // calculate OutActive.
        retval = new DataFlow::LocDFSet;
        usefulIter = useful->getOutUsefulIterator(stmt);
        varySet = mActive->getOutActiveSet(stmt);
        DataFlow::LocDFSetIterator varyIter1(*varySet);
        for ( ; varyIter1.isValid(); ++varyIter1 ) {
           OA_ptr<Location> loc = varyIter1.current();
           usefulIter->reset();
           for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
             if (loc->mayOverlap(*(usefulIter->current())) ) {
                 retval->insert(loc);
                 retval->insert(usefulIter->current());
             }
           }
        }
        mActive->copyIntoOutActive(stmt, retval);

        (*stmtIter)++;
    }

  for ( ; stmtIter->isValid(); (*stmtIter)++) {
      StmtHandle stmt = stmtIter->current();


      // OutActive of a predecessor statement is InActive for
      // the successor statement.
      mActive->copyIntoInActive(stmt, retval);

      // Making outActive
      retval = new DataFlow::LocDFSet;
      usefulIter = useful->getOutUsefulIterator(stmt);
      varySet = mActive->getOutActiveSet(stmt);
      DataFlow::LocDFSetIterator varyIter(*varySet);
      for ( ; varyIter.isValid(); ++varyIter ) {
           OA_ptr<Location> loc = varyIter.current();
           usefulIter->reset();
           for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
             if (loc->mayOverlap(*(usefulIter->current())) ) {
                 retval->insert(loc);
                 retval->insert(usefulIter->current());
             }
           }
      }
      mActive->copyIntoOutActive(stmt, retval);

    }
  }

  
  mActive->setNumIter(mSolver->getNumIter());
    
  if (debug) {
      std:: cout << std::endl << "%+%+%+%+ Vary numIter = " 
       << mSolver->getNumIter() 
       << " +%+%+%+%" << std::endl << std::endl;
  }
    
  return mActive;
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
OA_ptr<DataFlow::DataFlowSet> ManagerICFGVaryActive::initializeTop()
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::initializeNodeIN(OA_ptr<ICFG::NodeInterface> n)
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::initializeNodeOUT(OA_ptr<ICFG::NodeInterface> n)
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
ManagerICFGVaryActive::meet(OA_ptr<DataFlow::DataFlowSet> set1, 
                      OA_ptr<DataFlow::DataFlowSet> set2)
{
    if (debug) {
        std::cout << "ManagerICFGVaryActive::meet" << std::endl;
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

/*! 
    A helper function that determines active locations, whether
    the previous stmt was active, and which memory references in the
    previous and current stmt are active
*/
OA_ptr<DataFlow::LocDFSet> 
ManagerICFGVaryActive::calculateActive(
        OA_ptr<LocIterator> varyIter,
        OA_ptr<LocIterator> usefulIter, StmtHandle stmt)
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;

    if (debug) {
        std::cout << "\tcalculateActive ---------------------" << std::endl;
    }

    // get set of active locations
    varyIter->reset();
    for ( ; varyIter->isValid(); (*varyIter)++ ) {
        OA_ptr<Location> loc = varyIter->current();
        if (debug) { 
            std::cout << "\t\tinVary loc = ";
            loc->dump(std::cout,mIR);
        }
        usefulIter->reset();
        for ( ; usefulIter->isValid(); (*usefulIter)++ ) {
            if (debug) { 
                std::cout << "\t\tusefulIter loc = ";
                usefulIter->current()->dump(std::cout,mIR);
            }
            if (loc->mayOverlap(*(usefulIter->current())) ) {
                retval->insert(loc);
                retval->insert(usefulIter->current());
                if (debug) {
                  std::cout << "\t\tinserting active loc = ";
                  loc->dump(std::cout,mIR);
                  std::cout << "\t\tinserting active loc = ";
                  usefulIter->current()->dump(std::cout,mIR);
                }
            } 
        }
    }

    return retval;
}


//! OK to modify in set and return it again as result because
//! solver clones the BB in sets
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::transfer(ProcHandle proc,
                          OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt)
{    
    // convert incoming set to more specific subclass
    OA_ptr<DataFlow::LocDFSet> inRecast = in.convert<DataFlow::LocDFSet>();
    if (debug) {
        std::cout << "In transfer, stmt(hval=" << stmt.hval() << ")= ";
        mIR->dump(stmt,std::cout);
        std::cout << "\tinRecast = ";
        inRecast->dump(std::cout,mIR);
    }

   // determine the set of incoming active locations based on InUseful
    // and InVary
    mActive->copyIntoInActive(stmt, inRecast);

    // do not process statements that dont have Dep Information
    //if( mICFGDep->isDepSetEmpty(stmt) ) {
    //    return inRecast;
    //}

    // create outVary set
    OA_ptr<DataFlow::LocDFSet> outVary;
    outVary = new DataFlow::LocDFSet;

    // use dep pairs to determine what locations should be in OutVary
    DataFlow::LocDFSetIterator inLocIter(*inRecast);
    for ( ; inLocIter.isValid(); ++inLocIter ) {
      OA_ptr<Location> inLoc = inLocIter.current();
      if (debug) {
          std::cout << "\tinLoc = ";
          inLoc->dump(std::cout, mIR);
      }
      OA_ptr<LocIterator> locIterPtr;
      locIterPtr = mICFGDep->getMayDefIterator(stmt,inLoc);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          outVary->insert(locIterPtr->current());
      }
    }


    if (debug) {
        std::cout << "\toutVary = ";
        outVary->dump(std::cout,mIR);
    }

  // determine outActive from outVary and outUseful
    mActive->copyIntoOutActive(stmt, outVary);

    return outVary;
}

/*!
   Will get the indep vars if this procedure has any and add them
   to incoming LocDFSet
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::entryTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> in)
{
    // create retval as copy of in
    OA_ptr<DataFlow::LocDFSet> retval;
    OA_ptr<DataFlow::LocDFSet> inRecast = in.convert<DataFlow::LocDFSet>();
    retval = new DataFlow::LocDFSet(*inRecast);

    if (debug) {
      std::cout << "ManagerICFGVaryActive::entryTransfer\n\tinRecast set:";
      inRecast->output(*mIR);
      std::cout << endl;
    }

    // get iterator over indep MemRefExpr for procedure
    OA_ptr<MemRefExprIterator> indepIter = mIR->getIndepMemRefExprIter(proc);
    // get alias results for this procedure
    OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(proc);

    if (debug) {
      std::cout << "\tindelIter:\n";
    }
    for ( indepIter->reset(); indepIter->isValid(); (*indepIter)++ ) {
      OA_ptr<MemRefExpr> memref = indepIter->current();

      if (debug) {
        std::cout << "\t\tmemref: ";
        memref->output(*mIR);
        std::cout << std::endl;
      }
      // get may locs for memref
      OA_ptr<LocIterator> locIter = alias->getMayLocs(*memref,proc);
      for (locIter->reset(); locIter->isValid(); (*locIter)++ ) {
        if (debug) {
          std::cout << "\t\t\tinserting mayloc: ";
          (locIter->current())->output(*mIR);
          std::cout << "\n";
        }
        retval->insert(locIter->current());
      }
    }
    if (debug) {
        std::cout << "\tManagerICFGVaryActive, Indep locations for proc "
                  << mIR->toString(proc) << ": " << std::endl;
        retval->output(*mIR);
        std::cout << "----" << std::endl;
    }

    return retval;
}

/*!
   Just pass along out because this won't be called since we are a Forward
   analysis
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::exitTransfer(ProcHandle proc, OA_ptr<DataFlow::DataFlowSet> out)
{
    assert(0);
    return out;
}


//! Propagate a data-flow set from caller to callee
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::callerToCallee(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee)
{
  if (debug) {
    std::cout << "-------- ManagerICFGUseful::callerToCallee" << std::endl;
  }
  OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();

  if (debug) {
    std::cout << "    LocDFSet from Callee:\n";
    inRecast->dump(std::cout,mIR);
  }
  // forward flow: must be a call edge (on the way out)
  // need to do both regular and transitive callerToCallee conversions
  OA_ptr<DataFlow::LocDFSet> trans;
  OA_ptr<DataFlow::LocDFSet> non_trans;
  non_trans = inRecast->callerToCallee(caller,call,callee,mInterAlias,
                                       mParamBind,mIR);
  if (debug) {
    std::cout << "    LocDFSet in Callee from NonTrans:\n";
    non_trans->dump(std::cout,mIR);
  }
  trans = inRecast->callerToCalleeTransitive(caller,call,callee,mInterAlias,
                                             mParamBind,mIR);
  if (debug) {
    std::cout << "    LocDFSet in Callee from Trans:\n";
    trans->dump(std::cout,mIR);
  }
  // return the union of the two
  *trans = trans->setUnion(*non_trans);
  if (debug) {
    std::cout << "    LocDFSet in Callee from Trans/NonTrans Union:\n";
    trans->dump(std::cout,mIR);
  }

  DataFlow::LocDFSetIterator inLocIter(*inRecast);

   OA_ptr<Location> loc;
   for ( ; inLocIter.isValid(); ++inLocIter ) {
       loc = inLocIter.current();
       if (debug) {
           std::cout << "\t\tinVary loc = ";
           loc->dump(std::cout,mIR);
       }
       if( loc->isaSubSet() ) {
           if(loc.convert<LocSubSet>()->getLoc()->isaNamed()) {
             if(!loc.convert<LocSubSet>()->getLoc().convert<NamedLoc>()->isLocal()) {
                trans->insert(loc);
             }
           }
       } else {
         if( loc->isaNamed() ) {
             if(!loc.convert<NamedLoc>()->isLocal() ) {
                trans->insert(loc);
             }
         }
       }
  }

  return trans;
}
  
//! Propagate a data-flow set from callee to caller
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGVaryActive::calleeToCaller(ProcHandle callee,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle caller)
{
    OA_ptr<DataFlow::LocDFSet> inRecast = dfset.convert<DataFlow::LocDFSet>();
    return inRecast->calleeToCaller(callee,call,caller,mInterAlias,
                                    mParamBind,mIR);
}

//! Propagate a data-flow set from call node to return node
OA_ptr<DataFlow::DataFlowSet>
ManagerICFGVaryActive::callToReturn(ProcHandle caller,
    OA_ptr<DataFlow::DataFlowSet> dfset, CallHandle call, ProcHandle callee){

  // create retval as copy of dfset
  OA_ptr<DataFlow::LocDFSet> remapdfset = dfset.convert<DataFlow::LocDFSet>();
  OA_ptr<DataFlow::LocDFSet> retval;
  OA_ptr<DataFlow::DataFlowSet> temp = remapdfset->clone();
  retval = temp.convert<DataFlow::LocDFSet>();
  OA_ptr<LocIterator> varyIter = remapdfset->getLocIterator();

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
   * Remove NonLocal Locations from the VarySet.
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
   *           if mustLoc is found in the vary set then
   *              remove it from the vary set
   ******************************************              
   */

  // Remove NonLocal Locations from the Vary Set
  varyIter->reset();
  std::set<OA_ptr<Location> > removeLocs;
  for ( ; varyIter->isValid(); (*varyIter)++ ) {
       OA_ptr<Location> loc = varyIter->current();
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

  // Remove Parameters Modified in the Callee 
  // from varySet.
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
                     varyIter->reset();
                     for ( ; varyIter->isValid(); (*varyIter)++ ) {
                          OA_ptr<Location> varyloc = varyIter->current();
                           if (loc->mustOverlap(*varyloc)) {
                               retval->remove(varyloc);
                           }
                     }
                }
           }
      }
  }

  return retval;
}

  } // end of namespace Activity
} // end of namespace OA
