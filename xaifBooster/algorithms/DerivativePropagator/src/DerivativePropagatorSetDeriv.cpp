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

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSetDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  const std::string DerivativePropagatorSetDeriv::ourXAIFName("xaif:SetDeriv");
  const std::string DerivativePropagatorSetDeriv::our_myTarget_XAIFName("xaif:Tgt");
  const std::string DerivativePropagatorSetDeriv::our_mySource_XAIFName("xaif:Src");

  DerivativePropagatorSetDeriv::DerivativePropagatorSetDeriv(const Variable& theTarget,
							     const Variable& theSource) :
    DerivativePropagatorEntry(theTarget) { 
    theSource.copyMyselfInto(mySource);
    mySource.setId(1);
    mySource.setDerivFlag();
  }

  void
  DerivativePropagatorSetDeriv::printXMLHierarchy(std::ostream& os) const { 
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
  } // end of DerivativePropagatorSetDeriv::printXMLHierarchy

  void
  DerivativePropagatorSetDeriv::printMemberXMLHierarchy(const Variable& theVariable,
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
  } // end of DerivativePropagatorSetDeriv::printXMLHierarchy

  std::string DerivativePropagatorSetDeriv::debug () const { 
    std::ostringstream out;
    out << "DerivativePropagatorSetDeriv[" << this 
	<< DerivativePropagatorEntry::debug().c_str()
	<< ", mySource=" << mySource.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of DerivativePropagatorSetDeriv::debug

  void  DerivativePropagatorSetDeriv::getFactors(FactorList& theFactorList) const { 
    Factor aFactor;
    aFactor.setUnit();
    aFactor.setSource(mySource);
    theFactorList.push_back(aFactor);
  } 

  bool
  DerivativePropagatorSetDeriv::hasExpression(const Expression& anExpression) const {
    return (mySource.hasExpression(anExpression)
         || DerivativePropagatorEntry::hasExpression(anExpression));
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorSetDeriv::asInlinableSubroutineCall() const {
	  return asSourceTargetInlinableSubroutineCall("setderiv",mySource);
  }

}
