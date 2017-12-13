#ifndef _ALIASRANGE_INCLUDE_
#define _ALIASRANGE_INCLUDE_
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

namespace xaifBooster{ 

  /**
   * class to hold single and range alias information 
   * we don't want to split this so that making a range 
   * from two adjacent single addresses becomes easier
   */
  class AliasRange : public XMLPrintable { 
    
  public: 
  
    AliasRange(unsigned int lower,
	       unsigned int upper,
	       bool partial);
 
    ~AliasRange(){};

    virtual std::string debug() const;

    virtual void printXMLHierarchy(std::ostream& os) const; 

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name of member myLower/UpperAddress 
     * as specified in XAIF schema
     */
    static const std::string our_myLowerAddress_XAIFName;
    static const std::string our_myUpperAddress_XAIFName;

    /**
     * name of member myPartial 
     * as specified in XAIF schema
     */
    static const std::string our_myPartial_XAIFName;

    unsigned int max() const;
    unsigned int min() const;

    /**
     * almost operator '==' like but didn't 
     * use operator since people could 
     * epxect them to behave differently
     */
    bool sameAs(const AliasRange&) const;

    /**
     * almost operator '>' like but didn't 
     * use operator since people could 
     * epxect them to behave differently
     */
    bool isGreaterThan(const AliasRange&) const;

    /**
     * returns true for containment or 
     * equality
     */
    bool isContainedIn(const AliasRange&) const;

    /**
     * returns true for overlap but no containment
     */
    bool overlapsWith(const AliasRange&) const;

    /**
     * returns true if there is no gap between the ranges
     */
    bool bordersWith(const AliasRange&) const;

    /**
     * absorb the other range
     */
    void absorb(const AliasRange&);

    bool isPartial()const;

  private:

    unsigned int myLowerAddress;
    unsigned int myUpperAddress;

    void max(unsigned int);
    void min(unsigned int);
    
    /**
     * no def
     */
    AliasRange();

    /**
     * no def
     */
    AliasRange(const AliasRange&);

    /**
     * no def
     */
    AliasRange& operator=(const AliasRange&);

    /** 
     * indicates if this is a full 
     * or a partial reference
     */
    const bool myPartial;
    
  }; // end of class AliasRange

} // end of namespace xaifBooster

#endif
