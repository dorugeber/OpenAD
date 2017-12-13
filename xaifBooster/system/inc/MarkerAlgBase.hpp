#ifndef _MARKERALGBASE_INCLUDE_
#define _MARKERALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"

namespace xaifBooster {  
  
  class Marker;

  class MarkerAlgBase: public BasicBlockElementAlgBase { 

  public: 

    MarkerAlgBase(const Marker& theContaining);

    virtual ~MarkerAlgBase();

  private: 

    /** 
     * not defined
     */
    MarkerAlgBase();

    /** 
     * not defined
     */
    MarkerAlgBase(const MarkerAlgBase&);

    /** 
     * not defined
     */
    MarkerAlgBase& operator=(const MarkerAlgBase&);

  }; 

} // end of namespace 

#endif
