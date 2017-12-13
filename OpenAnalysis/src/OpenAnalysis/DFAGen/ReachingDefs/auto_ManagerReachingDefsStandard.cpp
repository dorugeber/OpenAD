
// ManagerReachingDefsStandard.cpp

#include "auto_ManagerReachingDefsStandard.hpp"

using namespace std;
using namespace OA::DataFlow;
using namespace OA::CFG;
using namespace OA::Alias;
using namespace OA::SideEffect;

namespace OA {
  namespace ReachingDefs {

static bool debug = false;


ManagerReachingDefsStandard::ManagerReachingDefsStandard(
    OA_ptr<ReachingDefsIRInterface> _ir)
    :
    mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerReachingDefsStandard:ALL", debug);
    mSolver = new CFGDFSolver(CFGDFSolver::Forward, *this);
}


OA_ptr<DataFlowSet> ManagerReachingDefsStandard::initializeTop()
{
    OA_ptr<ReachingDefsDFSet> retval;
    retval = new ReachingDefsDFSet();
    return retval;
}


OA_ptr<DataFlowSet> ManagerReachingDefsStandard::initializeBottom()
{
    OA_ptr<ReachingDefsDFSet> retval;
    retval = new ReachingDefsDFSet();
    return retval;
}


OA_ptr<DataFlowSet>
ManagerReachingDefsStandard::initializeNodeIN(OA_ptr<NodeInterface> n)
{
    OA_ptr<ReachingDefsDFSet> retval;
    retval = new ReachingDefsDFSet();
    return retval;
}


OA_ptr<DataFlowSet>
ManagerReachingDefsStandard::initializeNodeOUT(OA_ptr<NodeInterface> n)
{
    OA_ptr<ReachingDefsDFSet> retval;
    retval = new ReachingDefsDFSet();
    return retval;
}


OA_ptr<ReachingDefsStandard> ManagerReachingDefsStandard::performAnalysis(
    ProcHandle proc,
    OA_ptr<CFGInterface> cfg,
    OA_ptr<OA::Alias::Interface> alias,
    OA_ptr<InterSideEffectInterface> interSE)
{
    if (debug) {
        cout << "In ReachingDefs::ManagerReachingDefsStandard::performAnalysis"
             << endl;
    }

    // create the object we'll store results in
    mReachingDefsMap = new ReachingDefsStandard(proc, mIR);

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

    // use the dataflow solver to get the In and Out sets for the
    // basic blocks (BBs).
    mSolver->solve(cfg, ITERATIVE);

    // get exit node for CFG and determine what data flow values exit it.
    // store these data flow values in the result's exit information.
    OA_ptr<NodeInterface> node;
    node = cfg->getExit();
    OA_ptr<DataFlowSet> x = mSolver->getOutSet(node);
    OA_ptr<ReachingDefsDFSet> exitSet =
        x.convert<ReachingDefsDFSet>();
    for(ReachingDefsDFSet::iterator i = exitSet->begin();
        i != exitSet->end(); i++)
    {
        mReachingDefsMap->insertExit(*i);
    }


    return mReachingDefsMap;
}

void ManagerReachingDefsStandard::dumpset(OA_ptr<ReachingDefsDFSet> inSet) {
    OA_ptr<set<StmtHandle  > > setRecast;
    setRecast = inSet.convert<set<StmtHandle  > >();
    set<StmtHandle  >::iterator iter;
    iter = setRecast->begin();
    for(; iter != setRecast->end(); iter++) {
        cout << mIR->toString(*iter);
    }
    cout << endl;
}


OA_ptr<DataFlowSet>
ManagerReachingDefsStandard::meet(
    OA_ptr<DataFlowSet> set1orig,
    OA_ptr<DataFlowSet> set2orig)
{
    // convert the incoming sets to sets of the specific flow value.
    OA_ptr<ReachingDefsDFSet> set1 = set1orig.convert<ReachingDefsDFSet>();
    OA_ptr<ReachingDefsDFSet> set2 = set2orig.convert<ReachingDefsDFSet>();

    if (debug) {
        cout << "ManagerReachingDefsStandard::meet" << endl;
        cout << "\tset1 = ";
        dumpset(set1);
        cout << "\tset2 = ";
        dumpset(set2);
    }

    // perform the meet operator on the left hand set.  It's okay to modify the
    // set and return it as a result [since the solver passes a temporary set
    // in as set1.  See the CFGDFProblem header for more info.
    set1->unionEqu(*set2);

    if (debug) {
        cout << endl << "\tcalculated set = ";
        dumpset(set1);
    }

    return set1;
}

OA_ptr<DataFlowSet> ManagerReachingDefsStandard::genSet(StmtHandle stmt)
{
    OA_ptr<ReachingDefsDFSet> genSet;
    genSet =  new ReachingDefsDFSet();

    // Generate GEN code:
    OA_ptr<DFAGenDFSet<StmtHandle > > set1;
    set1 = new DFAGenDFSet<StmtHandle >();
    ReachingDefsDFSet::iterator iter1;
    StmtHandle iter1Val;

    {
        iter1Val = stmt;
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set2;
        set2 = new DFAGenDFSet<OA_ptr<Location> >(mStmt2MayDefMap[iter1Val]);
        bool cond1;
        cond1 = !set2->isEmpty();
        bool cond;
        cond = cond1;
        if(cond) {
            set1->insert(iter1Val);
        }
    }
    genSet = set1;

    return genSet;
}

OA_ptr<DataFlowSet> ManagerReachingDefsStandard::killSet(
    StmtHandle stmt, OA_ptr<DataFlowSet> xdfset)
{
    OA_ptr<ReachingDefsDFSet> x;
    x = xdfset.convert<ReachingDefsDFSet>();

    OA_ptr<ReachingDefsDFSet> killSet;
    killSet = new ReachingDefsDFSet();

    // Generate KILL code:
    OA_ptr<DFAGenDFSet<StmtHandle > > set3;
    set3 = new DFAGenDFSet<StmtHandle >();
    ReachingDefsDFSet::iterator iter2;
    StmtHandle iter2Val;

    for(iter2 = x->begin(); iter2 != x->end(); iter2++) {
        iter2Val = *iter2;
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set4;
        set4 = new DFAGenDFSet<OA_ptr<Location> >(mStmt2MayDefMap[iter2Val]);
        OA_ptr<DFAGenDFSet<OA_ptr<Location> > > set5;
        set5 = new DFAGenDFSet<OA_ptr<Location> >(mStmt2MustDefMap[stmt]);
        bool cond2;
        cond2 = set4->isSubset(*set5);
        bool cond;
        cond = cond2;
        if(cond) {
            set3->insert(iter2Val);
        }
    }
    killSet = set3;

    return killSet;
}

// Currently the transfer function used by DFAGen is assumed to be:
// Y = gen[n] U (X - kill[n]).
OA_ptr<DataFlowSet>
ManagerReachingDefsStandard::transfer(
    OA_ptr<DataFlowSet> xdfset,
    StmtHandle stmt)
{
    OA_ptr<set<StmtHandle> > xdfgenset;
    xdfgenset = xdfset.convert<set<StmtHandle> >();

    OA_ptr<ReachingDefsDFSet> x;
    x = new ReachingDefsDFSet(*xdfgenset);

    // if debug mode is on print information about transfer function
    if (debug) {
        cout << "In transfer: " << endl;
        cout << "\t Stmt  = " << mIR->toString(stmt) << endl;
        cout << "\t X     = ";
        dumpset(x);
    }

    OA_ptr<DataFlowSet> genBaseObj = genSet(stmt);
    OA_ptr<ReachingDefsDFSet> gen = genBaseObj.convert<ReachingDefsDFSet>();

    OA_ptr<DataFlowSet> killBaseObj = killSet(stmt, x);
    OA_ptr<ReachingDefsDFSet> kill = killBaseObj.convert<ReachingDefsDFSet>();


    // Add gen[n] to Y
    ReachingDefsDFSet::iterator genIter = gen->begin();
    for(; genIter != gen->end(); ++genIter) {
        StmtHandle  genVal;
        genVal = *genIter;
        mReachingDefsMap->insert(stmt, genVal);
    }
    // Add X - kill[n] to Y
    x->minusEqu(*kill);
    ReachingDefsDFSet::iterator xIter = x->begin();
    for(; xIter != x->end(); ++xIter) {
        StmtHandle  xVal;
        xVal = *xIter;
        mReachingDefsMap->insert(stmt, xVal);
    }

    // if debug mode is on print information about transfer function
    if(debug) {
        cout << "\t gen  = ";
        dumpset(gen);
        cout << "\t kill = ";
        dumpset(kill);
        cout << "\t Y    = ";
        dumpset(mReachingDefsMap->getReachingDefsSet(stmt));
    }

    return mReachingDefsMap->getReachingDefsSet(stmt);
}

  } // end of namespace ReachingDefs
} // end of namespace OA
