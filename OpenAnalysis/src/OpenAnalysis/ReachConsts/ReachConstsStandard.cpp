/*! \file
  
  \brief Implementation of ReachConsts::ReachConstsStandard

  \author Michelle Strout, Barbara Kreaseck
  \version $Id: ReachConstsStandard.cpp,v 1.8 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/


#include "ReachConstsStandard.hpp"
#include <Utils/Util.hpp>

namespace OA {
  namespace ReachConsts {

static bool debug = false;

//------------------------------------------------------------------
// ConstDef methods
//------------------------------------------------------------------

//! ConstDef constructor:  using given location and given constant and 
//! given ConstDefType. Use with caution as ConstDefType indicates 
//! ConstValBasicInterface usage in other routines.
ConstDef::ConstDef(OA_ptr<Location> locP, 
		   OA_ptr<ConstValBasicInterface> cvbP,
		   ConstDefType cdType) 
{
  OA_DEBUG_CTRL_MACRO("DEBUG_ReachConstsStandard:ALL", debug);
  mLocPtr = locP;
  mConstPtr = cvbP; 
  mCDType = cdType;
}

//! ConstDef constructor:  using given location and given constant and 
//! default ConstDefType VALUE.
ConstDef::ConstDef(OA_ptr<Location> locP, 
                   OA_ptr<ConstValBasicInterface> cvbP) 
{
  mLocPtr = locP;
  mConstPtr = cvbP; 
  if (cvbP.ptrEqual(0)) { 
    mCDType = TOP; 
  } else { 
    mCDType = VALUE;
  }
}

//! ConstDef constructor:  using given location and given ConstDefType
//! and default ConstValBasicInterface of 0.  Correct usage only involves
//! a ConstDefType of TOP or BOTTOM.  Using VALUE causes an assertion failure.
ConstDef::ConstDef(OA_ptr<Location> locP, ConstDefType cdType) {

  assert(cdType!=VALUE);

  mLocPtr = locP;
  mCDType = cdType; // only TOP or BOTTOM make sense in this constructor
  mConstPtr = 0;
}

//! ConstDef constructor:  using given location and default ConstDefType
//! of TOP and default ConstValBasicInterface of 0.
ConstDef::ConstDef(OA_ptr<Location> locP) {
  mLocPtr = locP; 
  mConstPtr = 0; 
  mCDType = TOP; // assume TOP
}

//! copy a ConstDef, not a deep copy, will refer to same Location
//! and ConstValBasicInterface as other
ConstDef& ConstDef::operator=(const ConstDef& other) {
  mLocPtr = other.getLocPtr();
  mConstPtr = other.getConstPtr();
  mCDType = other.getConstDefType();
  return *this;
}

//! Equality operator for ConstDef.  Just inspects location contents.
bool ConstDef::operator== (const ConstDef &other) const {
  //if (debug) { std::cout << "In ConstDef::operator==" << std::endl; }
  if (mLocPtr==other.getLocPtr()) { 
      return true;
  } else {
      return false;
  }
}

//! Just based on location, this way when insert a new ConstDef it can
//! override the existing ConstDef with same location
bool ConstDef::operator< (const ConstDef &other) const 
{ 
  if (mLocPtr<other.getLocPtr()) { 
      return true; 
  } else {
      return false;
  }
}
 
//! Equality method for ConstDef.  If locations are equal and 
//! const types are equal, then true for TOP or BOTTOM.  If locations
//! are equal and const types are both VALUE, then true when actual
//! const values are equal.  False otherwise.
bool ConstDef::equiv(const ConstDef& other){
  if (mLocPtr==other.getLocPtr()) {
    if (mCDType == other.getConstDefType()) {
      switch (mCDType) {
      case TOP: case BOTTOM: return true; break;
      case VALUE:
        if (mConstPtr.ptrEqual(0)&&other.getConstPtr().ptrEqual(0)) 
        { return true; }
        else if (!mConstPtr.ptrEqual(0)&&!other.getConstPtr().ptrEqual(0)) {
          if (mConstPtr==other.getConstPtr()) { return true; }
        }
        break;
      }
    }
  }
  return false;
}

/* this uses equivalence semantics
bool ConstDef::operator< (const ConstDef &other) const 
{ 
  if (*this == other) {
      return false;
  }
  if (mLocPtr<other.getLocPtr()) { 
      return true; 
  } else if (other.getLocPtr()<mLocPtr) {
      return false;
  } else {// loc's are equal 
    if (mCDType==other.getConstDefType()) {
      switch (mCDType) {
      case TOP: 
      case BOTTOM: return false; break; // these are equal
      case VALUE:
        assert(!mConstPtr.ptrEqual(0) && !other.getConstPtr().ptrEqual(0)); 
        // if locs are equal and types are both VALUE, then compare
        // based upon the string representation ??? 
        // (only have == and != for ConstValBasicInterface) 
        return ((mConstPtr->toString())<((other.getConstPtr())->toString()));
        break;
      }
    } else { // types are not equal
      // use enumeration values of ConstDefType to make a decision
      return (mCDType < other.getConstDefType());
    }
  }
  return false; // shouldn't get there
}
*/


//! Return a string that contains a character representation of
// a ConstDef.
std::string ConstDef::toString(OA_ptr<IRHandlesIRInterface> pIR) 
{
  std::ostringstream oss;
  oss << "<";
  oss << (*mLocPtr).toString(pIR);
  switch (mCDType) {
  case TOP: 
    oss << ",TOP>"; break;
  case BOTTOM: 
    oss << ",BOTTOM>"; break;
  case VALUE:
    oss << ",VALUE=" << (*mConstPtr).toString() << ">"; break;
  }
  return oss.str();
}

//------------------------------------------------------------------
// ConstDefSet methods
//------------------------------------------------------------------

void ConstDefSet::replace(OA_ptr<ConstDef> cd)
{
    replace(cd->getLocPtr(), cd->getConstPtr(), cd->getConstDefType());
}

//! replace any ConstDef in mSet with location locPtr 
//! with ConstDef(locPtr,constPtr,cdType)
void ConstDefSet::replace(OA_ptr<Location> locPtr,
                          OA_ptr<ConstValBasicInterface> constPtr,
                          ConstDefType cdType)
{
  if (debug) {
    int r,i;
    OA_ptr<ConstDef> cd;
    cd = new ConstDef(locPtr);
    r = removeANDtell(cd);
    cd = new ConstDef(locPtr,constPtr,cdType);
    i = insertANDtell(cd);
    std::cout << "ConstDefSet.replace: (removed " << r << ") "
              << "(inserted " << i << " "
      //          << (ConstDef(locPtr,constPtr,cdType)).toString(mIR) 
              << ")" << std::endl;
  } else {
    OA_ptr<ConstDef> cd; 
    cd = new ConstDef(locPtr); 
    remove(cd);
    cd = new ConstDef(locPtr,constPtr,cdType);
    insert(cd);
  }
}


//! operator == for a ConstDefSet cannot rely upon the == operator for the
// underlying sets, because the == operator of an element of a ConstDefSet, 
// namely a ConstDef, only considers the contents of the location pointer
// and not any of the other fields.  So, need to use ConstDef's equal() method
// here instead.
bool ConstDefSet::operator==(DataFlow::DataFlowSet &other) const
{

  // first dynamic cast to a ConstDefSet, if that doesn't work then 
  // other is a different kind of DataFlowSet and *this is not equal
  ConstDefSet& recastOther = dynamic_cast<ConstDefSet&>(other);
    /*
  try {
  } catch (?) {
  }
  */

  if (mSet->size() != recastOther.mSet->size()) {
    return false;
  }

  // FIXME: assumes that they both don't have all possible locations
  if (mDefaultType!=recastOther.mDefaultType) {
      return false;
  }

  // same size:  for every element in mSet, find the element in other.mSet
  std::set<OA_ptr<ConstDef> >::iterator set1Iter;
  for (set1Iter = mSet->begin(); set1Iter!=mSet->end(); ++set1Iter) {
    OA_ptr<ConstDef> cd1 = *set1Iter;
    std::set<OA_ptr<ConstDef> >::iterator set2Iter;
    set2Iter = recastOther.mSet->find(cd1);

    if (set2Iter == recastOther.mSet->end()) {
      return (false); // cd1 not found
    } else {
      OA_ptr<ConstDef> cd2 = *set2Iter;
      if (!(cd1->equiv(*cd2))) { // use full equiv operator
        return (false); // cd1 not equiv to cd2
      }
    }
  } // end of set1Iter loog

  // hopefully, if we got here, all elements of set1 equiv set2
  return(true);

}

//! find the ConstDef in this ConstDefSet with the given location (should
//! be at most one) return a ptr to that ConstDef
OA_ptr<ConstDef> ConstDefSet::find(OA_ptr<Location> locPtr) const
{
  OA_ptr<ConstDef> retval; retval = NULL;
  
  OA_ptr<ConstValBasicInterface> nullVal; nullVal = 0;
  OA_ptr<ConstDef> findCD; findCD = new ConstDef(locPtr,nullVal,TOP);

  std::set<OA_ptr<ConstDef> >::iterator cdIter = mSet->find(findCD);
  if (cdIter != mSet->end()) {
    retval = *cdIter;
  }
  return retval;
}

//! Return a string representing the contents of a ConstDefSet
std::string ConstDefSet::toString(OA_ptr<IRHandlesIRInterface> pIR){
  std::ostringstream oss;
  oss << "{";
  
  oss << "default = ";
  switch (mDefaultType) {
      case TOP:
          oss << "TOP,";
          break;
      case BOTTOM:
          oss << "BOTTOM,";
          break;
      default:
          assert(0);
          break;
  }
  // iterate over ConstDef's and have the IR print them out
  ConstDefSetIterator iter(*this);
  //std::set<OA_ptr<ConstDef> >::iterator iter;
  OA_ptr<ConstDef> cd;
  
  // first one
  if (iter.isValid()) {
    cd = iter.current();
    oss << cd->toString(pIR);
    ++iter;
  }
  
  // rest
  for (; iter.isValid(); ++iter) {
    cd = iter.current();
    oss << ", " << cd->toString(pIR); 
  }
  
  oss << "}";
  return oss.str();
}


//------------------------------------------------------------------
// ReachConstsStandard methods
//------------------------------------------------------------------

//! incomplete output of info for debugging, just lists stmts
//! and associated set of ConstDefs that are reaching const defs for the given
void ReachConstsStandard::dump(std::ostream& os, 
                             OA_ptr<OA::IRHandlesIRInterface> ir)
{
    std::map<StmtHandle, OA_ptr<ConstDefSet> >::iterator mapIter;
    for (mapIter = mReachConsts.begin(); mapIter != mReachConsts.end(); mapIter++) {
        StmtHandle s = mapIter->first;

        os << "StmtHandle(" << s.hval() << ") " << ir->toString(s) 
           << std::endl << "\treachConsts: " << std::endl;
        // iterate over reaching consts for statement s

        OA_ptr<ConstDefSet> cdSet = getReachConsts(s);
        cdSet->dump(os,ir);

        os << std::endl;

    os << "Mapping of MemRefHandles to constants" << std::endl;
    std::map<MemRefHandle,OA_ptr<ConstValBasicInterface> >::iterator mIter;
    for (mIter=mMemRef2ReachConst.begin(); mIter!=mMemRef2ReachConst.end();
         mIter++)
    {
        os << "\t" << ir->toString(mIter->first) << " --> ";
        OA_ptr<ConstValBasicInterface> constPtr;
        constPtr = mIter->second;
        if (constPtr.ptrEqual(0)) {
            os << "no constant value" << std::endl;
        } else {
            os << constPtr->toString() << std::endl;
        }
    }

        /*
        ConstDefSetIterator* sSetIter = getConstDefIterator(s); //should be one per stmt
        for ( ; (bool)(*sSetIter); ++(*sSetIter))
        {
          ConstDef cd = *sSetIter;
          Location* l = cd.getLocPtr();
          ConstValBasicInterface * cvb = cd.getConstPtr();

          os << "\t" << "<";
          l->dump(os,ir);
          os << "," << cvb->toString() << ">";
          os << std::endl;
        }
        */
    }
}



void ConstDef::output(OA::IRHandlesIRInterface &ir)
{
    sOutBuild->objStart("ConstDef");

    sOutBuild->fieldStart("Location");
    mLocPtr->output(ir);
    sOutBuild->fieldEnd("Location");

  // mCDType.output(ir);

    sOutBuild->fieldStart("mCDType");
    std::ostringstream oss;
    switch (mCDType) {
      case TOP:
         oss << ",TOP>"; break;
      case BOTTOM:
         oss << ",BOTTOM>"; break;
      case VALUE:
         oss << ",VALUE=" << (*mConstPtr).toString() << ">"; break;
     }
    sOutBuild->fieldEnd("mCDType");
     // return oss.str();

    sOutBuild->outputString( oss.str());

    sOutBuild->objEnd("ConstDef");
}



void ConstDefSet::output(OA::IRHandlesIRInterface &ir)
{
    sOutBuild->objStart("ConstDefSet");

    sOutBuild->listStart();
    std::set<OA::OA_ptr<ConstDef> >::iterator reg_mSet_iterator;
    for(reg_mSet_iterator  = mSet->begin();
        reg_mSet_iterator != mSet->end();
        reg_mSet_iterator++)
    {
        OA::OA_ptr<ConstDef>  item = *reg_mSet_iterator;
        sOutBuild->listItemStart();
        sOutBuild->fieldStart("ConstDef");
        item->output(ir);
        sOutBuild->fieldEnd("ConstDef");
        sOutBuild->listItemEnd();
    }
    sOutBuild->listEnd();
    sOutBuild->objEnd("ConstDefSet");
}




void ReachConstsStandard::output(OA::IRHandlesIRInterface &ir)
{
    sOutBuild->objStart("ReachConstsStandard");

    sOutBuild->mapStart("mReachConsts", "StmtHandle", "OA::OA_ptr<ConstDefSet> ");
    std::map<StmtHandle, OA::OA_ptr<ConstDefSet> >::iterator reg_mReachConsts_iterator;
    for(reg_mReachConsts_iterator = mReachConsts.begin();
        reg_mReachConsts_iterator != mReachConsts.end();
        reg_mReachConsts_iterator++)
    {
        const StmtHandle &first = reg_mReachConsts_iterator->first;
        OA::OA_ptr<ConstDefSet>  &second = reg_mReachConsts_iterator->second;
        sOutBuild->mapEntryStart();
        sOutBuild->mapKeyStart();
        sOutBuild->fieldStart("Statement");
        sOutBuild->outputIRHandle(first, ir);
        sOutBuild->fieldEnd("Statement");

        sOutBuild->mapKeyEnd();
        sOutBuild->mapValueStart();
        second->output(ir);
        sOutBuild->mapValueEnd();
        sOutBuild->mapEntryEnd();
    }
    sOutBuild->mapEnd("mReachConsts");

    sOutBuild->objEnd("ReachConstsStandard");
}






/*!
   Returns a tab-delimited string with the number of total memory references
   known and then the number of memory references associated with a constant
   value.
*/
std::string ReachConstsStandard::getMemRefConstInfo()
{
  std::ostringstream oss;
  int totalMemRef = 0, constMemRef = 0;

  std::map<MemRefHandle,OA_ptr<ConstValBasicInterface> >::iterator mIter;
  for (mIter=mMemRef2ReachConst.begin(); mIter!=mMemRef2ReachConst.end(); mIter++) {
    totalMemRef++;
    if (! mIter->second.ptrEqual(NULL)) {
      constMemRef++;
    }
  }

  oss << totalMemRef << "\t" << constMemRef;
  
  return oss.str();
}



  } // end of ReachConsts namespace
} // end of OA namespace
