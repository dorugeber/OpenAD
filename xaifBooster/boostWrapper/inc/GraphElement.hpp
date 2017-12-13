#ifndef  _GRAPHELEMENT_INCLUDE_
#define  _GRAPHELEMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp" 

namespace xaifBooster { 

  /** 
   * this is a GraphElement wrapper for vertices 
   * or edges to be 
   * used with the GraphWrapper class 
   * which doesn't have capacity for traversal
   * BTW, an attempt for a back reference to the 
   * containing graph leads to circular template 
   * definitions...
   * the template parameter is the boost descriptor type
   * \todo fix access issue with ctor an getDescriptor
   */ 
  template <class VertexOrEdgeDescriptor>
  class GraphElement : public virtual Debuggable { 
  public: 

    virtual ~GraphElement(){};

    std::string debug() const;

    /** 
     * ctor  and init need to be accessible from GraphWrapper
     * but currently both are public because of a compiler bug 
     *
     * template<class VertexType,class EdgeType> friend class GraphWrapper; 
     */

    /**
     * explicit construction only in derived classes
     * or in GraphWrapper
     * \todo this should be protected but we have a compiler error that causes 
     * the friend declaration to be ignored
     */
    GraphElement();

    /**
     * \todo this should be protected but we have a compiler error that causes 
     * the friend declaration to be ignored
     */
    const VertexOrEdgeDescriptor& 
    getDescriptor() const; 

    /**
     * set the descriptor only in GraphWrapper
     * \todo this should be protected but we have a compiler error that causes 
     * the friend declaration to be ignored
     */
    void init(const VertexOrEdgeDescriptor& aDescriptor);
    
    void setVisited() const; 

    bool wasVisited() const; 

    void resetVisited() const;

  protected:

    
  private:

    /**
     * disallow copy ctor for now
     */
    GraphElement(const GraphElement&);

    /**
     * disallow assignment, same rationale as with copy constructor
     */
    GraphElement& operator=(const GraphElement&);

    /** 
     * flag to determine if 
     * the descriptor has been initialized
     */
    bool myInitFlag;

    /** 
     * the internal back reference to the boost descriptor
     * which is initialized by init();
     */
    VertexOrEdgeDescriptor myBoostDescriptor;

    /** 
     * for visitor algorithms
     */
    mutable bool myVisitedFlag;

  }; // end of class GraphElement

} // end of namespace xaifBooster

#include "xaifBooster/boostWrapper/inc/TemplateImpl/GraphElement.cpp"

#endif
