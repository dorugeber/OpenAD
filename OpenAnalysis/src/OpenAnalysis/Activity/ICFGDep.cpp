/*! \file
  
  \brief Implementation of ICFGDep

  \author Michelle Strout
  \version $Id: ICFGDep.cpp,v 1.5 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ICFGDep.hpp"
#include <Utils/Util.hpp>

namespace OA {
  namespace Activity {

static bool debug = false;


//*****************************************************************
// Interface Implementation
//*****************************************************************
    
/*!
   Return an iterator over all locations whose definition may 
   depend on the given use location.
*/
OA_ptr<LocIterator> 
ICFGDep::getMayDefIterator(StmtHandle stmt, const OA_ptr<Location> use)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ICFGDep:ALL", debug);
    if (mDepDFSet[stmt].ptrEqual(0)) {
        mDepDFSet[stmt] = new DepDFSet;
    }
    return mDepDFSet[stmt]->getDefsIterator(use);
}

/*!
   Return an iterator over all locations that are differentiable
   locations used in the possible definition of the given location

   For now assuming that all defs depend on all uses.
*/
OA_ptr<LocIterator> 
ICFGDep::getDiffUseIterator(StmtHandle stmt, OA_ptr<Location> def)
{

    if (mDepDFSet[stmt].ptrEqual(0)) {
        mDepDFSet[stmt] = new DepDFSet;
    }
    if (debug) {
        mDepDFSet[stmt]->dump(std::cout);
    }
    return mDepDFSet[stmt]->getUsesIterator(def);
}
    
//! Return an iterator over all locations that are definitely
//! defined in the given stmt
OA_ptr<LocIterator> ICFGDep::getMustDefIterator(StmtHandle stmt)
{
  OA_ptr<LocSetIterator> retval;
  if (mMustDefMap[stmt].ptrEqual(0)) {
    OA_ptr<LocSet> emptySet; emptySet = new LocSet;
    retval = new LocSetIterator(emptySet);  
  } else {
    retval = new LocSetIterator(mMustDefMap[stmt]); 
  }
  return retval;
}


//*****************************************************************
// Construction methods
//*****************************************************************

//! Insert use,def dependence pair
void ICFGDep::insertDepForStmt(StmtHandle stmt, 
                                   OA_ptr<Location> use,
                                   OA_ptr<Location> def)
{
    assert(!use.ptrEqual(0));
    assert(!def.ptrEqual(0));

    // first make sure there is a DepDFSet for the given stmt
    if (mDepDFSet[stmt].ptrEqual(0)) {
        mDepDFSet[stmt] = new DepDFSet;
    }

    // then insert the dependence
    mDepDFSet[stmt]->insertDep(use,def);
}

//! Insert must def location 
void ICFGDep::insertMustDefForStmt(StmtHandle stmt, 
                                       OA_ptr<Location> def)
{
    if (mMustDefMap[stmt].ptrEqual(0)) {
        mMustDefMap[stmt] = new LocSet;
    }
    mMustDefMap[stmt]->insert(def);
 }
 
//*****************************************************************
// Output
//*****************************************************************
void ICFGDep::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    std::cout << "====================== Dep" << std::endl;

    OA_ptr<LocIterator> locIterPtr;
    std::map<StmtHandle,OA_ptr<LocSet> >::iterator mapIter;

    std::cout << "MustDefMap = " << std::endl;
    for (mapIter=mMustDefMap.begin(); mapIter!=mMustDefMap.end(); mapIter++) {
        std::cout << "\tstmt = " << ir->toString(mapIter->first) << std::endl;
        locIterPtr = getMustDefIterator(mapIter->first);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
            std::cout << "\t\t";
            locIterPtr->current()->dump(std::cout,ir);
            std::cout << std::endl;
        }
    }
    std::cout << "DepDFSets = " << std::endl;
    std::map<StmtHandle,OA_ptr<DepDFSet> >::iterator mapIter2;
    for (mapIter2=mDepDFSet.begin(); mapIter2!=mDepDFSet.end(); mapIter2++) {
        std::cout << "\tstmt = " << ir->toString(mapIter2->first) << std::endl;
        if (!mDepDFSet[mapIter2->first].ptrEqual(NULL)) {
            OA_ptr<DepDFSet> depDFSet = mapIter2->second;
            depDFSet->dump(os,ir);
        }
    }

    std::cout << std::endl;
}

void ICFGDep::output(OA::IRHandlesIRInterface& ir)
{

  // output MustDefMap
  sOutBuild->mapStart("MustDefMap", "StmtHandle", "LocSet");

  OA_ptr<LocIterator> locIterPtr;
  std::map<StmtHandle,OA_ptr<LocSet> >::iterator mapIter;

  for (mapIter=mMustDefMap.begin(); mapIter!=mMustDefMap.end(); mapIter++) {

    StmtHandle stmt = mapIter->first;
    if (stmt == StmtHandle(0)) { continue; }

    sOutBuild->mapEntryStart();
    sOutBuild->mapKey(ir.toString(stmt));
    sOutBuild->mapValueStart();

    locIterPtr = getMustDefIterator(mapIter->first);
    if (locIterPtr->isValid()) {
 
      sOutBuild->listStart();
      for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
        sOutBuild->listItemStart();
        
        OA_ptr<Location> loc;
        loc = locIterPtr->current();
        loc->output(ir);

        sOutBuild->listItemEnd();

      }
      sOutBuild->listEnd();

    }
    // removing a level of indent manually to accommodate a map Value
    // that has a list of objects
    ostringstream correct;
    correct << popIndt;
    sOutBuild->outputString( correct.str() );

    sOutBuild->mapValueEnd();

  }

  sOutBuild->mapEnd("MustDefMap");


  // output DepDFSets
  sOutBuild->mapStart("mDepDFSet", "StmtHandle", "DepDFSet");
  
  std::map<StmtHandle,OA_ptr<DepDFSet> >::iterator mapIter2;

  for (mapIter2=mDepDFSet.begin(); mapIter2!=mDepDFSet.end(); mapIter2++) {
    StmtHandle stmt = mapIter2->first;
    if (stmt == StmtHandle(0)) { continue; }

    sOutBuild->mapEntryStart();
    sOutBuild->mapKeyStart();

    ostringstream stmtlabel;
    stmtlabel << "stmt: " << ir.toString(stmt);
    sOutBuild->outputString( stmtlabel.str() );
    
    sOutBuild->mapKeyEnd();
    sOutBuild->mapValueStart();

    ostringstream indentMapValue;
    indentMapValue << pushIndt;
    sOutBuild->outputString( indentMapValue.str() );

    OA_ptr<DepDFSet> depDFSet = mapIter2->second;
    if (!depDFSet.ptrEqual(NULL)) {
      depDFSet->output(ir); 
    }
    // removing two levels of indent manually to accommodate a map Value
    // that has a list of objects manually indented
    ostringstream correct;
    correct << popIndt << popIndt;
    sOutBuild->outputString( correct.str() );

    sOutBuild->mapValueEnd();

  }
  sOutBuild->mapEnd("DepDFSets");

}

  } // end of Activity namespace
} // end of OA namespace
