// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/WhirlParentize.cxx,v 1.4 2007-10-08 18:28:33 utke Exp $


#include "Open64IRInterface/Open64BasicTypes.h"
#include "WhirlParentize.h"


namespace fortTkSupport { 

  WN* 
  FindParentWNBlock(const WN* wn_tree, const WN* wn)
  {
    if (!wn_tree || !wn) { return NULL; }

    OPERATOR opr = WN_operator(wn_tree);
    if (!OPERATOR_is_scf(opr)) {
      // 'wn_tree' is not structured control flow and cannot contain blocks
      return NULL;
    } else {

      WN* blkWN = NULL;
      if (opr == OPR_BLOCK) {
      
	// Test to see if 'wn' is a child of 'wn_tree'
	WN *kid = WN_first(wn_tree);
	while (kid) {
	
	  // Test this child
	  if (kid == wn) {
	    return const_cast<WN*>(wn_tree); // we found the parent block|
	  } 

	  // Recursively test 
	  if ( (blkWN = FindParentWNBlock(kid, wn)) ) {
	    return blkWN;
	  }

	  kid = WN_next(kid);
	}
      } else {
      
	// Recur on for non-block structured control flow
	for (INT kidno = 0; kidno < WN_kid_count(wn_tree); kidno++) {
	  WN* kid = WN_kid(wn_tree, kidno);
	  if ( (blkWN = FindParentWNBlock(kid, wn)) ) {
	    return blkWN;
	  }
	}

      }

      return NULL; // not found
    }
  }

  //***************************************************************************
    // WhirlParentMap
    //***************************************************************************

      // Note: whirl2f implementation of parentizing for each PU
      // W2F_Push_PU
      //   MEM_POOL_Push(&W2F_Parent_Pool);
      //   W2CF_Parent_Map = WN_MAP_Create(&W2F_Parent_Pool);
      //   W2CF_Parentize(pu);
      // W2F_Pop_PU
      //   WN_MAP_Delete(W2CF_Parent_Map);
      //   W2CF_Parent_Map = WN_MAP_UNDEFINED;
      //   MEM_POOL_Pop(&W2F_Parent_Pool);

    WhirlParentMap::WhirlParentMap() 
    {
      Ctor();
    }

  WhirlParentMap::WhirlParentMap(const WN* wn)
  {
    Ctor();
    Create(wn);
  }

  void 
  WhirlParentMap::Ctor()
  {
    // Create a pool to hold the parent map for every PU, one at a time.
    MEM_POOL_Initialize(&memPool, "WhirlParentMap_Pool", FALSE);
    MEM_POOL_Push(&memPool);   
    parentMap = WN_MAP_UNDEFINED;
  }

  WhirlParentMap::~WhirlParentMap() 
  { 
    MEM_POOL_Pop(&memPool);
    MEM_POOL_Delete(&memPool);
  }

  void 
  WhirlParentMap::Create(const WN* wn)
  {
    if (parentMap != WN_MAP_UNDEFINED) {
      Clear();
    }
  
    parentMap = WN_MAP_Create(&memPool);
    Parentize(wn);
  }

  void 
  WhirlParentMap::Clear()
  {
    WN_MAP_Delete(parentMap);
    parentMap = WN_MAP_UNDEFINED;
  }

  WN* 
  WhirlParentMap::FindBlock(const WN* wn)
  {
    WN* curWN = const_cast<WN*>(wn);
    while ( (curWN = Find(curWN)) ) {
      if (WN_operator(curWN) == OPR_BLOCK) {
	return curWN;
      }
    }
    return NULL;
  }

  // Parentize: Given a tree, initialize its parent pointers, overriding
  // anything that may have been in the map.  Does not update parent
  // pointer of the root node 'wn'.
  void 
  WhirlParentMap::Parentize(const WN* wn)
  {
    OPERATOR opr = WN_operator(wn);
  
    if (!OPERATOR_is_leaf(opr)) {
      if (opr == OPR_BLOCK) { // WN_opcode(wn) == OPC_BLOCK
	WN *kid = WN_first(wn);
	while (kid) {
	  Insert(kid, wn);
	  Parentize(kid);
	  kid = WN_next(kid);
	}
      } else {
	for (INT kidno = 0; kidno < WN_kid_count(wn); kidno++) {
	  WN* kid = WN_kid(wn, kidno);
	  if (kid) { 
	    Insert(kid, wn);
	    Parentize(kid);
	  }
	}
      }
    }
  }

}
