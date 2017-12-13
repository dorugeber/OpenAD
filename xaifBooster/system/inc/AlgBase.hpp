#ifndef _ALGBASE_INCLUDE_
#define _ALGBASE_INCLUDE_
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
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"

namespace xaifBooster {  

  /** 
   * the base template class for algorithms
   * associated with leaf classes in the 
   * xaifBooster internal representation. 
   * The template parameter will be the 
   * respective leaf class who has a pointer member to 
   * the algorithm instance
   * and this class in turn has a reference to the 
   * instance of AlgContaining
   */
  template <class AlgContaining>
  class AlgBase : public XMLPrintable, public GenericTraverseInvoke { 

  public:
    
    /**
     * ctor sets reference to container
     */
    AlgBase(const AlgContaining& theContaining);

    virtual ~AlgBase();

    /**
     * access container
     */
    const AlgContaining& getContaining() const;

    /** 
     * we refer back to AlgContaining::printXMLHierarchyImpl
     */
    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private:

    /** 
     * not defined
     */
    AlgBase();

    /** 
     * not defined
     */
    AlgBase(const AlgBase&);

    /** 
     * not defined
     */
    AlgBase& operator=(const AlgBase&);

    protected:

    /** 
     * the back reference to AlgContaining 
     * this is constant because by design 
     * all modifications are to be done on 
     * algorithm specific data. Otherwise 
     * we could not ensure consistency of information 
     * between algorithms to be combined.
     */
    const AlgContaining& myContaining; 

  }; // end of class AlgBase

} // end of namespace 

#include "xaifBooster/system/inc/TemplateImpl/AlgBase.cpp"

#endif
