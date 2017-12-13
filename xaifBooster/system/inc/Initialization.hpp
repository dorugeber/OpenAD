#ifndef _INITIALIZATION_INCLUDE_
#define _INITIALIZATION_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/utils/inc/XMLPrintable.hpp"

namespace xaifBooster { 

  /**
   * this class describes 
   * the initialization used in for loops
   */
  class Initialization : public XMLPrintable {
  public:

    Initialization (bool theActiveFlag);

    ~Initialization(){};

    /**
     * print XML hierarchy
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;
    
    Assignment& getAssignment();

    const Assignment& getAssignment() const;

  private:
    
    /** 
     * no def 
     */ 
    Initialization ();

    /** 
     * the assignment
     */
    Assignment myAssignment;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
