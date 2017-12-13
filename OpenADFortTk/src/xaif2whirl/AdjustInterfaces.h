#ifndef ADJUSTINTERFACES_H_INCLUDED 
#define ADJUSTINTERFACES_H_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

/**
 * for local explicit interfaces the front-end generates
 * references to local symbol table 
 * that do not reflect type changes made 
 * to definitions if they exist in the input source.
 * We need to adjust the type information.
 */
class AdjustInterfaces{

 public : 

  AdjustInterfaces(PU_Info* aPUInfoForest_p): myPUInfoForest_p(aPUInfoForest_p){
  };
  
  /** 
   * top level routine that does it
   * for the forest of PU trees
   */
  void doIt();
  
 private:

  /** 
   * recursively for each (sub) tree of PUs
   */ 
  void forPUInfoTree(PU_Info* aPUInfoTree_p);
  
  /** 
   * for each PU within a tree of PUs
   */ 
  void forPUInfo(PU_Info* aPUInfo_p);

  TY_IDX findPUSymbolType(ST* puName_ST_p,
			  ST* dummyLocal_ST_p,
			  INT wnKidIdx); 

  /**
   * find a PU in the forest
   */
  PU_Info* findPU(ST* puName_ST_p);

  /**
   * we don't own this
   */
  PU_Info* myPUInfoForest_p;


};

#endif
