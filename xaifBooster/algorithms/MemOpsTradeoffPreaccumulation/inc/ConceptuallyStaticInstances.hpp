#ifndef _MEMOPSTRADEOFFPREACCUMULATIONCONCEPTUALLYSTATICINSTANCES_INCLUDE_
#define _MEMOPSTRADEOFFPREACCUMULATIONCONCEPTUALLYSTATICINSTANCES_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/BasicBlockAlg.hpp"

namespace MemOpsTradeoffPreaccumulation { 

  class ConceptuallyStaticInstances { 
 
  public : 

    static ConceptuallyStaticInstances* instance();

    ~ConceptuallyStaticInstances();

    enum Heuristic{VERTEX, EDGE, FACE, FORWARD, REVERSE, MARKOWITZ, SIBLING, SIBLING2, SUCCPRED, ABSORB};

    typedef std::list<Heuristic> HeuristicList;

    HeuristicList& getList();

  private: 
    
    ConceptuallyStaticInstances();
    
    /** 
     * no def
     */
    ConceptuallyStaticInstances(const ConceptuallyStaticInstances&);

    /** 
     * no def
     */
    ConceptuallyStaticInstances& operator=(const ConceptuallyStaticInstances&);

    void createList();

    static ConceptuallyStaticInstances* ourInstance_p;

    HeuristicList* myList_p;
  
  }; // end of ConceptuallyStaticInstances
  
} 

#endif 
