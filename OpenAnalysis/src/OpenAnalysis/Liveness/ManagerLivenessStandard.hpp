// ManagerLivenessStandard.hpp

#ifndef LivenessManagerStandard_h
#define LivenessManagerStandard_h

//--------------------------------------------------------------------
// OpenAnalysis headers
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/LivenessIRInterface.hpp>
#include <OpenAnalysis/Liveness/LivenessStandard.hpp>

#include <OpenAnalysis/Alias/Interface.hpp>

#include <OpenAnalysis/DataFlow/CFGDFProblem.hpp>
#include <OpenAnalysis/Location/Location.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>
#include <OpenAnalysis/SideEffect/InterSideEffectInterface.hpp>
#include <OpenAnalysis/DataFlow/CFGDFSolver.hpp>
#include <OpenAnalysis/CFG/CFGInterface.hpp>

namespace OA {
  namespace Liveness {


class ManagerLivenessStandard 
    : public DataFlow::CFGDFProblem { 
public:
  ManagerLivenessStandard(OA_ptr<LivenessIRInterface> _ir);
  ~ManagerLivenessStandard () {}

  OA_ptr<LivenessStandard> performAnalysis(ProcHandle, 
        OA_ptr<CFG::CFGInterface> cfg, OA_ptr<Alias::Interface> alias, 
        OA_ptr<SideEffect::InterSideEffectInterface> interSE,
        DataFlow::DFPImplement algorithm);

private:
  OA_ptr<DataFlow::DataFlowSet> initializeTop();
  OA_ptr<DataFlow::DataFlowSet> initializeBottom();

  OA_ptr<DataFlow::DataFlowSet>
           initializeNodeIN(OA_ptr<CFG::NodeInterface> n);
  
  OA_ptr<DataFlow::DataFlowSet>
           initializeNodeOUT(OA_ptr<CFG::NodeInterface> n);
                     

  OA_ptr<DataFlow::DataFlowSet> 
  meet (OA_ptr<DataFlow::DataFlowSet> set1, OA_ptr<DataFlow::DataFlowSet> set2); 

  OA_ptr<DataFlow::DataFlowSet> 
  transfer(OA_ptr<DataFlow::DataFlowSet> in, OA::StmtHandle stmt); 

private: // member variables

  OA_ptr<LivenessIRInterface> mIR;
  OA_ptr<Alias::Interface> mAlias;
  OA_ptr<LivenessStandard> mLiveMap;
 // OA_ptr<LocIterator> mLocIter;
//  std::map<StmtHandle,std::set<OA_ptr<Location> > > mStmtMayDefMap;
//  std::map<StmtHandle,std::set<OA_ptr<Location> > > mStmtMustDefMap;
 std::map <StmtHandle, LocSet > mStmtMustDefMap; 
 std::map<StmtHandle, LocSet > mStmtMayUseMap;
  OA_ptr<DataFlow::CFGDFSolver> mSolver;
    
};

  } // end of Liveness namespace
} // end of OA namespace

#endif
