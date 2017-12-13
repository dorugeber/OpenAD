/*! \file
  
  \brief Implementation of Activity::ActiveStandard

  \author Michelle Strout
  \version $Id: ActiveStandard.cpp,v 1.9 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ActiveStandard.hpp"

namespace OA {
  namespace Activity {

ActiveStandard::ActiveStandard(ProcHandle p) 
{ mActiveStmtSet = new std::set<StmtHandle>;
  mActiveMemRefSet = new std::set<MemRefHandle>;
  mActiveLocSet = new DataFlow::LocDFSet;
  mActiveSymSet = new std::set<SymHandle>;
  mUnknownLocActive = false;
}

//! Return an iterator for set of active locations
OA_ptr<LocIterator> ActiveStandard::getActiveLocsIterator() 
{ OA_ptr<LocIterator> retval;
  retval = new DataFlow::LocDFSetIterator(*mActiveLocSet);
  return retval;
}

//! Return an iterator for set of active stmts
OA_ptr<StmtHandleIterator> ActiveStandard::getActiveStmtIterator() 
{
  OA_ptr<StmtHandleIterator> retval;
  retval = new ActiveStmtIterator(mActiveStmtSet);
  return retval;
}

//! Return an iterator for set of active memory references
OA_ptr<MemRefHandleIterator> ActiveStandard::getActiveMemRefIterator() 
{
  OA_ptr<MemRefHandleIterator> retval;
  retval = new ActiveMemRefIterator(mActiveMemRefSet);
  return retval;
}


//! Return an iterator for set of active symbols
OA_ptr<SymHandleIterator> ActiveStandard::getActiveSymIterator() 
{
  OA_ptr<SymHandleIterator> retval;
  retval = new ActiveSymIterator(mActiveSymSet);
  return retval;
}



//! Indicate whether the given sym is active or not
bool ActiveStandard::isActive(SymHandle sym)
{
  // an unknown location is active, therefore all symbols are active
  if (mUnknownLocActive) {
    return true;
  } else if (mActiveSymSet->find(sym) != mActiveSymSet->end()) {
    return true;
  } else {
    return false;
  }  
}

//! Indicate whether the given stmt is active or not
bool ActiveStandard::isActive(StmtHandle stmt)
{
  if (mActiveStmtSet->find(stmt) != mActiveStmtSet->end()) {
    return true;
  } else {
    return false;
  }  
}

//! Indicate whether the given memref is active or not
bool ActiveStandard::isActive(MemRefHandle memref)
{
  if (mActiveMemRefSet->find(memref) != mActiveMemRefSet->end()) {
    return true;
  } else {
    return false;
  }  
}

//*****************************************************************
// Construction methods 
//*****************************************************************

//! insert an active location
void ActiveStandard::insertLoc(OA_ptr<Location> loc) 
{ 
    // We only need Active Locations
    // code for getting Active Symbols is deprecated.

    mActiveLocSet->insert(loc); 

    if (loc->isaSubSet()) {
      OA_ptr<Location> baseLoc = loc->getBaseLoc();
      insertLoc(baseLoc);

    }
}

//! insert an active Stmt
void ActiveStandard::insertStmt(StmtHandle stmt) 
{ mActiveStmtSet->insert(stmt); }

//! insert an active MemRef
void ActiveStandard::insertMemRef(MemRefHandle memref) 
{ mActiveMemRefSet->insert(memref); }



//*****************************************************************
// Output
//*****************************************************************

//! incomplete output of info for debugging
void ActiveStandard::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    os << "ActiveLocSet = " << std::endl;
    mActiveLocSet->dump(os,ir);

    os << std::endl << "ActiveSymSet = " << std::endl;
    OA_ptr<SymHandleIterator> symIterPtr;
    symIterPtr = new ActiveSymIterator(mActiveSymSet);
    for ( ; symIterPtr->isValid(); (*symIterPtr)++ ) {
        os << ir->toString(symIterPtr->current()) << std::endl;
    }
    os << "mUnknownLocActive = " << mUnknownLocActive << std::endl;

    os << std::endl << "ActiveStmtList = " << std::endl;
    OA_ptr<StmtHandleIterator> stmtIterPtr = getActiveStmtIterator();
    for ( ; stmtIterPtr->isValid(); (*stmtIterPtr)++ ) {
        os << ir->toString(stmtIterPtr->current()) << std::endl;
    }

    os << std::endl << "ActiveMemRefList = " << std::endl;
    OA_ptr<MemRefHandleIterator> memrefIterPtr = getActiveMemRefIterator();
    for ( ; memrefIterPtr->isValid(); (*memrefIterPtr)++ ) {
        os << ir->toString(memrefIterPtr->current()) << std::endl;
    }

}


  } // end of Activity namespace
} // end of OA namespace
