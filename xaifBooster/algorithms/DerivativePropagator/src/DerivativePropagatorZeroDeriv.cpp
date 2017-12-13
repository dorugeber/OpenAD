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

#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorZeroDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  const std::string DerivativePropagatorZeroDeriv::ourXAIFName("xaif:ZeroDeriv");

  DerivativePropagatorZeroDeriv::DerivativePropagatorZeroDeriv(const Variable& theTarget) : DerivativePropagatorEntry(theTarget) { 
  }

  void
  DerivativePropagatorZeroDeriv::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName
       << " "
       << Variable::our_myDerivFlag_XAIFName.c_str()
       << "=\""
       << myTarget.getDerivFlag()
       << "\""
       << ">" 
       << std::endl;
    myTarget.printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagatorZeroDeriv::printXMLHierarchy

  std::string DerivativePropagatorZeroDeriv::debug () const { 
    std::ostringstream out;
    out << "DerivativePropagatorZeroDeriv[" << this 
	<< DerivativePropagatorEntry::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of DerivativePropagatorZeroDeriv::debug

  void  DerivativePropagatorZeroDeriv::getFactors(FactorList& theFactorList) const { 
    Factor aFactor;
    aFactor.setZero();
    theFactorList.push_back(aFactor);
  } 

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorZeroDeriv::asInlinableSubroutineCall() const {
	  if (!myInlinableSubroutineCall_p) {
		  myInlinableSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("zero_deriv");
		  std::string suffix;
		  myInlinableSubroutineCall_p->setId("asInlinableSubroutineCall");
		  ConcreteArgument& target=myInlinableSubroutineCall_p->addConcreteArgument(1);
		  getTarget().copyMyselfInto(target.getArgument().getVariable());
		  SymbolShape::SymbolShape_E effShape=getTarget().getEffectiveShape();
		  if (effShape==SymbolShape::SCALAR || !(getTarget().hasArrayAccess())) {
			  suffix+="_"+SymbolShape::toShortString(effShape);
			  myInlinableSubroutineCall_p->appendSuffix(suffix);
		  }
	  }
	  return *myInlinableSubroutineCall_p;
  }

}
