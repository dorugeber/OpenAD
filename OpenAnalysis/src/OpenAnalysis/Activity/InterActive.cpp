/*! \file
  
  \brief Implementation of InterActive

  \author Michelle Strout
  \version $Id: InterActive.cpp,v 1.7 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "InterActive.hpp"

namespace OA {
  namespace Activity {

InterActive::InterActive()
{
    mUnknownLocActive = false;
    mNumIterUseful = 0;
    mNumIterVary = 0;
    mSizeInBytes = 0;
}

//! Return an iterator over all procedures for which activity
//! analysis is available
OA_ptr<ProcHandleIterator> InterActive::getKnownProcIterator()
{         
    // create a procSet
    OA_ptr<std::set<ProcHandle> > procSet;
    procSet = new std::set<ProcHandle>;
    
    // loop through map to add to list of ProcHandle's
    std::map<ProcHandle,OA_ptr<Activity::ActiveStandard> >::iterator mIter;
    for (mIter=mProcToActiveMap.begin(); mIter != mProcToActiveMap.end(); mIter++)
    {
        procSet->insert(mIter->first);
    }

    OA_ptr<ActivityProcIter> retval;
    retval = new ActivityProcIter(procSet);
    return retval;
}

//! Indicate whether the given stmt is active or not within given proc
bool InterActive::isActive(ProcHandle proc, StmtHandle stmt)
{
    if (mProcToActiveMap[proc].ptrEqual(0)) {
      assert(0);  // we don't know about this proc
    } else {
      return mProcToActiveMap[proc]->isActive(stmt);
    }
    return false; // default return to make compilers happy
}

//! Indicate whether the given memref is active or not within given proc
bool InterActive::isActive(ProcHandle proc, MemRefHandle memref)
{
    if (mProcToActiveMap[proc].ptrEqual(0)) {
      assert(0);  // we don't know about this proc
    } else {
      return mProcToActiveMap[proc]->isActive(memref);
    }
    return false; // default return to make compilers happy
}


OA_ptr<LocIterator> 
InterActive::getActiveLocsIterator(ProcHandle proc)
{
    OA_ptr<LocIterator> retval;

    if (mProcToActiveMap[proc].ptrEqual(0)) {
      assert(0);  // we don't know about this proc
    } else {
      retval = mProcToActiveMap[proc]->getActiveLocsIterator();
    }

    return retval;
}

OA_ptr<StmtHandleIterator> 
InterActive::getActiveStmtIterator(ProcHandle proc)
{
    OA_ptr<StmtHandleIterator> retval;

    if (mProcToActiveMap[proc].ptrEqual(0)) {
      assert(0);  // we don't know about this proc
    } else {
      retval = mProcToActiveMap[proc]->getActiveStmtIterator();
    }

    return retval;
}

OA_ptr<MemRefHandleIterator> 
InterActive::getActiveMemRefIterator(ProcHandle proc) 
{
    OA_ptr<MemRefHandleIterator> retval;

    if (mProcToActiveMap[proc].ptrEqual(0)) {
      assert(0);  // we don't know about this proc
    } else {
      retval = mProcToActiveMap[proc]->getActiveMemRefIterator();
    }

    return retval;
}

int InterActive::getNumIterUseful() 
{ 
  return mNumIterUseful; 
}

int InterActive::getNumIterVary()
{
  return mNumIterVary;
}

int InterActive::getNumIterActive()
{
  return mNumIterActive;
}

int InterActive::getActiveSizeInBytes()
{
  return mSizeInBytes;
}

// *****************************************************************
// Construction methods 
// *****************************************************************

//! Associate the given procedure with the given Activity info
void InterActive::mapProcToActive(ProcHandle proc, 
    OA_ptr<Activity::ActiveStandard> active)
{
    mProcToActiveMap[proc] = active;

    // if the procedure has an UnknownLoc that is active then must indicate
    // that for whole program
    if (active->getUnknownLocActive()==true) {
        mUnknownLocActive = true;
    }
}

void InterActive::setNumIterUseful(int n)
{
  mNumIterUseful = n;
}

void InterActive::setNumIterVary(int n)
{
  mNumIterVary = n;
}

void InterActive::setNumIterActive(int n)
{
  mNumIterActive = n;
}

void InterActive::setActiveSizeInBytes(int n)
{
  mSizeInBytes = n;
}


//*****************************************************************
// Annotation Interface
//*****************************************************************
void InterActive::output(IRHandlesIRInterface &ir){

  sOutBuild->objStart("InterActive"); {

    // Loop through all known procedures
    OA_ptr<ProcHandleIterator> procIterPtr = getKnownProcIterator();
    for (; procIterPtr->isValid(); (*procIterPtr)++) {
      ProcHandle proc = procIterPtr->current();
      ostringstream oss;
      oss << "Procedure( " << ir.toString(proc) << " )";
      sOutBuild->fieldStart(oss.str()); {

        sOutBuild->objStart("ActiveLocSet"); {
          sOutBuild->listStart(); {
            OA_ptr<LocIterator> locIterPtr;
            locIterPtr = getActiveLocsIterator(proc);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
              sOutBuild->listItemStart(); {
                (locIterPtr->current())->output(ir);
              }sOutBuild->listItemEnd();
            }
          }sOutBuild->listEnd();
        }sOutBuild->objEnd("ActiveLocSet");

        sOutBuild->objStart("ActiveStmtSet"); {
          sOutBuild->listStart(); {
            ostringstream oss;
            oss << indt;
            OA_ptr<StmtHandleIterator> stmtIterPtr;
            stmtIterPtr= getActiveStmtIterator(proc);
            for ( ; stmtIterPtr->isValid(); (*stmtIterPtr)++ ) {
              sOutBuild->outputString(oss.str());
              sOutBuild->listItemStart(); {
                sOutBuild->outputIRHandle(stmtIterPtr->current(),ir);
              }sOutBuild->listItemEnd();
            }
          }sOutBuild->listEnd();
        }sOutBuild->objEnd("ActiveStmtSet");

        sOutBuild->objStart("ActiveMemRefSet"); {
          sOutBuild->listStart(); {
            ostringstream oss;
            oss << indt;
            OA_ptr<MemRefHandleIterator> memrefIterPtr;
            memrefIterPtr = getActiveMemRefIterator(proc);
            for ( ; memrefIterPtr->isValid(); (*memrefIterPtr)++ ) {
              sOutBuild->outputString(oss.str());
              sOutBuild->listItemStart(); {
                sOutBuild->outputIRHandle(memrefIterPtr->current(),ir);
              }sOutBuild->listItemEnd();
            }
          }sOutBuild->listEnd();
        }sOutBuild->objEnd("ActiveMemRefSet");

      }sOutBuild->fieldEnd("Procedure");
    }

    sOutBuild->fieldStart("mNumIterUseful"); {
      ostringstream oss; oss << mNumIterUseful;
      sOutBuild->outputString(oss.str());
    }sOutBuild->fieldEnd("mNumIterUseful");
    sOutBuild->fieldStart("mNumIterVary"); {
      ostringstream oss; oss << mNumIterVary;
      sOutBuild->outputString(oss.str());
    }sOutBuild->fieldEnd("mNumIterVary");
    sOutBuild->fieldStart("mNumIterActive"); {
      ostringstream oss; oss << mNumIterActive;
      sOutBuild->outputString(oss.str());
    }sOutBuild->fieldEnd("mNumIterActive");
  } sOutBuild->objEnd("InterActive");
  
}

//! incomplete output of info for debugging
void InterActive::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    std::cout << "====================== InterActive" << std::endl;

    // Loop through all known procedures
    OA_ptr<ProcHandleIterator> procIterPtr = getKnownProcIterator();
    for (; procIterPtr->isValid(); (*procIterPtr)++) {
        ProcHandle proc = procIterPtr->current();

        // print all sets for each procedure
        os << "Procedure( " << ir->toString(proc) << " )" << std::endl;

        OA_ptr<LocIterator> locIterPtr;

        os << "\tActiveLocSet = ";
        locIterPtr = getActiveLocsIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        os << std::endl;

        os << "\tActiveStmtList = " << std::endl;
        OA_ptr<StmtHandleIterator> stmtIterPtr = getActiveStmtIterator(proc);
        for ( ; stmtIterPtr->isValid(); (*stmtIterPtr)++ ) {
            os << ir->toString(stmtIterPtr->current()) << std::endl;
        }
        os << std::endl;

        os << "\tActiveMemRefList = " << std::endl;
        OA_ptr<MemRefHandleIterator> memrefIterPtr = getActiveMemRefIterator(proc);
        for ( ; memrefIterPtr->isValid(); (*memrefIterPtr)++ ) {
            os << ir->toString(memrefIterPtr->current()) << std::endl;
        }
        os << std::endl;
    }

    os << "\tgetActiveSizeInBytes() = " << getActiveSizeInBytes() << std::endl;

}


  } // end of Activity namespace
} // end of OA namespace
