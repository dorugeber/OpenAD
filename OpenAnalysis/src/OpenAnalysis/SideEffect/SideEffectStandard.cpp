/*! \file
  
  \brief Implementation of SideEffect::SideEffectStandard

  \author Michelle Strout
  \version $Id: SideEffectStandard.cpp,v 1.10 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "SideEffectStandard.hpp"

namespace OA {
  namespace SideEffect{

static bool debug = false;

//*****************************************************************
// Interface Implementation
//*****************************************************************


SideEffectStandard::SideEffectStandard()
        { OA_DEBUG_CTRL_MACRO("DEBUG_SideEffectStandard:ALL", debug);

          mUnknownLocSet = new LocSet;
          OA_ptr<Location> uLoc; 
          //uLoc = dynamic_cast<Location*>(new UnknownLoc());
          //MMS, even the SGI compiler shouldn't need the above
          uLoc = new UnknownLoc();
          mUnknownLocSet->insert(uLoc);
        }

OA_ptr<LocIterator> SideEffectStandard::getLMODIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mLMODSet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mLMODSet); 
  }
  return retval;
}

OA_ptr<LocIterator> SideEffectStandard::getMODIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mMODSet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mMODSet); 
  }
  return retval;
}

/*!
   Conservative estimate for DEF is that no locations are definitely
   modified.
*/
OA_ptr<LocIterator> SideEffectStandard::getLDEFIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mLDEFSet.ptrEqual(0)) {
    OA_ptr<LocSet> retSet;  retSet = new LocSet; // empty set
    retval = new LocSetIterator(retSet);  
  } else {
    retval = new LocSetIterator(mLDEFSet); 
  }
  return retval;
}

/*!
   Conservative estimate for DEF is that no locations are definitely
   modified.
*/
OA_ptr<LocIterator> SideEffectStandard::getDEFIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mDEFSet.ptrEqual(0)) {
    OA_ptr<LocSet> retSet;  retSet = new LocSet; // empty set
    retval = new LocSetIterator(retSet);  
  } else {
    retval = new LocSetIterator(mDEFSet); 
  }
  return retval;
}

OA_ptr<LocIterator> SideEffectStandard::getLUSEIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mLUSESet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mLUSESet); 
  }
  return retval;
}

OA_ptr<LocIterator> SideEffectStandard::getUSEIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mUSESet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mUSESet); 
  }
  return retval;
}

OA_ptr<LocIterator> SideEffectStandard::getLREFIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mLREFSet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mLREFSet); 
  }
  return retval;
}

OA_ptr<LocIterator> SideEffectStandard::getREFIterator()
{ 
  OA_ptr<LocSetIterator> retval;
  if (mREFSet.ptrEqual(0)) {
    // loc set with unknown loc
    retval = new LocSetIterator(mUnknownLocSet);  
  } else {
    retval = new LocSetIterator(mREFSet); 
  }
  return retval;
}
    
//*****************************************************************
// Other informational methods
//*****************************************************************
//! Return true if the given location is in the LMOD set 
bool SideEffectStandard::inLMOD(OA_ptr<Location> loc)
{
    if (mLMODSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mLMODSet->find(loc);
        return pos != mLMODSet->end();
    }
}

//! Return true if the given location is in the MOD set 
bool SideEffectStandard::inMOD(OA_ptr<Location> loc)
{
    if (mMODSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mMODSet->find(loc);
        return pos != mMODSet->end();
    }
}

//! Return true if the given location is in the LDEF set 
bool SideEffectStandard::inLDEF(OA_ptr<Location> loc)
{
    if (mLDEFSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mLDEFSet->find(loc);
        return pos != mLDEFSet->end();
    }
}

//! Return true if the given location is in the DEF set 
bool SideEffectStandard::inDEF(OA_ptr<Location> loc)
{
    if (mDEFSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mDEFSet->find(loc);
        return pos != mDEFSet->end();
    }
}

//! Return true if the given location is in the LUSE set 
bool SideEffectStandard::inLUSE(OA_ptr<Location> loc)
{
    if (mLUSESet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mLUSESet->find(loc);
        return pos != mLUSESet->end();
    }
}

//! Return true if the given location is in the USE set 
bool SideEffectStandard::inUSE(OA_ptr<Location> loc)
{
    if (mUSESet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mUSESet->find(loc);
        return pos != mUSESet->end();
    }
}

//! Return true if the given location is in the LREF set 
bool SideEffectStandard::inLREF(OA_ptr<Location> loc)
{
    if (mLREFSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mLREFSet->find(loc);
        return pos != mLREFSet->end();
    }
}

//! Return true if the given location is in the REF set 
bool SideEffectStandard::inREF(OA_ptr<Location> loc)
{
    if (mREFSet.ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mREFSet->find(loc);
        return pos != mREFSet->end();
    }
}

//*****************************************************************
// Construction methods
//*****************************************************************
//! Insert a location into the LMOD set 
void SideEffectStandard::insertLMOD(OA_ptr<Location> loc)
{
    if (mLMODSet.ptrEqual(0)) {
        mLMODSet = new LocSet;
    }
    mLMODSet->insert(loc);
}
    
//! Insert a location into the MOD set 
void SideEffectStandard::insertMOD(OA_ptr<Location> loc)
{
    if (debug) {
        std::cout << "SideEffectStandard::insertMOD: loc = ";
	loc->dump(std::cout);
    }
    if (mMODSet.ptrEqual(0)) {
        mMODSet = new LocSet;
        if (debug) {
            std::cout << "SideEffectStandard::insertMOD: creating new LocSet" 
                      << std::endl;
        }
    }
    mMODSet->insert(loc);
}
    
//! Insert a location into the LDEF set 
void SideEffectStandard::insertLDEF(OA_ptr<Location> loc)
{
    if (mLDEFSet.ptrEqual(0)) {
        mLDEFSet = new LocSet;
    }
    mLDEFSet->insert(loc);
}
    
//! Insert a location into the DEF set 
void SideEffectStandard::insertDEF(OA_ptr<Location> loc)
{
    if (mDEFSet.ptrEqual(0)) {
        mDEFSet = new LocSet;
    }
    mDEFSet->insert(loc);
}
    
//! Insert a location into the LUSE set 
void SideEffectStandard::insertLUSE(OA_ptr<Location> loc)
{
    if (mLUSESet.ptrEqual(0)) {
        mLUSESet = new LocSet;
    }
   if (debug) {
        std::cout << "LUseLocation insert = ";
        loc->dump(std::cout);
   }
                    
   mLUSESet->insert(loc);

}
    
//! Insert a location into the USE set 
void SideEffectStandard::insertUSE(OA_ptr<Location> loc)
{
    if (mUSESet.ptrEqual(0)) {
        mUSESet = new LocSet;
    }
    if (debug) {
        std::cout << "UseLocation insert = ";
	    loc->dump(std::cout);
    }
     
    mUSESet->insert(loc);

    if(debug)
    {
      LocSetIterator USElocIter(mUSESet);
      for (; USElocIter.isValid(); ++USElocIter) {
              OA_ptr<Location> loc = USElocIter.current();
              std::cout << "UseLocCompare";
              loc->dump(std::cout);   
              std::cout << std::endl;
      }
    }
}
    
//! Insert a location into the LREF set 
void SideEffectStandard::insertLREF(OA_ptr<Location> loc)
{
    if (mLREFSet.ptrEqual(0)) {
        mLREFSet = new LocSet;
    }
    mLREFSet->insert(loc);
}
    
//! Insert a location into the MOD set 
void SideEffectStandard::insertREF(OA_ptr<Location> loc)
{
    if (mREFSet.ptrEqual(0)) {
        mREFSet = new LocSet;
    }
    mREFSet->insert(loc);
}
    
//*************************************************************
//! Make the LMOD set empty, default is to have Unknown location
void SideEffectStandard::emptyLMOD()
{
    mLMODSet = new LocSet;
}

//! Make the MOD set empty, default is to have Unknown location
void SideEffectStandard::emptyMOD()
{
    mMODSet = new LocSet;
}

//! Make the LDEF set empty, default is empty, but want same interface
void SideEffectStandard::emptyLDEF()
{
    mLDEFSet = new LocSet;
}
    
//! Make the DEF set empty, default is empty, but want same interface
void SideEffectStandard::emptyDEF()
{
    mDEFSet = new LocSet;
}
    
//! Make the LUSE set empty, default is to have Unknown location
void SideEffectStandard::emptyLUSE()
{
    mLUSESet = new LocSet;
}

//! Make the USE set empty, default is to have Unknown location
void SideEffectStandard::emptyUSE()
{
    mUSESet = new LocSet;
}

//! Make the LREF set empty, default is to have Unknown location
void SideEffectStandard::emptyLREF()
{
    mLREFSet = new LocSet;
}

//! Make the REF set empty, default is to have Unknown location
void SideEffectStandard::emptyREF()
{
    mREFSet = new LocSet;
}

//*****************************************************************
// Output
//*****************************************************************


void SideEffectStandard::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    std::cout << "====================== SideEffect" << std::endl;

    OA_ptr<LocIterator> locIterPtr;

    std::cout << "\tLMOD = ";
    locIterPtr = getLMODIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << "\tMOD = ";
    locIterPtr = getMODIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << std::endl;

    std::cout << "\tLDEF = ";
    locIterPtr = getLDEFIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << "\tDEF = ";
    locIterPtr = getDEFIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << std::endl;

    std::cout << "\tLUSE = ";
    locIterPtr = getLUSEIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << "\tUSE = ";
    locIterPtr = getUSEIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << std::endl;

    std::cout << "\tLREF = ";
    locIterPtr = getLREFIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << "\tREF = ";
    locIterPtr = getREFIterator();
    for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
      locIterPtr->current()->dump(std::cout,ir);
    }
    std::cout << std::endl;

}

  } // end of UDDUChains namespace
} // end of OA namespace
