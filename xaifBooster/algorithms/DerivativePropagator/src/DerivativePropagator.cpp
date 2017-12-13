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

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagator.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSaxpy.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSetDeriv.hpp"
#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorZeroDeriv.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  const std::string DerivativePropagator::ourXAIFName("xaif:DerivativePropagator");

  DerivativePropagator::~DerivativePropagator(){
    for (InlinableSubroutineCallPList::iterator i(myPropagationAllocationList.begin()); i != myPropagationAllocationList.end(); ++i)
        if (*i)
          delete *i;
    for(EntryPList::const_iterator  entryList_iterator= myEntryPList.begin();
	entryList_iterator!= myEntryPList.end();
	++entryList_iterator)
      if (*entryList_iterator)
	delete *entryList_iterator;
  }

  void
  DerivativePropagator::printXMLHierarchyImpl(std::ostream& os,
					      const DerivativePropagator& aDerivativePropagator) { 
    if (!aDerivativePropagator.myEntryPList.size())
      return;
    aDerivativePropagator.printPropagationAllocationList(os);
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName 
       << ">" 
       << std::endl; 
    for(EntryPList::const_iterator  entryList_iterator= aDerivativePropagator.myEntryPList.begin();
	entryList_iterator!= aDerivativePropagator.myEntryPList.end();
	++entryList_iterator)
      (*entryList_iterator)->printXMLHierarchy(os);       
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagator::printXMLHierarchyImpl

  void
  DerivativePropagator::printXMLHierarchy(std::ostream& os) const { 
    printXMLHierarchyImpl(os,*this);
  } // end of DerivativePropagator::printXMLHierarchy

  std::string DerivativePropagator::debug () const { 
    std::ostringstream out;
    out << "DerivativePropagator[" << this 
	<< " #entries=" << myEntryPList.size()
	<< "]" << std::ends;  
    return out.str();
  } // end of DerivativePropagator::debug

  const DerivativePropagatorSetDeriv& 
  DerivativePropagator::addSetDerivToEntryPList(const Variable& theTarget,
						const Variable& theSource) { 
    DerivativePropagatorSetDeriv* theDPSD_p= new DerivativePropagatorSetDeriv(theTarget,
									      theSource);
    myEntryPList.push_back(theDPSD_p);
    return *theDPSD_p;
  } // end DerivativePropagator::addSetDerivToEntryPList()

  const DerivativePropagatorSetNegDeriv&
  DerivativePropagator::addSetNegDerivToEntryPList(const Variable& theTarget,
                                                   const Variable& theSource) {
    DerivativePropagatorSetNegDeriv* theDPSetNegDeriv_p = new DerivativePropagatorSetNegDeriv(theTarget,
                                                                                              theSource);
    myEntryPList.push_back(theDPSetNegDeriv_p);
    return *theDPSetNegDeriv_p;
  } // end DerivativePropagator::addSetNegDerivToEntryPList()

  const DerivativePropagatorIncDeriv&
  DerivativePropagator::addIncDerivToEntryPList(const Variable& theTarget,
                                                const Variable& theSource) {
    DerivativePropagatorIncDeriv* theDPIncDeriv_p = new DerivativePropagatorIncDeriv(theTarget,
                                                                                     theSource);
    myEntryPList.push_back(theDPIncDeriv_p);
    return *theDPIncDeriv_p;
  } // end DerivativePropagator::addIncDerivToEntryPList()

  const DerivativePropagatorDecDeriv&
  DerivativePropagator::addDecDerivToEntryPList(const Variable& theTarget,
                                                const Variable& theSource) {
    DerivativePropagatorDecDeriv* theDPDecDeriv_p = new DerivativePropagatorDecDeriv(theTarget,
                                                                                     theSource);
    myEntryPList.push_back(theDPDecDeriv_p);
    return *theDPDecDeriv_p;
  } // end DerivativePropagator::addDecDerivToEntryPList()

  DerivativePropagatorSaxpy& DerivativePropagator::addSaxpyToEntryPList(const Variable& thePartial,
									const Variable& theIndependent,
									const Variable& theDependent) {
    DerivativePropagatorSaxpy* aSaxpy_p=new DerivativePropagatorSaxpy(thePartial,
								      theIndependent,
								      theDependent);
    myEntryPList.push_back(aSaxpy_p);
    return *aSaxpy_p;
  } 

  DerivativePropagatorSaxpy& DerivativePropagator::addSaxpyToEntryPList(const Constant& thePartial,
									const Variable& theIndependent,
									const Variable& theDependent) {
    DerivativePropagatorSaxpy* aSaxpy_p=new DerivativePropagatorSaxpy(thePartial,
								      theIndependent,
								      theDependent);
    myEntryPList.push_back(aSaxpy_p);
    return *aSaxpy_p;
  } 

  void DerivativePropagator::addZeroDerivToEntryPList(const Variable& theTarget) { 
    myEntryPList.push_back(new DerivativePropagatorZeroDeriv(theTarget));
  } 

  const DerivativePropagator::EntryPList& DerivativePropagator::getEntryPList() const { 
    return myEntryPList;
  }

  DerivativePropagator::EntryPList& DerivativePropagator::getEntryPList() { 
    return myEntryPList;
  }

  bool
  DerivativePropagator::hasExpression(const Expression& anExpression) const {
    // iterate through the sequences, which in turn will check their respective derivative propagators
    for (EntryPList::const_iterator entryPI = myEntryPList.begin(); entryPI != myEntryPList.end(); ++entryPI)
      if ((*entryPI)->hasExpression(anExpression))
        return true;
    return false;
  }

  DerivativePropagator::InlinableSubroutineCallPList&
  DerivativePropagator::getPropagationAllocationList() {
    return myPropagationAllocationList;
  }

  const DerivativePropagator::InlinableSubroutineCallPList&
  DerivativePropagator::getPropagationAllocationList() const {
    return myPropagationAllocationList;
  }

  void
  DerivativePropagator::printPropagationAllocationList(std::ostream& os) const {
    for (InlinableSubroutineCallPList::const_iterator ali(myPropagationAllocationList.begin());
           ali != myPropagationAllocationList.end();
           ++ali)
          (*ali)->printXMLHierarchy(os);
  }

}
