/*! \file
  
  \brief Implementation of XAIF::AliasMapXAIF

  \author Michelle Strout
  \version $Id: AliasMapXAIF.cpp,v 1.5 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "AliasMapXAIF.hpp"

namespace OA {
  namespace XAIF {

    static bool debug = false;

    AliasMapXAIF::AliasMapXAIF(ProcHandle p) : mProcHandle(p) {}

    //! get iterator over all memory references that information is available for
    OA_ptr<MemRefHandleIterator> AliasMapXAIF::getMemRefIter() {
      // create an empty MemRefSet that will be given to iterator
      OA_ptr<MemRefSet> memrefSet; memrefSet = new MemRefSet;

      // put all memory references that we know about into the set
      std::map<MemRefHandle,int>::iterator mapIter;
      for (mapIter=mMemRefToIdMap.begin(); mapIter!=mMemRefToIdMap.end(); mapIter++) {
        memrefSet->insert(mapIter->first);
      }

      OA_ptr<MemRefHandleIterator> retIter; 
      retIter = new Alias::AliasMapMemRefIter(memrefSet);
      return retIter;
    } // end AliasMapXAIF::getMemRefIter()

    int AliasMapXAIF::getMapSetId(MemRefHandle ref) {
      std::map<MemRefHandle,int>::iterator pos;
      pos = mMemRefToIdMap.find(ref);
      if (pos != mMemRefToIdMap.end()) {
	return pos->second;
      }
      else {
	return 0;
      }
    } // end AliasMapXAIF::getMapSetId(MemRefHandle ref)

    int AliasMapXAIF::findMapSet(MemRefHandle ref) {
      std::map<MemRefHandle,int>::iterator pos;
      pos = mMemRefToIdMap.find(ref);
      if (pos != mMemRefToIdMap.end()) {
	return pos->second;
      }
      else {
	return AliasMapXAIF::SET_ID_NONE;
      }
    } // end AliasMapXAIF::findMapSet(MemRefHandle ref)

    int AliasMapXAIF::findMapSet(OA_ptr<std::set<LocTuple> > pLocTupleSet) {
      int retval = SET_ID_NONE;
      // loop through location sets and compare them to given location set
      std::map<int,OA_ptr<std::set<LocTuple> > >::iterator mapIter;
      for (mapIter=mIdToLocTupleSetMap.begin(); mapIter!=mIdToLocTupleSetMap.end(); mapIter++) {
	OA_ptr<std::set<LocTuple> > mapLocTupleSet = mapIter->second;
	if (pLocTupleSet == mapLocTupleSet) {
	  return mapIter->first;
	}
      }
      return retval;
    } // end AliasMapXAIF::findMapSet(OA_ptr<std::set<LocTuple> > pLocTupleSet)

    OA_ptr<IdIterator> AliasMapXAIF::getIdIterator() {
      OA_ptr<IdIterator> retval;
      retval = new IdIterator(mIdToLocTupleSetMap);
      return retval;
    } // end AliasMapXAIF::getIdIterator()

    //! get iterator over all locations in a particular set
    OA_ptr<LocTupleIterator> AliasMapXAIF::getLocIterator(int setId) {
      OA_ptr<LocTupleIterator> retval;
      retval = new LocTupleIterator(*mIdToLocTupleSetMap[setId]); 
      return retval;
    } // end AliasMapXAIF::getLocIterator()

    //! associate the given location set with the given mapSet
    void AliasMapXAIF::mapLocTupleSet(OA_ptr<std::set<LocTuple> > ltSet, int setId) {
      mIdToLocTupleSetMap[setId] = ltSet;
    } // end AliasMapXAIF::mapLocTupleSet()

    //! associate a MemRefHandle with the given mapset, means that the MemRefHandle can access
    //! all of the locations in the mapset, if only one full location then is a must access
    void AliasMapXAIF::mapMemRefToMapSet(MemRefHandle ref, int setId) {
      // if this memref is already pointed to one mapset then need
      // to take it out of mIdToMemRefSetMap for the current set id
      int currSetId = findMapSet(ref);
      if (currSetId != AliasMapXAIF::SET_ID_NONE) {
	mIdToMemRefSetMap[currSetId].erase(ref);
      } 
      mMemRefToIdMap[ref] = setId;
      mIdToMemRefSetMap[setId].insert(ref);
    } // end AliasMapXAIF::mapMemRefToMapSet()

    //! output for regression tests
    void AliasMapXAIF::output(IRHandlesIRInterface& ir) {
      sOutBuild->objStart("AliasMapXAIF");
      sOutBuild->outputIRHandle(mProcHandle, ir);

      // print locations for each set, ID : { LocTuple }
      sOutBuild->mapStart("mIdToLocTupleSetMap", "int", "OA_ptr<std::set<LocTuple> >");

      for (OA_ptr<IdIterator> idIterPtr = getIdIterator(); idIterPtr->isValid(); ++(*idIterPtr) ) {
	//os << "AliasMapSet[" << i << "] = { ";
	int i = idIterPtr->current();
	sOutBuild->mapEntryStart();
	sOutBuild->mapKey(OA::int2string(i));
	sOutBuild->mapValueStart();
	sOutBuild->listStart();
	for (OA_ptr<LocTupleIterator> locIterPtr = getLocIterator(i); locIterPtr->isValid(); ++(*locIterPtr) ) {
	  LocTuple loc = locIterPtr->current();
	  sOutBuild->listItemStart();
	  loc.output(ir);
	  sOutBuild->listItemEnd();
	}
	sOutBuild->listEnd();
	sOutBuild->mapValueEnd();
	sOutBuild->mapEntryEnd();
      }
      sOutBuild->mapEnd("mIdToLocTupleSetMap");

      // print all memrefs and their mapping
      sOutBuild->mapStart("mMemRefToIdMap", "MemRefHandle", "int");
      for (OA_ptr<MemRefHandleIterator> memIterPtr = getMemRefIter(); memIterPtr->isValid(); (*memIterPtr)++) {
	MemRefHandle memref = memIterPtr->current();
	sOutBuild->mapEntryStart();
	sOutBuild->mapKeyStart();
	sOutBuild->outputIRHandle(memref, ir);
	sOutBuild->mapKeyEnd();
	sOutBuild->mapValue(OA::int2string(findMapSet(memref)));
	sOutBuild->mapEntryEnd();
      }
      sOutBuild->mapEnd("mMemRefToIdMap");
      sOutBuild->objEnd("AliasMap");
    } // end AliasMapXAIF::output()

    //! incomplete output of info for debugging 
    void AliasMapXAIF::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir) {
      // print locations for each set, ID : { LocTuple }
      os << "============= AliasMapXAIF ============" << std::endl;
      OA_ptr<IdIterator> idIterPtr = getIdIterator();
      for ( ; idIterPtr->isValid(); ++(*idIterPtr) ) {
	int i = idIterPtr->current();
	os << "AliasMapSet[" << i << "] = { ";
	for (OA_ptr<LocTupleIterator> locIterPtr = getLocIterator(i); locIterPtr->isValid(); ++(*locIterPtr) ) {
	  LocTuple loc = locIterPtr->current();
	  os << ", ";
	  os << "< " << loc.getLocRange().getStart() << "..";
	  os << loc.getLocRange().getEnd() << ", " << loc.isFull();
	  os << " > ";
	}
	os << " }" << std::endl;
      }

      // print all memrefs and their mapping
      os << "MemRef mapping to AliasMap sets:" << std::endl;
      for (OA_ptr<MemRefHandleIterator> memIterPtr = getMemRefIter(); memIterPtr->isValid(); (*memIterPtr)++) {
	MemRefHandle memref = memIterPtr->current();
	os << "(" << memref << ") " << ir->toString(memref);
	os << " --> " << findMapSet(memref) << std::endl;
      }
    } // end AliasMapXAIF::dump()

  } // end of Alias namespace
} // end of OA namespace
