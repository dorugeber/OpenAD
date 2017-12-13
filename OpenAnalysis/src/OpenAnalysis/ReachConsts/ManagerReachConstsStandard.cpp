/*! \file
  
  \brief The AnnotationManager that generates ReachDefsStandards
         which map each statement to a set of reaching definitions
         that reach that statement.

  \authors Michelle Strout, Barbara Kreaseck
  \version $Id: ManagerReachConstsStandard.cpp,v 1.16 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerReachConstsStandard.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace ReachConsts {

#if defined(DEBUG_ALL) || defined(DEBUG_ManagerReachConsts)
static bool debug = true;
static bool extra_debug = true;
static bool meet_debug = true;
static bool transfer_debug = true;
#else
static bool debug = false;
static bool extra_debug = false;
static bool meet_debug = false;
static bool transfer_debug = false;
#endif

/*!
*/
ManagerReachConstsStandard::ManagerReachConstsStandard(OA_ptr<ReachConstsIRInterface> _ir) 
    : mIR(_ir) 
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerReachConstsStandard:ALL", debug);
    mSolver = new DataFlow::CFGDFSolver(DataFlow::CFGDFSolver::Forward,*this);
}

void ManagerReachConstsStandard::initializeTopAndBottom() 
{
    if (debug) { 
        std::cout << "In ManagerReachConsts::initializeTopAndBottom";
        std::cout << std::endl;
    }
    mAllTop = new ConstDefSet;
    mAllBottom = new ConstDefSet;
    // Initialize mAllTop and mAllBottom
    // For each statement:
    OA_ptr<OA::IRStmtIterator> sIt = mIR->getStmtIterator(mProc);
    for ( ; sIt->isValid(); (*sIt)++) {
      OA::StmtHandle stmt = sIt->current();
    
      // for each mem ref in this statement
      OA_ptr<MemRefHandleIterator> refIterPtr = mIR->getAllMemRefs(stmt);
      for (; refIterPtr->isValid(); (*refIterPtr)++) {
          MemRefHandle ref = refIterPtr->current();

          // get all may locations for this mem ref, must instead?
          OA_ptr<LocIterator> locIterPtr = mAlias->getMustLocs(ref);
          for (; locIterPtr->isValid(); ++(*locIterPtr)) {
            OA_ptr<Location> loc = locIterPtr->current();
            if (debug) { 
                std::cout << "\tMayLoc = ";
                loc->dump(std::cout, mIR);
            }
          
            // create an entry in mAllTop and mAllBottom for this location
            OA_ptr<ConstValBasicInterface> nullVal; nullVal = 0; 
            OA_ptr<ConstDef> cd; cd = new ConstDef(loc,nullVal,TOP);
            if (debug) { 
                std::cout << "\tBefore insert into mAllTop" << std::endl;
                mAllTop->dump(std::cout, mIR);
            }
            mAllTop->insert(cd);
            if (debug) { 
                std::cout << "\tAfter insert into mAllTop" << std::endl;
                mAllTop->dump(std::cout, mIR);
            }
            cd = new ConstDef(loc,nullVal,BOTTOM);
            mAllBottom->insert(cd);
          }
      }
      
      // must or may defines from procedure calls?
      // NO, until get interprocedural ReachConsts working
      // won't get any constant definitions from procedure calls
      // just kills of existing constant definitions
    }
}

OA_ptr<DataFlow::DataFlowSet> ManagerReachConstsStandard::initializeTop()
{
    if (mAllTop.ptrEqual(NULL)) {
      initializeTopAndBottom();
    }
    if (debug) { 
        std::cout << "mAllTop = ";
        mAllTop->dump(std::cout,mIR);
    }
    return mAllTop;
}

OA_ptr<DataFlow::DataFlowSet> ManagerReachConstsStandard::initializeBottom()
{
    if (mAllBottom.ptrEqual(NULL)) {
      initializeTopAndBottom();
    }
    if (debug) { 
        std::cout << "mAllBottom = ";
        mAllBottom->dump(std::cout,mIR);
    }
    return mAllBottom;
}

/*!
    Uses the CFGDFProblem::solve functionality to get In and Out
    constant definition sets for each basic block.  Then uses the 
    statement transfer function to get an In set for each stmt.
*/
OA_ptr<ReachConstsStandard> ManagerReachConstsStandard::performAnalysis(ProcHandle proc, 
    OA_ptr<CFG::CFGInterface> cfg, OA_ptr<Alias::Interface> alias,
    OA_ptr<SideEffect::InterSideEffectInterface> interSE,
    DataFlow::DFPImplement algorithm)
{
  mProc = proc;

  // store a new (waiting to be filled) ReachConstsStandard as member
  mRCS = new ReachConstsStandard(proc);

  // store Alias information for use within the transfer function
  mAlias = alias;

  // store CFG for use within initializeNode function
  mCFG = cfg;

  // store side-effect information for use within transfer function
  mInterSE = interSE;

  // use the dataflow solver to get 
  //    - the In and Out sets for the BBs
  //    - the relationship of memref->reachingConstant (set inside solve)
  //DataFlow::CFGDFProblem::solve(cfg);
  mSolver->solve(cfg,algorithm);

  
/*  OA_ptr<CFG::CFGInterface::NodesIterator> nIter
             = cfg->getNodesIterator();
 
  for ( ; nIter->isValid(); ++(*nIter)) {
 
        OA_ptr<CFG::CFGInterface::Node> node = nIter->current();
        OA_ptr<DataFlow::DataFlowSet> dfset = mSolver->getInSet(node); */

  // iterate over BBs:  using In sets and transfer function, 
  //    - create relationship of stmt->reachingConstDefSet
 /* std::map<OA_ptr<CFG::CFGInterface::Node>,
           OA_ptr<DataFlow::DataFlowSet> >::iterator mapIter;
  for (mapIter=mSolver->mNodeInSetMap.begin(); mapIter!=mSolver->mNodeInSetMap.end();
       mapIter++)
  {
      // CFG node
      OA_ptr<CFG::CFGInterface::Node> node = mapIter->first;

      // In set
      OA_ptr<DataFlow::DataFlowSet> dfset = mapIter->second;


      
      OA_ptr<ReachConsts::ConstDefSet> inSet 
          = dfset.convert<ReachConsts::ConstDefSet>();
      if (debug) { 
        std::cout << "ManReachConstsStd::perfAnal "
                  << "Node #" << node->getId() << ":" << std::endl;
        std::cout << "    inSet = " ;
        inSet->dump(std::cout,mIR); 
      }
      
      // get a copy of the IN set for BB
      OA_ptr<DataFlow::DataFlowSet> tempbase = inSet->clone();
      OA_ptr<ReachConsts::ConstDefSet> temp 
          = tempbase.convert<ReachConsts::ConstDefSet>();
      //OA_ptr<ReachConsts::ConstDefSet> temp = inSet->clone();

      // for each BB, iterate over the statements and find the 
      // ConstDefs for the statement
      OA_ptr<CFG::CFGInterface::NodeStatementsIterator> stmtIterPtr 
          = node->getNodeStatementsIterator();
      for (; stmtIterPtr->isValid(); ++(*stmtIterPtr)) { 
        StmtHandle s = stmtIterPtr->current();

        ConstDefSetIterator cdIter(*(temp.convert<ConstDefSet>()));
        for (; cdIter.isValid(); ++(cdIter)) {
          OA_ptr<ConstDef> constDef = cdIter.current();
          mRCS->insertConstDef(s,constDef);
        }

        // What is the use of temp ??? 
        // apply transfer function to temp set
        OA_ptr<DataFlow::DataFlowSet> tempdfset = transfer(temp,s);
        temp = tempdfset.convert<ReachConsts::ConstDefSet>();

        if (debug) { 
          std::cout << "ManReachConstsStd::perfAnal: after stmt "
                    << mIR->toString(s) << std::endl;
          std::cout << "    temp = ";
          temp->dump(std::cout,mIR); 
        } 
      }
  } */    
  return mRCS;
}

//------------------------------------------------------------------
// Implementing the callbacks for CFGDFProblem
//------------------------------------------------------------------
/*void ManagerReachConstsStandard::initializeNode(OA_ptr<CFG::CFGInterface::Node> n)
{
  if (n.ptrEqual(mCFG->getEntry())) {
    mNodeInSetMap[n]=mAllBottom->clone(); // from ManagerReachConstsStandard
    mNodeOutSetMap[n]=mAllBottom->clone();
  } else {
    mNodeInSetMap[n] = new ConstDefSet;
    mNodeOutSetMap[n]=mAllTop->clone(); // from ManagerReachConstsStandard
  }
}*/


OA_ptr<DataFlow::DataFlowSet>
ManagerReachConstsStandard::initializeNodeIN(OA_ptr<CFG::NodeInterface> n)
{
  if (mAllTop.ptrEqual(NULL)) {
    initializeTopAndBottom();
  }
  if (n.ptrEqual(mCFG->getEntry())) {
    return mAllBottom->clone();
  }
  OA_ptr<ConstDefSet> retval;
  retval = new ConstDefSet();
  return retval;
}

OA_ptr<DataFlow::DataFlowSet>
ManagerReachConstsStandard::initializeNodeOUT(OA_ptr<CFG::NodeInterface> n)
{
  if (mAllTop.ptrEqual(NULL)) {
    initializeTopAndBottom();
  }
  if (n.ptrEqual(mCFG->getEntry())) {
    return mAllBottom->clone();
  }
  return mAllTop->clone();
}

/*******************  First Attempt, didn't quite work
OA_ptr<DataFlow::DataFlowSet>
ManagerReachConstsStandard::initializeNodeIN(OA_ptr<CFG::CFGInterface::Node> n)
{
     if (mAllTop.ptrEqual(NULL)) {
          initializeTopAndBottom();
     }
     if (debug) {
          std::cout << "mAllTop = ";
          mAllTop->dump(std::cout,mIR);
     }
     return mAllTop;
}

OA_ptr<DataFlow::DataFlowSet>
ManagerReachConstsStandard::initializeNodeOUT(OA_ptr<CFG::CFGInterface::Node> n)
{
     if (mAllTop.ptrEqual(NULL)) {
           initializeTopAndBottom();
     }
     if (debug) {
           std::cout << "mAllTop = ";
           mAllTop->dump(std::cout,mIR);
     }
     return mAllTop;
}
***********************/

//! meet routine for ReachConsts
OA_ptr<DataFlow::DataFlowSet> 
ManagerReachConstsStandard::meet (OA_ptr<DataFlow::DataFlowSet> set1orig, 
                       OA_ptr<DataFlow::DataFlowSet> set2orig)
{
  // can change set1orig if wanted:  Usage in CFGDFProblem::atDGraphNode()
  // sets set1 = meet(set1, set2)

  OA_ptr<DataFlow::DataFlowSet> set1clone = set1orig->clone(); 
  OA_ptr<DataFlow::DataFlowSet> set2clone = set2orig->clone(); 
  OA_ptr<ConstDefSet> set1 = set1clone.convert<ConstDefSet>();
  OA_ptr<ConstDefSet> set2 = set2clone.convert<ConstDefSet>();

  if (meet_debug) {
    std::cout << "MEET --------------------------------" << std::endl;
    std::cout << ">>>>> before pairwise refinement" << std::endl;
    std::cout << "set1 =" << std::endl;
    set1->dump(std::cout,mIR);
    std::cout << "set2 =" << std::endl;
    set2->dump(std::cout,mIR);
  }

  ConstDefSetIterator set1Iter(*set1);
  ConstDefSetIterator set2Iter(*set2);

  bool changeOccurred = true; // set to true to enable initial pass
  // need to loop over two sets multiple times because there might be locations
  // A and B in set one that overlap each other and only A overlaps location
  // C that is in set two?
  while (changeOccurred) {
    changeOccurred = false; 

    ConstDefSet killSet1;
    ConstDefSet genSet1;

    // Perform pairwise meet over Set1 and Set2

    // for each ConstDefLoc in Set1
    set1Iter.reset();
    for (; set1Iter.isValid(); ++(set1Iter)) {
      OA_ptr<ConstDef> constDef1 = set1Iter.current();
      OA_ptr<Location> cdLocPtr1 = constDef1->getLocPtr();
      OA_ptr<ConstValBasicInterface> cdConstPtr1 = constDef1->getConstPtr();

      ConstDefSet killSet2;
      ConstDefSet genSet2;

      // for each ConstDefLoc in Set2
      set2Iter.reset();
      for (; set2Iter.isValid(); ++(set2Iter)) {
        OA_ptr<ConstDef> constDef2 = set2Iter.current();
        OA_ptr<Location> cdLocPtr2 = constDef2->getLocPtr();
        OA_ptr<ConstValBasicInterface> cdConstPtr2 = constDef2->getConstPtr();

        if (meet_debug) {
          std::cout << "in meet: pair " << constDef1->toString(mIR)
                    << " vs. " << constDef2->toString(mIR) << std::endl;
        }
        
        ManagerReachConstsStandard::MeetOp op = NOTHING;
        // if Loc1 mustOverlap Loc2,
        if (cdLocPtr1->mustOverlap(*cdLocPtr2)) {
          if (meet_debug) {std::cout << "\tmustOp=";}
          op = getMustMeetOp(constDef1, constDef2); // must only
        } else if (cdLocPtr1->mayOverlap(*cdLocPtr2)) {
          if (meet_debug) {std::cout << "\tmayOp=";}
          op = getMayOnlyMeetOp(constDef1, constDef2); // may only
        }

        OA_ptr<ConstDef> cd;
        OA_ptr<ConstValBasicInterface> nullVal;
        switch (op) {
        case CD1toBOTTOM:
          if (meet_debug) {std::cout << "CD1toBOTTOM" << std::endl;}
          killSet1.insert(constDef1);
          cd = new ConstDef(cdLocPtr1,nullVal,BOTTOM);
          genSet1.insert(cd);
          changeOccurred = true;
          break;
        case CD2toBOTTOM:
          if (meet_debug) {std::cout << "CD2toBOTTOM" << std::endl;}
          killSet2.insert(constDef2);
          cd = new ConstDef(cdLocPtr2,nullVal,BOTTOM);
          genSet2.insert(cd);
          changeOccurred = true;
          break;
        case BOTHtoBOTTOM:
          if (meet_debug) {std::cout << "BOTHtoBOTTOM" << std::endl;}
          killSet1.insert(constDef1);
          cd = new ConstDef(cdLocPtr1,nullVal,BOTTOM);
          genSet1.insert(cd);
          killSet2.insert(constDef2);
          cd = new ConstDef(cdLocPtr2,nullVal,BOTTOM);
          genSet2.insert(cd);
          changeOccurred = true;
          break;
        case CD1toVALUECD2:
          if (meet_debug) {std::cout << "CD1toVALUECD2" << std::endl;}
          killSet1.insert(constDef1);
          cd = new ConstDef(cdLocPtr1,cdConstPtr2,VALUE);
          genSet1.insert(cd);
          changeOccurred = true;
          break;
        case CD2toVALUECD1:
          if (meet_debug) {std::cout << "CD2toVALUECD1" << std::endl;}
          killSet2.insert(constDef2);
          cd = new ConstDef(cdLocPtr2,cdConstPtr1,VALUE);
          genSet2.insert(cd);
          changeOccurred = true;
          break;
        case NOTHING:
          if (meet_debug) {std::cout << "NOTHING" << std::endl;}
          break;
        }

      } // end of set2 iteration
      
      // Update set2
      // Apply Kills
      ConstDefSetIterator setIter(killSet2);
      for (; setIter.isValid(); ++(setIter)) {
        OA_ptr<ConstDef> constDef = setIter.current();
        if (meet_debug) {
          int k = set2->removeANDtell(constDef);
          std::cout << "MEET: killed "<<k<<" from set2: (cd "
                    << constDef->toString(mIR) 
                    << ")" << std::endl;
        } else {
          set2->remove(constDef);
        }
      }
      // Apply Gens
      ConstDefSetIterator vsetIter(genSet2);
      for (; vsetIter.isValid(); ++(vsetIter)) {
        OA_ptr<ConstDef> constDef = vsetIter.current();
        set2->replace(constDef->getLocPtr(),constDef->getConstPtr(),
                     constDef->getConstDefType());
        if (meet_debug) {
          std::cout << "MEET: Replacement in set2: (cd "
                    << constDef->toString(mIR) 
                    << ")" << std::endl;
        }
      }
    }// end of set1 iteration

    // Update set1
    // Apply Kills
    ConstDefSetIterator setIter(killSet1);
    for (; setIter.isValid(); ++(setIter)) {
      OA_ptr<ConstDef> constDef = setIter.current();
      if (meet_debug) {
        int k = set1->removeANDtell(constDef);
        std::cout << "MEET: killed "<<k<<" from set1: (cd "
                  << constDef->toString(mIR) << ")" << std::endl;
      } else {
        set1->remove(constDef);
      }
    }
    // Apply Gens
    ConstDefSetIterator vsetIter(genSet1);
    for (; vsetIter.isValid(); ++(vsetIter)) {
      OA_ptr<ConstDef> constDef = vsetIter.current();
      set1->replace(constDef->getLocPtr(),constDef->getConstPtr(),
                   constDef->getConstDefType());
      if (meet_debug) {
        std::cout << "MEET: Val replacement in set1: (cd "
                  << constDef->toString(mIR) << ")" << std::endl;
      }
    }
  } // end of while changeOccurred 

  // Now, what do I do with set1 and set2? (should be equiv)
  if (meet_debug) {
    std::cout << ">>>>> after pairwise refinement, set1 and set2 should be equiv" << std::endl;
    std::cout << "set1 =" << std::endl;
    set1->dump(std::cout,mIR);
    std::cout << "set2 =" << std::endl;
    set2->dump(std::cout,mIR);
  }

  // returning set1
  //set1orig = set1;
  //return set1orig;
  return set1;

}// end of meet function

/*!
    \brief Core of analysis algorithm.  Transfer function for a stmt.

    KILL is the set of reaching const definitions <Loc,Const> 
       where Loc MayOverlap a MayLoc for this statement
       (remember that MustOverlap is a subset of MayOverlap
             and that MustLoc is a subset of MayLoc)
    killREPLACE is the new ConstDef definition for the ConstDef in the KILL set
       These replace the reaching ConstDef with a ConstDefType of BOTTOM. 

    GEN:  When statement expression evaluates to a non-null ConstValBasic,
        GEN is the set of const definitions <Loc,Const> 
        where Loc is a MustLoc and MustOverlap for targets of this statement
        (The loc doesn't have to be in the inSet and it has to be a
         MustLoc.  Loop through all the MustLocs for the MemRefExpr and pair
         them up with the ConstValBasic for the expression. Insert these
         into the inSet, replacing any existing ConstDef for that MustLoc.

    Update inSet = inSet ( - KILLS + killREPLACE) + GEN
*/

//! Transfer function for ReachConsts, inReachConsts->stmt->outReachConsts
//
// Brief Algorithm:
// 1) Update mRCS mapping for all use memrefs for this statement
// 2) Generate/apply KILL SET based upon ReachConstsStmtType
// 3) Generate/apply GEN SET based upon ReachConstsStmtType
// 4) Update mRCS mapping for all def memrefs for this statement
//-----------------------------------------------------------------------
OA_ptr<DataFlow::DataFlowSet> 
ManagerReachConstsStandard::transfer(OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt) 
{
  ConstDefSet killSet;
  ConstDefSet replaceSet;
  ConstDefSet genSet;
  OA_ptr<DataFlow::DataFlowSet> inclone = in->clone();
  OA_ptr<ConstDefSet> inRecast = inclone.convert<ConstDefSet>();
  // need another copy because can't iterate over set and update it at same time
  inclone = in->clone();  // need this or get segfault
  OA_ptr<ConstDefSet> inRecastCopy = inclone.convert<ConstDefSet>();

  /*  Added this for loop by PLM 07/27/06. It is moved out of performAnalysis.
   *  This loop will add Constant Definitions for every statment 
  */ 
 
  ConstDefSetIterator cdIter(*inRecast);
  for (; cdIter.isValid(); ++(cdIter)) {
         OA_ptr<ConstDef> constDef = cdIter.current();
         mRCS->insertConstDef(stmt,constDef);
  }
             
  
  if (transfer_debug) {
    std::cout << "--- --- --- --- --- Top of Transfer " << std::endl;
    //std::cout << "--- inRecastSet = ";
    //inRecast->dump(std::cout,mIR);
  }

  // 1) Update mapping for all use memrefs for this statement
  setUseMemRef2Const(stmt, *inRecast);

  if (debug) {
      std::cout << "------------------------------------------------"
                << std::endl;
      std::cout << "transfer: stmt = (" << stmt.hval() << ": "
                << mIR->toString(stmt) << ") :" << std::endl;
      std::cout << "\tConstDefsIn: ";
      inRecast->dump(std::cout,mIR);
      std::cout.flush();
  }

  // KILLSET construction algorithm (incl. KILLSET application)
  //--------------------------------
  // for each MemRefHandle of the DefMemRefs for this stmt:
  //   for each MayLoc of this MemRefHandle: 
  //     if the Loc of a ConstDef in inSet MayOverlap the MayLoc:
  //       replace ConstDef in the inSet with 
  //               (ConstDef.getLocPtr(),BOTTOM)
  //--------------------------------
    
  { // beginning of KILLSET creation/application 
    
    // for each def mem ref for this statement
    OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(stmt);
    for (; defIterPtr->isValid(); (*defIterPtr)++) {
        MemRefHandle memref = defIterPtr->current();
        
        // for each mayLoc for this mem ref
        OA_ptr<LocIterator> locIterPtr = mAlias->getMayLocs(memref);
        for (; locIterPtr->isValid(); ++(*locIterPtr)) {
          OA_ptr<Location> mayLocPtr = locIterPtr->current();
          
          // for each ConstDefLoc in inSet
          OA_ptr<ConstDefSetIterator> inIter;
          inIter = new ConstDefSetIterator(*inRecastCopy);
          for (; inIter->isValid(); ++(*inIter)) {
            OA_ptr<ConstDef> constDef = inIter->current();
            OA_ptr<Location> cdLocPtr = constDef->getLocPtr();
            
            // if Loc mayOverLap MayLoc, replace any constDef with cdLocPtr
            // with ConstDef(cdLocPtr,BOTTOM)
            if (cdLocPtr->mayOverlap(*mayLocPtr)) {
              OA_ptr<ConstValBasicInterface> nullVal; nullVal = 0;
              inRecast->replace(cdLocPtr,nullVal,BOTTOM);
            }
          }
        }
    }

    // loop through all func calls in this statement and what they MOD
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle expr = callsiteItPtr->current();

      OA_ptr<LocIterator> locIterPtr;
      // MOD
      locIterPtr = mInterSE->getMODIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          OA_ptr<Location> modLocPtr = locIterPtr->current();
          
          // for each ConstDefLoc in inSet
          OA_ptr<ConstDefSetIterator> inIter;
          inIter = new ConstDefSetIterator(*inRecastCopy);
          for (; inIter->isValid(); ++(*inIter)) {
            OA_ptr<ConstDef> constDef = inIter->current();
            OA_ptr<Location> cdLocPtr = constDef->getLocPtr();
            
            // if Loc mayOverLap modLoc, replace any constDef with cdLocPtr
            // with ConstDef(cdLocPtr,BOTTOM)
            if (cdLocPtr->mayOverlap(*modLocPtr)) {
              OA_ptr<ConstValBasicInterface> nullVal; nullVal = 0;
              inRecast->replace(cdLocPtr,nullVal,BOTTOM);
            }
          }
       } 

    }
 
  } // end of KILLSET creation/application 


  // GENSET construction/application algorithm for EXPR_STMT
  //--------------------------------
  // for each  AssignPair -- <MemRefHandle,ExprTree> -- for this stmt:
  //   get ExprConst from ExprTree if it exists
  //   if ExprConst exists:
  //     for each MustLoc of the MemRefHandle
  //       replace any ConstDef(MustLoc) in inSet with
  //                   ConstDef(MustLoc,ExprConst,VALUE)
  //--------------------------------

  OA_ptr<AssignPairIterator> espIterPtr 
         = mIR->getAssignPairIterator(stmt);
  for ( ; espIterPtr->isValid(); (*espIterPtr)++) {
        // unbundle pair
        MemRefHandle mref = espIterPtr->currentTarget();
        ExprHandle expr = espIterPtr->currentSource();
        
        // getConstValBasic if possible
        OA_ptr<ConstValBasicInterface> cvbiPtr; cvbiPtr = 0;
        OA_ptr<ExprTree> eTreePtr = mIR->getExprTree(expr);

        // get ConstValBasicInterface by evaluating expression tree
        EvalToConstVisitor evalVisitor(mIR,mRCS);
        eTreePtr->acceptVisitor(evalVisitor);
        cvbiPtr = evalVisitor.getConstVal();

        if (!cvbiPtr.ptrEqual(NULL)) { // GEN a constant value
          if (transfer_debug) {
            std::cout << "After eval, cvbiPtr = (" << cvbiPtr <<")"<<std::endl;
            std::cout.flush();
            std::cout << "           *cvbiPTr = (" << cvbiPtr->toString()
                      << ")" << std::endl;
            std::cout.flush();
          }
          
          // for each MustLoc, replace any ConstDef(MustLoc,...) in inSet
          //     with new constant value: ConstDef(MustLoc,cvbiPtr,VALUE)
          OA_ptr<LocIterator> lIterPtr = mAlias->getMustLocs(mref);
          for ( ; lIterPtr->isValid() ; ++(*lIterPtr)) {
            OA_ptr<Location> lPtr = lIterPtr->current();
            inRecast->replace(lPtr,cvbiPtr,VALUE);
          }
        } // end of "if we have a usable const val"
  } // end of loop over AssignPairList
 
    
  // 4) update mRCS for all mayLocs(???) of the DefMemRefs(stmt)
  setDefMemRef2Const(stmt, *inRecast);
  
  if (debug) {
    std::cout << "\tConstDefsOut: ";
    inRecast->dump(std::cout,mIR);
  }
 
  return inRecast;
}

//! Decides meet operation for two ConstDefs that are mustLocs
ManagerReachConstsStandard::MeetOp 
ManagerReachConstsStandard::getMustMeetOp(OA_ptr<ConstDef> cd1, OA_ptr<ConstDef> cd2) 
{
  ConstDefType cdType1 = cd1->getConstDefType();
  OA_ptr<ConstValBasicInterface> cdConstPtr1 = cd1->getConstPtr();
  ConstDefType cdType2 = cd2->getConstDefType();
  OA_ptr<ConstValBasicInterface> cdConstPtr2 = cd2->getConstPtr();
  ManagerReachConstsStandard::MeetOp retval = NOTHING;

  // apply MustOverlap MEET rules
  switch (cdType1) {
  case TOP:
    switch (cdType2) {
    case TOP:
      // TOP MEET TOP (MustOverlap)
      retval = NOTHING; break;
    case VALUE:
      // TOP MEET VALUE (MustOverlap)
      retval = CD1toVALUECD2; break;
    case BOTTOM:      
      // TOP MEET BOTTOM (MustOverlap)
      retval = CD1toBOTTOM; break;
    }
    break;

  case VALUE:
    switch (cdType2) {
    case TOP:
      // VALUE MEET TOP (MustOverlap)
      retval = CD2toVALUECD1; break;
    case VALUE:
      // VALUE MEET VALUE (MustOverlap)
      if (cdConstPtr1 != cdConstPtr2) {
        retval = BOTHtoBOTTOM;
      } else {
        retval = NOTHING;
      }
      break;
    case BOTTOM:
      // VALUE MEET BOTTOM (MustOverlap)
      retval = CD1toBOTTOM; break;
    }
    break;
    
  case BOTTOM:
    switch (cdType2) {
    case TOP:     
      // BOTTOM MEET TOP (MustOverlap)
      retval = CD2toBOTTOM; break;
    case VALUE:
      // BOTTOM MEET VALUE (MustOverlap)
      retval = CD2toBOTTOM; break;
    case BOTTOM:
      // BOTTOM MEET BOTTOM (MustOverlap)
      retval = NOTHING; break;
    }
    break;
  } // end of switch (cdType1)
  
  return retval;
}

//! Decides meet operation for two ConstDefs that are may(not must)Locs
ManagerReachConstsStandard::MeetOp 
ManagerReachConstsStandard::getMayOnlyMeetOp(OA_ptr<ConstDef> cd1, OA_ptr<ConstDef> cd2) 
{
  ConstDefType cdType1 = cd1->getConstDefType();
  ConstDefType cdType2 = cd2->getConstDefType();
  ManagerReachConstsStandard::MeetOp retval = NOTHING;

  // apply (mayOverlap && !MustOverlap) MEET rules
  switch (cdType1) {
  case TOP:
    switch (cdType2) {
    case TOP:
      // TOP MEET TOP (MayOverlap && !MustOverlap)
      retval = NOTHING; break;
    case VALUE:
      // TOP MEET VALUE (MayOverlap && !MustOverlap)
      retval = BOTHtoBOTTOM; break;
    case BOTTOM:
      // TOP MEET BOTTOM (MayOverlap && !MustOverlap)
      retval = CD1toBOTTOM; break;
    }
    break;
    
  case VALUE:
    switch (cdType2) {
    case TOP:
      // VALUE MEET TOP (MayOverlap && !MustOverlap)
      retval = BOTHtoBOTTOM; break;      
    case VALUE:
      // VALUE MEET VALUE (MayOverlap && !MustOverlap)
      retval = BOTHtoBOTTOM; break;
    case BOTTOM:
      // VALUE MEET BOTTOM (MayOverlap && !MustOverlap)
      retval = CD1toBOTTOM; break;
    }
    break;
    
  case BOTTOM:
    switch (cdType2) {
    case TOP:     
      // BOTTOM MEET TOP (MayOverlap && !MustOverlap)
      retval = CD2toBOTTOM; break;
    case VALUE:
      // BOTTOM MEET VALUE (MayOverlap && !MustOverlap)
      retval = CD2toBOTTOM; break;
    case BOTTOM:    
      // BOTTOM MEET BOTTOM (MayOverlap && !MustOverlap)
      retval = NOTHING; break;
    }
    break;    
  } // end of switch (cdType1)
 
  return retval;
}

//! Sets mMemRef2ReachConst[useMemRef] for useMemRefs in given statement
//! ConstDefSet, in, should not be changed
void 
ManagerReachConstsStandard::setUseMemRef2Const(StmtHandle stmt, const ConstDefSet& in) {

  // for each use mem ref in this statement
  OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
  for (; useIterPtr->isValid(); (*useIterPtr)++) {
    MemRefHandle ref = useIterPtr->current();

    // get all may locations for this mem ref
    OA_ptr<LocIterator> locIterPtr = mAlias->getMayLocs(ref);
    
    // set up first location info
    OA_ptr<Location> loc = locIterPtr->current();
    if (transfer_debug) {
      std::cout << "Finding location (" << loc->toString(mIR) << ")" 
                << std::endl;
      std::cout.flush();
    }

    OA_ptr<ConstDef> cdPtr = in.find(loc);

    // keep track of useful info for this memref
    bool allValuesAgree = true;
    if (cdPtr.ptrEqual(NULL)) {
      allValuesAgree = false; // first location not found in inSet
      if (transfer_debug) {
        std::cout << "a useMemRef (" << mIR->toString(ref) << ") "
                  << "missing ConstDef for first mayLoc ( "
                  << loc->toString(mIR) << ")" << std::endl;
        std::cout.flush();
      }
    } else if (cdPtr->getConstDefType() != VALUE) {
      allValuesAgree = false;
      // first value of mayLoc for this memref is not a value
    } 

    // Now check that all other mayLocs agree
    for (; (allValuesAgree) && (locIterPtr->isValid()); ++(*locIterPtr)) {
      OA_ptr<Location> loc = locIterPtr->current();
      OA_ptr<ConstDef> cdNextPtr = in.find(loc);
      if (cdPtr.ptrEqual(NULL)) {
        allValuesAgree = false; // next location not found in inSet
      } else if (!((*cdPtr).equiv(*cdNextPtr))) {
        allValuesAgree = false; // next location not equiv to previous
      }
    }
    if (allValuesAgree) {
      // record the value in cdPtr to mapping for this memref
      mRCS->updateReachConst(ref,cdPtr->getConstPtr());
    } else {
      // record no constant value for this memref
      OA_ptr<ConstValBasicInterface> nullVal;  nullVal = 0;
      mRCS->updateReachConst(ref,nullVal);
    }
  } // end of useMemRef loop

  if (transfer_debug) {
    OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
    for (; useIterPtr->isValid(); (*useIterPtr)++) {
      MemRefHandle ref = useIterPtr->current();
      OA_ptr<ConstValBasicInterface> cvbiPtr = mRCS->getReachConst(ref);
      std::cout << "useMemRef(" << mIR->toString(ref) ;
      if (cvbiPtr.ptrEqual(NULL)) {
        std::cout << ") has no constant value." << std::endl;
      } else {
        std::cout  << ") maps to VALUE="
                   << cvbiPtr->toString() << std::endl;
      }
    }
  }
} // end of setUseMemRef2Const()

//! Sets mMemRef2ReachConst[defMemRef] for defMemRefs in given statement
//! ConstDefSet, in, should not be changed
void 
ManagerReachConstsStandard::setDefMemRef2Const(StmtHandle stmt, const ConstDefSet& in) {

  // update mRCS for all mayLocs(???) of the DefMemRefs(stmt)
  OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(stmt);
  for (; defIterPtr->isValid(); (*defIterPtr)++) {
    MemRefHandle ref = defIterPtr->current();
    
    // get all may locations for this mem ref
    OA_ptr<LocIterator> locIterPtr = mAlias->getMayLocs(ref);
    
    // set up first location info
    OA_ptr<Location> loc = locIterPtr->current();
    if (transfer_debug) {
      std::cout << "Finding def location (" << loc->toString(mIR) << ")" 
                << std::endl;
      std::cout.flush();
    }
    
    OA_ptr<ConstDef> cdPtr = in.find(loc);
    
    // keep track of useful info for this memref
    bool allValuesAgree = true;
    if (cdPtr.ptrEqual(NULL)) {
      allValuesAgree = false; // first location not found in inSet
      if (transfer_debug) {
        std::cout << "a defMemRef (" << mIR->toString(ref) << ") "
                  << "missing ConstDef for first mayLoc ( "
                  << loc->toString(mIR) << ")" << std::endl;
        std::cout.flush();
      }
    } else if (cdPtr->getConstDefType() != VALUE) {
      allValuesAgree = false;
      // first value of mayLoc for this memref is not a value
    } 
    
    // Now check that all other mayLocs agree
    for (; (allValuesAgree) && (locIterPtr->isValid()); ++(*locIterPtr)) {
      OA_ptr<Location> loc = locIterPtr->current();
      OA_ptr<ConstDef> cdNextPtr = in.find(loc);
      if (cdPtr.ptrEqual(NULL)) {
        allValuesAgree = false; // next location not found in inSet
      } else if (!((*cdPtr).equiv(*cdNextPtr))) {
        allValuesAgree = false; // next location not equiv to previous
      }
    }
    if (allValuesAgree) {
      // record the value in cdPtr to mapping for this memref
      mRCS->updateReachConst(ref,cdPtr->getConstPtr());
    } else {
      // record no constant value for this memref
      OA_ptr<ConstValBasicInterface> nullValue; nullValue = NULL;
      mRCS->updateReachConst(ref,nullValue);
    }
  } // end of defMemRef loop
  
  if (transfer_debug) {
    OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(stmt);
    for (; defIterPtr->isValid(); (*defIterPtr)++) {
      MemRefHandle ref = defIterPtr->current();
      OA_ptr<ConstValBasicInterface> cvbiPtr = mRCS->getReachConst(ref);
      std::cout << "defMemRef(" << mIR->toString(ref);
      if (cvbiPtr.ptrEqual(NULL)) {
        std::cout << ") has no constant value." << std::cout;
      } else {
        std::cout << ") maps to VALUE="
                  << (mRCS->getReachConst(ref))->toString() << std::endl;
      }
    }
  }
} // end of setDefMemRef2Const()

  } // end of namespace ReachConsts
} // end of namespace OA
