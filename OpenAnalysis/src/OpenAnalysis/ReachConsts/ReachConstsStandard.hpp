/*! \file
  
  \brief Maps StmtHandles to set of constant defs that reach
         each StmtHandle 

  \authors Michelle Strout, Barbara Kreaseck
  \version $Id: ReachConstsStandard.hpp,v 1.10 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ReachConstsStandard_hpp
#define ReachConstsStandard_hpp

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/DataFlow/DataFlowSet.hpp>
#include <OpenAnalysis/ReachConsts/Interface.hpp>
#include <OpenAnalysis/IRInterface/ReachConstsIRInterface.hpp>
#include <OpenAnalysis/IRInterface/ConstValBasicInterface.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>
#include <OpenAnalysis/Utils/GenOutputTool.hpp>


namespace OA {
  namespace ReachConsts {

class ConstDef;
class ConstDefSet;
class ConstDefSetIterator;

//! for a ConstDef ( really location-const pair ) const will not have
// a value if it should be TOP or BOTTOM --> TOP means that the value
// is as it came into the procedure, BOTTOM means that the value cannot
// be determined here due to conflicting defs.
enum ConstDefType {TOP, VALUE, BOTTOM};


//! Associates a location pointer with a constValBasic pointer.
// Might want to change this to be able to include Top or Bottom
// as the value associated with a location.
// Currently:
//  if constValBasic should be Bottom, constValBasic pointer is null (i.e. 0)
//  if constValBasic should be Top, then Location pointer is not in ConstDefSet
// Added a tag field: TOP,BOTTOM,VALUE
class ConstDef : public virtual Annotation
{
public:
  // constructors
  ConstDef(OA_ptr<Location> locP, 
           OA_ptr<ConstValBasicInterface> cvbP, ConstDefType cdType);
  ConstDef(OA_ptr<Location> locP, 
           OA_ptr<ConstValBasicInterface> cvbP);
  ConstDef(OA_ptr<Location> locP, ConstDefType cdType);
  ConstDef(OA_ptr<Location> locP);
  ConstDef(ConstDef& other) : mLocPtr(other.mLocPtr), 
                              mConstPtr(other.mConstPtr), 
                              mCDType(other.mCDType) {}
  ConstDef() { }
  ~ConstDef() { }

  // access
  OA_ptr<Location> getLocPtr() const { return mLocPtr; }
  OA_ptr<ConstValBasicInterface> getConstPtr() const { return mConstPtr; }
  ConstDefType getConstDefType() const { return mCDType; }

  // relationships
  ConstDef& operator= (const ConstDef& other);

  //! not doing a deep copy
  OA_ptr<ConstDef> clone() 
      { OA_ptr<ConstDef> retval; retval = new ConstDef(*this);
        return retval;
      }

  void output(IRHandlesIRInterface& pIR);
   

  //! operator== just compares content of locPtr 
  bool operator== (const ConstDef &other) const ;
  //! method equiv compares all parts of ConstDef as appropriate
  bool equiv(const ConstDef& other);

  bool operator!= (const ConstDef &other) const { return !(*this==other); }
  bool operator< (const ConstDef &other) const ;
  bool sameLoc (const ConstDef &other) const {
    if (mLocPtr==(other.getLocPtr())) { 
        return true; 
    } else {
        return false;
    }
  }

  // debugging
  std::string toString(OA_ptr<IRHandlesIRInterface> pIR);
  
  void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> pIR) {
    os << toString(pIR) << std::endl;
  }
  
private:

  
  OA_ptr<Location> mLocPtr;
  OA_ptr<ConstValBasicInterface> mConstPtr;
  ConstDefType mCDType;
};

//! Set of ConstDef* (intended for use with CFGDFProblem, core data
// members of ReachConstsStandard
class ConstDefSet  : public virtual DataFlow::DataFlowSet,
                     public virtual Annotation
{
public:
  // construction
  //ConstDefSet() { mSet = new std::set<OA_ptr<ConstDef> >; }
  //ConstDefSet(const ConstDefSet& other) : mSet(other.mSet) {}
  ConstDefSet() : mDefaultType(TOP) { mSet = new std::set<OA_ptr<ConstDef> >; }
  ConstDefSet(ConstDefType defaultType) : mDefaultType(defaultType) 
    { mSet = new std::set<OA_ptr<ConstDef> >; }
  ConstDefSet(const ConstDefSet& other) 
      : mDefaultType(other.mDefaultType),mSet(other.mSet) {}
  ~ConstDefSet() { }


  void output(IRHandlesIRInterface& ir);
  
  /*! After the assignment operation, the lhs ConstDefSet will point
      to the same instances of ConstDef's that the rhs points to.  Use
      clone if you want separate instances of the ConstDef's
  */
  ConstDefSet& operator= (const ConstDefSet& other) 
      {
        mSet = other.mSet;
        mDefaultType = other.mDefaultType;
        return *this;
      }
  OA_ptr<DataFlow::DataFlowSet> clone()
      { OA_ptr<ConstDefSet> retval;
        retval = new ConstDefSet(mDefaultType);
        std::set<OA_ptr<ConstDef> >::iterator defIter;
        for (defIter=mSet->begin(); defIter!=mSet->end(); defIter++) {
          OA_ptr<ConstDef> def = *defIter;
          retval->insert(def->clone());
        }
        return retval;
      }

  void insert(OA_ptr<ConstDef> h) { mSet->insert(h); }
  void remove(OA_ptr<ConstDef> h) { removeANDtell(h); }
  int insertANDtell(OA_ptr<ConstDef> h) 
      { return (int)((mSet->insert(h)).second); }
  int removeANDtell(OA_ptr<ConstDef> h) { return (mSet->erase(h)); }

  //! replace any ConstDef in mSet with location locPtr 
  // with ConstDef(locPtr,constPtr,cdType)
  // must use this instead of insert because std::set::insert will just see
  // that the element with the same locptr is already in the set and then not
  // insert the new element
  void replace(OA_ptr<Location> locPtr,
               OA_ptr<ConstValBasicInterface> constPtr,
               ConstDefType cdType);
  void replace(OA_ptr<ConstDef> cd);

  // relationship
  // param for these can't be const because will have to 
  // dynamic cast to specific subclass
  bool operator ==(DataFlow::DataFlowSet &other) const;
  bool operator !=(DataFlow::DataFlowSet &other) const
  { return (!(*this==other)); }

  //! need this one for stl containers
  bool operator==(const ConstDefSet& other) const 
  { return ConstDefSet::operator==(const_cast<ConstDefSet&>(other)); }

  bool empty() const { return mSet->empty(); }
  
  //! Return pointer to a copy of a ConstDef in this set with matching locPtr
  //! NULL is returned if no ConstDef in this set matches locPtr
  OA_ptr<ConstDef> find(OA_ptr<Location> locPtr) const;

  // debugging
  
  std::string toString(OA_ptr<IRHandlesIRInterface> pIR);
  void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> pIR)
  { os << toString(pIR) << std::endl; }

  void dump(std::ostream &os) {}

  //! DefaultConstDef is used for ManagerICFGReachConst
  ConstDefType getDefaultConstDef() const { return mDefaultType; }
  //! For construction purposes only, DefaultConstType only TOP or BOTTOM
  void setDefaultConstDef(ConstDefType cdt) 
  { assert(cdt!=VALUE); mDefaultType = cdt; }

private:
  ConstDefType mDefaultType;  // TOP or BOTTOM

protected:
  
  OA_ptr<std::set<OA_ptr<ConstDef> > > mSet;

  friend class ConstDefSetIterator;
};
    

  
//! Iterator over ConstDef's in a ConstDefSet
class ConstDefSetIterator {
public:
  ConstDefSetIterator (ConstDefSet& CDSet) : mSet(CDSet.mSet) 
      { assert(!mSet.ptrEqual(NULL)); mCDIter = mSet->begin(); }
  ~ConstDefSetIterator () {}
  
  void operator ++ () { if (isValid()) mCDIter++; }
  //! is the iterator at the end
  bool isValid() { return (mCDIter != mSet->end()); }
  //! return copy of current node in iterator
  OA_ptr<ConstDef> current() { assert(isValid()); return (*mCDIter); }
  //! reset iterator to beginning of set
  void reset() { mCDIter = mSet->begin(); }

private:
  OA_ptr<std::set<OA_ptr<ConstDef> > > mSet;
  std::set<OA_ptr<ConstDef> >::iterator mCDIter;
};

class ReachConstsStandard : public virtual Interface, 
                            public virtual Annotation
{
  public:
    ReachConstsStandard(ProcHandle p) {}
    ~ReachConstsStandard() {}

    //! Return the set of reaching constants for a given stmt
    OA_ptr<ConstDefSet> getReachConsts(StmtHandle s) 
    { if (mReachConsts[s].ptrEqual(NULL)) {
        mReachConsts[s] = new ConstDefSet;
      }
      return mReachConsts[s]; 
    }

    //! Return the ConstValBasicInterface of the reaching constant
    //! for the given memory reference
    //! Returns NULL if no constant value exists for this MemRefHandle
    OA_ptr<ConstValBasicInterface> getReachConst(MemRefHandle h) 
    { OA_ptr<ConstValBasicInterface> retval; retval = NULL;
      if (mMemRef2ReachConst.find(h)!= mMemRef2ReachConst.end())
      { retval= mMemRef2ReachConst[h];}  
      return retval; 
    }

    void output(IRHandlesIRInterface& ir);

    //*****************************************************************
    // Construction methods 
    //*****************************************************************

    //! insert a reaching const def into the given stmt's const def set
    void insertConstDef(StmtHandle s, OA_ptr<ConstDef> cd, ConstDefType t=TOP)
    { if (mReachConsts[s].ptrEqual(NULL)) {
        mReachConsts[s] = new ConstDefSet(t);
      }
      // have to call replace because only compare ConstsDefs by loc
      mReachConsts[s]->replace(cd);  
    }

    //! reset a statement's const def set to null
    //! needed because no access to final NodeIn sets in CFGDFSolver 
    //! forces us to save a statement's const def set during every transfer
    void resetConstDefSet(StmtHandle s, ConstDefType t=TOP)
    { mReachConsts[s] = new ConstDefSet(t); }

    //! change reaching constant for a given memory reference
    void updateReachConst(MemRefHandle ref, 
                          OA_ptr<ConstValBasicInterface> cPtr)
    { mMemRef2ReachConst[ref]=cPtr; }

    //*****************************************************************
    // Output
    //*****************************************************************

    //! incomplete output of info for debugging, just lists stmts
    //! and associated set of stmts that are reaching defs for the given
    //! stmt
    void dump(std::ostream& os, OA_ptr<OA::IRHandlesIRInterface> ir);

    //! get iterator over ConstDefSet for given statememt handle
    OA_ptr<ConstDefSetIterator> getConstDefIterator(StmtHandle s) 
    {
        OA_ptr<ConstDefSetIterator> retval;
        retval = new ConstDefSetIterator(*(mReachConsts[s]));
        return retval;
    }

    //! report number of memory references with constant value
    std::string getMemRefConstInfo();

  private:
    
    // data members
    std::map<StmtHandle, OA_ptr<ConstDefSet> > mReachConsts;
    std::map<MemRefHandle,OA_ptr<ConstValBasicInterface> > mMemRef2ReachConst;

};


  } // end of ReachConsts namespace
} // end of OA namespace

#endif

