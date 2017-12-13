/*! \file
  
  \brief The AnnotationManager that generates ReachDefsStandards
         which map each statement to a set of reaching definitions
         that reach that statement.

  \authors Michelle Strout
  \version $Id: ManagerReachDefsStandard.cpp,v 1.33 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerReachDefsStandard.hpp"


namespace OA {
  namespace ReachDefs {

static bool debug = false;


/*!
*/
ManagerReachDefsStandard::ManagerReachDefsStandard(OA_ptr<ReachDefsIRInterface> _ir) 
    : mIR(_ir)
{
   mSolver = new DataFlow::CFGDFSolver(DataFlow::CFGDFSolver::Forward,*this);
}

OA_ptr<DataFlow::DataFlowSet> ManagerReachDefsStandard::initializeTop()
{
    OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> >  retval;
    retval = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> ManagerReachDefsStandard::initializeBottom()
{
    OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> >  retval;
    retval = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
    return retval;
}

/*!
    \brief    Used to perform analysis when not using AQM

    Uses the CFGDFProblem::solve functionality to get In and Out
    reaching definition sets for each basic block.  Then uses the 
    statement transfer function to get an In set for each stmt.
*/
OA_ptr<ReachDefsStandard> ManagerReachDefsStandard::performAnalysis(ProcHandle proc, 
    OA_ptr<CFG::CFGInterface> cfg, OA_ptr<Alias::Interface> alias,
    OA_ptr<SideEffect::InterSideEffectInterface> interSE,
    DataFlow::DFPImplement algorithm)
{
  if (debug) {
    std::cout << "In ReachDefs::ManagerReachDefsStandard::performAnalysis" << std::endl;
  }
  mReachDefMap = new ReachDefsStandard(proc);

  // store Alias information for use within the transfer function
  mAlias = alias;

  // get mapping of statements to locations they may and must define
  OA_ptr<OA::IRStmtIterator> sIt = mIR->getStmtIterator(proc);
  for ( ; sIt->isValid(); (*sIt)++) {
    OA::StmtHandle stmt = sIt->current();
    if (debug) {
      std::cout<< "\tstmt (" << stmt.hval() << ") = ";
      mIR->dump(stmt,std::cout);
    } 
    
    // initialize each stmt to define an empty set of locations
    mStmtMustDefMap[stmt];
    mStmtMayDefMap[stmt];

    // locations that each statement may or must def
    OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(stmt);
    for (; defIterPtr->isValid(); (*defIterPtr)++) {
        MemRefHandle ref = defIterPtr->current();
        if (debug) {
          std::cout << "\t\tdef ref (" << ref.hval() << ") = ";
          mIR->dump(ref,std::cout);
        }

        OA_ptr<LocIterator> locIterPtr = alias->getMustLocs(ref);
        for (; locIterPtr->isValid(); ++(*locIterPtr)) {
            mStmtMustDefMap[stmt].insert(locIterPtr->current());
        }

        locIterPtr = alias->getMayLocs(ref);
        for (; locIterPtr->isValid(); ++(*locIterPtr)) {
            mStmtMayDefMap[stmt].insert(locIterPtr->current());
        }
    }
 
    // must or may defines from procedure calls
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle expr = callsiteItPtr->current();

      OA_ptr<LocIterator> locIterPtr;
      
      // MOD
      locIterPtr = interSE->getMODIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          OA_ptr<Location> modLocPtr = locIterPtr->current();
          mStmtMayDefMap[stmt].insert(modLocPtr);
      }
      
      // DEF
      locIterPtr = interSE->getDEFIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          OA_ptr<Location> defLocPtr = locIterPtr->current();
          mStmtMustDefMap[stmt].insert(defLocPtr);
      }
    }

  } // loop over statements

  // use the dataflow solver to get the In and Out sets for the BBs
  //DataFlow::CFGDFProblem::solve(cfg);
  //
  mSolver->solve(cfg,algorithm);  
  
  // get exit node for CFG and determine what definitions reach that node
  OA_ptr<CFG::NodeInterface> node;
  
  node = cfg->getExit();

  OA_ptr<DataFlow::DataFlowSet> x = mSolver->getOutSet(node);
      //mNodeInSetMap[node];
  OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> > inSet 
    = x.convert<DataFlow::IRHandleDataFlowSet<StmtHandle> >();

  DataFlow::IRHandleIterator<StmtHandle> rdIter(inSet);
  for (; rdIter.isValid(); ++rdIter) {
      StmtHandle reachDef = rdIter.current();
      mReachDefMap->insertExitReachDef(reachDef);
  }
  return mReachDefMap;

}

//------------------------------------------------------------------
// Implementing the callbacks for CFGDFProblem
//------------------------------------------------------------------
/*void ManagerReachDefsStandard::initializeNode(OA_ptr<CFG::Interface::Node> n)
{
    mNodeInSetMap[n] = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
    mNodeOutSetMap[n] = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
}
*/

/*!
 *  *    Not doing anything special at entries and exits.
 *   *     */
OA_ptr<DataFlow::DataFlowSet>
ManagerReachDefsStandard::initializeNodeIN(OA_ptr<CFG::NodeInterface> n)
{
     OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> >  retval;
     retval = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
     return retval;
}

OA_ptr<DataFlow::DataFlowSet>
ManagerReachDefsStandard::initializeNodeOUT(OA_ptr<CFG::NodeInterface> n)
{
     OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> >  retval;
     retval = new DataFlow::IRHandleDataFlowSet<StmtHandle>;
     return retval;
}



OA_ptr<DataFlow::DataFlowSet> 
ManagerReachDefsStandard::meet (OA_ptr<DataFlow::DataFlowSet> set1orig, 
                       OA_ptr<DataFlow::DataFlowSet> set2orig)
{
    OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> > set1
        = set1orig.convert<DataFlow::IRHandleDataFlowSet<StmtHandle> >();
    if (debug) {
        std::cout << "ManagerReachDefsStandard::meet" << std::endl;
        std::cout << "\tset1 = ";
        set1->dump(std::cout,mIR);
        std::cout << ", set2 = ";
        set2orig->dump(std::cout,mIR);
    }
       
    DataFlow::IRHandleDataFlowSet<StmtHandle> retval 
        = set1->setUnion(*set2orig);
    if (debug) {
        std::cout << std::endl << "\tretval set = ";
        retval.dump(std::cout,mIR);
        std::cout << std::endl;
    }
       
    return retval.clone();
}

/*!
    \brief Core of analysis algorithm.  Transfer function for a stmt.

    KILL is the set of reaching definitions (statements) that may
    define the set of locations that are must defined in this
    statement

    DEF is this statement if it may define anything
*/

OA_ptr<DataFlow::DataFlowSet> 
ManagerReachDefsStandard::transfer(OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt) 
{
    OA_ptr<DataFlow::IRHandleDataFlowSet<StmtHandle> > inRecast 
        = in.convert<DataFlow::IRHandleDataFlowSet<StmtHandle> >();

    if (debug) {
        std::cout << "In transfer, stmt(hval=" << stmt.hval() << ")= ";
        mIR->dump(stmt,std::cout);
    }
        
    
    // KILL: will kill all statements that may have defined 
    // locations that this statement must defines
         
    // for each stmt that is a reaching definition
    DataFlow::IRHandleIterator<StmtHandle> inIter(inRecast);
    for (; inIter.isValid(); ) {
        StmtHandle reachdef = inIter.current();
        if (debug) {
          std::cout << "\treachdef in Inset = ";
          mIR->dump(reachdef,std::cout);
        }

        // insert reachDef into results
        mReachDefMap->insertReachDef(stmt,reachdef);

        // get set of locations that were may defed in reaching definition
        // and are must defed in the current stmt
        OA_ptr<std::set<OA_ptr<Location> > > killLocSetPtr 
            = intersectLocSets(mStmtMayDefMap[reachdef],
                                         mStmtMustDefMap[stmt]);

        if (debug) {
          std::cout << "\tMayDefs for reachdef = ";
          dumpLocSet(mStmtMayDefMap[reachdef],std::cout,mIR);
          std::cout << "\tMustDefs for stmt(hval=" << stmt.hval() << ")= ";
          dumpLocSet(mStmtMustDefMap[stmt],std::cout,mIR);
          std::cout << "\tkillLocSet for stmt(hval=" << stmt.hval() << "(= ";
          dumpLocSet(*killLocSetPtr,std::cout,mIR);
        }

        // if that set is nonempty and a superset of all the 
        // may defs in the reaching definition then we will go 
        // ahead and kill the reach def
        if ( !killLocSetPtr->empty() 
             && subSetOf(mStmtMayDefMap[reachdef],*killLocSetPtr) ) 
        {
            ++inIter ;
            if (debug) {
              std::cout << "\tStmt in Kill set: ";
              mIR->dump(reachdef,std::cout);
            }
            // delete statement that is in the kill subset
            inRecast->remove(reachdef);
        } else {
            ++inIter;
        }
    }

    // DEF: if this statement has any defs then it is put into the
    //      set of reaching definitions

    if (!mStmtMayDefMap[stmt].empty()) {
        // put into dataflow set as a reaching definition
        inRecast->insert(stmt);
        if (debug) {
          std::cout << "\tStmt in Def set: ";
          mIR->dump(stmt,std::cout);
        }
    }

    return inRecast;
}

  } // end of namespace ReachDefs
} // end of namespace OA
