#include "InterfaceData.h"
#include "Diagnostics.h"
#include "Open64IRInterface/WhirlGlobalStateUtils.h"

InterfaceData::PuIFaceRenamePairListPairListPairList InterfaceData::ourInterfacesByPU;
InterfaceData::ChildToParentMap InterfaceData::ourChildToParentMap;

void InterfaceData::collect(PU_Info* aPUForrest_p) { 
  if (!aPUForrest_p) { return; }
  // Loop through all the PU_Infos 
  PU_Info* aPUInfoTree_p;
  for (aPUInfoTree_p=aPUForrest_p; aPUInfoTree_p!= NULL; aPUInfoTree_p=PU_Info_next(aPUInfoTree_p)) {
    forPUInfoTree(aPUInfoTree_p);
  }
} 

void InterfaceData::forPUInfoTree(PU_Info* aPUInfoTree_p) { 
  if (!aPUInfoTree_p) { return; }
  forPUInfo(aPUInfoTree_p);
  for (PU_Info *aPUInfosubtree_p = PU_Info_child(aPUInfoTree_p); 
       aPUInfosubtree_p != NULL; 
       aPUInfosubtree_p = PU_Info_next(aPUInfosubtree_p)) {
    ourChildToParentMap[aPUInfosubtree_p]=aPUInfoTree_p;
    forPUInfoTree(aPUInfosubtree_p);
  }
}

PU_Info* InterfaceData::getParentOf(PU_Info* child) { 
  ChildToParentMap::iterator it(ourChildToParentMap.find(child));
  if (it!=ourChildToParentMap.end()) 
    return (*it).second;
  return NULL; 
} 

InterfaceData::PuIFaceRenamePairListPairListPair* 
InterfaceData::findPUData(PU_Info* thePU_WN_p) { 
  for(PuIFaceRenamePairListPairListPairList::iterator it=ourInterfacesByPU.begin(); 
      it!=ourInterfacesByPU.end();
      ++it) { 
    if (WN_st_idx(PU_Info_tree_ptr(thePU_WN_p))==WN_st_idx(PU_Info_tree_ptr((*it).first))) { 
      return &(*it);
    }
  }
  return NULL; 
}

InterfaceData::IFaceRenamePairListPair* 
InterfaceData::findInterfaceData(PuIFaceRenamePairListPairListPair* moduleData, 
				 WN* theInterface_WN_p) { 
  for(IFaceRenamePairListPairList::iterator it=moduleData->second.begin(); 
      it!=moduleData->second.end();
      ++it) {
    if ((*it).first.second==NULL) { // not renamed
      if (WN_st_idx(theInterface_WN_p)==WN_st_idx((*it).first.first)) { 
	return &(*it);
      }
    } 
    else { // is renamed 
      if (WN_st_idx(theInterface_WN_p)==ST_st_idx((*it).first.second)) { 
	return &(*it);
      }
    }
  }
  return NULL; 
}

InterfaceData::RenamePair* 
InterfaceData::findSpecific(IFaceRenamePairListPair* interfaceData, 
			    WN* theInterface_WN_p) { 
  for(RenamePairList::iterator it=interfaceData->second.begin(); 
      it!=interfaceData->second.end();
      ++it) { 
    if ((*it).second==NULL) { // not renamed
      if (WN_st_idx(theInterface_WN_p)==WN_st_idx((*it).first)) { 
	return &(*it);
      }
    }
    else { // is renamed
      if (WN_st_idx(theInterface_WN_p)==ST_st_idx((*it).second)) { 
	return &(*it);
      }
    }      
  }
  return NULL; 
}

void InterfaceData::forPUInfo(PU_Info* aPUInfo_p) { 
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
      PuIFaceRenamePairListPairListPair* ifDataByModule;
      if ((ifDataByModule=findPUData(aPUInfo_p))==NULL) { 
	// don't have the module yet
	ourInterfacesByPU.push_back(PuIFaceRenamePairListPairListPair(aPUInfo_p,IFaceRenamePairListPairList()));
	ifDataByModule=findPUData(aPUInfo_p);
      }
      IFaceRenamePairListPair* ifDataByInterface;
      if ((ifDataByInterface=findInterfaceData(ifDataByModule,curWN_p))==NULL) { 
	// don't have the interface yet
	ifDataByModule->second.push_back(IFaceRenamePairListPair(RenamePair(curWN_p,NULL),
									 RenamePairList()));
	ifDataByInterface=findInterfaceData(ifDataByModule,curWN_p);
      }
      for (int i=0; i< WN_kid_count(curWN_p); ++i) { 
	WN* specificWN_p=WN_kid(curWN_p,i);
	if (specificWN_p && WN_operator(specificWN_p)==OPR_FUNC_ENTRY) {
	  RenamePair* specificData;
	  if ((specificData=findSpecific(ifDataByInterface,specificWN_p))==0) {
	    // don't have it yet
	    ifDataByInterface->second.push_back(RenamePair(specificWN_p,NULL));
	    specificData=findSpecific(ifDataByInterface,specificWN_p);
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

void InterfaceData::dump() { 
  PU_Info* currentPUI=Current_PU_Info;
  std::cout << "InterfaceData::dumpData: start" << std::endl; 
  for(PuIFaceRenamePairListPairListPairList::iterator mIt=ourInterfacesByPU.begin(); 
      mIt!=ourInterfacesByPU.end();
      ++mIt) { 
    PU_SetGlobalState((*mIt).first);
    std::cout << "\tPU: ";
    if (WN_st(PU_Info_tree_ptr((*mIt).first))) { 
      std::cout << ST_name(WN_st(PU_Info_tree_ptr((*mIt).first)));
    }
    else { 
      std::cout << "<NULL>";
    }
    std::cout << std::endl;
    for(IFaceRenamePairListPairList::iterator iIt=(*mIt).second.begin(); 
	iIt!=(*mIt).second.end();
	++iIt) { 
      std::cout << "\t\tIF:(curr:";
      if (WN_st((*iIt).first.first)) { 
	std::cout << ST_name(WN_st((*iIt).first.first));
      }
      else { 
	std::cout << "<NULL>";
      }
      std::cout << ",old:";
      if ((*iIt).first.second) { 
	std::cout << ST_name((*iIt).first.second);
      }
      else { 
	std::cout << "<NULL>";
      }
      std::cout << ")" <<  std::endl;
      for (RenamePairList::iterator sIt=(*iIt).second.begin();
	   sIt!=(*iIt).second.end();
	   ++sIt) { 
	std::cout << "\t\t\tSpec:(curr:";
	if (WN_st((*sIt).first)) { 
	  std::cout << ST_name(WN_st((*sIt).first));
	}
	else {
	  std::cout << "<NULL>";
	}
	std::cout << ",old:";
	if ((*sIt).second){ 
	  std::cout << ST_name((*sIt).second);
	}
	else { 
	  std::cout << "<NULL>";
	}
	std::cout << ")" <<  std::endl;
      }
    }
  }
  std::cout << "InterfaceData::dumpData: stop" << std::endl; 
  PU_SetGlobalState(currentPUI);
}
 
void InterfaceData::findAndAdjustInterface(PU_Info* pu,WN* oldWN_p, ST* newST_p) {
  std::cout << "InterfaceData::findAndAdjustInterface(" 
	    << ST_name(WN_st(PU_Info_tree_ptr(pu)))
	    << ", "
	    << ST_name(WN_st(oldWN_p))
	    << ","
	    << ST_name(newST_p)
	    << ")" << std::endl; 
  PuIFaceRenamePairListPairListPair* ifDataByModule;
  if ((ifDataByModule=findPUData(pu))==NULL) { 
    // don't have the PU
    FORTTK_WMSG("InterfaceData::findAndAdjustInterface: PU " 
		<< ST_name(WN_st(PU_Info_tree_ptr(pu))) 
		<< " unknown");
    return;
  }
  IFaceRenamePairListPair* ifDataByInterface(NULL);
  int instancesFound=0;
  bool alreadyRenamed=false; 
  RenamePair* renamePair(NULL);
  if ((ifDataByInterface=findInterfaceData(ifDataByModule,oldWN_p))==NULL) { 
    // may be this is a specific name: 
    for(IFaceRenamePairListPairList::iterator iIt=ifDataByModule->second.begin(); 
	iIt!=ifDataByModule->second.end();
	++iIt) {
      RenamePair* tmpRenamePair(NULL);
      if (tmpRenamePair=findSpecific(&(*iIt),oldWN_p)) { 
	renamePair=tmpRenamePair;
	instancesFound+=1;
      }
    }
  }
  if (instancesFound>1)
    FORTTK_DIE("InterfaceData::findAndAdjustInterface: more than one match for " << ST_name(WN_st(oldWN_p)));
  if (ifDataByInterface==NULL and instancesFound==0)  // no interfaces known for 'old'
    return; 
  if (ifDataByInterface) // is a generic name
    renamePair=&(ifDataByInterface->first);
  // see if it was renamed already
  if (renamePair->second) { 
    if (WN_st_idx(renamePair->first)!=ST_st_idx(newST_p)) 
          FORTTK_DIE("InterfaceData::findAndAdjustInterface: cannot rename " 
		     << ST_name(renamePair->second) 
		     << " to "
		     << ST_name(newST_p)
		     << " because it is already renamed to "
		     << ST_name(WN_st(renamePair->first)));
    return; // already renamed
  }
  renamePair->second=WN_st(renamePair->first);
  WN_st_idx(renamePair->first)=ST_st_idx(newST_p);
} 
