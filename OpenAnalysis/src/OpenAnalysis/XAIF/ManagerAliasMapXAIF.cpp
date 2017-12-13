/*! \file
  
  \brief The AnnotationManager that filters UDDUChains for XAIF.

  \authors Michelle Strout
  \version $Id: ManagerAliasMapXAIF.cpp,v 1.18 2005/03/17 21:47:47 mstrout Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "ManagerAliasMapXAIF.hpp"
#include <Utils/Util.hpp>

namespace OA {
  namespace XAIF {

    bool debug = false;

    /*!
     */
    ManagerAliasMapXAIF::ManagerAliasMapXAIF(OA_ptr<XAIFIRInterface> _ir) : mIR(_ir) {
      OA_DEBUG_CTRL_MACRO("DEBUG_ManagerAliasMapXAIF:ALL", debug);
    }

    /*!
     */
    OA_ptr<XAIF::AliasMapXAIF> ManagerAliasMapXAIF::performAnalysis(ProcHandle proc, 
								    OA_ptr<Alias::Interface> alias) {
      OA_ptr<AliasMapXAIF> retAliasMap; retAliasMap = new AliasMapXAIF(proc);

      if(debug) {
	std::cout << "The Procedure is" << mIR->toString(proc) << std::endl;
      }

      //---------------------------------
      // First iterate over all locations and collect the locations
      // into groups based on their base symbol or stmt.
      // Therefore all locations that are statically equivalenced will result
      // in only one entry and all locations that are partial or full subsets
      // of a datastructure like an array will be in the same set.
      // The sets will be indexed by SymHandle or StmtHandle

      // named locations indexed by symbol and invisible locations indexed by base location symbol
      std::map<SymHandle,LocSet> symToLocSetMap;
      // unnamed locations indexed by StmtHandle
      std::map<ExprHandle,LocSet> exprToLocSetMap;
  
      // for each stmt
      for (OA_ptr<OA::IRStmtIterator> sItPtr = mIR->getStmtIterator(proc); sItPtr->isValid(); (*sItPtr)++) {
	OA::StmtHandle stmt = sItPtr->current();
	if (debug) {
	  std::cout << "===========================" << std::endl << "stmt = ";
	  mIR->dump(stmt,std::cout);
	  std::cout << std::endl;
	}

	// get all memory references
	for (OA_ptr<MemRefHandleIterator> mrItPtr = mIR->getAllMemRefs(stmt); mrItPtr->isValid(); (*mrItPtr)++) {
	  MemRefHandle memref = mrItPtr->current();
	  if (debug) {
	    std::cout << "memref = " << mIR->toString(memref) << std::endl;
	  }

	  // get locations and insert into appropriate set
	  for (OA_ptr<LocIterator> mayIterPtr = alias->getMayLocs(memref); mayIterPtr->isValid(); (*mayIterPtr)++) {
	    OA_ptr<Location> loc = mayIterPtr->current();
          
	    if (loc->isaNamed()) { // NamedLoc
	      OA_ptr<NamedLoc> namedLoc = loc.convert<NamedLoc>();
	      SymHandle sym = namedLoc->getSymHandle();
	      symToLocSetMap[sym].insert(namedLoc.convert<Location>());
	    }
	    else if (loc->isaUnnamed()) { // UnnamedLoc
	      OA_ptr<UnnamedLoc> unnamedLoc = loc.convert<UnnamedLoc>();
	      ExprHandle expr = unnamedLoc->getExprHandle();
	      exprToLocSetMap[expr].insert(unnamedLoc.convert<Location>());
          
	    }
	    else if (loc->isaInvisible()) { // Invisible Loc
	      OA_ptr<InvisibleLoc> invLoc = loc.convert<InvisibleLoc>();
	      OA_ptr<MemRefExpr> mre = invLoc->getMemRefExpr();
	      assert(mre->isaRefOp());
	      OA_ptr<RefOp> refOp = mre.convert<RefOp>();
	      symToLocSetMap[refOp->getBaseSym()].insert(loc);

	    }
	    else if (loc->isaSubSet()) { // LocSubSets
	      // get base location from locsubset
	      OA_ptr<Location> base = loc->getBaseLoc();

	      // FIXME: refactor with above
	      if (base->isaNamed()) { // NamedLoc
		OA_ptr<NamedLoc> namedLoc = base.convert<NamedLoc>();
		SymHandle sym = namedLoc->getSymHandle();
		symToLocSetMap[sym].insert(loc);
                  
	      }
	      else if (base->isaUnnamed()) { // UnnamedLoc
		OA_ptr<UnnamedLoc> unnamedLoc = base.convert<UnnamedLoc>();
		ExprHandle expr = unnamedLoc->getExprHandle();
		exprToLocSetMap[expr].insert(loc);
	      }
	      else if (base->isaInvisible()) { // Invisible Loc
		OA_ptr<InvisibleLoc> invLoc = base.convert<InvisibleLoc>();
		OA_ptr<MemRefExpr> mre = invLoc->getMemRefExpr();
		assert(mre->isaRefOp());
		OA_ptr<RefOp> refOp = mre.convert<RefOp>();
		symToLocSetMap[refOp->getBaseSym()].insert(loc);
	      }
	    } // end LocSubSets
	    else if (loc->isaUnknown()) { // UnknownLoc
	      // do nothing
	    }
	  }
	}
      }
              
      //---------------------------------
      // Create a location tuple for all locations based on the set they
      // are a part of and by maintaining a unique id.
      int uniqueId = 1;

      std::map<OA_ptr<Location>,LocTuple> locToLocTuple;
      std::map<SymHandle,LocSet>::iterator symMapIter;
      if (debug) {std::cout << "===========DUMP LOCSETMAP================" << std::endl;}
      for (symMapIter=symToLocSetMap.begin(); symMapIter!=symToLocSetMap.end(); symMapIter++) {
	// within each location set count the number of LocSubSets that are partial but have full accuracy,
	// that is the number of virtual location ids that will be needed if have any partial accuracy
	// sub set locations then will add in two virtual location ids
	int count = 0;
	bool partialFlag = false;
	LocSet::iterator locIter;
	LocSet locSet;
	locSet = symMapIter->second;
	for (locIter=locSet.begin(); locIter!=locSet.end(); locIter++) {
	  OA_ptr<Location> loc = *locIter;
	  if (debug) { std::cout << "loc = "; loc->dump(std::cout,mIR); }

	  if (loc->isaSubSet()) {
	    if (partialFlag==false) { // first time we see this
	      count += 2;  // 2 spaces for all partial references to a loc
	    }
	    partialFlag = true;
	  }
	  else if(loc->isaNamed()) {
	    count++;
	  }
	  else if(loc->isaUnnamed()) { 
	    count++;
	  }
	  else if(loc->isaUnknown()) { 
	    count++;
	  }
	  else if(loc->isaInvisible()) { 
	    OA_ptr<InvisibleLoc> invisibleLoc = loc.convert<InvisibleLoc>();
	    OA_ptr<MemRefExpr> mre = invisibleLoc->getMemRefExpr();
	    assert(mre->isaRefOp());
	    OA_ptr<RefOp> refOp = mre.convert<RefOp>();
	    if(refOp->isaSubSetRef()) {
	      count += 2;
	    }
	    else {
	      count++;
	    }
	  }
	}
	if (count == 0) { count = 1; }
	if (debug) { std::cout << "count = " << count << std::endl; }
      
	// if there were partials then start currentOffset 2 in
	int currentSubOffset = 0;
	if (partialFlag==true) {
	  currentSubOffset = 2;
	} 
	if (debug) {std::cout << "------------SET locToLocTuple--------------" << std::endl;}
	// now loop over all locations in this list and map to a LocTuple
	for (locIter=locSet.begin(); locIter!=locSet.end(); locIter++) {
	  OA_ptr<Location> loc = *locIter;

	  if (loc->isaSubSet()) {
	    OA_ptr<LocSubSet> subLoc = loc.convert<LocSubSet>();
	    locToLocTuple[loc] = LocTuple(uniqueId,uniqueId+count-1,false);
	  }
	  else {
	    locToLocTuple[loc] = LocTuple(uniqueId,uniqueId+count-1,true);
	  }
	  if (debug) { std::cout << "locToLocTuple["; loc->dump(std::cout,mIR); std::cout << "]=" << locToLocTuple[loc].getLocRange().getStart() << " - " << locToLocTuple[loc].getLocRange().getEnd() << std::endl; }
	}
	if (debug) {std::cout << "------------DONE locToLocTuple--------------" << std::endl;}
	// must increase uniqueId by at least one
	uniqueId += count;
      }
      if (debug) {std::cout << "===========DONE WITH LOCSETMAP================" << std::endl;}

      if (debug) {std::cout << "===========DUMP exprToLocSetMap================" << std::endl;}
      std::map<ExprHandle,LocSet>::iterator exprMapIter;
      for (exprMapIter=exprToLocSetMap.begin(); exprMapIter!=exprToLocSetMap.end(); exprMapIter++ ) {
	// within each location set count the number of LocSubSets that are partial but have full accuracy,
	// that is the number of virtual location ids that will be needed if have any partial accuracy
	// sub set locations then will add in two virtual location ids
	int count = 0;
	LocSet::iterator locIter;
	LocSet locSet;
	locSet= exprMapIter->second;

	// now loop over all locations in this list and map to a LocTuple
	if (debug) {std::cout << "------------SET locToLocTuple--------------" << std::endl;}
	for (locIter=locSet.begin(); locIter!=locSet.end(); locIter++) {
	  OA_ptr<Location> loc = *locIter;
	  if (loc->isaUnnamed()) {
	    count+=2;
	    locToLocTuple[loc] = LocTuple(uniqueId,uniqueId+count-1,false);
	  }
	  else {
	    assert(0);
	  }
	  if (debug) { std::cout << "locToLocTuple["; loc->dump(std::cout,mIR); std::cout << "]=" << locToLocTuple[loc].getLocRange().getStart() << " - " << locToLocTuple[loc].getLocRange().getEnd() << std::endl; }
	}
	if (debug) {std::cout << "------------DONE locToLocTuple--------------" << std::endl;}
	uniqueId += count;
      }
      if (debug) {std::cout << "===========DONE WITH DUMP exprToLocSetMap================" << std::endl;}

      //---------------------------------
      // Then iterate over the aliasing information again using the newly determined LocTuples 
  
      // get all memory references that alias analysis has info for
      if (debug) {std::cout << "==== MemRefHandles that alias analysis has info for" << std::endl;}
      for (OA_ptr<MemRefHandleIterator> mrItPtr = alias->getMemRefIter(); mrItPtr->isValid(); (*mrItPtr)++) {
	MemRefHandle memref = mrItPtr->current();
	if (debug) { std::cout << "for memref = " << mIR->toString(memref) << std::endl; } 
	// trying to determine what set we will map this memref to
	int setId = Alias::AliasMap::SET_ID_NONE;

	OA_ptr<std::set<LocTuple> > maySet;
	maySet = new std::set<LocTuple>;
	bool foundUnknown = false;
	for (OA_ptr<LocIterator> mayIterPtr = alias->getMayLocs(memref); mayIterPtr->isValid(); (*mayIterPtr)++) {
	  OA_ptr<Location> loc = mayIterPtr->current();
	  if (loc->isaUnknown()) {
	    foundUnknown = true;
	  }
	  else {
	    if (debug) { std::cout << "   inserting locToLocTuple["; loc->dump(std::cout,mIR); std::cout << "]="  << locToLocTuple[loc].getLocRange().getStart() << " - " << locToLocTuple[loc].getLocRange().getEnd() << std::endl; } 
	    maySet->insert(locToLocTuple[loc]);
	  }
	} 

	if (foundUnknown) { // if contains the UnknownLoc just map to special 0 alias map set
	  setId = 0;
	}
	else if (maySet->empty()) {
	  // if the set is empty then map to special 1 alias map set
	  // (this is an addressOf MemRef and does not access a location)
	  setId = 1;
	}
	else { // The loc set is neither unknown nor empty => either find an existing map set or create a new one
	  setId = retAliasMap->findMapSet(maySet);
	  if (setId == Alias::AliasMap::SET_ID_NONE) {
	    setId = getNextSetId();
	    retAliasMap->mapLocTupleSet(maySet,setId); // insert all locations into this new set
	  }
	}
	if (debug) { std::cout << "   mapping memref to setId=" << setId << std::endl; } 
	retAliasMap->mapMemRefToMapSet(memref,setId);
      }
      return retAliasMap;
    } // end ManagerAliasMapXAIF::performAnalysis()

    int ManagerAliasMapXAIF::sCurrentStartId = 2;

    int ManagerAliasMapXAIF::getNextSetId() {
      return sCurrentStartId++;
    } // end ManagerAliasMapXAIF::getNextSetId()

  } // end of namespace XAIF
} // end of namespace OA
