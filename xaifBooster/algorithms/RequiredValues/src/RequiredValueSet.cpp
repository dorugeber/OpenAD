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

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValueSet.hpp"

using namespace xaifBooster;

namespace xaifBoosterRequiredValues { 

  RequiredValueSet::RequiredValueSet() {
  } // end RequiredValueSet::RequiredValueSet()

  RequiredValueSet::~RequiredValueSet() {
    for (RequiredValuePList::const_iterator reqValI = myRequiredValuesPList.begin(); reqValI != myRequiredValuesPList.end(); ++reqValI)
      if (*reqValI)
	delete (*reqValI);
  } // end RequiredValueSet::~RequiredValueSet()

  std::string
  RequiredValueSet::comparisonResultToString(ComparisonResult_E aComparisonResult) {
    std::ostringstream out;
    switch (aComparisonResult) {
      case LESSTHAN:
        out << "less_than" << std::ends;
        break;
      case GREATERTHAN:
        out << "greater_than" << std::ends;
        break;
      case EQUAL:
        out << "equal" << std::ends;
        break;
      case INCOMPARABLE:
        out << "incomparable" << std::ends;
        break;
      default:
        THROW_LOGICEXCEPTION_MACRO("RequiredValueSet::comparisonResultToString:"
                                   << " unknown comparison result");
        break;
    } // end switch
    return out.str();
  } // end RequiredValueSet::comparisonResultToString()

  std::string
  RequiredValueSet::debug() const {
    std::ostringstream out;
    out << "xaifBoosterRequiredValues::RequiredValueSet[" << this
	<< ", reqVals=";
    for (RequiredValuePList::const_iterator reqValI = myRequiredValuesPList.begin(); reqValI != myRequiredValuesPList.end(); ++reqValI)
      out << std::endl << "\t(" << (*reqValI)->debug().c_str() << ")";
    out << std::endl << "]" << std::ends;
    return out.str();
  } // end RequiredValueSet::debug()

  void RequiredValueSet::addValueToRequiredSet(const Expression& anExpression,
                                               const ControlFlowGraphVertex& aControlFlowGraphVertex,
                                               const std::string anOriginStr) {
    // ensure that this expression isn't already in the set
    RequiredValuePList::const_iterator reqValPListI;
    for (reqValPListI = myRequiredValuesPList.begin(); reqValPListI != myRequiredValuesPList.end(); ++reqValPListI) 
      if (&(*reqValPListI)->getExpression() == &anExpression)
        break;
    if (reqValPListI == myRequiredValuesPList.end())
      myRequiredValuesPList.push_back(new RequiredValue (anExpression,
                                                         aControlFlowGraphVertex,
                                                         anOriginStr));
/*
    // Also include the index expression if this variable is an array reference
    if (theVariable.hasArrayAccess()) {
      const ArrayAccess::IndexTripletListType& theIndexTripletList(theVariable.getArrayAccess().getIndexTripletList());
      for (ArrayAccess::IndexTripletListType::const_iterator anIndexTripletListTypeCI = theIndexTripletList.begin();
	   anIndexTripletListTypeCI != theIndexTripletList.end();
	   ++anIndexTripletListTypeCI) {
	for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI = (*anIndexTripletListTypeCI)->getIndexPairList().begin();
	     anIndexPairListCI!=(*anIndexTripletListTypeCI)->getIndexPairList().end();
	     ++anIndexPairListCI) {
	  const Expression& theIndexExpression (*((*anIndexPairListCI).second));
	  if (!theIndexExpression.isConstant())
	    addVarsInExpressionToRequiredSet(theIndexExpression,
					     theStatementId,
					     theOrigin);
	} // end iterate over index pairs
      } // end interate over index triplet lists
    } // end if array access
*/
  } // end RequiredValueSet::addValueToRequiredSet()

  const RequiredValueSet::RequiredValuePList&
  RequiredValueSet::getRequiredValuesPList() const {
    return myRequiredValuesPList;
  } // end RequiredValueSet::getRequiredValuesPList()

} // end namespace xaifBoosterRequiredValues

