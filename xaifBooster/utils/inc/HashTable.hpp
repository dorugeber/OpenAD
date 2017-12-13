#ifndef _HASHTABLE_INCLUDE_
#define _HASHTABLE_INCLUDE_
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

  template <class HashTableElement> 
  class HashTable : virtual public Debuggable {
  public:

    class NotFound : public LogicException {
    public:
        NotFound(std::string aFileName,
		 int aLineNumber,
		 std::string what):
                 LogicException(aFileName,aLineNumber,what){};
    };

    HashTable() {};

    ~HashTable() {};

    virtual std::string debug() const;

    bool hasElement(const std::string& aKey) const;

    HashTableElement& getElement(const std::string& aKey);
    const HashTableElement& getElement(const std::string& aKey) const;

    void addElement(std::string theKey,
		    const HashTableElement& theElement);

    void removeElement(std::string theKey);

    //  protected:
    
    struct myLtOp;

    struct myLtOp {
      bool operator()(const std::string s1, const std::string s2) const {
	return (s1<s2);
      }
    };

    //  public:

    typedef
    std::map<std::string, 
	     HashTableElement, 
	     myLtOp> InternalHashMapType;

  protected:

    InternalHashMapType myHashMap; 
    
  public:
    
    const InternalHashMapType& getInternalHashMap() const;

    InternalHashMapType& getInternalHashMap();
    
  }; // end of class HashTable

} // end of namespace 

#include "xaifBooster/utils/inc/TemplateImpl/HashTable.cpp" 

#endif
