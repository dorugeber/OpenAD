// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef  _ITERATORWRAPPER_INCLUDE_
#include "xaifBooster/boostWrapper/inc/IteratorWrapper.hpp"
#endif

#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::IteratorWrapper(BoostGraphType& aBoostGraph,
					  BoostIteratorType& aBoostIterator)
		    : myBoostIterator(aBoostIterator),
		      myBoostGraph(aBoostGraph),
		      myInitFlag(true) {
  }

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>&
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::operator++() { 
    if (!myInitFlag) 
      THROW_LOGICEXCEPTION_MACRO("IteratorWrapper::operator++() instance not initialized");
    ++myBoostIterator; // passing it on
    return *this; 
  }

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>&
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::operator--() { 
    if (!myInitFlag) 
      THROW_LOGICEXCEPTION_MACRO("IteratorWrapper::operator--() instance not initialized");
    --myBoostIterator; // passing it on
    return *this; 
  }

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  Value& 
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::operator*() const { 
    if (!myInitFlag) 
      THROW_LOGICEXCEPTION_MACRO("IteratorWrapper::operator*() instance not initialized");
    return *(boost::get(boost::get(BoostContentType(),
				   myBoostGraph), // get the map
			*myBoostIterator)); // look up the item in the map
  }

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  bool 
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::operator==(const IteratorWrapper& rhs) const { 
    return (myBoostIterator==rhs.myBoostIterator);
  }  

  template <class BoostGraphType,
	    class BoostIteratorType,
	    class BoostContentType,
	    class Value>
  bool 
  IteratorWrapper<BoostGraphType,
		  BoostIteratorType,
		  BoostContentType,
		  Value>::operator!=(const IteratorWrapper& rhs) const { 
    return (myBoostIterator!=rhs.myBoostIterator);
  }  

} // end of namespace
