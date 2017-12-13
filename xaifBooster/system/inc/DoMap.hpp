#ifndef _DOMAP_INCLUDE_
#define _DOMAP_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/system/inc/StatementIdSetMap.hpp"

namespace xaifBooster { 

  /**
   * map to hold DoMapEntry information 
   */
  class DoMap : public StatementIdSetMap,
		public XMLPrintable{

  public:
    
    DoMap(){};

    ~DoMap(){};
    
    static const std::string ourXAIFName;

    void printXMLHierarchy(std::ostream& os) const; 

  private: 

    /**
     * no def
     */
    DoMap(const DoMap&);

    /**
     * no def
     */
    DoMap& operator=(const DoMap&);

  }; // end of class DoMap

} // end of namespace 
                                                                     
#endif
