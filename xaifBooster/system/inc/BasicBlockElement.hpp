#ifndef _BASICBLOCKELEMENT_INCLUDE_
#define _BASICBLOCKELEMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"

#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

namespace xaifBooster { 

  /**
   * base class for elements found in a Basic Block
   */
  class BasicBlockElement : public XMLPrintable,
			    public ObjectWithId,
			    public GenericTraverseInvoke {
  public:
    
    BasicBlockElement ();

    ~BasicBlockElement();

    virtual std::string debug() const ;

    /**
     * access the algorithm
     */
    BasicBlockElementAlgBase& getBasicBlockElementAlgBase() const;

    /**
     * actual implementation for printing xaif
     * always invoked by the graph
     */
    virtual void printXMLHierarchyImpl(std::ostream& os) const {};

    /**
     * checks the corresponding algorithm (if any) for \p anExpression
     */
    virtual bool hasExpression(const Expression& anExpression) const;

  protected:
    /**
     * if required then an algorithm object is allocated dynamically
     * by the factory and the pointer is set
     */
    BasicBlockElementAlgBase* myBasicBlockElementAlgBase_p;

  }; // end of class BasicBlockElement

  typedef std::list<const BasicBlockElement*> CBasicBlockElementPList;
 
} // end of namespace xaifBooster
                                                                     
#endif

