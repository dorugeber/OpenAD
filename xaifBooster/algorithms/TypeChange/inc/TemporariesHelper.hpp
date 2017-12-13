#ifndef _XAIFBOOSTERTYPECHANGE_TEMPORARIESHELPER_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_TEMPORARIESHELPER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string> 
#include <vector>

#include "xaifBooster/utils/inc/NameCreator.hpp"

#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/Scope.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"

using namespace xaifBooster;

namespace xaifBoosterTypeChange {

  /**
   * class to support creation of temporary variables
   */
  class TemporariesHelper {
  public:

    /** 
     * \param contextAnnotation so we can identify in the xaif file who made the temporary
     * \param theExpression whose value is supposed to be held in the temporary
     * \param theTopVertex is the top vertex in theExpression (i.e. specifying a sub expression)
     */
    TemporariesHelper(const std::string& contextAnnotation,
                      const Expression& theExpression,
                      const ExpressionVertex& theTopVertex);

    TemporariesHelper(const std::string& contextAnnotation,
                      const Variable& variableToMatch);

    ~TemporariesHelper();

    /**
     * \param aScope - Scope in which the symbol is made
     */
    Symbol& makeTempSymbol(Scope& aScope, bool activeType=false);

    /**
     * returns true if the temporary would need to be allocated
     * can be called before makeTempSymbol
     */
    bool needsAllocation();

    /** 
     * \return a variable that can serve a s model to allocate the temporary
     */ 
    const Variable& allocationModel();

    /**
     * \param aScope - Scope in which the symbol is made
     * \param aNameCreator - used to determine the name of the new variable
     * \param isActive - Flag indicating whether or not the new variable is active
     */
    Symbol& makeTempSymbol(Scope& aScope,
                           const NameCreator& aNameCreator,
                           bool isActive);

    SymbolShape::SymbolShape_E getSymbolShape();

  private:

    /// no def
    TemporariesHelper();

    /// no def
    TemporariesHelper(const TemporariesHelper&);

    /// no def
    TemporariesHelper & operator=(const TemporariesHelper&);

    typedef std::vector<DimensionBounds*> DimensionBoundsPVector;

    void typeInfo(const ExpressionVertex& theTopVertex);

    void typeInfo(const Variable& aVariable);

    void typeInfo(const Constant & aConstant); 

    void setTypeInfo();

    /**
     * populate the effective dimension bounds
     * \param aVariable in the dimension bounds are set
     */
    void populateDimensionBounds(const Variable& aVariable);

    void setDimensionBounds(Symbol& aNewSymbol);

    const std::string myContextAnnnotation;

    const Expression* myExpression_p;

    const ExpressionVertex* myTopVertex_p;

    const Variable* myVariable_p;

    SymbolShape::SymbolShape_E myShape;

    SymbolType::SymbolType_E myType;

    FrontEndType myFrontEndType;

    DimensionBoundsPVector myDimensionBoundsPVector;

    bool myTypeInfo;

    const Variable* myAllocationModel_p;

  }; // end of class TemporariesHelper

}

#endif
