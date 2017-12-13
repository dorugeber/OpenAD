#include "AdjustInterfaces.h"
#include "Diagnostics.h"
#include "Open64IRInterface/WhirlGlobalStateUtils.h"

void AdjustInterfaces::doIt() { 
  if (!myPUInfoForest_p) { return; }
  // Loop through all the PU_Infos 
  PU_Info* aPUInfoTree_p;
  for (aPUInfoTree_p=myPUInfoForest_p; aPUInfoTree_p!= NULL; aPUInfoTree_p=PU_Info_next(aPUInfoTree_p)) {
    forPUInfoTree(aPUInfoTree_p);
  }
} 

void AdjustInterfaces::forPUInfoTree(PU_Info* aPUInfoTree_p) { 
  if (!aPUInfoTree_p) { return; }
  forPUInfo(aPUInfoTree_p);
  for (PU_Info *aPUInfosubtree_p = PU_Info_child(aPUInfoTree_p); 
       aPUInfosubtree_p != NULL; 
       aPUInfosubtree_p = PU_Info_next(aPUInfosubtree_p)) {
    forPUInfoTree(aPUInfosubtree_p);
  }
}

TY_IDX copyTypeAdjust(TY_IDX dummyTypeIdx,
		      TY_IDX properPUTypeIdx) {
  if (dummyTypeIdx==properPUTypeIdx) { 
    FORTTK_MSG(2, "copyType: identical type index >" 
	       << TY_IDX_index(properPUTypeIdx)
	       << "<");
  }
  FORTTK_ASSERT(TY_kind(dummyTypeIdx) != KIND_SCALAR 
		&& 
		TY_kind(properPUTypeIdx) != KIND_SCALAR,
		"copyType: scalar type");
  TY_IDX copiedChildTypeIdx, ty_idx;
  TY& ty = New_TY(ty_idx); // sets 'ty_idx'
  TY_Init(ty, 
	  TY_size(dummyTypeIdx), 
	  TY_kind(dummyTypeIdx),
	  TY_mtype(dummyTypeIdx),
	  TY_name_idx(dummyTypeIdx));
  if (TY_kind(dummyTypeIdx) == KIND_POINTER ) { 
    FORTTK_ASSERT(TY_kind(properPUTypeIdx) == KIND_POINTER,"copyType: not a pointer");
    if (TY_kind(TY_pointed(properPUTypeIdx)) != KIND_SCALAR) {
      copiedChildTypeIdx = copyTypeAdjust(TY_pointed(dummyTypeIdx),
					  TY_pointed(properPUTypeIdx));
      Set_TY_pointed(ty,copiedChildTypeIdx);
    }
    else { 
      FORTTK_ASSERT(TY_kind(TY_pointed(dummyTypeIdx)) == KIND_SCALAR,"copyType: not a scalar");
      Set_TY_pointed(ty,TY_pointed(properPUTypeIdx));
    }
  }
  else if (TY_kind(dummyTypeIdx) == KIND_ARRAY ) { 
    FORTTK_ASSERT(TY_kind(properPUTypeIdx) == KIND_ARRAY,"copyType: not an array");
    if (TY_kind(TY_etype(properPUTypeIdx)) != KIND_SCALAR) {
      copiedChildTypeIdx = copyTypeAdjust(TY_etype(dummyTypeIdx),
					  TY_etype(properPUTypeIdx));
      Set_TY_etype(ty,copiedChildTypeIdx);
    }
    else { 
      FORTTK_ASSERT(TY_kind(TY_etype(dummyTypeIdx)) == KIND_SCALAR,"copyType: not a scalar");
      Set_TY_etype(ty,TY_etype(properPUTypeIdx));
    }
    Set_TY_arb(ty,TY_arb(dummyTypeIdx));
  }
  else 
    FORTTK_DIE("copyType: no logic to handle type kind: " <<  TY_kind(dummyTypeIdx)); 
  return ty_idx;
} 

void AdjustInterfaces::forPUInfo(PU_Info* aPUInfo_p) { 
  PU_SetGlobalState(aPUInfo_p);
  WN* thePU_WN_p = PU_Info_tree_ptr(aPUInfo_p);
  WN_TREE_CONTAINER<PRE_ORDER> aWNPtree(thePU_WN_p);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
  bool skipKids=false;
  while (aWNPtreeIterator != aWNPtree.end()) { 
    WN* curWN_p = aWNPtreeIterator.Wn();
    OPERATOR opr = WN_operator(curWN_p);
    if (opr==OPR_INTERFACE) {  // interfaces
      skipKids=true;
      WN* interfaceFuncWN_p=WN_kid0(curWN_p);
      if (interfaceFuncWN_p) {
        ST* puName_ST_p=WN_st(interfaceFuncWN_p); 
        if (!ST_is_in_module(puName_ST_p)) {  // leave  module procedure interfaces alone
          for (INT kidIdx = 0; kidIdx < WN_kid_count(interfaceFuncWN_p); ++kidIdx) {
            ST* dummyLocal_ST_p=WN_st(WN_kid(interfaceFuncWN_p, kidIdx));
            TY_IDX properPUTypeIndex=findPUSymbolType(puName_ST_p,
          					    dummyLocal_ST_p,
          					    kidIdx);
            TY_IDX dummyLocalTypeIndex=ST_type(dummyLocal_ST_p);
            if (properPUTypeIndex && properPUTypeIndex!=dummyLocalTypeIndex){
              FORTTK_MSG(2,"considering adjustments in interface named " 
          	       << ST_name(puName_ST_p) << " for variable " 
          	       << ST_name(dummyLocal_ST_p) 
          	       << " from "
          	       << TY_IDX_index(ST_type(dummyLocal_ST_p))
          	       << " to " 
          	       << TY_IDX_index(properPUTypeIndex)); 
              if (TY_kind(dummyLocalTypeIndex) != KIND_SCALAR) { 
                properPUTypeIndex=copyTypeAdjust(dummyLocalTypeIndex,
          				       properPUTypeIndex);
              }
              else { 
                FORTTK_ASSERT(TY_kind(properPUTypeIndex) == KIND_SCALAR, 
          		    "AdjustInterfaces::forPUInfo: type kind mismatch for symbol " 
          		    << ST_name(dummyLocal_ST_p) << " referenced in " << ST_name(puName_ST_p));
              }
              FORTTK_MSG(2,"in interface named " 
          	       << ST_name(puName_ST_p) << " adjusting type for variable " 
          	       << ST_name(dummyLocal_ST_p) 
          	       << " from "
          	       << TY_IDX_index(ST_type(dummyLocal_ST_p))
          	       << " to " 
          	       << TY_IDX_index(properPUTypeIndex)); 
              // do the surgery on the type in the symbol table 
              Set_ST_type(dummyLocal_ST_p,properPUTypeIndex);
            }
          }
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

TY_IDX AdjustInterfaces::findPUSymbolType(ST* puName_ST_p,
					  ST* dummyLocal_ST_p,
					  INT wnKidIdx) { 
  TY_IDX theTypeIndex=0;
  PU_Info* thePU=findPU(puName_ST_p);
  if (!thePU)
    return theTypeIndex; 
  const char* dummyLocalName=ST_name(dummyLocal_ST_p);
  const char* puName=ST_name(puName_ST_p);
  // temporarily reset the global state
  PU_Info* currentPUI=Current_PU_Info;
  PU_SetGlobalState(thePU);
  WN* thePU_WN_p = PU_Info_tree_ptr(thePU);
  WN_TREE_CONTAINER<PRE_ORDER> aWNPtree(thePU_WN_p);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
  bool skipKids=false;
  while (aWNPtreeIterator != aWNPtree.end()) { 
    WN* curWN_p = aWNPtreeIterator.Wn();
    OPERATOR opr = WN_operator(curWN_p);
    if (opr==OPR_FUNC_ENTRY && strcmp(ST_name(WN_st(curWN_p)),puName)==0) {
      // found it, now go by position: 
      if (WN_kid(curWN_p,wnKidIdx) && WN_has_sym(WN_kid(curWN_p,wnKidIdx))) { 
	FORTTK_MSG(2,"for " << puName << " matched " << dummyLocalName << " to " << ST_name(WN_st(WN_kid(curWN_p,wnKidIdx))) << " for position " << wnKidIdx);
	theTypeIndex=ST_type(WN_st(WN_kid(curWN_p,wnKidIdx)));
	PU_SetGlobalState(currentPUI);
	return theTypeIndex; 
      }
    }
    ++aWNPtreeIterator;
  }
  PU_SetGlobalState(currentPUI);
  FORTTK_ASSERT_WARN(0, "AdjustInterfaces::findPUSymbol: symbol " << dummyLocalName << " referenced in interface " << puName << " not found in the definition, go by parameter position ");
  return theTypeIndex; 	
} 

PU_Info* AdjustInterfaces::findPU(ST* puName_ST_p) { 
  PU_Info* aPUInfoTree_p;
  for (aPUInfoTree_p=myPUInfoForest_p; aPUInfoTree_p!= NULL; aPUInfoTree_p=PU_Info_next(aPUInfoTree_p)) {
    if (aPUInfoTree_p->proc_sym==ST_st_idx(*puName_ST_p))
      return aPUInfoTree_p;
  }
  FORTTK_ASSERT_WARN(0,"AdjustInterfaces::findPU: cannot find definition for interface " << ST_name(puName_ST_p));
  return 0; 
}
		
