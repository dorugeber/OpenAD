/*! \file
  
  \brief Implementation of Activity::ActivePerStmt

  \author Michelle Strout
  \version $Id: ActivePerStmt.cpp,v 1.1 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ActivePerStmt.hpp"

namespace OA {
  namespace Activity {

ActivePerStmt::ActivePerStmt() 
{ 
  mNumIter = 0;
}


OA_ptr<DataFlow::LocDFSet> ActivePerStmt::getInActiveSet(StmtHandle stmt)
{
    if (mInActive.find(stmt)!=mInActive.end()) {
        return mInActive[stmt];
    } else {
        OA_ptr<DataFlow::LocDFSet> emptySet; 
        emptySet = new DataFlow::LocDFSet;
        return emptySet;
    }
}

OA_ptr<DataFlow::LocDFSet> ActivePerStmt::getOutActiveSet(StmtHandle stmt)
{
    if (mOutActive.find(stmt)!=mOutActive.end()) {
        return mOutActive[stmt];
    } else {
        OA_ptr<DataFlow::LocDFSet> emptySet; 
        emptySet = new DataFlow::LocDFSet;
        return emptySet;
    }
}

//*****************************************************************
// Construction methods 
//*****************************************************************

void ActivePerStmt::copyIntoInActive(StmtHandle s, 
                                   OA_ptr<DataFlow::LocDFSet> dfset)
{
    OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
    mInActive[s] = cloneDFset.convert<DataFlow::LocDFSet>();
}

void ActivePerStmt::copyIntoOutActive(StmtHandle s, 
                                   OA_ptr<DataFlow::LocDFSet> dfset)
{
    OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
    mOutActive[s] = cloneDFset.convert<DataFlow::LocDFSet>();
}


//*****************************************************************
// Annotation Interface
//*****************************************************************
void ActivePerStmt::output(IRHandlesIRInterface &ir)
{
  sOutBuild->mapStart("ActiveSets","StmtHandle","ActiveLocDFSet"); {
    std::map<StmtHandle, OA_ptr<DataFlow::LocDFSet> >::iterator mapIter;
    for (mapIter = mInActive.begin(); mapIter != mInActive.end(); mapIter++) {
      StmtHandle stmt = mapIter->first;
      OA_ptr<DataFlow::LocDFSet> inActiveSet = mapIter->second;
      OA_ptr<DataFlow::LocDFSet> outActiveSet = mOutActive[stmt];
      if (outActiveSet.ptrEqual(0)) { 
        outActiveSet = new DataFlow::LocDFSet();
      }
      sOutBuild->mapEntryStart(); {
        sOutBuild->mapKeyStart(); {
          sOutBuild->outputIRHandle(stmt,ir);
        } sOutBuild->mapKeyEnd();
        sOutBuild->mapValueStart(); {
          sOutBuild->objStart("InActiveSet"); {
            inActiveSet->output(ir);
          } sOutBuild->objEnd("InActiveSet");
          sOutBuild->objStart("OutActiveSet"); {
            outActiveSet->output(ir);
          } sOutBuild->objEnd("OutActiveSet");
        } sOutBuild->mapValueEnd();
      } sOutBuild->mapEntryEnd();
    }
  } sOutBuild->mapEnd("ActiveSets");
}

//*****************************************************************
// Output
//*****************************************************************

//! incomplete output of info for debugging
void ActivePerStmt::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    os << "============================ ActivePerStmt" << std::endl;

    // iterate over all stmts we know about
    std::map<StmtHandle, 
             OA_ptr<DataFlow::LocDFSet> >::iterator mapIter;
    for (mapIter = mInActive.begin(); mapIter != mInActive.end(); mapIter++) {
        StmtHandle s = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> inActSet = mapIter->second;
        OA_ptr<DataFlow::LocDFSet> outActSet = mOutActive[s];
        if (outActSet.ptrEqual(0)) {
          outActSet = new DataFlow::LocDFSet();
        }

        os << "StmtHandle(" << ir->toString(s) << ")" 
           << std::endl << "\tInActive: " << std::endl;
        inActSet->dump(os,ir);
        os << "\tOutActive: " << std::endl;
        outActSet->dump(os,ir);
    }
}



  } // end of Activity namespace
} // end of OA namespace
