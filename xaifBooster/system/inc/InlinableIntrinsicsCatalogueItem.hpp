#ifndef _INLINABLEINTRINSICSCATALOGUEITEM_INCLUDE_
#define _INLINABLEINTRINSICSCATALOGUEITEM_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/InlinableIntrinsicsExpression.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/ShapeChange.hpp"
#include <vector>

namespace xaifBooster { 

  /**
   * a class that contains all the 
   * definitions for a particular 
   * intrinsic
   */
  class InlinableIntrinsicsCatalogueItem { 
    
  public: 

    /**
     * we preallocate the vector with the resp. size
     * and check any argument position against this size
     * the ctor will also preallocate an Expression for the 
     * function but will not fill it. 
     * \todo JU: why do we preallocate the expression for the function?
     */
    InlinableIntrinsicsCatalogueItem(unsigned int aNumberOfArguments,
				     bool aNonSmoothFlag,
				     ShapeChange::ShapeChange_E theShapeChange);

    ~InlinableIntrinsicsCatalogueItem();

    /** 
     * see
     * myExpressionVector for explanation 
     * of position parameter
     */
    InlinableIntrinsicsExpression& addPartial(unsigned int aPosition,
					      PartialDerivativeKind::PartialDerivativeKind_E aPartialDerivativeKind);
    
    typedef std::vector<InlinableIntrinsicsExpression*> ExpressionVector;

    /** 
     * the ExpressionVector element can be set only once with addPartial
     */
    bool isExpressionVectorElementSet(unsigned int Position) const;

    /**
     * will throw an exception if the element had not been set with 
     */
    const InlinableIntrinsicsExpression& getExpressionVectorElement(unsigned int Position) const;

    InlinableIntrinsicsExpression& getFunction();
    
    const InlinableIntrinsicsExpression& getFunction() const;

    unsigned int getNumberOfArguments() const;

    static const std::string ourXAIFName;
    static const std::string our_myName_XAIFName;
    static const std::string our_myNrArgs_XAIFName;
    static const std::string our_myNonSmoothFlag_XAIFName;
    static const std::string ourFunctionXAIFName;
    static const std::string our_myFunctionType_XAIFName;
    static const std::string our_myFunctionBuiltinName_XAIFName;
    static const std::string ourPartialXAIFName;
    static const std::string our_myPartialId_XAIFName;
    static const std::string our_myPartialType_XAIFName;
    static const std::string our_myNonValueInquiryFlag_XAIFName;
    static const std::string our_myShapeChange_XAIFName;

    /** 
     * this method is const but on first invocation will fill the 
     * myUsedPositionalArguments and flip myDataComplete
     * disallowing the addition of further partials
     * \todo implementation to be fixed
     */
    const PositionSet& getUsedPositionalArguments(const PositionSet& theActiveArgumentPositions) const;

    /** 
     * returns flag value
     */
    bool isNonSmooth() const; 

    ShapeChange::ShapeChange_E getShapeChange() const; 

  private:
    
    /**
     * no def
     */
    InlinableIntrinsicsCatalogueItem();

    /**
     * no def
     */
    InlinableIntrinsicsCatalogueItem(const InlinableIntrinsicsCatalogueItem&);

    /**
     * no def
     */
    InlinableIntrinsicsCatalogueItem& operator=(const InlinableIntrinsicsCatalogueItem&);

    /** 
     * the number of Arguments for this intrinsic
     */
    unsigned int myNumberOfArguments;

    /**
     * the Vector of all partials
     * however the 0th element is always 
     * reserved for a potential expression for 
     * the function itself, i.e. this has myNumberOfArguments+1 
     * elements;
     * items in this Vector are newed up
     * by addPartial and deleted by 
     * this class' dtor
     */
    ExpressionVector myExpressionVector;

    /**
     * set of argument positions used anywhere in a  
     * partial of this intrinsic, counting starts with 1, 
     * the 0 position denotes the intrinsic's value
     * this is the union of all 
     * InlinableIntrinsicsExpression::myUsedPositionalArguments
     * kept in myExpressionVector
     */
    PositionSet myUsedPositionalArguments;

    /** 
     * indicates the setting of the catalogue item's 
     * data is complete,i.e. myUsedPositionalArguments was set
     */
    bool myDataComplete;

    /** 
     * a flag in the intrinsics catalogue indicating a nonsmooth 
     * behavior
     */
    const bool myNonSmoothFlag;

    /** 
     * indicating a shape change from argument to result
     */
    const ShapeChange::ShapeChange_E myShapeChange;

  }; // end of class InlinableIntrinsicsCatalogueItem
  
} 

#endif
