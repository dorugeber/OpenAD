#include "Open64IRInterface/WhirlGlobalStateUtils.h"
#include "Open64IRInterface/wn_attr.h"
#include "AdjustFunctions.h"
#include "Diagnostics.h"
#include "XlateExpression.h"

namespace xaif2whirl { 

  extern TY_IDX ActiveTypeTyIdx;     
  extern TY_IDX ActiveTypeInitializedTyIdx; 

  void AdjustFunctions::doIt() { 
    if (!myPUInfoForest_p) { return; }
    // Loop through all the PU_Infos 
    PU_Info* aPUInfoTree_p;
    for (aPUInfoTree_p=myPUInfoForest_p; aPUInfoTree_p!= NULL; aPUInfoTree_p=PU_Info_next(aPUInfoTree_p)) {
      forPUInfoTree(aPUInfoTree_p);
    }
  } 

  void AdjustFunctions::forPUInfoTree(PU_Info* aPUInfoTree_p) { 
    if (!aPUInfoTree_p) { return; }
    forPUInfo(aPUInfoTree_p);
    for (PU_Info *aPUInfosubtree_p = PU_Info_child(aPUInfoTree_p); 
	 aPUInfosubtree_p != NULL; 
	 aPUInfosubtree_p = PU_Info_next(aPUInfosubtree_p)) {
      forPUInfoTree(aPUInfosubtree_p);
    }
  }

  bool AdjustFunctions::doSymbol(ST* theST_p) { 
    return (ST_level(theST_p)==1 
	    &&
	    (ST_type(theST_p)==ActiveTypeTyIdx
	     ||
	     ST_type(theST_p)==ActiveTypeInitializedTyIdx));
  } 

  void AdjustFunctions::forSubTree(WN_TREE_CONTAINER<PRE_ORDER>& aWNPtree) { 
    WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
    bool skipKids=false;
    while (aWNPtreeIterator != aWNPtree.end()) { 
      WN* currWN_p = aWNPtreeIterator.Wn();
      OPERATOR opr = WN_operator(currWN_p);
      if (WN_has_sym(currWN_p)
	  &&
	  doSymbol(WN_st(currWN_p))) { 
	switch (opr) { 
	case OPR_STID: {
	    // do the rhs first :
	    WN_TREE_CONTAINER<PRE_ORDER> subTree(WN_kid0(currWN_p));
	    forSubTree(subTree);
	    // make a new LDID
	    WN* newLDID_p=WN_Ldid(TY_mtype(WN_Tree_Type(currWN_p)),
				  0,
				  WN_st_idx(currWN_p),
				  0,
				  0);
	    WN* wrappedLDID_p=XlateExpression::createValueSelector(newLDID_p);
	    // make a new ISTORE
	    TY_IDX lhsTY_IDX=WN_Tree_Type(wrappedLDID_p);
	    WN* newIstore_p = WN_Istore(TY_mtype(lhsTY_IDX), 
					0, 
					lhsTY_IDX, 
					wrappedLDID_p, 
					WN_COPY_Tree(WN_kid0(currWN_p)) , 
					0);
	    aWNPtreeIterator.Replace(newIstore_p);
	    skipKids=true;
	    break;
	  }
	default: { 
	  aWNPtreeIterator.Replace(XlateExpression::createValueSelector(currWN_p));
	  skipKids=true;
	  break;
	}
	}
      }
      // advance the iterator
      if (skipKids){
	aWNPtreeIterator.WN_TREE_next_skip();
	skipKids=false;
      }
      else
	++aWNPtreeIterator;
    }

  }

  void AdjustFunctions::forPUInfo(PU_Info* aPUInfo_p) { 
    PU_SetGlobalState(aPUInfo_p);
    WN* thePU_WN_p = PU_Info_tree_ptr(aPUInfo_p);
    WN_TREE_CONTAINER<PRE_ORDER> aWNPtree(thePU_WN_p);
    WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
    bool skipKids=false;
    while (aWNPtreeIterator != aWNPtree.end()) { 
      WN* currWN_p = aWNPtreeIterator.Wn();
      OPERATOR opr = WN_operator(currWN_p);
      if (opr==OPR_FUNC_ENTRY) {
	ST* st_p=WN_st(currWN_p);
	TY_IDX funtype(ST_pu_type(st_p));
	TY_IDX return_ty = TY_ret_type(funtype);
	if (!(return_ty != (TY_IDX) 0 && TY_kind(return_ty) != KIND_VOID)) { 
	  // not a function
	  // std::cout << "skipping over :" << ST_name(st_p) << std::endl; 
	  skipKids=true;
	}
	else { 
	  // std::cout << "not  skipping over :" << ST_name(st_p) << std::endl; 
	  WN_TREE_CONTAINER<PRE_ORDER> subTree(currWN_p);
	  forSubTree(subTree);
	} 
      }
      // advance the iterator
      if (skipKids){
	aWNPtreeIterator.WN_TREE_next_skip();
	skipKids=false;
      }
      else
	++aWNPtreeIterator;
    }
  }

} 

