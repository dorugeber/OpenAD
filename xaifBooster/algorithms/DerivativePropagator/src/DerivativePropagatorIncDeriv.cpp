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

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorIncDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator {

  const std::string DerivativePropagatorIncDeriv::ourXAIFName("xaif:IncDeriv");
  const std::string DerivativePropagatorIncDeriv::our_myTarget_XAIFName("xaif:Tgt");
  const std::string DerivativePropagatorIncDeriv::our_mySource_XAIFName("xaif:Src");

  DerivativePropagatorIncDeriv::DerivativePropagatorIncDeriv(const Variable& theTarget,
                                                             const Variable& theSource) :
    DerivativePropagatorEntry(theTarget) {
    theSource.copyMyselfInto(mySource);
    mySource.setId(1);
    mySource.setDerivFlag();
  } // end DerivativePropagatorIncDeriv::DerivativePropagatorIncDeriv

  void
  DerivativePropagatorIncDeriv::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl; 
    printMemberXMLHierarchy(myTarget,our_myTarget_XAIFName,os);
    printMemberXMLHierarchy(mySource,our_mySource_XAIFName,os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str()
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end DerivativePropagatorIncDeriv::printXMLHierarchy()

  void
  DerivativePropagatorIncDeriv::printMemberXMLHierarchy(const Variable& theVariable,
                                                        const std::string& aName,
                                                        std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << aName.c_str() 
       << " "
       << Variable::our_myDerivFlag_XAIFName.c_str()
       << "=\""
       << theVariable.getDerivFlag()
       << "\""
       << ">" 
       << std::endl; 
    theVariable.printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << aName.c_str()
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end DerivativePropagatorIncDeriv::printXMLHierarchy()

  std::string DerivativePropagatorIncDeriv::debug() const { 
    std::ostringstream out;
    out << "DerivativePropagatorIncDeriv[" << this 
	<< DerivativePropagatorEntry::debug().c_str()
	<< ", mySource=" << mySource.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end DerivativePropagatorIncDeriv::debug()

  void  DerivativePropagatorIncDeriv::getFactors(FactorList& theFactorList) const {
    Factor aFactor;
    aFactor.setUnit();
    aFactor.setSource(mySource);
    theFactorList.push_back(aFactor);
  }

  bool DerivativePropagatorIncDeriv::isIncremental() const {
    return true;
  }

  bool
  DerivativePropagatorIncDeriv::hasExpression(const Expression& anExpression) const {
    return (mySource.hasExpression(anExpression)
         || DerivativePropagatorEntry::hasExpression(anExpression));
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorIncDeriv::asInlinableSubroutineCall() const {
	  return asSourceTargetInlinableSubroutineCall("inc_deriv",mySource);
  }

}
