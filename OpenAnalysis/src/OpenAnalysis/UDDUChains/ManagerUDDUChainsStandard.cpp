/*! \file
  
  \brief The AnnotationManager that generates UDDUChains.

  \authors Michelle Strout
  \version $Id: ManagerUDDUChainsStandard.cpp,v 1.22 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerUDDUChainsStandard.hpp"

namespace OA {
  namespace UDDUChains {

bool debug = false;

/*!
*/

ManagerUDDUChainsStandard::ManagerUDDUChainsStandard(OA_ptr<UDDUChainsIRInterface> _ir) : mIR(_ir) 
{
 OA_DEBUG_CTRL_MACRO("DEBUG_ManagerUDDUChainsStandard:ALL", debug);
}


/*!
*/

OA_ptr<UDDUChainsStandard> ManagerUDDUChainsStandard::performAnalysis(ProcHandle proc, 
        OA_ptr<Alias::Interface> alias, OA_ptr<ReachDefs::Interface> reachDefs,
        OA_ptr<SideEffect::InterSideEffectInterface> interSE)
{
  OA_ptr<UDDUChainsStandard> aUDDUChains;
  aUDDUChains = new UDDUChainsStandard(proc);

  // for each reaching definition for a statement, need to determine 
  // if that reaching definition has a def (due to memref or function call)
  // that may alias a use (due to memref or function call) in the statement

  if (debug) {
    std::cout << "ManagerUDDUChainsStandard::performAnalysis: ";
    std::cout << std::endl;
  }

  // for each statement in the procedure
  OA_ptr<OA::IRStmtIterator> stmtIterPtr = mIR->getStmtIterator(proc);
  for ( ; stmtIterPtr->isValid(); (*stmtIterPtr)++) {
    OA::StmtHandle stmt = stmtIterPtr->current();
    if (debug) {
      std::cout << "\tstmt (" << stmt.hval() << ") = " << mIR->toString(stmt);
      std::cout << std::endl;
    }
 
    // for each stmt keep track of number of reaching defs that are must
    // reaching defs, if there are no must reaching defs and there
    // were uses, we should add
    // StmtHandle(0) into the udchain indicating that the define might be
    // happening out of scope
    int mustCountForStmt = 0;
    int mayUseCountForStmt = 0;

    // loop over defs in this statement to make sure their du-chain
    // is initialized to empty
    OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(stmt);
    for (; defIterPtr->isValid(); (*defIterPtr)++) {
        MemRefHandle def = defIterPtr->current();
        aUDDUChains->insertMemRefDef(def);
    }
  
    // loop over uses in this statement
    OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
    for (; useIterPtr->isValid(); (*useIterPtr)++) {
        mayUseCountForStmt++;
        MemRefHandle use = useIterPtr->current();
        aUDDUChains->insertMemRefUse(use);

        if (debug) {
          std::cout << "\t\tuse (" << use.hval() << ") = " << mIR->toString(use);
          std::cout << std::endl;
        }

        // keep track of must reaching defs for this use
        int mustCountForUse = 0;

        // for each reaching definition
        OA_ptr<ReachDefs::Interface::ReachDefsIterator> rdIterPtr 
            = reachDefs->getReachDefsIterator(stmt);
        for (; rdIterPtr->isValid(); ++(*rdIterPtr)) {
          StmtHandle reachStmt = rdIterPtr->current();
          if (debug) {
            std::cout << "\t\treachStmt (" << reachStmt.hval() 
                      << ")= " << mIR->toString(reachStmt);
            std::cout << std::endl;
          }

          // loop over defs due to memref in that reaching definition
          OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(reachStmt);
          for (; defIterPtr->isValid(); (*defIterPtr)++) {
            MemRefHandle def = defIterPtr->current();
            aUDDUChains->insertMemRefDef(def);

            if (debug) {
              std::cout << "\t\t\tdef (" << def.hval() << ") = " 
                        << mIR->toString(def);
              std::cout << std::endl;
            }
 
            // if the def may aliases the use then add this
            // information to UDDUChains
            if (alias->alias(use,def)==Alias::MAYALIAS ||
                alias->alias(use,def)==Alias::MUSTALIAS ) 
            {
              if (debug) { std::cout << "line 115, inserting ";
                std::cout << "reachStmt = " << mIR->toString(reachStmt);
                std::cout << ", stmt = " << mIR->toString(stmt);
                std::cout << ", def = " << mIR->toString(def);
                std::cout << ", use = " << mIR->toString(use);
                std::cout << std::endl;
              }
              aUDDUChains->insertDefUse(reachStmt,stmt);
              aUDDUChains->insertMemRefDefStmtUse(def,stmt);
              aUDDUChains->insertStmtDefMemRefUse(reachStmt,use);
            }
            // count must defs
            if (alias->alias(use,def)==Alias::MUSTALIAS) {
              mustCountForStmt++;
              mustCountForUse++;
            }

          }
          
          // loop over defs due to function calls in that reaching definition
          // must or may defines from procedure calls
          OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(reachStmt);
          for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
            CallHandle expr = callsiteItPtr->current();

            OA_ptr<LocIterator> locIterPtr;
      
            // MOD
            locIterPtr = interSE->getMODIterator(expr);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
              OA_ptr<Location> modLoc= locIterPtr->current();
           
              // if the def may aliases the use then add this
              // information to UDDUChains
              if (locMayAliasMemRef(alias,modLoc,use))
              {
                if (debug) { std::cout << "line 152, inserting ";
                  std::cout << "reachStmt = " << mIR->toString(reachStmt);
                  std::cout << ", stmt = " << mIR->toString(stmt);
                  std::cout << ", use = " << mIR->toString(use);
                  std::cout << std::endl;
                }
                aUDDUChains->insertDefUse(reachStmt,stmt);
                aUDDUChains->insertStmtDefMemRefUse(reachStmt,use);
              }
              if (locMustAliasMemRef(alias,modLoc,use)) {
                mustCountForStmt++;
                mustCountForUse++;
              }
            }
          } // loop over callsites

        } // loop over reaching definitions

        // if current use had no must reaching defs then add StmtHandle(0)
        if (mustCountForUse==0) {
          if (debug) { std::cout << "line 169, inserting ";
            std::cout << "reachStmt = StmtHandle(0)";
            std::cout << ", stmt = " << mIR->toString(stmt);
            std::cout << ", use = " << mIR->toString(use);
            std::cout << std::endl;
          }
          aUDDUChains->insertDefUse(StmtHandle(0),stmt);
          aUDDUChains->insertStmtDefMemRefUse(StmtHandle(0),use);
        }
          
    } // loop over uses in current stmt
    
    // loop over uses due to function calls in this statement
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle expr = callsiteItPtr->current();

      // USE
      OA_ptr<LocIterator> locIterPtr;
      locIterPtr = interSE->getUSEIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
        mayUseCountForStmt++;
        OA_ptr<Location> useLoc= locIterPtr->current();

        int mustCountForUse=0;

        // for each reaching definition
        OA_ptr<ReachDefs::Interface::ReachDefsIterator> rdIterPtr 
            = reachDefs->getReachDefsIterator(stmt);
        for (; rdIterPtr->isValid(); ++(*rdIterPtr)) {
          StmtHandle reachStmt = rdIterPtr->current();
          if (debug) {
            std::cout << "\t\treachStmt (" << reachStmt.hval() 
                      << ")= " << mIR->toString(reachStmt);
            std::cout << std::endl;
          }

          // loop over defs due to memref in that reaching definition
          OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(reachStmt);
          for (; defIterPtr->isValid(); (*defIterPtr)++) {
            MemRefHandle def = defIterPtr->current();
            aUDDUChains->insertMemRefDef(def);
 
            // if the def may aliases the use loc then add this
            // information to UDDUChains
            if (locMayAliasMemRef(alias,useLoc,def))
            {
              if (debug) { std::cout << "line 213, inserting ";
                std::cout << "reachStmt = " << mIR->toString(reachStmt);
                std::cout << ", stmt = " << mIR->toString(stmt);
                std::cout << std::endl;
              }
              aUDDUChains->insertDefUse(reachStmt,stmt);
              aUDDUChains->insertMemRefDefStmtUse(def,stmt);
             // aUDDUChains->insertStmtDefMemRefUse(reachStmt,use);
            }
            if (locMustAliasMemRef(alias,useLoc,def)) {
              mustCountForStmt++;
            }

          }
          
          // loop over defs due to function calls in that reaching definition
          // must or may defines from procedure calls
          OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(reachStmt);
          for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
            CallHandle expr = callsiteItPtr->current();

            OA_ptr<LocIterator> locIterPtr;
      
            // MOD
            locIterPtr = interSE->getMODIterator(expr);
            for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
              OA_ptr<Location> modLoc= locIterPtr->current();
           
              // if the def may aliases the use then add this
              // information to UDDUChains
              if (modLoc->mayOverlap(*useLoc)) {
                if (debug) { std::cout << "line 243, inserting ";
                  std::cout << "reachStmt = " << mIR->toString(reachStmt);
                  std::cout << ", stmt = " << mIR->toString(stmt);
                  std::cout << std::endl;
                }
                aUDDUChains->insertDefUse(reachStmt,stmt);
              }
              if (modLoc->mustOverlap(*useLoc)) {
                mustCountForStmt++;
                mustCountForUse++;
              }
            } // loop over locations modified by callsite
          } // loop over callsites in reachstmt

        } // loop over reaching statements

        if (mustCountForUse==0) {
           if (debug) { std::cout << "line 169, inserting ";
              std::cout << "reachStmt = StmtHandle(0)";
              std::cout << ", stmt = " << mIR->toString(stmt);
              std::cout << std::endl;
           }
           aUDDUChains->insertDefUse(StmtHandle(0),stmt);
        }

      } // loop over uses in callsite
    } // loop over callsites

    // if there were no must reaching definitions for this statement then
    // add StmtHandle(0) into the ud chain
    if (mustCountForStmt==0 && mayUseCountForStmt>0) {
        // loop over uses in this statement
        OA_ptr<MemRefHandleIterator> useIterPtr = mIR->getUseMemRefs(stmt);
        for (; useIterPtr->isValid(); (*useIterPtr)++) {
            MemRefHandle use = useIterPtr->current();
            if (debug) { std::cout << "line 272, inserting ";
              std::cout << "reachStmt = StmtHandle(0)";
              std::cout << ", stmt = " << mIR->toString(stmt);
              std::cout << ", use = " << mIR->toString(use);
              std::cout << std::endl;
            }
            aUDDUChains->insertDefUse(StmtHandle(0),stmt);
            aUDDUChains->insertStmtDefMemRefUse(StmtHandle(0),use);
        }
    }
 
  } // loop over stmts

  // loop over all exit reaching definitions, insert an out-of-scope use for
  // all non-local locations that are may defined in those reaching definitions
  OA_ptr<ReachDefs::Interface::ReachDefsIterator> rdIterPtr 
      = reachDefs->getExitReachDefsIterator();
  for (; rdIterPtr->isValid(); ++(*rdIterPtr)) {
    StmtHandle reachStmt = rdIterPtr->current();
    if (debug) {
      std::cout << "\t\treachStmt (" << reachStmt.hval() 
                << ")= " << mIR->toString(reachStmt);
      std::cout << std::endl;
    }

    // loop over defs due to memref in that reaching definition
    OA_ptr<MemRefHandleIterator> defIterPtr = mIR->getDefMemRefs(reachStmt);
    for (; defIterPtr->isValid(); (*defIterPtr)++) {
      MemRefHandle def = defIterPtr->current();

      // get all may locations for that def and if the may location is not 
      // local insert a possible out-of-scope use
      OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(def);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
        OA_ptr<Location> loc = locIterPtr->current();
        if (debug) {
          std::cout << "\t\tloc = ";
          loc->dump(std::cout, mIR);
        }
        // need to get base location if we have a subset
        if (loc->isaSubSet()) {
          loc = loc->getBaseLoc(); 
        }
        if (debug) {
          std::cout << "\t\tloc = ";
          loc->dump(std::cout, mIR);
        }
        // all non-local locations
        if (!loc->isLocal()) {
          if (debug) { std::cout << "line 313, inserting ";
              std::cout << "reachStmt = " << mIR->toString(reachStmt);
              std::cout << ", stmt = StmtHandle(0)";
              std::cout << ", def = " << mIR->toString(def);
              std::cout << std::endl;
          }
          aUDDUChains->insertMemRefDefStmtUse(def,StmtHandle(0));
          aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));
        // determine if this is a reference parameter
        } else if(loc->isaInvisible()) {
            /*OA_ptr<InvisibleLoc> nloc = loc.convert<InvisibleLoc>();
            OA_ptr<MemRefExpr> mre = nloc->getMemRefExpr();
            OA_ptr<RefOp> refop;
            if(mre->isaRefOp()) {
                 aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));
                 aUDDUChains->insertMemRefDefStmtUse(def,StmtHandle(0));
            } else{
                assert(0);
            }
            */

            aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));
            aUDDUChains->insertMemRefDefStmtUse(def,StmtHandle(0));

        }// else if (loc->isaNamed()) {
         // OA_ptr<NamedLoc> nloc = loc.convert<NamedLoc>();
         // assert(0); // MMS 8/9/06, need to revamp without isRefParam

          /*! temporarily commented out by PLM 08/22/06.
           * This functionality is deprecated and I am not sure how to
           * replace this: need to ask Michelle
          if (mIR->isRefParam(nloc->getSymHandle())) {
            if (debug) { std::cout << "line 325, inserting ";
              std::cout << "reachStmt = " << mIR->toString(reachStmt);
              std::cout << ", stmt = StmtHandle(0)";
              std::cout << ", def = " << mIR->toString(def);
              std::cout << std::endl;
            }
            aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));
            aUDDUChains->insertMemRefDefStmtUse(def,StmtHandle(0));
//          }
          */
       // }
          
      }
    }
    // loop over defs due to function calls in that reaching definition
    // must or may defines from procedure calls
    OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(reachStmt);
    for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
      CallHandle expr = callsiteItPtr->current();

      OA_ptr<LocIterator> locIterPtr;
      
      // MOD
      locIterPtr = interSE->getMODIterator(expr);
      for ( ; locIterPtr->isValid(); (*locIterPtr)++) {
        OA_ptr<Location> modLoc= locIterPtr->current();
        if (!modLoc->isLocal()) {
          if (debug) { std::cout << "line 343, inserting ";
              std::cout << "reachStmt = " << mIR->toString(reachStmt);
              std::cout << ", stmt = StmtHandle(0)";
              std::cout << std::endl;
          }
          aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));
        }
         else if(modLoc->isaInvisible()) {
            aUDDUChains->insertDefUse(reachStmt,StmtHandle(0));

        }
      }
    }
  }
 
 
  return aUDDUChains;

  
}

/*! Determines whether the given location is definitely referenced by the given
    memory reference.
*/
bool ManagerUDDUChainsStandard::locMustAliasMemRef(OA_ptr<Alias::Interface> alias,
                                        OA_ptr<Location> loc,
                                        MemRefHandle memref)
{
    bool retval = false;

    // get iterator for all locations that the memref must reference
    OA_ptr<LocIterator> locIterPtr = alias->getMustLocs(memref);
    for (; locIterPtr->isValid(); (*locIterPtr)++) {
        if (loc->mustOverlap(*(locIterPtr->current())) ) {
            retval = true;
        }
    }

    return retval;
}
/*! Determines whether the given location can be referenced by the given
    memory reference.
*/
bool ManagerUDDUChainsStandard::locMayAliasMemRef(OA_ptr<Alias::Interface> alias,
                                        OA_ptr<Location> loc,
                                        MemRefHandle memref)
{
    bool retval = false;

    // get iterator for all locations that the memref may reference
    OA_ptr<LocIterator> locIterPtr = alias->getMayLocs(memref);
    for (; locIterPtr->isValid(); (*locIterPtr)++) {
        if (loc->mayOverlap(*(locIterPtr->current())) ) {
            retval = true;
        }
    }

    return retval;
}

  } // end of namespace UDDUChains
} // end of namespace OA
