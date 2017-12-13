#ifndef _INTRINSIC_INCLUDE_
#define _INTRINSIC_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/IntrinsicAlgBase.hpp"
#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "NonInlinableIntrinsicsCatalogueItem.hpp"

namespace xaifBooster { 

  class Expression;

  /** 
   * this is an intrinsic (such as an operator 
   * or a function) within an Expression
   */
  class Intrinsic : public ExpressionVertex,
		    public ObjectWithAnnotation {

  public:

    Intrinsic(const std::string& aName,
	      bool makeAlgorithm=true);

    ~Intrinsic(){};

    void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os, const Expression& theExpression) const {};

    std::string debug() const;

    static const std::string ourXAIFName;
    static const std::string our_myName_XAIFName;
    static const std::string our_myId_XAIFName;

    std::string equivalenceSignature() const;

    /** 
     * the only useful implementation for this virtual
     * throws an exception if isInlinable() is false
     */
    virtual const InlinableIntrinsicsCatalogueItem& getInlinableIntrinsicsCatalogueItem() const;

    /**
     * throws an exception if isInlinable() is true
     */
    virtual const NonInlinableIntrinsicsCatalogueItem& getNonInlinableIntrinsicsCatalogueItem() const;

    ExpressionVertex& createCopyOfMyself(bool withAlgorithm=false) const;

    IntrinsicAlgBase& getIntrinsicAlgBase() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);
    
    /** 
     * looks at the catalogue
     */
    bool isNonSmooth() const; 

    const std::string& getName() const;

    /**
     * @return true
     */
    virtual bool isIntrinsic() const;

    /**
     * inlinable or not
     */
    bool isInlinable() const;

  private : 

    /** 
     * no def
     */
    Intrinsic();

    /**
     * no def
     */
    Intrinsic (const Intrinsic&);

    /**
     * no def
     */
    Intrinsic& operator=(const Intrinsic&);

    /**
     * \todo replace with reference to IntrinsicCatalogue
     */
    const std::string myName;

    /**
     * sets myInlinableIntrinsicsCatalogueItem_p or myNonInlinableIntrinsicsCatalogueItem_p
     */
    void getInfo() const;

    /**
     * set if this is an inlinable intrinsic
     */
    mutable const InlinableIntrinsicsCatalogueItem* myInlinableIntrinsicsCatalogueItem_p;

    /**
     * set if this is a noninlinable intrinsic
     */
    mutable const NonInlinableIntrinsicsCatalogueItem* myNonInlinableIntrinsicsCatalogueItem_p;

  }; // end of class Intrinsic
 
} // end of namespace xaifBooster
                                                                     
#endif
