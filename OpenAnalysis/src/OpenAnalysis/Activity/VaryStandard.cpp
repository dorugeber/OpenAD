/*! \file
  
  \brief Implementation of Activity::VaryStandard

  \author Michelle Strout
  \version $Id: VaryStandard.cpp,v 1.6 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "VaryStandard.hpp"

namespace OA {
  namespace Activity {

VaryStandard::VaryStandard(ProcHandle p) 
{ 
    mIndepLocSet = new DataFlow::LocDFSet; 
}

OA_ptr<LocIterator> VaryStandard::getIndepSetIterator() 
{ 
  OA_ptr<LocIterator> retval;
  retval = new DataFlow::LocDFSetIterator(*mIndepLocSet);
  return retval;
}

OA_ptr<LocIterator> VaryStandard::getOutVaryIterator(StmtHandle s)
{ 
    OA_ptr<LocIterator> retval;
    if (mOutVary[s].ptrEqual(0)) {
        mOutVary[s] = new DataFlow::LocDFSet;
    }
    retval = new DataFlow::LocDFSetIterator(*mOutVary[s]);
    return retval;
}

OA_ptr<DataFlow::LocDFSet> VaryStandard::getInVarySet(ExprHandle call)
{
    if (mInVary.find(call)!=mInVary.end()) {
        return mInVary[call];
    } else {
        OA_ptr<DataFlow::LocDFSet> emptySet; 
        emptySet = new DataFlow::LocDFSet;
        return emptySet;
    }
}

//*****************************************************************
// Construction methods 
//*****************************************************************

void VaryStandard::insertIndepLoc(OA_ptr<Location> loc) 
{ 
    mIndepLocSet->insert(loc); 
}

void VaryStandard::copyIntoOutVary(StmtHandle s, 
                                   OA_ptr<DataFlow::LocDFSet> dfset)
{
    OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
    mOutVary[s] = cloneDFset.convert<DataFlow::LocDFSet>();
}

void VaryStandard::copyIntoInVary(ExprHandle call, 
                                  OA_ptr<DataFlow::LocDFSet> dfset)
{
    OA_ptr<DataFlow::DataFlowSet> cloneDFset = dfset->clone();
    mInVary[call] = cloneDFset.convert<DataFlow::LocDFSet>();
}

   
//*****************************************************************
// Output
//*****************************************************************

//! incomplete output of info for debugging
void VaryStandard::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    os << "============================ VaryStandard" << std::endl;
    os << "IndepSet = " << std::endl;
    mIndepLocSet->dump(os,ir);

    // iterate over all stmts we know about
    std::map<StmtHandle, 
             OA_ptr<DataFlow::LocDFSet> >::iterator mapIter;
    for (mapIter = mOutVary.begin(); mapIter != mOutVary.end(); mapIter++) {
        StmtHandle s = mapIter->first;

        os << "StmtHandle(" << s.hval() << ") " << ir->toString(s) 
           << std::endl << "\tOutVary: " << std::endl;
        mapIter->second->dump(os,ir);
    }
}



  } // end of Activity namespace
} // end of OA namespace
