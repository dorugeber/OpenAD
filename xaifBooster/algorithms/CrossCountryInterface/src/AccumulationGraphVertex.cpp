// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/Constant.hpp"
#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/system/inc/ExpressionEdgeAlgBase.hpp"
#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/PartialDerivativeKind.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphEdge.hpp"

#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface {

  // Constructor for leaves that correspond to direct copy edges in the LCG
  AccumulationGraphVertex::AccumulationGraphVertex() :
   myExpressionEdge_p(NULL),
   myRemainderGraphEdge_p(NULL),
   myHasOperationFlag(false),
   myPartialDerivativeKind(PartialDerivativeKind::LINEAR_ONE),
   myValue(0),
   ValueHasBeenSet(false),
   myLHSVariable_p(NULL) {
  } // end AccumulationGraphVertex::AccumulationGraphVertex()

  // Constructor for leaves (those that don't correspond to direct copy edges in the LCG).
  AccumulationGraphVertex::AccumulationGraphVertex(const ExpressionEdge& aExpressionEdge) :
   myExpressionEdge_p(&aExpressionEdge),
   myRemainderGraphEdge_p(NULL),
   myHasOperationFlag(false) {
    const xaifBoosterLinearization::ExpressionEdgeAlg& theLinearizedExpressionEdgeAlg(
     dynamic_cast<const xaifBoosterLinearization::ExpressionEdgeAlg&>(aExpressionEdge.getExpressionEdgeAlgBase())
    );
    // we must be sure to set (myPartialDerivativeKind, myValue, ValueHasBeenSet, myLHSVariable_p) in each case
    switch (theLinearizedExpressionEdgeAlg.getPartialDerivativeKind()) {
      case PartialDerivativeKind::LINEAR_ONE: {
	myPartialDerivativeKind = PartialDerivativeKind::LINEAR_ONE;
	myValue = 0;
	ValueHasBeenSet = false;
	myLHSVariable_p = NULL;
	break;
      } // end case LINEAR_ONE
      case PartialDerivativeKind::LINEAR_MINUS_ONE: {
	myPartialDerivativeKind = PartialDerivativeKind::LINEAR_MINUS_ONE;
	myValue = 0;
	ValueHasBeenSet = false;
	myLHSVariable_p = NULL;
	break;
      } // end case LINEAR_MINUS_ONE
      case PartialDerivativeKind::LINEAR: {
        // (FIXME) HACK: set PDK to nonlinear if PDK is linear but not an isolated constant so that no attempt is made to fold in the value
        if (theLinearizedExpressionEdgeAlg.hasConcretePartialAssignment()) {
	  myPartialDerivativeKind = PartialDerivativeKind::NONLINEAR;
	  myValue = 0;
	  ValueHasBeenSet = false;
	  myLHSVariable_p = &theLinearizedExpressionEdgeAlg.getConcretePartialAssignment().getLHS();
        } // end if there's a concrete partial assignment
        else { // no partial assignment has been created, so there must be a constant set
	  myPartialDerivativeKind = PartialDerivativeKind::LINEAR;
          myValue = theLinearizedExpressionEdgeAlg.getConcreteConstant().getdouble();
	  ValueHasBeenSet = true;
	  myLHSVariable_p = NULL;
	} // end RHS has 1 vertex (Constant)
	break;
      } // end case LINEAR
      case PartialDerivativeKind::NONLINEAR: {
	  myPartialDerivativeKind = PartialDerivativeKind::NONLINEAR;
	  myValue = 0;
	  ValueHasBeenSet = false;
	  myLHSVariable_p = &theLinearizedExpressionEdgeAlg.getConcretePartialAssignment().getLHS();
	break;
      } // end case NONLINEAR
      default:
	THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::AccumulationGraphVertex: Invalid PDK ("
				   << PartialDerivativeKind::toString(theLinearizedExpressionEdgeAlg.getPartialDerivativeKind())
				   << ") for ExpressionEdgeAlg of ExpressionEdge passed to constructor");
	break;
    } // end switch (PDK)
  } // end AccumulationGraphVertex::AccumulationGraphVertex(const ExpressionEdge& theExpressionEdge)

  AccumulationGraphVertex::AccumulationGraphVertex(const JacobianAccumulationExpressionVertex::Operation_E& anOpType) :
   myExpressionEdge_p(NULL),
   myRemainderGraphEdge_p(NULL),
   myHasOperationFlag(true),
   myOperationType(anOpType),
   myPartialDerivativeKind(PartialDerivativeKind::NOT_SET),
   myValue(0),
   ValueHasBeenSet(false),
   myLHSVariable_p(NULL) {
  } // end AccumulationGraphVertex::AccumulationGraphVertex(const std::string anOpName)

  const bool
  AccumulationGraphVertex::hasExpressionEdge() const {
    return (myExpressionEdge_p != NULL);
  }

  const ExpressionEdge&
  AccumulationGraphVertex::getExpressionEdge() const {
    if (!myExpressionEdge_p)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getExpressionEdge: not set")
    return *myExpressionEdge_p;
  }

  void AccumulationGraphVertex::setRemainderGraphEdge(const LinearizedComputationalGraphEdge& anLCGEdge) {
    if (myRemainderGraphEdge_p)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::setRemainderGraphEdge: already set")
    myRemainderGraphEdge_p = &anLCGEdge;
  } // end AccumulationGraphVertex::setRemainderGraphEdge()

  const LinearizedComputationalGraphEdge& AccumulationGraphVertex::getRemainderGraphEdge() const {
    if (!myRemainderGraphEdge_p)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getRemainderGraphEdge: not set")
    return *myRemainderGraphEdge_p;
  } // end AccumulationGraphVertex::getRemainderGraphEdge()

  const bool AccumulationGraphVertex::hasRemainderGraphEdge() const {
    return (myRemainderGraphEdge_p != NULL);
  } // end AccumulationGraphVertex::hasRemainderGraphEdge()

  const bool
  AccumulationGraphVertex::hasOperation() const {
    return myHasOperationFlag;
  }

  JacobianAccumulationExpressionVertex::Operation_E AccumulationGraphVertex::getOperation() const {
    return myOperationType;
  } // end AccumulationGraphVertex::getOperation()

  void AccumulationGraphVertex::setPartialDerivativeKind(const PartialDerivativeKind::PartialDerivativeKind_E aPartialDerivativeKind) {
    myPartialDerivativeKind = aPartialDerivativeKind;
  } // end AccumulationGraphVertex::setPartialDerivativeKind()

  const PartialDerivativeKind::PartialDerivativeKind_E AccumulationGraphVertex::getPartialDerivativeKind() const {
    return myPartialDerivativeKind;
  } // end AccumulationGraphVertex::getPartialDerivativeKind()

  void AccumulationGraphVertex::setValue(double aValue) {
    if (ValueHasBeenSet)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::setValue: already set")
    ValueHasBeenSet = true;
    myValue = aValue;
  } // end AccumulationGraphVertex::setValue()

  double AccumulationGraphVertex::getValue() const {
    if (!ValueHasBeenSet)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getValue: not set")
    return myValue;
  } // end AccumulationGraphVertex::getValue()

  const bool AccumulationGraphVertex::hasValue() const {
    return ValueHasBeenSet;
  } // end AccumulationGraphVertex::hasValue()

  bool
  AccumulationGraphVertex::isConstant() const {
    if (hasValue()
     || (getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_ONE)
     || (getPartialDerivativeKind() == PartialDerivativeKind::LINEAR_MINUS_ONE)
     || (getPartialDerivativeKind() == PartialDerivativeKind::LINEAR)
    ) return true;
    else
      return false;
  }

  void AccumulationGraphVertex::setLHSVariable(const Variable& aVariable) {
    if (myLHSVariable_p)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getLHSVariable: already set")
    myLHSVariable_p = &aVariable;
  } // end AccumulationGraphVertex::setLHSVariable()

  const Variable& AccumulationGraphVertex::getLHSVariable() const {
    if (!myLHSVariable_p)
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getLHSVariable: not set")
    return *myLHSVariable_p;
  } // end AccumulationGraphVertex::getLHSVariable()

  const bool AccumulationGraphVertex::hasLHSVariable() const {
    return (myLHSVariable_p != NULL);
  } // end AccumulationGraphVertex::hasLHSVariable()

  std::string AccumulationGraphVertex::debug() const {
    std::ostringstream out;
    out << "AccumulationGraphVertex[" << Vertex::debug().c_str();
    // ExpressionEdge
    out << ",myExpressionEdge_p=";
    if (myExpressionEdge_p) out << ">" << myExpressionEdge_p->debug().c_str();
    else out << myExpressionEdge_p;
    // LHSVariable
    out << ",myLHSVariable_p=";
    if (myLHSVariable_p) out << ">" << myLHSVariable_p->debug().c_str();
    else out << myLHSVariable_p;
    // RemainderGraphEdge
    out << ",myRemainderGraphEdge_p=";
    if (myRemainderGraphEdge_p) out << ">" << myRemainderGraphEdge_p->debug().c_str();
    else out << myRemainderGraphEdge_p;
    out << ",myOperationType=" << myOperationType
	<< ",myPartialDerivativeKind=" << PartialDerivativeKind::toString(myPartialDerivativeKind)
	<< ",myValue=" << myValue
	<< ",ValueHasBeenSet=" << ValueHasBeenSet
	<< "]" << std::ends;
    return out.str();
  } // end AccumulationGraphVertex::debug()

  std::string
  AccumulationGraphVertex::getColorString() const {
    switch(getPartialDerivativeKind()) {
      case PartialDerivativeKind::LINEAR_ONE:
        return "red";
      case PartialDerivativeKind::LINEAR_MINUS_ONE:
        return "pink";
      case PartialDerivativeKind::LINEAR:
        return "blue";
      case PartialDerivativeKind::NONLINEAR:
        return "black";//"goldenrod4"
      case PartialDerivativeKind::PASSIVE:
      //THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getColorString: ACCVertex PDK is PASSIVE");
        return "grey";
      case PartialDerivativeKind::NOT_SET:
        THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getColorString: ACCVertex PDK is NOT_SET");
      //return "orange";
      default:
        THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getColorString: unexpected ACCVertex PDK");
    } // end switch PDK
  }

  std::string
  AccumulationGraphVertex::getValueString() const {
    std::ostringstream out;
    switch(getPartialDerivativeKind()) {
      case PartialDerivativeKind::LINEAR_ONE:
        out << "1";
        break;
      case PartialDerivativeKind::LINEAR_MINUS_ONE:
        out << "-1";
        break;
      case PartialDerivativeKind::LINEAR:
        out << getValue();
        break;
      case PartialDerivativeKind::NONLINEAR:
        out << this;
        break;
      case PartialDerivativeKind::PASSIVE:
      case PartialDerivativeKind::NOT_SET:
      default:
        THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getValueString: unexpected PDK for " << debug().c_str());
        break;
    } // end switch PDK
    return out.str();
  }

  std::string
  AccumulationGraphVertex::getOperationString() const {
    if (hasOperation()) {
      if (myOperationType == xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::ADD_OP)
        return "+";
      else if (myOperationType == xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex::MULT_OP)
        return "*";
      else
        THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getOperationString: invalid operation");
    }
    else
      return "";
  }

  std::string
  AccumulationGraphVertex::getLHSString() const {
    std::ostringstream out;
    if (!hasLHSVariable())
      THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getLHSString: no LHS Variable");
    out << getLHSVariable().getVariableSymbolReference().getSymbol().getId();
    return out.str();
  }

  std::string
  AccumulationGraphVertex::getRHSString() const {
    std::ostringstream out;
    if (hasOperation()) {
      out << getOperationString().c_str();
      if (isConstant()) out << " = ";
    }
    if (isConstant()) {
      out  << getValueString().c_str();
    }
    return out.str();
  }

  std::string
  AccumulationGraphVertex::getLabelString() const {
    std::ostringstream out;
    if (hasLHSVariable()) {
      out << getLHSString().c_str();
      if (hasOperation() || isConstant())
        out << " = ";
    }
    out << getRHSString().c_str();
    if (getPartialDerivativeKind() == PartialDerivativeKind::PASSIVE) {
    //THROW_LOGICEXCEPTION_MACRO("AccumulationGraphVertex::getLabelString: unexpected PDK PASSIVE for " << debug().c_str());
      out << " [PASSIVE]";
    }
    return out.str();
  }

} // end namespace xaifBoosterCrossCountryInterface

