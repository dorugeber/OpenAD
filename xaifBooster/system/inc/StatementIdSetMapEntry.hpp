#ifndef _STATEMENTIDSETMAPENTRY_INCLUDE_
#define _STATEMENTIDSETMAPENTRY_INCLUDE_
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

#include "xaifBooster/utils/inc/XMLPrintable.hpp"

#include "xaifBooster/system/inc/StatementIdSet.hpp"

namespace xaifBooster{ 

  /**
   * entry to a map, contains the key itself (for convenience replicated)
   * and the set of statement IDs
   */
  class StatementIdSetMapEntry : public XMLPrintable{ 
    
  public: 
  
    StatementIdSetMapEntry(unsigned int theKey); 
 
    ~StatementIdSetMapEntry();

    std::string debug() const ; 

    static const std::string ourXAIFName;
    static const std::string our_myKey_XAIFName;
    static const std::string our_StatementId_XAIFName;
    static const std::string our_IdRef_XAIFName;

    void printXMLHierarchy(std::ostream& os) const; 

    const StatementIdSet& getStatementIdSet() const; 

    void insert(const ObjectWithId::Id& anId);

  private:

    /**
     * keys start with 1
     * in XAIF schema the default is 0 
     * which refers to a dummy instance in the
     * DuUdMap
     */
    const unsigned int myKey;

    /**
     * no def
     */
    StatementIdSetMapEntry();

    /**
     * no def
     */
    StatementIdSetMapEntry(const StatementIdSetMapEntry&);

    /**
     * no def
     */
    StatementIdSetMapEntry& operator=(const StatementIdSetMapEntry&);

    StatementIdSet myStatementIdSet;
    
  }; 

} 

#endif
