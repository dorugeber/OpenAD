#ifndef _BOOLEANOPERATION_INCLUDE_
#define _BOOLEANOPERATION_INCLUDE_
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
#include "xaifBooster/system/inc/BooleanOperationType.hpp"
#include "xaifBooster/system/inc/BooleanOperationAlgBase.hpp"

namespace xaifBooster { 

  class BooleanOperation : public ExpressionVertex {

  public:

    BooleanOperation(BooleanOperationType::BooleanOperationType_E theType,
		     bool makeAlgorithm=true);

    ~BooleanOperation(){};

    void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os, const Expression& theExpression) const {};

    std::string debug() const;

    virtual std::string equivalenceSignature() const;

    static const std::string ourXAIFName;
    static const std::string our_myType_XAIFName;
    static const std::string our_myId_XAIFName;

    virtual ExpressionVertex& createCopyOfMyself(bool withAlgorithm=false) const;

    BooleanOperationAlgBase& getBooleanOperationAlgBase() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    BooleanOperationType::BooleanOperationType_E getType()const;

  private : 

    /** 
     * no def
     */
    BooleanOperation ();

    /**
     * no def
     */
    BooleanOperation (const BooleanOperation&);

    /**
     * no def
     */
    BooleanOperation& operator=(const BooleanOperation&);


    /**
     * the type of the boolean operation
     */
    const BooleanOperationType::BooleanOperationType_E myType;

  }; // end of class BooleanOperation
 
} // end of namespace xaifBooster
                                                                     
#endif
