/*! \file
  
  \brief DataflowSet that consists of a set of IRHandles.

  \authors Michelle Strout (June 2004)
  \version $Id: IRHandleDataFlowSet.hpp,v 1.12 2007/10/08 19:19:11 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>


  Don't actually create an IRHandleDataFlowSet.  Instead inherit 
  privately from it to get implementation of DataFlowSet operations
  for a set of IRHandle objects.
*/

#ifndef IRHandleDataFlowSet_h
#define IRHandleDataFlowSet_h

#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>
#include <OpenAnalysis/DataFlow/DataFlowSet.hpp>

// abstract interface that this class implements
//#include <OpenAnalysis/DataFlow/DataFlowSet.hpp>

namespace OA {
  namespace DataFlow {

template <class T> class IRHandleIterator;

template <class T> 
class IRHandleDataFlowSet : public virtual DataFlowSet {
public:
  IRHandleDataFlowSet() {}
  IRHandleDataFlowSet(const IRHandleDataFlowSet<T> &other) : mSet(other.mSet) {}
  ~IRHandleDataFlowSet() {}

  OA_ptr<DataFlowSet> clone()
    { OA_ptr<IRHandleDataFlowSet<T> > retval;
      retval = new IRHandleDataFlowSet<T>(*this); return retval; }
  
  // param for these can't be const because will have to 
  // dynamic cast to specific subclass
  //IRHandleDataFlowSet<T>& operator =(IRHandleDataFlowSet<T> &other)
  //{ 
  //  mSet = other.mSet;
  //  return *this;
 // }


  bool operator ==(DataFlowSet &other) const
  { 
    IRHandleDataFlowSet<T>& recastOther 
        = dynamic_cast<IRHandleDataFlowSet<T>&>(other);
    return mSet == recastOther.mSet; 
  }

  bool operator !=(DataFlowSet &other) const
  { 
    IRHandleDataFlowSet<T>& recastOther 
        = dynamic_cast<IRHandleDataFlowSet<T>&>(other);
    return mSet != recastOther.mSet; 
  }

  //! intersects both sets into this
  IRHandleDataFlowSet<T>& setUnion(DataFlowSet &other)
  { 
    IRHandleDataFlowSet<T>& recastOther 
        = dynamic_cast<IRHandleDataFlowSet<T>&>(other);
    std::set<T> temp;
    std::set_union(mSet.begin(), mSet.end(), 
                   recastOther.mSet.begin(), recastOther.mSet.end(),
                   std::inserter(temp,temp.end()));
    mSet = temp;
    return *this;
  }

  //! intersects both sets into this
 /* IRHandleDataFlowSet<T>& setIntersect(IRHandleDataFlowSet<T> &other)
  { 
    std::set<T> temp;
    std::set_intersection(mSet.begin(), mSet.end(), 
                          other.mSet.begin(), other.mSet.end(),
                          std::inserter(temp,temp.end()));
    mSet = temp;
    return *this;
  }*/


  IRHandleDataFlowSet<T>& setIntersect(DataFlowSet &other)
  {
    IRHandleDataFlowSet<T>& recastOther
        = dynamic_cast<IRHandleDataFlowSet<T>&>(other);
    std::set<T> temp;
    std::set_intersection(mSet.begin(), mSet.end(),
                   recastOther.mSet.begin(), recastOther.mSet.end(),
                   std::inserter(temp,temp.end()));
    mSet = temp;
    return *this;
  }


  void dump(std::ostream &os)
  {
      os << "IRHandleDataFlowSet: mSet = ";
      // iterate over IRHandle's and print out hvals
      typename std::set<T>::iterator iter;
      for (iter=mSet.begin(); iter!=mSet.end(); iter++) {
          os << (*iter).hval() << ", ";
      }
      os << std::endl;
  }

  void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> ir)
  {
      os << "IRHandleDataFlowSet: mSet = ";
      // iterate over IRHandle's and have the IR print them out
      typename std::set<T>::iterator iter;
      for (iter=mSet.begin(); iter!=mSet.end(); iter++) {
          os << ir->toString(*iter) << ", ";
      }
      os << std::endl;
  }

  // methods specific to IRHandleDataFlowSet
  void insert(T h) { mSet.insert(h); }
  void remove(T h) { mSet.erase(h); }

  //! indicate whether the given element is in the set
  bool handleInSet(T h) { return (mSet.find(h)!=mSet.end()); }

  //! whether or not the set is empty
  bool empty() { return mSet.empty(); }

protected:
  std::set<T> mSet;

  friend class IRHandleIterator<T>;

};

//--------------------------------------------------------------------
/*! An iterator over IRHandles in the set.
*/
//--------------------------------------------------------------------
//template <class T> class std::set<T>::iterator;

template <class T>
class IRHandleIterator {
public:
    // have to get OA_ptr to a set because otherwise the set
    // might disappear before user of this iterator references it
    IRHandleIterator (OA_ptr<IRHandleDataFlowSet<T> > DFSet) : mDFSet(DFSet) 
    { hIter = mDFSet->mSet.begin(); }
    ~IRHandleIterator () {}

    void operator ++ () { if (hIter != mDFSet->mSet.end()) hIter++; }
    void operator++(int) { ++*this; }  // postfix
    
    //! is the iterator at the end
    bool isValid() const { return (hIter != mDFSet->mSet.end()); }
    
    //! return current node
    T current() const { return (*hIter); }

private:
    OA_ptr<IRHandleDataFlowSet<T> > mDFSet;
    typename std::set<T>::iterator hIter;
};
 

  } // end of DataFlow namespace
} // end of OA namespace

#endif
