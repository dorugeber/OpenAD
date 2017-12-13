#ifndef _XAIFBOOSTERSAVEVALUEACROSS_SAVEACROSS_INCLUDE_
#define _XAIFBOOSTERSAVEVALUEACROSS_SAVEACROSS_INCLUDE_
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

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/Expression.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

using namespace xaifBooster;

namespace xaifBoosterSaveValuesAcross {

  /**
   * Class that implements the storing values across statements,
   * as might be required when the statement has a side effect.
   * This is accomplished by creating an assignment to a temporary variable.
   * Current uses include saving the value of array indices for arguments at their value
   * before a subroutine call in cases where the index may be overwritten in the subroutine.
   * This arises in TypeChange (needed for type conversion back), and in adjoints.
   */
  class SaveValuesAcross : public XMLPrintable {
  public:

    SaveValuesAcross();

    ~SaveValuesAcross();

    /**
     * Calls printXMLHierarchy on each saved value, which prints its assignment
     */
    void printXMLHierarchy(std::ostream& os) const;

    std::string debug() const;

    /**
     * Determines whether \p anArgument has already been saved.
     */
    bool isSavedAcross(const Argument& anArgument) const;

    /**
     * Saves the ExpressionVertex \p anArgument by creating an assignment
     * that stores the value in a temporary before the statement in question.
     */
    void saveValue(const Argument& anArgument,
		   const BasicBlock& theBasicBlock);

    void populateReplacementPairsList(Expression::VariablePVariableSRPPairList& aListToBePopulated) const;

    //void populatePriorToCallAssignmentsList(PlainBasicBlock::BasicBlockElementList& aListToBePopulated) const;

    /**
     * checks each of the saved values for \p anExpression
     */
    bool hasExpression(const Expression& theExpression) const;

    class SavedValue {
    public:
      SavedValue(const Argument& anArgument,
                 VariableSymbolReference& aVSR,
                 const Assignment& anAssignment);

      ~SavedValue();

      const Argument& getArgument() const;
      const Assignment& getAssignment() const;
      const VariableSymbolReference& getVariableSymbolReference() const;
      VariableSymbolReference& getVariableSymbolReference();

      void printXMLHierarchy(std::ostream& os) const;

      /**
       * checks the assignment for \p anExpression
       */
      bool hasExpression(const Expression& theExpression) const;

    private:

      const Argument* myArgument_p;

      VariableSymbolReference* myTempVarVSR_p;

      const Assignment* myAssignment_p;

       /// no def 
      SavedValue();
      
       /// no def 
      SavedValue(const SavedValue&);
      
       /// no def 
      SavedValue& operator=(const SavedValue&);

    }; // end struct SavedValue

    typedef std::list<SavedValue*> SavedValuePList;

    const SavedValuePList& getSavedValuePList() const;

  private: 
    
    SavedValuePList mySavedValuePList;

    /// no def 
    SaveValuesAcross(const SaveValuesAcross&);

    /// no def 
    SaveValuesAcross& operator=(const SaveValuesAcross&);

  }; // end class SaveValuesAcross

} // end namespace xaifBoosterSaveValuesAcross
                                                                     
#endif

