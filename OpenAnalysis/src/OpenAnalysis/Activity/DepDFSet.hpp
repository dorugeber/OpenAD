/*! \file
  
  \brief DataflowSet that maps a differentiable use loc to the def locs 
         that depend on it vice versa.

  \authors Michelle Strout 
  \version $Id: DepDFSet.hpp,v 1.3 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef DepDDFSet_h
#define DepDDFSet_h

#include <iostream>
#include <map>

// abstract interface that this class implements
#include <OpenAnalysis/DataFlow/DataFlowSet.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>

namespace OA {
  namespace Activity {

class DepIterator;

/*!
   A set of differentiable dependence pairs, <useLoc,defLoc>.
   Dependences to self are implicit unless removed with removeDep.
 */
    class DepDFSet : public virtual DataFlow::DataFlowSet,
                     public virtual Annotation {
public:
  DepDFSet();
  //! copy constructor
  DepDFSet(const DepDFSet &other); 
  ~DepDFSet() {}

  //! assignment
  DepDFSet& operator=(const DepDFSet &other);

  //*****************************************************************
  // DataFlowSet Interface Implementation
  //*****************************************************************
  OA_ptr<DataFlow::DataFlowSet> clone();
  
  // param for these can't be const because will have to 
  // dynamic cast to specific subclass
  bool operator ==(DataFlow::DataFlowSet &other) const;

  bool operator !=(DataFlow::DataFlowSet &other) const;

  //*****************************************************************
  // Methods specific to DepDFSet
  //*****************************************************************

  
  //! unions this DepDFSet with other and stores result in this
  DepDFSet& setUnion(DataFlow::DataFlowSet &other);

  //! If <a,b> in *this and <b,c> in other then add <a,c> to *this
  DepDFSet& compose(DataFlow::DataFlowSet &other);
  //OA_ptr<DepDFSet> compose(OA_ptr<DepDFSet> other);

  //! get an iterator over deps
  OA_ptr<DepIterator> getDepIterator() const;

  //! get an iterator over locations that depend on the given 
  //! differentiable use
  OA_ptr<LocIterator> getDefsIterator(const OA_ptr<Location> use) const;

  //! get an iterator over differentiable locations that the
  //! given def depends on
  OA_ptr<LocIterator> getUsesIterator(OA_ptr<Location> def) const;

  //! intersects this DepDFSet with other into self
//  DepDFSet& setIntersect(DepDFSet &other);

//  DepDFSet& setDifference(DepDFSet &other);

  //*****************************************************************
  // Annotation Interface
  //*****************************************************************
  void output(OA::IRHandlesIRInterface& ir);

  //*****************************************************************
  // Output
  //*****************************************************************

  void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> ir);
  void dump(std::ostream &os);
  void dump(std::ostream &os) const;

  //*****************************************************************
  // Construction
  //*****************************************************************
  
  void insertDep(OA_ptr<Location> use, OA_ptr<Location> def);

  //! needed to indicate when implicit reflexive pairs (eg. <a,a>)
  //! are not in the Dep set due to a must define of a
  void removeImplicitDep(OA_ptr<Location> use, OA_ptr<Location> def);

  typedef std::map<OA_ptr<Location>,OA_ptr<DataFlow::LocDFSet> >
      LocToLocDFSetMap;
  
  //! helper function that determines if the given location overlaps
  //! with anything in the set of locations whose implicit pair loc,loc
  //! has been killed due to a must def
  bool isImplicitRemoved(const OA_ptr<Location> loc) const;


private:
  //! helper function for iterators, makes all implicit deps explicit
  //! except those that have been removed explicitly
  //void makeImplicitExplicit();
  //bool mMakeImplicitExplicitMemoized;

  friend class DepIterator;

  // It is implicitly assumed that dep <a,a> is in each DepDFSet
  // for all locations.  This is a set of locations where this
  // implicit dep has been removed with removeImplicitDep.
  // If <a,a> is then explicitly inserted with insertDep, then
  // anything that overlaps with a will be removed from this set
  OA_ptr<DataFlow::LocDFSet> mImplicitRemoves;

  LocToLocDFSetMap mUseToDefsMap;
  LocToLocDFSetMap mDefToUsesMap;

  // maintain sets of the uses and defs as well
  OA_ptr<DataFlow::LocDFSet> mUses;
  OA_ptr<DataFlow::LocDFSet> mDefs;

};

class DepIterator {
  public:
    DepIterator(OA_ptr<DepDFSet> dfSet) : mDFSet(dfSet) 
      { mIter = mDFSet->mUseToDefsMap.begin(); 
        if (mIter!=mDFSet->mUseToDefsMap.end()) {
          mDefIter = new DataFlow::LocDFSetIterator(*(mIter->second));
          while (!mDefIter->isValid() && mIter!=mDFSet->mUseToDefsMap.end()) {
            mIter++;
            mDefIter = new DataFlow::LocDFSetIterator(*(mIter->second));
          }
        }
      }
    virtual ~DepIterator() {}

    OA_ptr<Location> use() const 
      { return mIter->first; }
    OA_ptr<Location> def() const
      { return mDefIter->current(); }

    bool isValid() const
      { if (mIter!=mDFSet->mUseToDefsMap.end() && mDefIter->isValid())
          { return true; } else { return false; }
      }
                    
    void operator++()
      {
        (*mDefIter)++;
        while (!mDefIter->isValid() && mIter!=mDFSet->mUseToDefsMap.end()) {
            mIter++;
            if (mIter!=mDFSet->mUseToDefsMap.end()) {
                mDefIter = new DataFlow::LocDFSetIterator(*(mIter->second));
            }
        }
      }

    void operator++(int) { ++*this; }

  private:
    OA_ptr<DepDFSet> mDFSet;
    DepDFSet::LocToLocDFSetMap::const_iterator mIter;
    OA_ptr<LocIterator> mDefIter;

};

  } // end of DataFlow namespace
} // end of OA namespace

#endif
