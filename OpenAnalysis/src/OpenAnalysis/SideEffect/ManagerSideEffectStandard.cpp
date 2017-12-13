/*! \file
  
  \brief The AnnotationManager that generates SideEffectStandard
         which maps each procedure to the sets of locations belonging to
         LMOD, MOD, LUSE, USE, LDEF, DEF, LREF, and REF.  

  \authors Michelle Strout
  \version $Id: ManagerSideEffectStandard.cpp,v 1.15 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerSideEffectStandard.hpp"
#include <Utils/Util.hpp>


namespace OA {
  namespace SideEffect {

static bool debug = false;

/*! concrete LocationVisitor that determines that determines whether
    the visited location involves a symbol that is referenced within
    the procedure or not.
    If the symbol is referenced then mRef will be true.
    If the location does not involve a symbol (ie. UnnamedLoc)
    then mRef will be true as well.
    A set of referenced symbols must be passed in to the constructor.
*/
/* Deprecated, replace functionality if necessary MMS 7/6/06
class RefVisitor : public virtual LocationVisitor {
  private:
    std::set<SymHandle>& mRefSymSet;
  public:
    bool mRef;

    RefVisitor(std::set<SymHandle> &pRefSymSet)
        : mRefSymSet(pRefSymSet), mRef(true) {}
    ~RefVisitor() {}
    void visitNamedLoc(NamedLoc& loc) 
      { 
        if (mRefSymSet.find(loc.getSymHandle()) != mRefSymSet.end())
        { mRef = true; } 
        else { mRef = false; }
      }
    void visitUnnamedLoc(UnnamedLoc& loc) { mRef = true; }
    void visitInvisibleLoc(InvisibleLoc& loc) { mRef = true; }
    void visitUnknownLoc(UnknownLoc& loc) { mRef = true; }
    void visitLocSubSet(LocSubSet& loc) { 
        // have this visitor visit the base location
        OA_ptr<Location> baseLoc = loc.getLoc();
        baseLoc->acceptVisitor(*this);
    }
};
*/

ManagerSideEffectStandard::ManagerSideEffectStandard(OA_ptr<SideEffectIRInterface> _ir) : mIR(_ir)
{
 OA_DEBUG_CTRL_MACRO("DEBUG_ManagerSideEffectStandard:ALL", debug);
}


/*!
    \brief    Used to perform analysis when not using AQM

    Currently just does an intraprocedural side-effect analysis.
    We conservatively assume that function calls define and use the
    UnknownLoc, which overlaps with all possible locations.

*/
OA_ptr<SideEffectStandard> 
ManagerSideEffectStandard::performAnalysis(ProcHandle proc, 
                                 OA_ptr<Alias::Interface> alias,
                                 OA_ptr<InterSideEffectInterface> inter)
{
  mProc = proc;
  if (debug) {
      std::cout << "ManagerSideEffectStandard: proc = " << mIR->toString(proc)
                << std::endl;
  }

  // create a new (waiting to be filled) SideEffectStandard as member
  OA_ptr<SideEffectStandard> retSideEffect;
  retSideEffect= new SideEffectStandard();

  //alias->dump(std::cout,mIR);

  // empty out all the sets
  retSideEffect->emptyLMOD();
  retSideEffect->emptyMOD();
  retSideEffect->emptyLDEF();
  retSideEffect->emptyDEF();
  retSideEffect->emptyLUSE();
  retSideEffect->emptyUSE();
  retSideEffect->emptyLREF();
  retSideEffect->emptyREF();
  
  // loop through statements
  OA_ptr<OA::IRStmtIterator> sIt = mIR->getStmtIterator(proc);
  for ( ; sIt->isValid(); (*sIt)++) {
    OA::StmtHandle stmt = sIt->current();
    if (debug) {
        std::cout << "\tstmt = ";
        mIR->dump(stmt,std::cout);
    }

    // for each use memory reference in the stmt
    OA_ptr<MemRefHandleIterator> mrIterPtr = mIR->getUseMemRefs(stmt);
    for (; mrIterPtr->isValid(); (*mrIterPtr)++ )
    {
      MemRefHandle memref = mrIterPtr->current();
      if (debug) {
        std::cout << "\tmemref = ";
        mIR->dump(memref,std::cout);
      }

      // each location the memory reference may reference
      OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(memref);
      for (; locIterPtr->isValid(); ++(*locIterPtr)) {

        OA_ptr<Location> useLoc = locIterPtr->current();
        if (debug) {
            std::cout << "useLoc = ";
            useLoc->dump(std::cout, mIR);
        }

        // DEPRECATED, no longer necessary, alias analysis only maps
        // to locations visible within the procedure, MMS 7/6/06
        // will only put into these sets if the location is referenced
        // in the procedure
        //useLoc->acceptVisitor(refVisitor);
        //if  (refVisitor.mRef == true) {

        // put each location used through a memory reference in LREF and REF
        retSideEffect->insertLREF( useLoc);
        retSideEffect->insertREF( useLoc);
    
        // put each location used through a memory reference in LUSE 
        // and USE if the location is not already in DEF
        // FIXME?: assuming everything is used in a statement before 
        // anything
        // is defined, is this not the case in things like short circuits?
        
        // CONSERVATIVE FIXME: just do all USEs, to do upward exposed need
        // CFG, to do only USEd, need to calc DEF first
        //if (!retSideEffect->inDEF(useLoc)) {
            
                retSideEffect->insertLUSE(useLoc);
                retSideEffect->insertUSE(useLoc);
        //    }
        //}
      }
    }

    // for each def memory reference in the stmt
    mrIterPtr = mIR->getDefMemRefs(stmt);
    for (; mrIterPtr->isValid(); (*mrIterPtr)++ )
    {
      MemRefHandle memref = mrIterPtr->current();
      if (debug) {
        std::cout << "\tmemref = ";
        mIR->dump(memref,std::cout);
      }
      
      // put each location may defined into LMOD and MOD
      OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(memref);
      for (; locIterPtr->isValid(); ++(*locIterPtr)) {
        OA_ptr<Location> mayDef = locIterPtr->current();
        if (debug) {
            std::cout << "mayDef = ";
            mayDef->dump(std::cout, mIR);
        }

        // AGAIN, DEPRECATED
        // will only put into these sets if the location is referenced
        // in the procedure
        //mayDef->acceptVisitor(refVisitor);
        //if  (refVisitor.mRef == true) {
        retSideEffect->insertLMOD(mayDef);
        retSideEffect->insertMOD(mayDef);
        //}
      } 

      // put each location must defined into LDEF and DEF
      locIterPtr = alias->getMustLocs(memref);
      for (; locIterPtr->isValid(); ++(*locIterPtr)) {
        OA_ptr<Location> mustDef = locIterPtr->current();

        // will only put into these sets if the location is referenced
        // in the procedure
        //mustDef->acceptVisitor(refVisitor);
        //if (refVisitor.mRef == true) {
        retSideEffect->insertLDEF(mustDef);
        retSideEffect->insertDEF(mustDef);
        //}
      } 
    }
    
    // Iterate over procedure calls of a statement
    // to determine their effect
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle call = callsiteItPtr->current();
      if (debug) {
        std::cout << "\tcall = " << mIR->toString(call) << std::endl;
      }

      OA_ptr<LocIterator> locIterPtr;
      
      // MOD
      locIterPtr = inter->getMODIterator(call);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          retSideEffect->insertMOD(locIterPtr->current());
          if (debug) {
            std::cout << "\tinserting loc = ";
            locIterPtr->current()->dump(std::cout,mIR);
            std::cout << "\tinto MOD" << std::endl;
          }
      } 
      // DEF
      locIterPtr = inter->getDEFIterator(call);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          retSideEffect->insertDEF(locIterPtr->current());
      } 
      // USE
      locIterPtr = inter->getUSEIterator(call);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          //if (!retSideEffect->inDEF(locIterPtr->current())) {
          //FIXME: don't know order of use vs def in func call
          OA_ptr<Location> Loc = locIterPtr->current();
          if (debug) {
              std::cout << "SideEffectStandard USE retSideEffect  = ";
              Loc->dump(std::cout);
              std::cout << std::endl;
          }
          retSideEffect->insertUSE(Loc);  
         // retSideEffect->insertUSE(locIterPtr->current());
          //}
      } 
      // REF
      locIterPtr = inter->getREFIterator(call);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
          retSideEffect->insertREF(locIterPtr->current());
      } 
    }
	
  } // end of loop over statements

  return retSideEffect;
}
  } // end of namespace SideEffect
} // end of namespace OA
