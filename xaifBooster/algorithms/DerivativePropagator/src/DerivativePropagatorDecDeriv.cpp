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

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorDecDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator {

  const std::string DerivativePropagatorDecDeriv::ourXAIFName("xaif:DecDeriv");
  const std::string DerivativePropagatorDecDeriv::our_myTarget_XAIFName("xaif:Tgt");
  const std::string DerivativePropagatorDecDeriv::our_mySource_XAIFName("xaif:Src");

  DerivativePropagatorDecDeriv::DerivativePropagatorDecDeriv(const Variable& theTarget,
                                                             const Variable& theSource) :
    DerivativePropagatorEntry(theTarget) {
    theSource.copyMyselfInto(mySource);
    mySource.setId(1);
    mySource.setDerivFlag();
  } // end DerivativePropagatorDecDeriv::DerivativePropagatorDecDeriv

  void
  DerivativePropagatorDecDeriv::printXMLHierarchy(std::ostream& os) const { 
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
  } // end DerivativePropagatorDecDeriv::printXMLHierarchy()

  void
  DerivativePropagatorDecDeriv::printMemberXMLHierarchy(const Variable& theVariable,
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
  } // end DerivativePropagatorDecDeriv::printXMLHierarchy()

  std::string DerivativePropagatorDecDeriv::debug() const { 
    std::ostringstream out;
    out << "DerivativePropagatorDecDeriv[" << this 
	<< DerivativePropagatorEntry::debug().c_str()
	<< ", mySource=" << mySource.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end DerivativePropagatorDecDeriv::debug()

  void  DerivativePropagatorDecDeriv::getFactors(FactorList& theFactorList) const {
    Factor aFactor;
    aFactor.setNegativeUnit();
    aFactor.setSource(mySource);
    theFactorList.push_back(aFactor);
  }

  bool DerivativePropagatorDecDeriv::isIncremental() const {
    return true;
  }

  bool
  DerivativePropagatorDecDeriv::hasExpression(const Expression& anExpression) const {
    return (mySource.hasExpression(anExpression)
         || DerivativePropagatorEntry::hasExpression(anExpression));
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorDecDeriv::asInlinableSubroutineCall() const {
	  return asSourceTargetInlinableSubroutineCall("dec_deriv",mySource);
  }

}
