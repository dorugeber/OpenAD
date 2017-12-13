
// ManagerLivenessStandard.cpp


#include "ManagerLivenessStandard.hpp"


namespace OA {
  namespace Liveness {

static bool debug = false;


/*!
*/
ManagerLivenessStandard::ManagerLivenessStandard(OA_ptr<LivenessIRInterface> _ir) 
    : mIR(_ir)
{
   OA_DEBUG_CTRL_MACRO("DEBUG_ManagerLivenessStandard:ALL", debug);
   mSolver = new DataFlow::CFGDFSolver(DataFlow::CFGDFSolver::Backward,*this);
}

OA_ptr<DataFlow::DataFlowSet> ManagerLivenessStandard::initializeTop()
{
    OA_ptr<DataFlow::LocDFSet>  retval;
    retval = new DataFlow::LocDFSet;
    return retval;
}

OA_ptr<DataFlow::DataFlowSet> ManagerLivenessStandard::initializeBottom()
{
    OA_ptr<DataFlow::LocDFSet> retval;
    retval = new DataFlow::LocDFSet;
    return retval;
}

OA_ptr<LivenessStandard> ManagerLivenessStandard::performAnalysis(ProcHandle proc, 
    OA_ptr<CFG::CFGInterface> cfg, OA_ptr<Alias::Interface> alias,
    OA_ptr<SideEffect::InterSideEffectInterface> interSE,
    DataFlow::DFPImplement algorithm)
{
  if (debug) {
    std::cout << "In Liveness::ManagerLivenessStandard::performAnalysis" << std::endl;
  }
  mLiveMap = new LivenessStandard(proc);

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
    mStmtMayUseMap[stmt];

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

    }
 
    // must or may defines from procedure calls
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle expr = callsiteItPtr->current();

      OA_ptr<LocIterator> locIterPtr;
      
      // DEF
      locIterPtr = interSE->getDEFIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          OA_ptr<Location> defLocPtr = locIterPtr->current();
          mStmtMustDefMap[stmt].insert(defLocPtr);
      }
    }

   
   OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
    for (; useIterPtr->isValid(); (*useIterPtr)++) {
        MemRefHandle useref = useIterPtr->current();
        if (debug) {
          std::cout << "\t\tuse ref (" << useref.hval() << ") = ";
          mIR->dump(useref,std::cout);
        }

        OA_ptr<LocIterator> uselocIterPtr = alias->getMayLocs(useref);
        for (; uselocIterPtr->isValid(); ++(*uselocIterPtr)) {
            mStmtMayUseMap[stmt].insert(uselocIterPtr->current());
        }

    }

  
  }//loop over statements

  // use the dataflow solver to get the In and Out sets for the BBs
  //DataFlow::CFGDFProblem::solve(cfg);
  //

  mSolver->solve(cfg,algorithm);  
  
  // get exit node for CFG and determine what definitions reach that node
  OA_ptr<CFG::NodeInterface> node;
  
  node = cfg->getExit();

  OA_ptr<DataFlow::DataFlowSet> x = mSolver->getOutSet(node);

  OA_ptr<DataFlow::LocDFSet> inSet
    = x.convert<DataFlow::LocDFSet >();

   
  return mLiveMap;

}

OA_ptr<DataFlow::DataFlowSet>
ManagerLivenessStandard::initializeNodeIN(OA_ptr<CFG::NodeInterface> n)
{
     OA_ptr<DataFlow::LocDFSet>  retval;
     retval = new DataFlow::LocDFSet;
     return retval;
}

OA_ptr<DataFlow::DataFlowSet>
ManagerLivenessStandard::initializeNodeOUT(OA_ptr<CFG::NodeInterface> n)
{
     OA_ptr<DataFlow::LocDFSet>  retval;
     retval = new DataFlow::LocDFSet;
     return retval;
}



OA_ptr<DataFlow::DataFlowSet> 
ManagerLivenessStandard::meet (OA_ptr<DataFlow::DataFlowSet> set1orig, 
                       OA_ptr<DataFlow::DataFlowSet> set2orig)
{
    OA_ptr<DataFlow::LocDFSet> set1
        = set1orig.convert<DataFlow::LocDFSet>();
    if (debug) {
        std::cout << "ManagerLivenessStandard::meet" << std::endl;
        std::cout << "\tset1 = ";
        set1->dump(std::cout,mIR);
        std::cout << ", set2 = ";
        set2orig->dump(std::cout,mIR);
    }
       
    DataFlow::LocDFSet retval 
        = set1->setUnion(*set2orig);
    if (debug) {
        std::cout << std::endl << "\tretval set = ";
        retval.dump(std::cout,mIR);
        std::cout << std::endl;
    }
       
    return retval.clone();
}


OA_ptr<DataFlow::DataFlowSet> 
ManagerLivenessStandard::transfer(OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt) 
{
    OA_ptr<DataFlow::LocDFSet> inRecast 
        = in.convert<DataFlow::LocDFSet>();

    if (debug) {
        std::cout << "In transfer, stmt(hval=" << stmt.hval() << ")= ";
        mIR->dump(stmt,std::cout);
    }


    DataFlow::LocDFSetIterator inIter(*inRecast);
    for (; inIter.isValid(); ++inIter) {
        OA_ptr<Location> livevar = inIter.current();
        if (debug) {
          std::cout << "\tlivevar in Inset = ";
          livevar->dump(std::cout,mIR);
        }
   
       mLiveMap->insertLive(stmt,livevar);
  }
         LocSet::iterator setIter;
         for (setIter=mStmtMustDefMap[stmt].begin(); setIter!=mStmtMustDefMap[stmt].end(); setIter++) {
                       OA_ptr<Location> loc = *setIter;
                       inRecast->remove(loc);
                       mLiveMap->removeLive(stmt,loc);
 
    }

         LocSet::iterator gensetIter;
         for (gensetIter=mStmtMayUseMap[stmt].begin(); gensetIter!=mStmtMayUseMap[stmt].end(); gensetIter++) {
                       OA_ptr<Location> loc = *gensetIter;
                       inRecast->insert(loc);
                       mLiveMap->insertLive(stmt,loc);

    }

    return inRecast;
 }

  } // end of namespace Liveness
} // end of namespace OA
