#ifndef INTERFACEDATA_H_INCLUDED 
#define INTERFACEDATA_H_INCLUDED

#include <list>
#include <utility>
#include "Open64IRInterface/Open64BasicTypes.h"

/**
 * collect info about interfaces
 */
class InterfaceData{

 public : 

  static void collect(PU_Info* aPUInfoForest_p);

  static void dump();

  static void findAndAdjustInterface(PU_Info* pu,
				     WN* oldWN_p,
				     ST* newST_p);
  
  static PU_Info* getParentOf(PU_Info* child);

  /**
   * child to parent map
   */
  typedef std::map<PU_Info*,PU_Info*> ChildToParentMap;


  /** 
   *  FUNC_ENTRY/INTERFACE and new ST* (if renamed) 
   */
  typedef std::pair<WN*,ST*> RenamePair;

  /** 
   * list of the above  in the interface block
   * (reminder: cannot use a std::set here because we need 
   * modifiable elements to rename (ST*) name if needed)
   */
  typedef std::list<RenamePair> RenamePairList; 

  /** 
   * pair with INTERFACE node as key
   */
  typedef std::pair<RenamePair, RenamePairList> IFaceRenamePairListPair; 

  /** 
   * list of the above
   */
  typedef std::list<IFaceRenamePairListPair> IFaceRenamePairListPairList;
  /** 
   * paired with PU node
   */
  typedef std::pair<PU_Info*, IFaceRenamePairListPairList> PuIFaceRenamePairListPairListPair;
  /** 
   * list of the above
   */
  typedef std::list<PuIFaceRenamePairListPairListPair> PuIFaceRenamePairListPairListPairList;

 private:

  /**
   * the data structure we collect into:
   */
  static PuIFaceRenamePairListPairListPairList ourInterfacesByPU;

  /** 
   * track parents 
   */
  static ChildToParentMap ourChildToParentMap;

  /** 
   * recursively for each (sub) tree of PUs
   */ 
  static void forPUInfoTree(PU_Info* aPUInfoTree_p);
  
  /** 
   * for each PU within a tree of PUs
   */ 
  static void forPUInfo(PU_Info* aPUInfo_p);

  static PuIFaceRenamePairListPairListPair* findPUData(PU_Info* aPUInfo_p);

  /**
   *find by 'old' name
   */
  static IFaceRenamePairListPair* findInterfaceData(PuIFaceRenamePairListPairListPair* moduleData, 
						    WN* theInterface_WN_p);

  /**
   *find by 'old' name
   */
  static InterfaceData::RenamePair* findSpecific(IFaceRenamePairListPair* interfaceData, 
						 WN* theInterface_WN_p); 


};

#endif
