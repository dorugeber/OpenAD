// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <sstream>

#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/SaveValuesAcross/inc/SaveValuesAcross.hpp"

using namespace xaifBooster;

namespace xaifBoosterSaveValuesAcross {

  SaveValuesAcross::SaveValuesAcross() {
  } // end SaveValuesAcross::SaveValuesAcross()

  SaveValuesAcross::~SaveValuesAcross() {
    // delete the SaveValuesAcross::SavedValues that we own
    for (SaveValuesAcross::SavedValuePList::iterator saI = mySavedValuePList.begin(); saI != mySavedValuePList.end(); ++saI)
      if (*saI)
        delete *saI;
  } // end SaveValuesAcross::~SaveValuesAcross()

  void
  SaveValuesAcross::printXMLHierarchy(std::ostream& os) const {
    for (SaveValuesAcross::SavedValuePList::const_iterator saI = mySavedValuePList.begin(); saI != mySavedValuePList.end(); ++saI)
      if (*saI)
        (*saI)->printXMLHierarchy(os);
  } // end SaveValuesAcross::printXMLHierarchy()

  std::string
  SaveValuesAcross::debug () const {
    std::ostringstream out;
    out << "SaveValuesAcross[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end SaveValuesAcross::debug()

  bool
  SaveValuesAcross::isSavedAcross(const Argument& anArgument) const {
    SaveValuesAcross::SavedValuePList::const_iterator saI;
    for (saI = mySavedValuePList.begin(); saI != mySavedValuePList.end(); ++saI)
      if (anArgument.getVariable().equivalentTo((*saI)->getArgument().getVariable()))
	break;
    return (saI != mySavedValuePList.end());
  } // end SaveValuesAcross::isSavedAcross()

  void
  SaveValuesAcross::saveValue(const Argument& anArgument,
                              const BasicBlock& theBasicBlock) {
    Assignment* theNewAssignment_p (new Assignment(false));
    theNewAssignment_p->setId("assignment_to_temporary_for_saving_value_across_statement");
    // create a new symbol and add a new VariableSymbolReference in the Variable
    VariableSymbolReference* theNewVariableSymbolReference_p =
     new VariableSymbolReference(theBasicBlock.getScope().getSymbolTable().addUniqueAuxSymbol(SymbolKind::VARIABLE,
											      SymbolType::INTEGER_STYPE,
											      SymbolShape::SCALAR,
											      false),
				 theBasicBlock.getScope());
    theNewVariableSymbolReference_p->setId("1");
    theNewVariableSymbolReference_p->setAnnotation("xaifBoosterSaveValuesAcross::SaveValuesAcross::saveValue");
    // pass it on to the LHS and relinquish ownership
    theNewAssignment_p->getLHS().supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
    theNewAssignment_p->getLHS().getAliasMapKey().setTemporary();
    theNewAssignment_p->getLHS().getDuUdMapKey().setTemporary();
    // set the RHS
    theNewAssignment_p->getRHS().supplyAndAddVertexInstance(anArgument.createCopyOfMyself());
    // save the replacement pairs and the assignment
    mySavedValuePList.push_back(new SaveValuesAcross::SavedValue(anArgument, *theNewVariableSymbolReference_p, *theNewAssignment_p));
  } // end SaveValuesAcross::saveValue()

  void
  SaveValuesAcross::populateReplacementPairsList(Expression::VariablePVariableSRPPairList& aListToBePopulated) const {
    // this should possibly become deprecated?  It's currently used by typeChange::subroutinecallalg
    aListToBePopulated.clear();
    for (SaveValuesAcross::SavedValuePList::const_iterator saI = mySavedValuePList.begin(); saI != mySavedValuePList.end(); ++saI)
      aListToBePopulated.push_back(std::make_pair(&((*saI)->getArgument().getVariable()),
                                                  &(*saI)->getVariableSymbolReference()));
  } // end SaveValuesAcross::populateReplacementPairsList()

  bool
  SaveValuesAcross::hasExpression(const Expression& anExpression) const {
    for (SavedValuePList::const_iterator svI = mySavedValuePList.begin(); svI != mySavedValuePList.end(); ++svI)
      if ((*svI)->hasExpression(anExpression))
        return true;
    return false;
  } // end SaveValuesAcross::hasExpression()

  const SaveValuesAcross::SavedValuePList&
  SaveValuesAcross::getSavedValuePList() const {
    return mySavedValuePList;
  } // end SaveValuesAcross::getSavedValuePList()

  SaveValuesAcross::SavedValue::SavedValue(const Argument& anArgument,
                                           VariableSymbolReference& aVSR,
                                           const Assignment& anAssignment) :
    myArgument_p (&anArgument),
    myTempVarVSR_p (&aVSR),
    myAssignment_p (&anAssignment) {
  } // end SaveValuesAcross::SavedValue::SavedValue()

  SaveValuesAcross::SavedValue::~SavedValue() {
    if (myAssignment_p)
      delete myAssignment_p;
  } // end SaveValuesAcross::SavedValue::~SavedValue()

  const Argument&
  SaveValuesAcross::SavedValue::getArgument() const {
    if (!myArgument_p)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterSaveValuesAcross::SavedValue::getArgument: not set");
    return *myArgument_p;
  } // end SaveValuesAcross::SavedValue::getArgument()

  const Assignment&
  SaveValuesAcross::SavedValue::getAssignment() const {
    if (!myAssignment_p)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterSaveValuesAcross::SavedValue::getAssignment: not set");
    return *myAssignment_p;
  } // end SaveValuesAcross::SavedValue::getAssignment()

  const VariableSymbolReference&
  SaveValuesAcross::SavedValue::getVariableSymbolReference() const {
    if (!myTempVarVSR_p)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterSaveValuesAcross::SavedValue::getVariableSymbolReference: not set");
    return *myTempVarVSR_p;
  } // end SaveValuesAcross::SavedValue::getVariableSymbolReference()

  VariableSymbolReference&
  SaveValuesAcross::SavedValue::getVariableSymbolReference() {
    if (!myTempVarVSR_p)
      THROW_LOGICEXCEPTION_MACRO("xaifBoosterSaveValuesAcross::SavedValue::getVariableSymbolReference: not set");
    return *myTempVarVSR_p;
  } // end SaveValuesAcross::SavedValue::getVariableSymbolReference()

  void
  SaveValuesAcross::SavedValue::printXMLHierarchy(std::ostream& os) const {
    if (myAssignment_p)
      myAssignment_p->printXMLHierarchy(os);
  } // end SaveValuesAcross::SavedValue::printXMLHierarchy()

  bool
  SaveValuesAcross::SavedValue::hasExpression(const Expression& anExpression) const {
    return myAssignment_p->hasExpression(anExpression);
  } // end SaveValuesAcross::SavedValue::hasExpression()

} // end namespace xaifBoosterSaveValuesAcross

