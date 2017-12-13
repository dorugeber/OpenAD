/*! \file
  
  \brief Maps StmtHandles to set of locations that are in the 
         set InUseful based on a set of dependent location.

  \authors Michelle Strout
  \version $Id: UsefulStandard.hpp,v 1.8 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef UsefulStandard_hpp
#define UsefulStandard_hpp

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/ReachDefs/Interface.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>

namespace OA {
  namespace Activity {



    class UsefulStandard : public virtual Annotation {
  public:
    UsefulStandard(ProcHandle p); 
    ~UsefulStandard() {}

    //! Return an iterator for set of dependent locations
    //! the analysis calculated InUseful from
    OA_ptr<LocIterator> getDepSetIterator(); 

    //! Return an iterator that will iterate over the set of locations
    //! that are in InUseful set for stmt (above the stmt, code-wise)
    OA_ptr<LocIterator> getInUsefulIterator(StmtHandle s);

    //! Return an iterator that will iterate over the set of locations
    //! that are in OutUseful set for stmt (below the stmt, code-wise)
    OA_ptr<LocIterator> getOutUsefulIterator(StmtHandle s);

    //OA_ptr<DataFlow::LocDFSet> getFinalUseful();

    //! access the number of iterations over all nodes for Useful
    int getNumIter() { return mNumIter; }

    //*****************************************************************
    // Construction methods 
    //*****************************************************************

    //! insert an dependent location
    void insertDepLoc(OA_ptr<Location> loc) 
    { mDepLocSet->insert(loc); }

    //! copy into the InUseful set for a statement
    void copyIntoInUseful(StmtHandle s, 
                          OA_ptr<DataFlow::LocDFSet> dfset)
    {
      OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
      mInUseful[s] = cloneDFset.convert<DataFlow::LocDFSet>();
    }

    //! copy into the OutUseful set for a call
    void copyIntoOutUseful(StmtHandle s, 
                           OA_ptr<DataFlow::LocDFSet> dfset)
    {
      OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
      mOutUseful[s] = cloneDFset.convert<DataFlow::LocDFSet>();
    }

    //! copy into the CallOutUseful set for a call
    void copyIntoCallOutUseful(CallHandle c,
                               OA_ptr<DataFlow::LocDFSet> dfset)
    {
      OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
      mCallOutUseful[c] = cloneDFset.convert<DataFlow::LocDFSet>();
    }

    //! set the final Useful or InUseful for the entry node of proc
    //void mapFinalUseful(OA_ptr<DataFlow::LocDFSet> final);

    //!  set the number of iterations over all nodes for Useful
    void setNumIter(int n) { mNumIter = n; }

    //! attach UsefulStandard to procedure
    //void mapProcToUseful(ProcHandle proc, OA_ptr<UsefulStandard> useful)
    //             { mProcToUsefulMap[proc] = useful; }
      
    //*****************************************************************
    // Output
    //*****************************************************************

    //! incomplete output of info for debugging, just lists stmts
    //! and associated set of InUseful locations
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface &ir);

  private:
    // data members
    std::map<StmtHandle,OA_ptr<DataFlow::LocDFSet> >  mInUseful;
    std::map<StmtHandle,OA_ptr<DataFlow::LocDFSet> >  mOutUseful;
    std::map<CallHandle,OA_ptr<DataFlow::LocDFSet> >  mCallOutUseful;
    OA_ptr<DataFlow::LocDFSet>  mDepLocSet;
    //OA_ptr<DataFlow::LocDFSet>  mFinalInUseful;
    //std::map<ProcHandle,OA_ptr<UsefulStandard> > mProcToUsefulMap;
    int mNumIter;

};

  } // end of Activity namespace
} // end of OA namespace

#endif

