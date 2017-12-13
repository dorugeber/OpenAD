#ifndef _DUUDMAP_INCLUDE_
#define _DUUDMAP_INCLUDE_
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
#include "xaifBooster/system/inc/StatementIdSetMap.hpp"
#include "xaifBooster/system/inc/DuUdMapDefinitionResult.hpp"
#include "xaifBooster/system/inc/DuUdMapUseResult.hpp"

namespace xaifBooster { 


  /**
   * map to hold DuUdMapEntry information 
   */
  class DuUdMap : public StatementIdSetMap ,
		  public XMLPrintable {

  public:
    
    static const std::string ourXAIFName;

    DuUdMap(){};

    ~DuUdMap(){};

    void printXMLHierarchy(std::ostream& os) const; 

    /** 
     * aKey is the key of a right-hand-side variable
     * which is to be checked for definitions in
     * statements listed in anIdList
     * and we use statementId to distinguish 
     * loop carried dependencies
     */
    const DuUdMapDefinitionResult definition(const StatementIdSetMapKey& aKey,
					     const ObjectWithId::Id& statementId,
					     const StatementIdList& anIdList) const;

    /** 
     * aKey is the key of a left-hand-side variable
     * which is to be checked for uses in
     * statements listed in the idLists
     * and we use statementId to distinguish 
     * loop carried dependencies
     */
    const DuUdMapUseResult use(const StatementIdSetMapKey& aKey,
			       const ObjectWithId::Id& statementId,
			       const DuUdMapUseResult::StatementIdLists& idLists) const;
    
    /** 
     * this returns true if the use-def chains for both 
     * keys are provably identical and false otherwise;
     * the keys have to come from the same ControlFlowGraph
     * as we don't require global uniqueness of statement ids
     * Note: sameDefinition returning false does not 
     * imply the definitions are disjoint
     */
    bool sameDefinition(const StatementIdSetMapKey& aKey,
			const StatementIdSetMapKey& anotherKey) const;


    /** 
     * this returns true if the use-def chains for both 
     * keys are provably disjoint and false otherwise;
     * the keys have to come from the same ControlFlowGraph
     * as we don't require global uniqueness of statement ids
     * Note: disjointDefinition returning false does not 
     * imply the definitions are the same
     */
    bool disjointDefinition(const StatementIdSetMapKey& aKey,
			    const StatementIdSetMapKey& anotherKey) const;

  private: 

    /**
     * no def
     */
    DuUdMap(const DuUdMap&);

    /**
     * no def
     */
    DuUdMap& operator=(const DuUdMap&);

  };

} 
                                                                     
#endif
