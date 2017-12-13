#ifndef _CONSTANT_INCLUDE_
#define _CONSTANT_INCLUDE_
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
#include "xaifBooster/system/inc/BaseConstant.hpp"
#include "xaifBooster/system/inc/ConstantAlgBase.hpp"

namespace xaifBooster { 

  class Constant : public ExpressionVertex,
		   public BaseConstant {
  public:

    Constant(const SymbolType::SymbolType_E aType,
	     bool makeAlgorithm=true);
    Constant(int anIntVal,
	     bool makeAlgorithm=true);
    ~Constant(){};

    void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os, const Expression& theExpression) const {};

    std::string debug() const;

    virtual std::string equivalenceSignature() const;

    static const std::string ourXAIFName;
    static const std::string our_myValue_XAIFName;
    static const std::string our_myId_XAIFName;

    virtual ExpressionVertex& createCopyOfMyself(bool withAlgorithm=false) const;

    ConstantAlgBase& getConstantAlgBase() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

  private : 

    /**
     * no def
     */
    Constant ();

    /**
     * no def
     */
    Constant (const Constant&);

    /**
     * no def
     */
    Constant& operator=(const Constant&);

  }; // end of class Constant
 
} // end of namespace xaifBooster
                                                                     
#endif
