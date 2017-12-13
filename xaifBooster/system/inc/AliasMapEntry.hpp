#ifndef _ALIASMAPENTRY_INCLUDE_
#define _ALIASMAPENTRY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AliasSet.hpp"
#include "xaifBooster/utils/inc/XMLPrintable.hpp"


namespace xaifBooster{ 

  /**
   * class to hold alias sets referred to by a Variable
   * through AliasMap
   */
  class AliasMapEntry : public XMLPrintable { 
    
  public: 
  
    AliasMapEntry(unsigned int theKey) : 
      myKey(theKey) {}; 
 
    ~AliasMapEntry(){};

    std::string debug() const ; 

    void printXMLHierarchy(std::ostream& os) const; 

    const AliasSet& getAliasSet() const;

    AliasSet& getAliasSet();

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for member myKey as represented in XAIF schema
     */
    static const std::string our_myKey_XAIFName;

    bool disjointFrom(const AliasMapEntry& theOtherAliasMapEntry) const;

    bool mustAlias(const AliasMapEntry& theOtherAliasMapEntry) const;

  private:

    /** 
     * set of aliases for this Variable
     */
    AliasSet myAliasSet;

    /**
     * keys start with 1
     * in XAIF schema the default is 0 
     * but does not have an instance in 
     * AliasMap
     */
    const unsigned int myKey;

    /**
     * no def
     */
    AliasMapEntry();

    /**
     * no def
     */
    AliasMapEntry(const AliasMapEntry&);

    /**
     * no def
     */
    AliasMapEntry& operator=(const AliasMapEntry&);
    
  }; // end of class AliasMapEntry

} // end of namespace xaifBooster

#endif
