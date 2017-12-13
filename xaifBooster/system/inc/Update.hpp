#ifndef _UPDATE_INCLUDE_
#define _UPDATE_INCLUDE_
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
   * the update used in for loops
   * Note that in XAIF it can be in place 
   * of an Assignment tag so we reuse 
   * PlainAssignment tag definitions
   */
  class Update : public XMLPrintable {
  public:

    Update (bool theActiveFlag);

    ~Update(){};

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
     * ho def 
     */
    Update ();

    /** 
     * the assignment
     */
    Assignment myAssignment;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
