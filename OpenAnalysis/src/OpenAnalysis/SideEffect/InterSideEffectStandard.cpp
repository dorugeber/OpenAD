/*! \file
  
  \brief Implementation of SideEffect::InterSideEffectStandard

  \author Michelle Strout
  \version $Id: InterSideEffectStandard.cpp,v 1.10 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "InterSideEffectStandard.hpp"
#include "SideEffectStandard.hpp"

namespace OA {
  namespace SideEffect{

static bool debug = false;

//*****************************************************************
// Interface Implementation
//*****************************************************************

InterSideEffectStandard::InterSideEffectStandard()
{ 
    mDefaultSideEffect = new SideEffectStandard;
}

//! Return an iterator over all procedures for which interprocedural
//! side effect information is available 
OA_ptr<ProcIterator> InterSideEffectStandard::getKnownProcIterator()
{
    // create a procSet
    OA_ptr<std::set<ProcHandle> > procSet;
    procSet = new std::set<ProcHandle>;

    // loop through all maps to add to list of ProcHandle's
    ProcToSideEffectMap::iterator mIter;
    for (mIter=mProcToSideEffectMap.begin(); 
         mIter != mProcToSideEffectMap.end(); mIter++) 
    {
        procSet->insert(mIter->first);
    }

    OA_ptr<InterSideEffectProcIter> retval;
    retval = new InterSideEffectProcIter(procSet);
    return retval;
}


//*************************************************************************

// For the next eight methods,
// If don't have SideEffect information for the given call
// then return iterator from default/empty SideEffect
// analysis results

OA_ptr<LocIterator> 
InterSideEffectStandard::getLMODIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getLMODIterator();
  } else {
    return mCallToSideEffectMap[call]->getLMODIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getMODIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getMODIterator();
  } else {
    return mCallToSideEffectMap[call]->getMODIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLDEFIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getLDEFIterator();
  } else {
    return mCallToSideEffectMap[call]->getLDEFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getDEFIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getDEFIterator();
  } else {
    return mCallToSideEffectMap[call]->getDEFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLUSEIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getLUSEIterator();
  } else {
    return mCallToSideEffectMap[call]->getLUSEIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getUSEIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getUSEIterator();
  } else {
    return mCallToSideEffectMap[call]->getUSEIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLREFIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getLREFIterator();
  } else {
    return mCallToSideEffectMap[call]->getLREFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getREFIterator(CallHandle call)
{
  if (mCallToSideEffectMap[call].ptrEqual(0)) {
    return mDefaultSideEffect->getREFIterator();
  } else {
    return mCallToSideEffectMap[call]->getREFIterator(); 
  }
}

//*************************************************************************

OA_ptr<LocIterator> 
InterSideEffectStandard::getLMODIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getLMODIterator();
  } else {
    return mProcToSideEffectMap[p]->getLMODIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getMODIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getMODIterator();
  } else {
    return mProcToSideEffectMap[p]->getMODIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLDEFIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getLDEFIterator();
  } else {
    return mProcToSideEffectMap[p]->getLDEFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getDEFIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getDEFIterator();
  } else {
    return mProcToSideEffectMap[p]->getDEFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLUSEIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getLUSEIterator();
  } else {
    return mProcToSideEffectMap[p]->getLUSEIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getUSEIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getUSEIterator();
  } else {
    return mProcToSideEffectMap[p]->getUSEIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getLREFIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getLREFIterator();
  } else {
    return mProcToSideEffectMap[p]->getLREFIterator(); 
  }
}

OA_ptr<LocIterator> 
InterSideEffectStandard::getREFIterator(ProcHandle p)
{
  if (mProcToSideEffectMap[p].ptrEqual(0)) {
    return mDefaultSideEffect->getREFIterator();
  } else {
    return mProcToSideEffectMap[p]->getREFIterator(); 
  }
}
    
//*****************************************************************
// Other informational methods
//*****************************************************************
/*
//! Return true if the given location is in the LMOD set for given proc
bool SideEffectStandard::inLMOD(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToLMODSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToLMODSet[p]->find(loc);
        return pos != mProcToLMODSet[p]->end();
    }
}

//! Return true if the given location is in the MOD set for given proc
bool SideEffectStandard::inMOD(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToMODSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToMODSet[p]->find(loc);
        return pos != mProcToMODSet[p]->end();
    }
}

//! Return true if the given location is in the LDEF set for given proc
bool SideEffectStandard::inLDEF(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToLDEFSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToLDEFSet[p]->find(loc);
        return pos != mProcToLDEFSet[p]->end();
    }
}

//! Return true if the given location is in the DEF set for given proc
bool SideEffectStandard::inDEF(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToDEFSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToDEFSet[p]->find(loc);
        return pos != mProcToDEFSet[p]->end();
    }
}

//! Return true if the given location is in the LUSE set for given proc
bool SideEffectStandard::inLUSE(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToLUSESet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToLUSESet[p]->find(loc);
        return pos != mProcToLUSESet[p]->end();
    }
}

//! Return true if the given location is in the USE set for given proc
bool SideEffectStandard::inUSE(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToUSESet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToUSESet[p]->find(loc);
        return pos != mProcToUSESet[p]->end();
    }
}

//! Return true if the given location is in the LREF set for given proc
bool SideEffectStandard::inLREF(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToLREFSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToLREFSet[p]->find(loc);
        return pos != mProcToLREFSet[p]->end();
    }
}

//! Return true if the given location is in the REF set for given proc
bool SideEffectStandard::inREF(ProcHandle p, OA_ptr<Location> loc)
{
    if (mProcToREFSet[p].ptrEqual(0)) {
        return false;
    } else {
        LocSet::iterator pos = mProcToREFSet[p]->find(loc);
        return pos != mProcToREFSet[p]->end();
    }
}
*/
//*****************************************************************
// Construction methods
//*****************************************************************
//! Associate the given procedure with the given intraprocedural
//! SideEffect information
void InterSideEffectStandard::mapProcToSideEffect(ProcHandle proc, 
    OA_ptr<OA::SideEffect::SideEffectStandard> sideEffect)
{
    mProcToSideEffectMap[proc] = sideEffect;
}

//! Get the SideEffect information associated with the given procedure
OA_ptr<OA::SideEffect::SideEffectStandard> 
InterSideEffectStandard::getSideEffectResults(ProcHandle proc)
{
    return mProcToSideEffectMap[proc];
}


//! Associate the given call with the given intraprocedural
//! SideEffect information
void InterSideEffectStandard::mapCallToSideEffect(CallHandle call, 
    OA_ptr<OA::SideEffect::SideEffectStandard> sideEffect)
{
    if (sideEffect.ptrEqual(0) ) { std::cout << "sideEffect.ptrEqual(0)" << std::endl; }
    if (debug) {
        std::cout << "mapCallToSideEffect(call = " << call.hval() << std::endl;
    }
    mCallToSideEffectMap[call] = sideEffect;
}

int InterSideEffectStandard::getLocCount(ProcHandle proc)
{
    if (mProcToSideEffectMap[proc].ptrEqual(0)) {
        return 0;
    }

    int count = 0;
    OA_ptr<LocIterator> locIterPtr;

    // LMOD
    locIterPtr = mProcToSideEffectMap[proc]->getLMODIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // MOD
    locIterPtr = mProcToSideEffectMap[proc]->getMODIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // LDEF
    locIterPtr = mProcToSideEffectMap[proc]->getLDEFIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // DEF
    locIterPtr = mProcToSideEffectMap[proc]->getDEFIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // LUSE
    locIterPtr = mProcToSideEffectMap[proc]->getLUSEIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // USE
    locIterPtr = mProcToSideEffectMap[proc]->getUSEIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // LREF
    locIterPtr = mProcToSideEffectMap[proc]->getLREFIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }
    // REF
    locIterPtr = mProcToSideEffectMap[proc]->getREFIterator();
    for (; locIterPtr->isValid(); (*locIterPtr)++ ) {
        count++;
    }

    return count;
}
    
void InterSideEffectStandard::initCallSideEffect(CallHandle call)
{
    mCallToSideEffectMap[call] = new SideEffectStandard;
    // empty out all the sets
    mCallToSideEffectMap[call]->emptyLMOD();
    mCallToSideEffectMap[call]->emptyMOD();
    mCallToSideEffectMap[call]->emptyLDEF();
    mCallToSideEffectMap[call]->emptyDEF();
    mCallToSideEffectMap[call]->emptyLUSE();
    mCallToSideEffectMap[call]->emptyUSE();
    mCallToSideEffectMap[call]->emptyLREF();
    mCallToSideEffectMap[call]->emptyREF();
}

//! Insert a location into the LMOD set for the given call
void InterSideEffectStandard::insertLMOD(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertLMOD(loc);
}
    
//! Insert a location into the MOD set for the given call
void InterSideEffectStandard::insertMOD(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertMOD(loc);
}
    
//! Insert a location into the LDEF set for the given call
void InterSideEffectStandard::insertLDEF(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertLDEF(loc);
}
    
//! Insert a location into the DEF set for the given call
void InterSideEffectStandard::insertDEF(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertDEF(loc);
}
    
//! Insert a location into the LUSE set for the given call
void InterSideEffectStandard::insertLUSE(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertLUSE(loc);
}
    
//! Insert a location into the USE set for the given call
void InterSideEffectStandard::insertUSE(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertUSE(loc);
}
    
//! Insert a location into the LREF set for the given call
void InterSideEffectStandard::insertLREF(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertLREF(loc);
}
    
//! Insert a location into the REF set for the given call
void InterSideEffectStandard::insertREF(CallHandle p, OA_ptr<Location> loc)
{
    mCallToSideEffectMap[p]->insertREF(loc);
}
    
//*************************************************************
/*
//! Make the LMOD set empty, default is to have Unknown location
void SideEffectStandard::emptyLMOD(ProcHandle p)
{
    mProcToLMODSet[p] = new LocSet;
}

//! Make the MOD set empty, default is to have Unknown location
void SideEffectStandard::emptyMOD(ProcHandle p)
{
    mProcToMODSet[p] = new LocSet;
}

//! Make the LDEF set empty, default is empty, but want same interface
void SideEffectStandard::emptyLDEF(ProcHandle p)
{
    mProcToLDEFSet[p] = new LocSet;
}
    
//! Make the DEF set empty, default is empty, but want same interface
void SideEffectStandard::emptyDEF(ProcHandle p)
{
    mProcToDEFSet[p] = new LocSet;
}
    
//! Make the LUSE set empty, default is to have Unknown location
void SideEffectStandard::emptyLUSE(ProcHandle p)
{
    mProcToLUSESet[p] = new LocSet;
}

//! Make the USE set empty, default is to have Unknown location
void SideEffectStandard::emptyUSE(ProcHandle p)
{
    mProcToUSESet[p] = new LocSet;
}

//! Make the LREF set empty, default is to have Unknown location
void SideEffectStandard::emptyLREF(ProcHandle p)
{
    mProcToLREFSet[p] = new LocSet;
}

//! Make the REF set empty, default is to have Unknown location
void SideEffectStandard::emptyREF(ProcHandle p)
{
    mProcToREFSet[p] = new LocSet;
}
*/
//*****************************************************************
// Output
//*****************************************************************
void InterSideEffectStandard::dump(std::ostream& os, 
                                   OA_ptr<IRHandlesIRInterface> ir)
{
    std::cout << "====================== InterSideEffect" << std::endl;

    // Loop through all known procedures
    OA_ptr<ProcIterator> procIterPtr = getKnownProcIterator();
    for (; procIterPtr->isValid(); (*procIterPtr)++) {
        ProcHandle proc = procIterPtr->current();

        // print all sets for each procedure
        std::cout << "Procedure( " << ir->toString(proc) << " )" << std::endl;

        OA_ptr<LocIterator> locIterPtr;

        std::cout << "\tLMOD = ";
        locIterPtr = getLMODIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tMOD = ";

        locIterPtr = getMODIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLDEF = ";
        locIterPtr = getLDEFIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tDEF = ";
        locIterPtr = getDEFIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLUSE = ";
        locIterPtr = getLUSEIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tUSE = ";
        locIterPtr = getUSEIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLREF = ";
        locIterPtr = getLREFIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tREF = ";
        locIterPtr = getREFIterator(proc);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;
    }
    
    // Loop through all known call sites
    CallToSideEffectMap::iterator mapIter;
    for (mapIter=mCallToSideEffectMap.begin();
         mapIter!=mCallToSideEffectMap.end(); mapIter++)
    {
        CallHandle call = mapIter->first;

        // print all sets for each procedure
        std::cout << "Call( " << ir->toString(call) << " )" << std::endl;

        OA_ptr<LocIterator> locIterPtr;

        std::cout << "\tLMOD = ";
        locIterPtr = getLMODIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tMOD = ";

        locIterPtr = getMODIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLDEF = ";
        locIterPtr = getLDEFIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tDEF = ";
        locIterPtr = getDEFIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLUSE = ";
        locIterPtr = getLUSEIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tUSE = ";
        locIterPtr = getUSEIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;

        std::cout << "\tLREF = ";
        locIterPtr = getLREFIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << "\tREF = ";
        locIterPtr = getREFIterator(call);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
          locIterPtr->current()->dump(std::cout,ir);
        }
        std::cout << std::endl;
    }



}

  } // end of UDDUChains namespace
} // end of OA namespace
