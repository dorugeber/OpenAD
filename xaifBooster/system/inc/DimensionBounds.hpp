#ifndef _DIMENSIONBOUNDS_INCLUDE_
#define _DIMENSIONBOUNDS_INCLUDE_
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
#include "xaifBooster/system/inc/IndexOrder.hpp"

namespace xaifBooster { 

  /** 
   * DimensionBounds for a single dimension of vectors
   * matrices, etc.
   */
  class DimensionBounds : public XMLPrintable {
  public:

    DimensionBounds (int aLower, 
		     int anUpper);

    ~DimensionBounds(){};

    /**
     * print XML hierarchy
     */
    void printXMLHierarchy(std::ostream& os) const;

    /**
     * print debug information
     */
    std::string debug() const ;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for myLower as specified in XAIF schema
     */
    static const std::string our_myLower_XAIFName;

    /**
     * name for myUpper as specified in XAIF schema
     */
    static const std::string our_myUpper_XAIFName;

    int getLower()const;
    int getUpper()const;

    static IndexOrder::IndexOrder_E getIndexOrder();

  private: 

    /** 
     * no def
     */
    DimensionBounds ();

    const int myLower;

    const int myUpper;

    /**
     * eventually we should control this either 
     * through a command line flag or an
     * xaif attribute
     */
    static IndexOrder::IndexOrder_E ourIndexOrder;

  }; // end of class DimensionBounds
 
} // end of namespace xaifBooster
                                                                     
#endif
