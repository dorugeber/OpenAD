#ifndef CleanUpWhirl_H_INCLUDED 
#define CleanUpWhirl_H_INCLUDED

#include "Open64BasicTypes.h"

/**
 * clean up whirl problems we 
 * cannot fix in the front-end 
 * because of ordering issues in 
 * the processing of one PU at a time
 */
class CleanUpWhirl {

 public : 

  static void forPUInfoForest(PU_Info* aPUInfoForest_p);

 private:

  static void forPUInfoTree(PU_Info* aPUInfoTree_p,
			    PU_Info* aTopPUInfo_p);
  
  static void forPUInfo(PU_Info* aPUInfo_p,
			PU_Info* aTopPUInfo_p);

  static PU_Info* findModulePU(PU_Info* aPUInfoForest_p,
					     ST* moduleName_ST_p);

  static ST* findModuleSymbol(ST* moduleName_ST_p,
			      ST* dummyLocal_ST_p,
			      PU_Info* aTopPUInfo_p); 
};

#endif
