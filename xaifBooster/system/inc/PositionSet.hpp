#ifndef _POSITIONSET_INCLUDE_
#define _POSITIONSET_INCLUDE_
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
#include "xaifBooster/utils/inc/Debuggable.hpp"

namespace xaifBooster{ 

  /**
   * class to implement a set of unique 
   * argument positions represented as integers
   * where counting starts at 1 and the 
   * 0 position is reserved for the function value
   */
  class PositionSet: public Debuggable { 
    
  public: 
    PositionSet(){}; 
    ~PositionSet(){};

    /**
     * adds aSet to this Set,
     * duplicate elements are ignored
     */
    void add(const PositionSet& aSet);

    /**
     * adds aPosition to this Set,
     * duplicate elements are ignored
     */
    void add(unsigned int aPosition);

    /** 
     * checks if element is in this set
     */
    bool has(unsigned int aPosition) const;

    std::string debug() const ;

  private:

    typedef std::list<unsigned int> IntList;
    
    /** 
     * the positions held in a list
     * the list is ordered
     */
    IntList myIntList;

  }; // end of class PositionSet

} // end of namespace xaifBooster

#endif
