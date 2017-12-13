/*! \file
  
  \brief The AnnotationManager that generates ICFGDep.

  \authors Michelle Strout
  \version $Id: ManagerICFGDep.cpp,v 1.5 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerICFGDep.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace Activity {

static bool debug = false;


/*!
*/
ManagerICFGDep::ManagerICFGDep(OA_ptr<ActivityIRInterface> _ir) : mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerICFGDep:ALL", debug);
    mSolver = new DataFlow::ICFGDFSolver(DataFlow::ICFGDFSolver::Forward,*this);
}


/*!
    In transfer function creates a DepDFSet for each stmt.
*/
OA_ptr<ICFGDep> ManagerICFGDep::performAnalysis(
                    OA_ptr<ICFG::ICFGInterface> icfg,
                    OA_ptr<DataFlow::ParamBindings> paramBind,
                    OA_ptr<Alias::InterAliasInterface> interAlias,
                    DataFlow::DFPImplement algorithm)
{
  if (debug) {
    std::cout << "In ManagerICFGDep::performAnalysis" << std::endl;
  }
  mDep = new ICFGDep();

  // store results that will be needed in callbacks
  mICFG = icfg;
  mParamBind = paramBind;
  mInterAlias = interAlias;
  
  // call iterative data-flow solver for ICFG
  mSolver->solve(icfg,algorithm);

  return mDep;

}

//========================================================
// implementation of ICFGDFProblem interface
//========================================================
//--------------------------------------------------------
// initialization callbacks
//--------------------------------------------------------
OA_ptr<DataFlow::DataFlowSet>  
ManagerICFGDep::initializeTop()
{
    OA_ptr<DepDFSet>  retval;
    retval = new DepDFSet;
    return retval;
}

/*!
   Not doing anything special at entries and exits.
 */
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGDep::initializeNodeIN(OA_ptr<ICFG::NodeInterface> n)
{
    return initializeTop();
}

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGDep::initializeNodeOUT(OA_ptr<ICFG::NodeInterface> n)
{
    return initializeTop();
}

//--------------------------------------------------------
// solver callbacks 
//--------------------------------------------------------
 
/*! Not propagating any useful information over the ICFG
*/
OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGDep::meet (OA_ptr<DataFlow::DataFlowSet> set1, 
                      OA_ptr<DataFlow::DataFlowSet> set2)
{
    return set1;
}

/*!
    \brief Core of analysis algorithm.  Transfer function for a stmt.

    Create a DepDFSet for this stmt. 
*/

OA_ptr<DataFlow::DataFlowSet> 
ManagerICFGDep::transfer(ProcHandle proc,
                         OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt) 
{
    if (debug) {
        std::cout << "\tIn transfer, stmt(hval=" << stmt.hval() << ")= ";
        mIR->dump(stmt,std::cout);
    }

    // get alias results for this procedure
    OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(proc);
    
    // new DepDFSet for this stmt
    OA_ptr<DepDFSet> stmtDepDFSet;
    stmtDepDFSet = new DepDFSet;
    
    // set of must defs and differentiable uses for this statement
    LocSet mustDefSet;
    LocSet mayDefSet;
    LocSet diffUseSet;

    // set of expressions to analyze for differentiable uses
    std::set<ExprHandle> exprSet;

    OA_ptr<AssignPairIterator> espIterPtr 
            = mIR->getAssignPairIterator(stmt);
    
    if(!espIterPtr.ptrEqual(0)) {
        
        for ( ; espIterPtr->isValid(); ++(*espIterPtr)) {
            // unbundle pair
            MemRefHandle mref = espIterPtr->currentTarget();
            ExprHandle expr = espIterPtr->currentSource();
            if (debug) {
                std::cout << "\tmref = " << mIR->toString(mref) << ", ";
                std::cout << "expr = " << mIR->toString(expr) << std::endl;
            }

            // add this to list of expressions we need to analyze
            exprSet.insert(expr);
            if (debug) { 
                std::cout << "Inserting memref = expr into exprSet" 
                          << std::endl;
                OA_ptr<ExprTree> etree = mIR->getExprTree(expr);
                etree->dump(std::cout,mIR); 
            }

            // keep track of def mustlocs
            OA_ptr<LocIterator> locIterPtr = alias->getMustLocs(mref);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
              if (debug) {
                std::cout << "Inserting into mustDefSet: ";
                (locIterPtr->current())->output(*mIR);
                std::cout << std::endl;
              }
                mustDefSet.insert(locIterPtr->current());
            }
            // maylocs need to be used for Dep pairs so that we get
            // conservative activity results
            locIterPtr = alias->getMayLocs(mref);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
              if (debug) {
                std::cout << "Inserting into mayDefSet: ";
                (locIterPtr->current())->output(*mIR);
                std::cout << std::endl;
              }
                mayDefSet.insert(locIterPtr->current());
            }
        }
    
    // all other statement types just get all uses and defs
    } else {
        if (debug) {
            std::cout << "\tstmt is not EXPR_STMT, stmt = "
                      << mIR->toString(stmt) << std::endl;
        }
    
        OA_ptr<MemRefHandleIterator> mrIterPtr = mIR->getUseMemRefs(stmt);
        
        for (; mrIterPtr->isValid(); (*mrIterPtr)++ ) {
            MemRefHandle mref = mrIterPtr->current();
            if (debug) {
              std::cout << "getting mayLocs for memrefHandle: "
                        << mIR->toString(mref) << std::endl;
            }
            OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(mref);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
              if (debug) {
                std::cout << "Inserting into diffUseSet: ";
                (locIterPtr->current())->output(*mIR);
                std::cout << std::endl;
              }
                diffUseSet.insert(locIterPtr->current());
            }
        }
        
        mrIterPtr = mIR->getDefMemRefs(stmt);
        for (; mrIterPtr->isValid(); (*mrIterPtr)++ ) {
            MemRefHandle mref = mrIterPtr->current();
            OA_ptr<LocIterator> locIterPtr = alias->getMustLocs(mref);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
                mustDefSet.insert(locIterPtr->current());
            }
            locIterPtr = alias->getMayLocs(mref);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
                mayDefSet.insert(locIterPtr->current());
            }
        }
    }

    // iterate over all calls in the statement
    // and find all expressions that are callsite params and not 
    // reference parameters
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
        CallHandle call = callsiteItPtr->current();
        if (debug) {
          std::cout << "\nhandling all callsite params, ";
          std::cout << "call = " << mIR->toString(call) << std::endl;
        }

        // looping over actual params 
        OA_ptr<IRCallsiteParamIterator> cspIterPtr 
          = mIR->getCallsiteParams(call); 
        for ( ; cspIterPtr->isValid(); ++(*cspIterPtr)) { 
          ExprHandle param = cspIterPtr->current(); 
          exprSet.insert(param); 
        } 
    }

    // get differentiable locations from all expressions in stmt
    if (debug) { std::cout << "ExprTree's:" << std::endl; }
    DifferentiableLocsVisitor dlVisitor(alias);
    std::set<ExprHandle>::iterator exprIter;
    for (exprIter=exprSet.begin(); exprIter!=exprSet.end(); exprIter++) {
        OA_ptr<ExprTree> etree = mIR->getExprTree(*exprIter);
        if (debug) { etree->dump(std::cout,mIR); }
        etree->acceptVisitor(dlVisitor);
        OA_ptr<LocIterator> locIterPtr 
            = dlVisitor.getDiffLocsIterator();
        if (debug) {
          std::cout << "getting diffLocsIterator\n";
        }
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          if (debug) {
            std::cout << "Inserting into diffUseSet: ";
            (locIterPtr->current())->output(*mIR);
            std::cout << std::endl;
          }
            diffUseSet.insert(locIterPtr->current());
        }
    }
    
    // store must defs in Dep results as well
    // and remove the associated implicit dep from DepDFSet
    LocSet::iterator useIter, defIter;
    for (defIter=mustDefSet.begin(); defIter!=mustDefSet.end(); defIter++) {
        mDep->insertMustDefForStmt(stmt,*defIter);
        stmtDepDFSet->removeImplicitDep(*defIter,*defIter);
    }
   
    // map all uses to may defs and vice versa for this statement
    // have to do this after removing implicit deps in case it will be
    // explicitly put back in
    OA_ptr<Location> use, def;
    for (useIter=diffUseSet.begin(); useIter!=diffUseSet.end(); useIter++) {
      for (defIter=mayDefSet.begin(); defIter!=mayDefSet.end(); defIter++) {
          stmtDepDFSet->insertDep(*useIter,*defIter);
      }
    }

    // map stmtDepDFSet to stmt in depResults
    mDep->mapStmtToDeps(stmt, stmtDepDFSet);

    return in;
}

  } // end of namespace Activity
} // end of namespace OA
