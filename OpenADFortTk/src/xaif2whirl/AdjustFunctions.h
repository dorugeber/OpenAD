#ifndef ADJUSTFUNCTIONS_H_INCLUDED 
#define ADJUSTFUNCTIONS_H_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

namespace xaif2whirl {

  /**
   * for  functions any 
   * references to global symbol table entries
   * need to be adjusted for active types
   */
  class AdjustFunctions{

  public : 

    AdjustFunctions(PU_Info* aPUInfoForest_p): myPUInfoForest_p(aPUInfoForest_p){
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

      void forSubTree(WN_TREE_CONTAINER<PRE_ORDER>& aWNPtree);
 
      /** 
       * does the symbol meet the criteria
       */
      bool doSymbol(ST* theST_p); 

      /**
       * we don't own this
       */
      PU_Info* myPUInfoForest_p;


  };

}

#endif
