// LivenessStandard.hpp

#ifndef LivenessStandard_hpp
#define LivenessStandard_hpp

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Liveness/Interface.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>
#include <OpenAnalysis/Utils/GenOutputTool.hpp>
#include <OpenAnalysis/Location/Location.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>
#include <OpenAnalysis/Utils/Util.hpp>

namespace OA {
  namespace Liveness {



class LivenessStandard : public virtual Annotation 
{
  public:
    LivenessStandard(ProcHandle p){} 
    ~LivenessStandard() {}

    OA_ptr<LocIterator> getLivenessIterator(StmtHandle s); 

    OA_ptr<LocIterator> getExitLivenessIterator(); 
// TODO
 //    Live will be a set of locations
    void insertLive(StmtHandle s, OA_ptr<Location> Live)
    { 
      if(mLiveness[s].ptrEqual(0)) {
        mLiveness[s] = new LocSet;
      }
      mLiveness[s]->insert(Live);    
    }



     void removeLive(StmtHandle s, OA_ptr<Location> Live)
    {
      if(mLiveness[s].ptrEqual(0)) {
        mLiveness[s] = new LocSet;
      }
      mLiveness[s]->erase(Live);
    }


    void insertExitLive(OA_ptr<Location> Live)
    { 
      mExitLiveness->insert(Live); 
    }


    void output(IRHandlesIRInterface& pIR);

    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

  private:
    std::map<StmtHandle,OA_ptr<LocSet> > mLiveness;
    OA_ptr<LocSet> mExitLiveness;
}; 

  } // end of Liveness namespace
} // end of OA namespace

#endif

