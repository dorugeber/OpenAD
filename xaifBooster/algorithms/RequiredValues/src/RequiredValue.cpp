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

#include "xaifBooster/algorithms/RequiredValues/inc/RequiredValue.hpp"

using namespace xaifBooster;

namespace xaifBoosterRequiredValues { 

  RequiredValue::RequiredValue(const Expression& anExpression,
                               const ControlFlowGraphVertex& aControlFlowGraphVertex,
                               const std::string& anOriginStr) :
    myExpression_p (&anExpression),
    myControlFlowGraphVertex_p (&aControlFlowGraphVertex),
    myOriginStr (anOriginStr) {
  } // end RequiredValue::RequiredValue()

  RequiredValue::~RequiredValue() {
  } // end RequiredValue::~RequiredValue()

  std::string
  RequiredValue::debug() const {
    std::ostringstream out;
    out << "RequiredValue[" << this
        << ",Expression=" << myExpression_p->debug().c_str() 
        << ",ControlFlowGraphVertex=" << myControlFlowGraphVertex_p->debug().c_str()
        << ",origin=" << myOriginStr;
    out << "]" << std::ends;
    return out.str();
  } // end RequiredValue::debug()

  const Expression&
  RequiredValue::getExpression() const {
    return *myExpression_p;
  } // end RequiredValue::getValueEV()

  const ControlFlowGraphVertex&
  RequiredValue::getControlFlowGraphVertex() const {
    return *myControlFlowGraphVertex_p;
  } // end RequiredValue::getControlFlowGraphVertex()

  std::string
  RequiredValue::getOriginStr() const {
    return myOriginStr;
  } // end RequiredValue::getOrigin()

  bool
  RequiredValue::isArgument() const {
    if (myExpression_p->numVertices() > 1)
      return false;
    return myExpression_p->getMaxVertex().isArgument();
  } // end RequiredValue::isArgument()

  const Argument&
  RequiredValue::getArgument() const {
    if (!isArgument())
      THROW_LOGICEXCEPTION_MACRO("RequiredValue::getArgument:"
                                 << "the expression for this required value does not consist of just a single argument");
    return dynamic_cast<const Argument&>(myExpression_p->getMaxVertex());
  } // end RequiredValue::getArgument()

  void
  RequiredValue::populateArgumentPList(Expression::CArgumentPList argumentListToBePopulated) const {
    myExpression_p->appendArguments(argumentListToBePopulated);
  } // end RequiredValue::populateArgumentPList

} // end namespace xaifBoosterRequiredValues

