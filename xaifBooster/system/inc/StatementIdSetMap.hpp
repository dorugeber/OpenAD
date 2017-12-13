#ifndef _STATEMENTIDSETMAP_INCLUDE_
#define _STATEMENTIDSETMAP_INCLUDE_
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

#include "xaifBooster/utils/inc/Debuggable.hpp"

#include "xaifBooster/system/inc/StatementIdSetMapEntry.hpp"

namespace xaifBooster { 

  class StatementIdSetMapKey;

  class StatementIdSetMap: virtual public Debuggable {

  public:
    
    StatementIdSetMap(){};

    ~StatementIdSetMap();

    virtual std::string debug() const ; 

    typedef std::vector<StatementIdSetMapEntry*> StatementIdSetMapEntryPVector;

    StatementIdSetMapEntry& addEntry(unsigned int theKey); 

    const StatementIdSetMapEntry& getEntry(const StatementIdSetMapKey& theKey) const; 

    const StatementIdSetMapEntryPVector& getEntries() const; 

  private: 

    /**
     * no def
     */
    StatementIdSetMap(const StatementIdSetMap&);

    /**
     * no def
     */
    StatementIdSetMap& operator=(const StatementIdSetMap&);

    /** 
     * the vector containing all the entries
     */
    StatementIdSetMapEntryPVector myStatementIdSetMapEntryPVector;

  };

} 
                                                                     
#endif
