#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_MARKERALG_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATIONTAPEADJOINT_MARKERALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/MarkerAlgBase.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulationTapeAdjoint/inc/BasicBlockElementAlg.hpp"

using namespace xaifBooster;

namespace xaifBooster { 
  class Marker;
}

namespace xaifBoosterBasicBlockPreaccumulationTapeAdjoint {  

  /** 
   * class to implement reversal of BasicBlockElements
   */
  class MarkerAlg : public MarkerAlgBase,
		    public BasicBlockElementAlg {

  public:
    
    MarkerAlg(const Marker& theContainingMarker);

    virtual ~MarkerAlg(){};

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private: 

    /** 
     * no def
     */
    MarkerAlg();

    /** 
     * no def
     */
    MarkerAlg(const MarkerAlg&);

    /** 
     * no def
     */
    MarkerAlg& operator=(const MarkerAlg&);

  }; // end of class MarkerAlg
 
} 
                                                                     
#endif
