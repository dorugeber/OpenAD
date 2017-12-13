#ifndef  _ITERATORWRAPPER_INCLUDE_
#define  _ITERATORWRAPPER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

namespace xaifBooster { 

  template<class,class> class GraphWrapper;

  /**
   * wrapper around boost  iterator
   * with the main purpose of returning 
   * property map contents which should hide 
   * the bypass through the descriptor and 
   * map lookup
   * where BoostGraphType and BoostIteratorType
   * are to be specified along with the 
   * value, i.e. the respective vertex or edge
   * we don't bother with postfix in/decrement
   */
  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  class IteratorWrapper { 
  public:

    ~IteratorWrapper() {};

    IteratorWrapper& operator++();

    IteratorWrapper& operator--();

    /** 
     * the dereference operator
     * the main reason for the wrapper class returning 
     * the contents of the map rather than the descriptor
     * which would need a subsequent lookup 
     * in the property maps which we like to hide
     */
    Value& operator*() const;

    bool operator==(const IteratorWrapper& rhs) const;

    bool operator!=(const IteratorWrapper& rhs) const; 

  protected:
    
    template<class,class> friend class GraphWrapper;

    /** 
     * private representation of the boost iterator
     */
    BoostIteratorType myBoostIterator;

    /** 
     * private reference to the boost graph
     * needed to look up the property map 
     * (JU: s we want to store a reference to the map directly?)
     */
    BoostGraphType& myBoostGraph;
    
    /** 
     * internal state to determine proper initialization 
     */
    bool myInitFlag;
    
    /**
     * the ctor not to be called directly but only from 
     * GraphWrapper which is a friend
     */
    IteratorWrapper(BoostGraphType& aBoostGraph,
		    BoostIteratorType& aBoostIterator);

  }; // end of class IteratorWrapper

} // end of namespace

#include "xaifBooster/boostWrapper/inc/TemplateImpl/IteratorWrapper.cpp"

#endif
