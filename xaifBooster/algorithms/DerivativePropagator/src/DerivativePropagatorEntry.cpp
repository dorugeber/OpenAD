// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorEntry.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  void DerivativePropagatorEntry::Factor::setZero() { 
    if (myFactorKind!=NOT_SET)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setZero: already set");
    myFactorKind=ZERO_FACTOR;
  }  

  void DerivativePropagatorEntry::Factor::setUnit() { 
    if (myFactorKind!=NOT_SET)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setUnit: already set");
    myFactorKind=UNIT_FACTOR;
  }  

  void DerivativePropagatorEntry::Factor::setNegativeUnit() {
    if (myFactorKind != NOT_SET)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setNegativeUnit: already set");
    myFactorKind = NEGATIVE_UNIT_FACTOR;
  } // end DerivativePropagatorEntry::Factor::setNegativeUnit()

  void DerivativePropagatorEntry::Factor::setConstant(const Constant& theConstant) { 
    if (myFactorKind!=NOT_SET)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setConstant: already set");
    myFactorKind=CONSTANT_FACTOR;
    myFactor.myConstant_p=&theConstant;
  }  

  const Constant& DerivativePropagatorEntry::Factor::getConstant() const { 
    if (myFactorKind!=CONSTANT_FACTOR)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::getConstant: not CONSTANT_FACTOR");
    return *myFactor.myConstant_p;
  }  

  void DerivativePropagatorEntry::Factor::setVariable(const Variable& theVariable) { 
    if (myFactorKind!=NOT_SET)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setVariable: already set");
    myFactorKind=VARIABLE_FACTOR;
    myFactor.myVariable_p=&theVariable;
  }  

  const Variable& DerivativePropagatorEntry::Factor::getVariable() const { 
    if (myFactorKind!=VARIABLE_FACTOR)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::getVariable: not VARIABLE_FACTOR");
    return *myFactor.myVariable_p;
  }  

  DerivativePropagatorEntry::Factor::FactorKind_E 
  DerivativePropagatorEntry::Factor::getKind() const { 
    return myFactorKind;
  }

  void DerivativePropagatorEntry::Factor::setSource(const Variable& theVariable) { 
    if (mySource_p)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::setSource: already set");
    mySource_p=&theVariable;
  }  

  const Variable& DerivativePropagatorEntry::Factor::getSource() const { 
    if (!mySource_p)
      THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorEntry::Factor::getSource: not set");
    return *mySource_p;
  }  

  DerivativePropagatorEntry::DerivativePropagatorEntry(const Variable& theTarget) : myInlinableSubroutineCall_p(NULL) {
    theTarget.copyMyselfInto(myTarget);
    myTarget.setId(1);
    myTarget.setDerivFlag();
  }  

  DerivativePropagatorEntry::~DerivativePropagatorEntry() {
	  if (myInlinableSubroutineCall_p) {
		  delete myInlinableSubroutineCall_p;
	  }
  }

  const Variable& DerivativePropagatorEntry::getTarget() const { 
    return myTarget;
  }  

  std::string DerivativePropagatorEntry::debug () const { 
    std::ostringstream out;
    out << "DerivativePropagatorEntry[" << this 
	<< ", myTarget=" << myTarget.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } 

  bool
  DerivativePropagatorEntry::hasExpression(const Expression& anExpression) const {
    return myTarget.hasExpression(anExpression);
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorEntry::asSourceTargetInlinableSubroutineCall(const std::string& name, const Variable& theSource) const {
	  if (!myInlinableSubroutineCall_p) {
		  std::string suffix;
     	  bool inlinable=true;
		  myInlinableSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(name);
		  myInlinableSubroutineCall_p->setId("asInlinableSubroutineCall");
		  ConcreteArgument& target=myInlinableSubroutineCall_p->addConcreteArgument(1);
		  getTarget().copyMyselfInto(target.getArgument().getVariable());
		  SymbolShape::SymbolShape_E effShape=getTarget().getEffectiveShape();
		  if (effShape!=SymbolShape::SCALAR && getTarget().hasArrayAccess())
			  inlinable=false;
		  suffix+="_"+SymbolShape::toShortString(effShape);
		  ConcreteArgument& source=myInlinableSubroutineCall_p->addConcreteArgument(2);
		  theSource.copyMyselfInto(source.getArgument().getVariable());
		  effShape=theSource.getEffectiveShape();
		  if (effShape!=SymbolShape::SCALAR && theSource.hasArrayAccess())
			  inlinable=false;
		  suffix+="_"+SymbolShape::toShortString(effShape);
     	  if (inlinable)
     		  myInlinableSubroutineCall_p->appendSuffix(suffix);
	  }
	  return *myInlinableSubroutineCall_p;
  }

}

