#ifndef _GUARDEDMAP_INCLUDE_
#define _GUARDEDMAP_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include <map>

#include "Debuggable.hpp"
#include "LogicException.hpp"

namespace xaifBooster { 

  template <class GuardedMapKey, class GuardedMapElement> 
  class GuardedMap : virtual public Debuggable {
  public:

    class NotFound : public LogicException {
    public:
        NotFound(std::string aFileName,
		 int aLineNumber,
		 std::string what):
                 LogicException(aFileName,aLineNumber,what){};
    };

    GuardedMap() {};

    ~GuardedMap() {};

    virtual std::string debug() const;

    bool hasElement(const GuardedMapKey& aKey) const;

    GuardedMapElement& getElement(const GuardedMapKey& aKey);
    const GuardedMapElement& getElement(const GuardedMapKey& aKey) const;

    void addElement(GuardedMapKey theKey,
		    const GuardedMapElement& theElement);

    void removeElement(GuardedMapKey theKey);

    //  protected:
    
    struct myLtOp;

    struct myLtOp {
      bool operator()(const GuardedMapKey s1, const GuardedMapKey s2) const {
	return (s1<s2);
      }
    };

    //  public:

    typedef
    std::map<GuardedMapKey, 
	     GuardedMapElement, 
	     myLtOp> InternalMapType;

  protected:

    InternalMapType myMap; 
    
  public:
    
    const InternalMapType& getInternalMap() const;

    InternalMapType& getInternalMap();
    
  }; // end of class GuardedMap

} // end of namespace 

#include "xaifBooster/utils/inc/TemplateImpl/GuardedMap.cpp" 

#endif
