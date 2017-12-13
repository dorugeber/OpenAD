// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef _GUARDEDMAP_INCLUDE_
#include "xaifBooster/utils/inc/GuardedMap.hpp"
#endif 

#include <sstream>
#include "xaifBooster/utils/inc/LogicException.hpp"


namespace xaifBooster { 

  template <class GuardedMapKey, class GuardedMapElement> 
  std::string 
  GuardedMap<GuardedMapKey, GuardedMapElement>::debug() const {
    std::ostringstream out;
      out << "GuardedMap[" << this 
	  << ",#items=" << myMap.size() 
	  << "]" << std::ends; 
      return out.str();
  } // end of GuardedMap::debug

  template <class GuardedMapKey, class GuardedMapElement> 
  const GuardedMapElement&
  GuardedMap<GuardedMapKey, GuardedMapElement>::getElement(const GuardedMapKey& aKey) const {
    typename InternalMapType::const_iterator theFinder=
      myMap.find(aKey);
    if(theFinder==myMap.end()) {
      THROW_EXCEPTION_MACRO(NotFound,"GuardedMap.getElement: no element with key >" << aKey->debug().c_str() << "<");
    }
    return (*theFinder).second;
  } // end of const GuardedMap::getElement

  template <class GuardedMapKey, class GuardedMapElement> 
  GuardedMapElement&
  GuardedMap<GuardedMapKey, GuardedMapElement>::getElement(const GuardedMapKey& aKey) {
    typename InternalMapType::iterator theFinder=
      myMap.find(aKey);
    if(theFinder==myMap.end())
      THROW_EXCEPTION_MACRO(NotFound,"GuardedMap.getElement: no element with key >" << aKey->debug().c_str() << "<");
    return (*theFinder).second;
  } // end of GuardedMap::getElement

  template <class GuardedMapKey, class GuardedMapElement> 
  bool
  GuardedMap<GuardedMapKey, GuardedMapElement>::hasElement(const GuardedMapKey& aKey) const {
    typename InternalMapType::const_iterator theFinder=
      myMap.find(aKey);
    return (theFinder!=myMap.end());
  } // end of GuardedMap::hasElement

  template <class GuardedMapKey, class GuardedMapElement> 
  void
  GuardedMap<GuardedMapKey, GuardedMapElement>::addElement(GuardedMapKey theKey,
					    const GuardedMapElement& theElement) { 
    typename InternalMapType::iterator myMap_iterator=
      myMap.find(theKey);
    if (myMap_iterator==myMap.end()) 
      myMap.insert(std::make_pair(theKey,theElement));
    else 
      THROW_LOGICEXCEPTION_MACRO("GuardedMap::addElement key >" 
				 << theElement->debug().c_str()
			   << "< already exists");
  } // end of GuardedMap<GuardedMapKey, GuardedMapElement>::addElement

  template <class GuardedMapKey, class GuardedMapElement> 
  void 
  GuardedMap<GuardedMapKey, GuardedMapElement>::removeElement(GuardedMapKey theKey) { 
    myMap.erase(theKey);
  } // end of GuardedMap::removeElement

  template <class GuardedMapKey, class GuardedMapElement> 
  const typename GuardedMap<GuardedMapKey, GuardedMapElement>::InternalMapType& 
  GuardedMap<GuardedMapKey, GuardedMapElement>::getInternalMap() const { 
    return myMap;
  }  // end of const GuardedMap::getInternalMap

  template <class GuardedMapKey, class GuardedMapElement> 
  typename GuardedMap<GuardedMapKey, GuardedMapElement>::InternalMapType& 
  GuardedMap<GuardedMapKey, GuardedMapElement>::getInternalMap() { 
    return myMap;
  }  // end of GuardedMap::getInternalMap

} // end of namespace
