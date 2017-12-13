#ifndef _ALGBASEGRAPHELEMENT_INCLUDE_
#define _ALGBASEGRAPHELEMENT_INCLUDE_
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
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"

namespace xaifBooster {  

  /** 
   * \see AlgBase ; the difference lies in the XML unparsing 
   * for edges that need to reference their containing 
   * graph in order to be determine source and target
   * via boost methods
   */
  template <class AlgContaining, class GraphType>
  class AlgBaseGraphElement : public virtual Debuggable, 
                              public GenericTraverseInvoke { 

  public:
    
    /**
     * ctor sets reference to container
     */
    AlgBaseGraphElement(const AlgContaining& theContaining);

    virtual ~AlgBaseGraphElement();

    /**
     * access container
     */
    const AlgContaining& getContaining() const;

    /** 
     * we refer back to AlgContaining::printXMLHierarchyImpl
     */
    virtual void printXMLHierarchy(std::ostream& os,
				   const GraphType& theGraph) const;
    /**
     * print for vertices (by default AlgContaining is printed)
     * \todo remove this
     */
    virtual void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private:

    /** 
     * not defined
     */
    AlgBaseGraphElement();

    /** 
     * not defined
     */
    AlgBaseGraphElement(const AlgBaseGraphElement&);

    /** 
     * not defined
     */
    AlgBaseGraphElement& operator=(const AlgBaseGraphElement&);

    /** 
     * the back reference to AlgContaining 
     * this is constant because by design 
     * all modifications are to be done on 
     * algorithm specific data. Otherwise 
     * we could not ensure consistency of information 
     * between algorithms to be combined.
     */
    const AlgContaining& myContaining; 

  }; // end of class AlgBaseGraphElement

} // end of namespace 

#include "xaifBooster/system/inc/TemplateImpl/AlgBaseGraphElement.cpp"

#endif
