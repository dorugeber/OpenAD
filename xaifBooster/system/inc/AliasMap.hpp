#ifndef _ALIASMAP_INCLUDE_
#define _ALIASMAP_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <vector>
#include <list>
#include "xaifBooster/utils/inc/XMLPrintable.hpp"

namespace xaifBooster { 

  class AliasMapEntry;
  class AliasMapKey;


  /**
   * map to hold AliasMapEntry information 
   * supplied from the front end through XML
   */
  class AliasMap : public XMLPrintable {

  public:
    
    AliasMap(){};

    ~AliasMap();

    /**
     * adds the AliasMapEntry to the table, 
     * the key is supposed to be an integer
     * given as a string that will be converted
     */
    AliasMapEntry&  
    addAliasMapEntry(const std::string& aKey);

    typedef std::list<const AliasMapKey*> AliasMapKeyPList;

    /**
     * check disjunction of alias sets 
     */
    bool mayAlias(const AliasMapKey& theKey, const AliasMapKeyPList& theList) const;

    /**
     * check disjunction of alias sets 
     */
    bool mayAlias(const AliasMapKey& theKey, const AliasMapKey& theOtherKey) const;

    /** 
     * establish must alias
     * this will throw an exception if we involve temporaries, 
     * i.e. right now we don't support identification of temporaries...
     */
    bool mustAlias(const AliasMapKey& theKey,
		   const AliasMapKey& theOtherKey) const;

    /**
     * check if addresses in theKey are a subset of addresses associated with keys in theList 
     */
    bool subSet(const AliasMapKey& theKey, const AliasMapKeyPList& theList) const;

    std::string debug() const ; 

    void printXMLHierarchy(std::ostream& os) const; 

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

  private: 

    /**
     * since this map is potentially large
     * and has random access so
     * we use a vector as representation
     * and convert all XML integer key strings into 
     * ints.
     */
    typedef std::vector<AliasMapEntry*> AAVector;

    AAVector myAAVector;

    /**
     * no def
     */
    AliasMap(const AliasMap&);

    /**
     * no def
     */
    AliasMap& operator=(const AliasMap&);

    /** 
     * this key is a regular key
     * theOtherKey may not be a regular key
     */
    bool haveNonEmptyIntersection(const AliasMapKey& thisKey,
				  const AliasMapKey& theOtherKey) const;

  }; // end of class AliasMap

} // end of namespace 
                                                                     
#endif
