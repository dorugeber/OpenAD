#ifndef _SCOPECONTAINMENT_INCLUDE_
#define _SCOPECONTAINMENT_INCLUDE_
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
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/system/inc/EdgeTraversable.hpp"

namespace xaifBooster { 

  class Scopes;

  /** 
   * ScopeContainment represents the relationship 
   * between scopes. 
   * Note that this this particular case the source is 
   * the child and the target the containing parent Scope. 
   * This choice was made to facilitate the more frequently 
   * used up-traversal from a Scope to the containing 
   * Scope.
   */
  class ScopeContainment : public EdgeTraversable, 
			   public XMLPrintable,
			   public ObjectWithId {
  public:

    ScopeContainment (){};

    ~ScopeContainment(){};

    void printXMLHierarchy(std::ostream& os,
			   const Scopes& theScopes) const;

    void printXMLHierarchy(std::ostream& os) const {};

    std::string debug() const ;

    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for source as represented in XAIF schema
     */
    static const std::string our_source_XAIFName;

    /**
     * name for target as represented in XAIF schema
     */
    static const std::string our_target_XAIFName;

  }; // end of class ScopeContainment
 
} // end of namespace xaifBooster
                                                                     
#endif
