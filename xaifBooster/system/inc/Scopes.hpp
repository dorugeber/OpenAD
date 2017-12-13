#ifndef _SCOPES_INCLUDE_
#define _SCOPES_INCLUDE_
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
#include "xaifBooster/boostWrapper/inc/GraphWrapper.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/ScopeContainment.hpp"

namespace xaifBooster { 
 
  class SymbolReference;
  class Symbol;

  /**
   * scopes is the graph (tree) of scopes 
   */
  class Scopes : public GraphWrapper<Scope, ScopeContainment> , 
		 public XMLPrintable {
  public:

    Scopes();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    Scope& getGlobalScope(); 

    const Scope& getGlobalScope() const; 
    
    const Scope& getScopeById(const std::string& theScopeId) const;

    Scope& getScopeById(const std::string& theScopeId);

    enum PathRelation_E{ 
      NO_PATH = 0 ,
      PARENT_CHILD = 1, 
      CHILD_PARENT = 2,
      SAME_SCOPE= 3
    };

    PathRelation_E onScopePath(const Symbol& firstSymbol,
			       const Symbol& secondSymbol) const;

    /** 
     * iterate through all SymbolTables 
     * to find symbols requiring forced passivation
     * this should happen AFTER all scopes are 
     * initialized.
     */
    void forcedPassivation();
    
  private:
    
    /**
     * the global scope, i.e. the 
     * root of the scope tree (graph)
     * this is not to be deleted 
     * explicitely.
     */
    Scope* myGlobalScope_p;

    void findGlobalScope(); 

    /** 
     * recursively invoked helper for the public onScopePath
     */
    PathRelation_E onScopePath(const Scope& aScope,
			       const Symbol& firstSymbol,
			       bool& foundFirst,
			       const Symbol& secondSymbol,
			       bool& foundSecond) const;

  }; // end of class Scopes

} // end of namespace xaifBooster
                                                                     
#endif
