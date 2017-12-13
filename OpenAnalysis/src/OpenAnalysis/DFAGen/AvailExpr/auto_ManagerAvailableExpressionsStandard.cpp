
// ManagerAvailableExpressionsStandard.cpp

#include "auto_ManagerAvailableExpressionsStandard.hpp"

using namespace std;
using namespace OA::DataFlow;
using namespace OA::CFG;
using namespace OA::Alias;
using namespace OA::SideEffect;

namespace OA {
  namespace AvailableExpressions {

static bool debug = false;


ManagerAvailableExpressionsStandard::ManagerAvailableExpressionsStandard(
    OA_ptr<AvailableExpressionsIRInterface> _ir)
    :
    mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerAvailableExpressionsStandard:ALL", debug);
    mSolver = new CFGDFSolver(CFGDFSolver::Forward, *this);
}


OA_ptr<DataFlowSet> ManagerAvailableExpressionsStandard::initializeTop()
{
    OA_ptr<AvailableExpressionsDFSet> retval;
    retval = new AvailableExpressionsDFSet();
    return retval;
}


OA_ptr<DataFlowSet> ManagerAvailableExpressionsStandard::initializeBottom()
{
    OA_ptr<AvailableExpressionsDFSet> retval;
    retval = new AvailableExpressionsDFSet();
    return retval;
}


OA_ptr<DataFlowSet>
ManagerAvailableExpressionsStandard::initializeNodeIN(OA_ptr<NodeInterface> n)
{
    OA_ptr<AvailableExpressionsDFSet> retval;
    retval = new AvailableExpressionsDFSet();
    return retval;
}


OA_ptr<DataFlowSet>
ManagerAvailableExpressionsStandard::initializeNodeOUT(OA_ptr<NodeInterface> n)
{
    OA_ptr<AvailableExpressionsDFSet> retval;
    retval = new AvailableExpressionsDFSet();
    return retval;
}


OA_ptr<AvailableExpressionsStandard> ManagerAvailableExpressionsStandard::performAnalysis(
    ProcHandle proc,
    OA_ptr<CFGInterface> cfg,
    OA_ptr<OA::Alias::Interface> alias,
    OA_ptr<InterSideEffectInterface> interSE)
{
    if (debug) {
        cout << "In AvailableExpressions::ManagerAvailableExpressionsStandard::performAnalysis"
             << endl;
    }

    // create the object we'll store results in
    mAvailableExpressionsMap = new AvailableExpressionsStandard(proc, mIR);

    // store Alias information for use within the transfer function
    mAlias = alias;

    {

        // maycode for defs[s]
        OA_ptr<IRStmtIterator> stmtIter;
        stmtIter = mIR->getStmtIterator(proc);
        for(; stmtIter->isValid(); ++(*stmtIter)) {
            StmtHandle stmt = stmtIter->current();

            OA_ptr<MemRefHandleIterator> defsIter;
            defsIter = mIR->getDefMemRefs(stmt);
            for (; defsIter->isValid(); (*defsIter)++) {
                MemRefHandle ref = defsIter->current();

                OA_ptr<LocIterator> locIter;
                locIter = alias->getMayLocs(ref);
                for (; locIter->isValid(); ++(*locIter)) {
                    mStmt2MayDefMap[stmt].insert(locIter->current());
                }
            }
        }
    } {

        // mustcode for defs[s]
        OA_ptr<IRStmtIterator> stmtIter;
        stmtIter = mIR->getStmtIterator(proc);
        for(; stmtIter->isValid(); ++(*stmtIter)) {
            StmtHandle stmt = stmtIter->current();

            OA_ptr<MemRefHandleIterator> defsIter;
            defsIter = mIR->getDefMemRefs(stmt);
            for (; defsIter->isValid(); (*defsIter)++) {
                MemRefHandle ref = defsIter->current();
                OA_ptr<LocIterator> locIter;
                locIter = alias->getMayLocs(ref);
                for (; locIter->isValid(); ++(*locIter)) {
                    mStmt2MustDefMap[stmt].insert(locIter->current());
                }
            }
        }
    }
    {

      OA_ptr<IRStmtIterator> stmtIter;
      OA_ptr<ExprHandleIterator> iterHdlExpr;
      OA_ptr<NewExprTree> eTree;

      iterHdlExp = getExprHandleIterator(OA::StmtHandle stmt);

      // iterate through all statements in the procedure
      stmtIter = mIR->getStmtIterator(proc);
      for(; stmtIter->isValid(); ++(*stmtIter)) {
          StmtHandle stmt = stmtIter->current();

          // iterate through all expressions in the statement
          iterHdlExpr = mIR->getExprHandleIterator(stmt);
          for(; iterHdlExpr->isValid(); ++(*iterHdlExpr)) {
              ExprHandle hExp = iterHdlExpr->current();
              
              eTree = mIR->getNewExprTree(hExp);
              mStmt2MayExprMap[hExp].insert(eTree);
          }
      }
    } {

      OA_ptr<IRStmtIterator> stmtIter;
      OA_ptr<ExprHandleIterator> iterHdlExpr;
      OA_ptr<NewExprTree> eTree;

      iterHdlExp = getExprHandleIterator(OA::StmtHandle stmt);

      // iterate through all statements in the procedure
      stmtIter = mIR->getStmtIterator(proc);
      for(; stmtIter->isValid(); ++(*stmtIter)) {
          StmtHandle stmt = stmtIter->current();

          // iterate through all expressions in the statement
          iterHdlExpr = mIR->getExprHandleIterator(stmt);
          for(; iterHdlExpr->isValid(); ++(*iterHdlExpr)) {
              ExprHandle hExp = iterHdlExpr->current();
              
              eTree = mIR->getNewExprTree(hExp);
              mStmt2MayExprMap[hExp].insert(eTree);
          }
      }
    }
    {

        // maycode for uses[s]
        OA_ptr<IRStmtIterator> stmtIter;
        stmtIter = mIR->getStmtIterator(proc);
        for(; stmtIter->isValid(); ++(*stmtIter)) {
            StmtHandle stmt = stmtIter->current();

            OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
            for (; useIterPtr->isValid(); (*useIterPtr)++) {
                MemRefHandle ref = useIterPtr->current();
                OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(ref);
                for (; locIterPtr->isValid(); ++(*locIterPtr)) {
                    mStmt2MayUseMap[stmt].insert(locIterPtr->current());
                }
            }
        }
    } {

        // mustcode for uses[s]
        OA_ptr<IRStmtIterator> stmtIter;
        stmtIter = mIR->getStmtIterator(proc);
        for(; stmtIter->isValid(); ++(*stmtIter)) {
            StmtHandle stmt = stmtIter->current();

            OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
            for (; useIterPtr->isValid(); (*useIterPtr)++) {
                MemRefHandle ref = useIterPtr->current();
                OA_ptr<LocIterator> locIterPtr = alias->getMustLocs(ref);
                for (; locIterPtr->isValid(); ++(*locIterPtr)) {
                    mStmt2MustUseMap[stmt].insert(locIterPtr->current());
                }
            }
        }
    }

    // use the dataflow solver to get the In and Out sets for the
    // basic blocks (BBs).
    mSolver->solve(cfg, ITERATIVE);

    // get exit node for CFG and determine what data flow values exit it.
    // store these data flow values in the result's exit information.
    OA_ptr<NodeInterface> node;
    node = cfg->getExit();
    OA_ptr<DataFlowSet> x = mSolver->getOutSet(node);
    OA_ptr<AvailableExpressionsDFSet> exitSet =
        x.convert<AvailableExpressionsDFSet>();
    for(AvailableExpressionsDFSet::iterator i = exitSet->begin();
        i != exitSet->end(); i++)
    {
        mAvailableExpressionsMap->insertExit(*i);
    }


    return mAvailableExpressionsMap;
}

void ManagerAvailableExpressionsStandard::dumpset(OA_ptr<AvailableExpressionsDFSet> inSet) {
    OA_ptr<set<OA_ptr<NewExprTree>  > > setRecast;
    setRecast = inSet.convert<set<OA_ptr<NewExprTree>  > >();
    set<OA_ptr<NewExprTree>  >::iterator iter;
    iter = setRecast->begin();
    for(; iter != setRecast->end(); iter++) {
        cout << (*iter)->toString(mIR);
    }
    cout << endl;
}


OA_ptr<DataFlowSet>
ManagerAvailableExpressionsStandard::meet(
    OA_ptr<DataFlowSet> set1orig,
    OA_ptr<DataFlowSet> set2orig)
{
    // convert the incoming sets to sets of the specific flow value.
    OA_ptr<AvailableExpressionsDFSet> set1 = set1orig.convert<AvailableExpressionsDFSet>();
    OA_ptr<AvailableExpressionsDFSet> set2 = set2orig.convert<AvailableExpressionsDFSet>();

    if (debug) {
        cout << "ManagerAvailableExpressionsStandard::meet" << endl;
        cout << "\tset1 = ";
        dumpset(set1);
        cout << "\tset2 = ";
        dumpset(set2);
    }

    // perform the meet operator on the left hand set.  It's okay to modify the
    // set and return it as a result [since the solver passes a temporary set
    // in as set1.  See the CFGDFProblem header for more info.
    set1->intersectEqu(*set2);

    if (debug) {
        cout << endl << "\tcalculated set = ";
        dumpset(set1);
    }

    return set1;
}

OA_ptr<DataFlowSet> ManagerAvailableExpressionsStandard::genSet(StmtHandle stmt)
{
    OA_ptr<AvailableExpressionsDFSet> genSet;
    genSet =  new AvailableExpressionsDFSet();

    // Generate GEN code:
    OA_ptr<AvailableExpressionsDFSet> set1;
    set1 = new AvailableExpressionsDFSet(mStmt2MustExprMap[stmt]);
    genSet = set1;

    return genSet;
}

OA_ptr<DataFlowSet> ManagerAvailableExpressionsStandard::killSet(
    StmtHandle stmt, OA_ptr<DataFlowSet> xdfset)
{
    OA_ptr<AvailableExpressionsDFSet> x;
    x = xdfset.convert<AvailableExpressionsDFSet>();

    OA_ptr<AvailableExpressionsDFSet> killSet;
    killSet = new AvailableExpressionsDFSet();

    // Generate KILL code:
    OA_ptr<DFAGenDFSet<OA_ptr<NewExprTree> > > set2;
    set2 = new DFAGenDFSet<OA_ptr<NewExprTree> >();
    AvailableExpressionsDFSet::iterator iter1;
    OA_ptr<NewExprTree> iter1Val;

    for(iter1 = x->begin(); iter1 != x->end(); iter1++) {
        iter1Val = *iter1
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set3;
        set3 = new DFAGenDFSet<OA_ptr<Location> >(mStmt2MayUseMap[iter1Val]);
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set4;
        set4 = new DFAGenDFSet<OA_ptr<Location> >(mStmt2MayDefMap[stmt]);
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set5;
        set5 = set3->intersect(set4);
        bool cond1;
        cond1 = !set5->isEmpty();
        bool cond;
        cond = cond1;
        if(cond) {
            set2->insert(iter1Val);
        }
    }
    killSet = set2;

    return killSet;
}

// Currently the transfer function used by DFAGen is assumed to be:
// Y = gen[n] U (X - kill[n]).
OA_ptr<DataFlowSet>
ManagerAvailableExpressionsStandard::transfer(
    OA_ptr<DataFlowSet> xdfset,
    StmtHandle stmt)
{
    OA_ptr<set<".FLOWTYPE" > > xdfgenset;
    xdfgenset = xdfset.convert<set<OA_ptr<NewExprTree>  > >();

    OA_ptr<AvailableExpressionsDFSet> x;
    x = new AvailableExpressionsDFSet(*xdfgenset);

    // if debug mode is on print information about transfer function
    if (debug) {
        cout << "In transfer: " << endl;
        cout << "\t Stmt  = " << mIR->toString(stmt) << endl;
        cout << "\t X     = ";
        dumpset(x);
    }

    OA_ptr<DataFlowSet> genBaseObj = genSet(stmt);
    OA_ptr<AvailableExpressionsDFSet> gen = genBaseObj.convert<AvailableExpressionsDFSet>();

    OA_ptr<DataFlowSet> killBaseObj = killSet(stmt, x);
    OA_ptr<AvailableExpressionsDFSet> kill = killBaseObj.convert<AvailableExpressionsDFSet>();


    // Add gen[n] to Y
    AvailableExpressionsDFSet::iterator genIter = gen->begin();
    for(; genIter != gen->end(); ++genIter) {
        OA_ptr<NewExprTree>  genVal;
        genVal = *genIter;
        mAvailableExpressionsMap->insert(stmt, genVal);
    }
    // Add X - kill[n] to Y
    x->minusEqu(*kill);
    AvailableExpressionsDFSet::iterator xIter = x->begin();
    for(; xIter != x->end(); ++xIter) {
        OA_ptr<NewExprTree>  xVal;
        xVal = *xIter;
        mAvailableExpressionsMap->insert(stmt, xVal);
    }

    // if debug mode is on print information about transfer function
    if(debug) {
        cout << "\t gen  = ";
        dumpset(gen);
        cout << "\t kill = ";
        dumpset(kill);
        cout << "\t Y    = ";
        dumpset(mAvailableExpressionsMap->getAvailableExpressionsSet(stmt));
    }

    return mAvailableExpressionsMap->getAvailableExpressionsSet(stmt);
}

  } // end of namespace AvailableExpressions
} // end of namespace OA
