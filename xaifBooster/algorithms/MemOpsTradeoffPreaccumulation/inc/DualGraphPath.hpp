#ifndef _DUALGRAPHPATH_INCLUDE_
#define _DUALGRAPHPATH_INCLUDE_
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
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphPath.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/DualGraphVertex.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace MemOpsTradeoffPreaccumulation { 

  class DualGraphPath { 
    
  public: 

    DualGraphPath();
    ~DualGraphPath(){};

    typedef std::list<DualGraphVertex*> Path;
    Path myPath;
    unsigned int pathNum;

    void setComplete();
    bool isComplete() const;
    
  private:
    
    bool complete;

  }; // end of class

}// end of namespace

#endif 
