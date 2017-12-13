/*! \file
  
  \brief Declarations of the AnnotationManager that generates a ReachConstsStandard

  \authors Michelle Strout, Barbara Kreaseck
  \version $Id: ManagerReachConstsStandard.hpp,v 1.8 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ReachConstsManagerStandard_h
#define ReachConstsManagerStandard_h

//--------------------------------------------------------------------
// OpenAnalysis headers
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/ReachConstsIRInterface.hpp>
#include <OpenAnalysis/ReachConsts/ReachConstsStandard.hpp>

#include <OpenAnalysis/CFG/CFGInterface.hpp>
//#include <OpenAnalysis/CFG/CFGStandard.hpp>

#include <OpenAnalysis/Alias/Interface.hpp>

#include <OpenAnalysis/DataFlow/CFGDFProblem.hpp>
#include <OpenAnalysis/DataFlow/IRHandleDataFlowSet.hpp>
#include <OpenAnalysis/SideEffect/InterSideEffectInterface.hpp>


#include <OpenAnalysis/IRInterface/ConstValBasicInterface.hpp>
#include <OpenAnalysis/ExprTree/EvalToConstVisitor.hpp>
#include <OpenAnalysis/DataFlow/CFGDFSolver.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>


namespace OA {
  namespace ReachConsts {


/*! 
   The AnnotationManager for ReachConstsStandard.
   This class can build an ReachConstsStandard, 
   (eventually) read one in from a file, and write one out to a file.
*/
class ManagerReachConstsStandard 
    : private DataFlow::CFGDFProblem { 
      //??? eventually public OA::AnnotationManager
public:
  ManagerReachConstsStandard(OA_ptr<ReachConstsIRInterface> _ir);
  ~ManagerReachConstsStandard () {}

  OA_ptr<ReachConstsStandard> performAnalysis(ProcHandle, 
        OA_ptr<CFG::CFGInterface> cfg, OA_ptr<Alias::Interface> alias,
        OA_ptr<SideEffect::InterSideEffectInterface> interSE,
        DataFlow::DFPImplement algorithm);

  //--------------------------------------------------------
  // For use within InterMPICFGDFProblem
  // DESIGN Note:  Not in CFGDFProblem because Managers inherit
  // from CFGDFProblem privately and I don't know if I want to
  // switch that to public.  See 406 in C++ book.
  // Note that these methods get a ptr to the DataFlowSet
  // and therefore any changes made will be noticed next time
  // performAnalysis is called.
  //--------------------------------------------------------
  // Shifted to CFGDFProblem */
  // 
  //OA_ptr<DataFlow::DataFlowSet> getInSet(OA_ptr<CFG::Interface::Node> node)
  //  { return mNodeInSetMap[node]; }
  //OA_ptr<DataFlow::DataFlowSet> getOutSet(OA_ptr<CFG::Interface::Node> node)
  //  { return mNodeOutSetMap[node]; }

  //------------------------------------------------------------------
  // Implementing the callbacks for CFGDFProblem
  // The DataFlowSet subclass used is ConstDefSet
  //------------------------------------------------------------------
private:
  OA_ptr<DataFlow::DataFlowSet>  initializeTop();

  OA_ptr<DataFlow::DataFlowSet>  initializeBottom();

  //! helper method for initializeTop and initializeBottom
  void initializeTopAndBottom();
  

  void initializeNode(OA_ptr<CFG::NodeInterface> n);
  //! Should generate an in and out DataFlowSet for node
  OA_ptr<DataFlow::DataFlowSet>
            initializeNodeIN(OA_ptr<CFG::NodeInterface> n);
  OA_ptr<DataFlow::DataFlowSet>
            initializeNodeOUT(OA_ptr<CFG::NodeInterface> n);

  
  OA_ptr<DataFlow::DataFlowSet> 
  meet (OA_ptr<DataFlow::DataFlowSet> set1, 
        OA_ptr<DataFlow::DataFlowSet> set2); 

  OA_ptr<DataFlow::DataFlowSet> 
  transfer(OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt); 

  //------------------------------------------------------------------
  // Helper routines for the callbacks
  //------------------------------------------------------------------
  
  //! Sets mMemRef2ReachConst[useMemRef] for useMemRefs in given statement
  //! ConstDefSet, in, should not be changed
  void setUseMemRef2Const(StmtHandle stmt, const ConstDefSet& in);

  //! Sets mMemRef2ReachConst[defMemRef] for defMemRefs in given statement
  //! ConstDefSet, in, should not be changed
  void setDefMemRef2Const(StmtHandle stmt, const ConstDefSet& in);

  enum MeetOp {
    CD1toBOTTOM,CD2toBOTTOM,BOTHtoBOTTOM,CD1toVALUECD2,CD2toVALUECD1,NOTHING
  };

  //! Decides meet operation for two ConstDefs that are mustLocs
  MeetOp getMustMeetOp(OA_ptr<ConstDef> cd1, OA_ptr<ConstDef> cd2);

  //! Decides meet operation for two ConstDefs that are may(not must)Locs
  MeetOp getMayOnlyMeetOp(OA_ptr<ConstDef> cd1, OA_ptr<ConstDef> cd2);

private: // member variables

  OA_ptr<ReachConstsIRInterface> mIR;
  ProcHandle mProc;
  OA_ptr<Alias::Interface> mAlias;
  std::map<StmtHandle,std::set<OA_ptr<Location> > > mStmtMayDefMap;
  std::map<StmtHandle,std::set<OA_ptr<Location> > > mStmtMustDefMap;
  OA_ptr<ConstDefSet> mAllTop;
  OA_ptr<ConstDefSet> mAllBottom;
  OA_ptr<CFG::CFGInterface> mCFG;
  OA_ptr<SideEffect::InterSideEffectInterface> mInterSE;
  OA_ptr<ReachConstsStandard> mRCS;
 // Added by PLM 07/26/06
  OA_ptr<DataFlow::CFGDFSolver> mSolver;
 
   };

  } // end of ReachConsts namespace
} // end of OA namespace

#endif
