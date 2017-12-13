#ifndef _CALLGRAPH_INCLUDE_
#define _CALLGRAPH_INCLUDE_
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
#include "xaifBooster/system/inc/GraphWrapperTraversable.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"
#include "xaifBooster/system/inc/CallGraphEdge.hpp"
#include "xaifBooster/system/inc/Scopes.hpp"
#include "xaifBooster/system/inc/AliasMap.hpp"
#include "xaifBooster/system/inc/DuUdMap.hpp"
#include "xaifBooster/system/inc/DoMap.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraphAlgBase.hpp"
 

namespace xaifBooster { 

  /**
   * the CallGraph is the top level structure in the XAIF 
   * representation. It describes targets of calls (subroutines)
   * as vertices and the calls being made as edges
   */
  class CallGraph : public GraphWrapperTraversable<CallGraphVertex, CallGraphEdge> , 
		    public XMLPrintable { 
  public:

    ~CallGraph();
    
    /**
     * print XML hierarchy
     * \todo add printing DuUd Map
     */
    virtual void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;

    /**
     * print debug information
     */
    virtual std::string debug() const ;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for mySchemaInstance as specified in XAIF schema
     */
    static const std::string our_mySchemaInstance_XAIFName;

    /**
     * name for myXAIFInstance as specified in XAIF schema
     */
    static const std::string our_myXAIFInstance_XAIFName;

    /**
     * name for mySchemaLocation as specified in XAIF schema
     */
    static const std::string our_mySchemaLocation_XAIFName;

    /**
     * name for myProgramName as specified in XAIF schema
     */
    static const std::string our_myProgramName_XAIFName;

    /**
     * name for myPrefix as specified in XAIF schema
     */
    static const std::string our_myPrefix_XAIFName;

    /**
     * get the scope tree
     */
    Scopes& getScopeTree();

    /**
     * get the AliasMap
     */
    AliasMap& getAliasMap();

    /**
     * get the AliasMap
     */
    const AliasMap& getAliasMap() const;

    /**
     * get the DuUdMap
     */
    DuUdMap& getDuUdMap();

    /**
     * get the DuUdMap
     */
    const DuUdMap& getDuUdMap() const;

    /**
     * get the DoMap
     */
    DoMap& getDoMap();

    /**
     * get the DoMap
     */
    const DoMap& getDoMap() const;

    void setProgramName(const std::string& aProgramName);

    const std::string& getSchemaLocation() const;

    void resetSchemaLocation(const std::string& aNewLocation);

    const std::string& getPrefix() const; 

    /**
     * get algorithm
     */
    CallGraphAlgBase& getCallGraphAlgBase();
                                                                                
    /**
     * get algorithm
     */
    const CallGraphAlgBase& getCallGraphAlgBase() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    const CallGraphVertex& getSubroutineBySymbolReference(const SymbolReference& aSymbolReference) const;

    /** 
     * gets the subroutine by the name in the program
     * i.e. the plain name without front-end decorations
     */
    const CallGraphVertex& getSubroutineByPlainName(const std::string& aPlainName) const;

  private:

    /**
     * CallGraphs are supposed to be created 
     * through ConceptuallyStaticInstances::createCallGraph
     */
    CallGraph(const std::string& aSchemaInstance,
	      const std::string& anXAIFInstance,
	      const std::string& aSchemaLocation,
	      const std::string& aPrefix);


    /**
     * in order to access the ctor
     */
    friend  void ConceptuallyStaticInstances::createCallGraph(const std::string& aSchemaInstance,
							      const std::string& anXAIFInstance,
							      const std::string& aSchemaLocation,
							      const std::string& aPrefix);

    
    /** 
     * no def
     */
    CallGraph();

    /**
     * the CallGraph has a handle to the 
     * scope tree
     */
    Scopes myScopeTree;

    /**
     * the CallGraph owns the map for 
     * alias information
     */
    AliasMap myAliasMap;

    /**
     * the CallGraph owns the map for 
     * def-use/use-def information
     */
    DuUdMap myDuUdMap;

    /**
     * the CallGraph owns the map for 
     * def-overwrite information
     */
    DoMap myDoMap;

    /**
     * the schema instance 
     */
    const std::string mySchemaInstance;

    /**
     * the XAIF instance 
     */
    const std::string myXAIFInstance;

    /**
     * the schema location 
     */
    std::string mySchemaLocation;

    /**
     * an optional program name 
     */
    std::string myProgramName;

    /**
     * the prefix 
     */
    const std::string myPrefix;

    /**
     * this will be set to point a dynamically instance
     * during construction and deleted during
     * destruction
     */
    CallGraphAlgBase* myCallGraphAlgBase_p;

  }; // end of class CallGraph

} // end of namespace xaifBooster
                                                                     
#endif
