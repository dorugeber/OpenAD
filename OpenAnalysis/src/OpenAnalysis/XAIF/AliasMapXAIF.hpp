/*! \file
  
  \brief Maps MemRefHandle's to a particular alias map set.

  \authors Michelle Strout
  \version $Id: AliasMapXAIF.hpp,v 1.6 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>


  Each map set has an id and a set of virtual storage locations 
  that memory reference in the set might reference.
  The virtual locations that XAIF expects are tuples where the first
  entry is a numeric range and the second entry is a bit indicating
  whether the range is fully or partially accessed.
*/

#ifndef AliasMapXAIF_H
#define AliasMapXAIF_H

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/Alias/AliasMap.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExpr.hpp>
#include <OpenAnalysis/Location/Locations.hpp>

#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>

namespace OA {
  namespace XAIF {

typedef std::set<MemRefHandle> MemRefSet;
//typedef std::set<OA_ptr<Location::Location> > LocSet;

//! Numeric range to indicate a number of locations in an array
class LocRange : public virtual Annotation {
  public:
    LocRange(int start, int end) : mStart(start), mEnd(end) {}
    ~LocRange() {}

    int getStart() const { return mStart; }
    int getEnd() const { return mEnd; }

    void output(IRHandlesIRInterface& ir)
    {
        sOutBuild->objStart("LocRange");
        sOutBuild->field("mStart", OA::int2string(mStart) );
        sOutBuild->field("mEnd", OA::int2string(mEnd) );
        sOutBuild->objEnd("LocRange");
    }
  private:
    int mStart, mEnd;
};

//! A range and a bit to indicate whether the access is to the full range
//! or just some unknown part of the range
class LocTuple : public virtual Annotation {
  public:
    LocTuple() : mRange(0,0), mFullOverlap(false) {}
    LocTuple(int start, int end, bool fullOverlap) 
        : mRange(start,end), mFullOverlap(fullOverlap) {}
    ~LocTuple() {}

    LocRange getLocRange() const { return mRange; }
    bool isFull() const { return mFullOverlap; }

    bool operator==(const LocTuple& other) const
    { if (mRange.getStart() == other.getLocRange().getStart()
          && mRange.getEnd() == other.getLocRange().getEnd()
          && mFullOverlap == other.mFullOverlap) 
      { return true;} else { return false; }
    }

    bool operator!=(const LocTuple& other) const 
    { return !(*this == other); }

    bool operator<(const LocTuple& other) const
    { if (*this == other)  { return false; }
      else if (mRange.getStart() < other.mRange.getStart()) { return true; }
      else if (mRange.getStart() > other.mRange.getStart()) { return false; }
      else if (mRange.getEnd() < other.mRange.getEnd()) { return true; }
      else if (mRange.getEnd() > other.mRange.getEnd()) { return false; }
      else if (mFullOverlap == true) { return true; }
      else { return false; }
    }

    void output(IRHandlesIRInterface& ir) 
    {
        sOutBuild->objStart("LocTuple");
        sOutBuild->fieldStart("mRange");
        mRange.output(ir);
        sOutBuild->fieldEnd("mRange");
        sOutBuild->field("mFullOverlap",OA::bool2string(mFullOverlap));
        sOutBuild->objEnd("LocTuple");
    }
       
  private:
    LocRange mRange;
    bool mFullOverlap;
};


//! iterator over ids
class IdIterator{
  public:
    IdIterator(std::map<int,OA_ptr<std::set<LocTuple> > >& aMap)
      {
        // iterate through ids and put them in a set
        std::map<int,OA_ptr<std::set<LocTuple> > >::iterator mapIter;
        for (mapIter=aMap.begin(); mapIter!=aMap.end(); mapIter++) {
            mSet.insert(mapIter->first);
        }
        reset();
      }
    ~IdIterator() {}

    void operator++() { if (isValid()) mIter++; }
    bool isValid() const  { return mIter!=mSet.end(); }
    int current() const  { return *mIter; }
    void reset() { mIter = mSet.begin(); }
  private:
    std::set<int> mSet;
    std::set<int>::iterator mIter;
};

//! iterator over locations
class LocTupleIterator {
  public:
    LocTupleIterator(std::set<LocTuple>& pSet)
      {
        // copy the set so we don't depend on aSet hanging around in memory
        mSet = pSet;
        reset();
      }
    ~LocTupleIterator() {}

    void operator++() { if (isValid()) mIter++; }
    bool isValid() const  { return mIter!=mSet.end(); }
    LocTuple current() const  { return *mIter; }
    void reset() { mIter = mSet.begin(); }
  private:
    std::set<LocTuple> mSet;
    std::set<LocTuple>::iterator  mIter;
};


class AliasMapXAIF : public virtual Annotation {
  public:
    AliasMapXAIF(ProcHandle p);
    ~AliasMapXAIF() {}

    static const int SET_ID_NONE = -1; 

    //! get iterator over all memory references that information is
    //! available for
    //! User must delete the iterator
    OA_ptr<MemRefHandleIterator> getMemRefIter();

    //*****************************************************************
    // Info methods unique to Alias::AliasMapXAIF
    //*****************************************************************

    //! get unique id for the alias map set for this memory reference
    //! This method is NOT used for construction, and CANNOT return SET_ID_NONE
    int getMapSetId(MemRefHandle ref); 
     
    //! get unique id for the alias map set for this memory reference
    //! SET_ID_NONE indicates that this memory reference doesn't map to any of the existing AliasMap sets
    int findMapSet(MemRefHandle ref);

    //! get id for an alias map set that contains an equivalent set of locations.
    //! SET_ID_NONE indicates that no equivalent location set was found in any AliasMap sets
    int findMapSet(OA_ptr<std::set<LocTuple> > pLocTupleSet);

    //! get iterator over ids for this alias map
    OA_ptr<IdIterator> getIdIterator();

    //! get iterator over all locations in a particular set
    OA_ptr<LocTupleIterator> getLocIterator(int setId); 

    //*****************************************************************
    // Construction methods 
    //*****************************************************************
    
    //! associate the given location set with the given mapSet
    void mapLocTupleSet(OA_ptr<std::set<LocTuple> > ltSet, int setId);

    //! associate a MemRefHandle with the given mapSet,
    //! means that the MemRefHandle can access all of the locations
    //! in the mapSet
    void mapMemRefToMapSet(MemRefHandle ref, int setId);

    //*****************************************************************
    // Output
    //*****************************************************************

    void output(IRHandlesIRInterface& ir);

    //! incomplete output of info for debugging, just lists map 
    //! set Ids and associated set of locations and mapping of 
    //! memrefs to the map set Ids
    void dump(std::ostream& os, OA_ptr<OA::IRHandlesIRInterface> ir);

  private:
    // data members
    ProcHandle mProcHandle; // procedure these sets are associated with

    // keep track of id mapping to location sets and status with a map
    std::map<int,OA_ptr<std::set<LocTuple> > > mIdToLocTupleSetMap; 

    // what memory references map to this alias map set
    std::map<int,MemRefSet> mIdToMemRefSetMap;

    // the location set a MemRefHandle maps to
    std::map<MemRefHandle,int> mMemRefToIdMap;  

    // set of memrefexpr for a memref
    std::map<MemRefHandle,std::set<OA_ptr<MemRefExpr> > > 
        mMemRefToMRESetMap;
};


  } // end of Alias namespace
} // end of OA namespace

#endif

