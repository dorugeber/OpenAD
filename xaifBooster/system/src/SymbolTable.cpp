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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/SymbolTable.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

namespace xaifBooster { 

  const std::string SymbolTable::ourXAIFName("xaif:SymbolTable");
  
  SymbolTable::SymbolTable() { 
  } // end of SymbolTable::SymbolTable

  SymbolTable::~SymbolTable() { 
    for(HashTableSymbolP::InternalHashMapType::iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++)
      delete((*myHashMap_iterator).second);
  }

  Symbol& 
  SymbolTable::addSymbol(const std::string& aName, 
			 const SymbolKind::SymbolKind_E& aKind,
			 const SymbolType::SymbolType_E& aType,
			 const SymbolShape::SymbolShape_E& aShape,
			 bool anActiveTypeFlag,
			 bool aTempFlag) { 
    if (myHashTableSymbolP.hasElement(aName)) 
      THROW_LOGICEXCEPTION_MACRO(debug().c_str() 
				 << "::addSymbol new" 
				 << Symbol(aName,
					   aKind,
					   aType,
					   aShape,
					   anActiveTypeFlag,
					   aTempFlag).debug().c_str() 
				 << " colliding with existing " 
				 << myHashTableSymbolP.getElement(aName)->debug().c_str());
    Symbol* theNewSymbol_p(new Symbol(aName,
				      aKind,
				      aType,
				      aShape,
				      anActiveTypeFlag,
				      aTempFlag));
    myHashTableSymbolP.addElement(aName,theNewSymbol_p);
    return *theNewSymbol_p;
  } // end of SymbolTable::addSymbol

  Symbol&  
  SymbolTable::addUniqueAuxSymbol(const SymbolKind::SymbolKind_E& aKind,
				  const SymbolType::SymbolType_E& aType,
				  const SymbolShape::SymbolShape_E& aShape,
				  bool anActiveTypeFlag) { 
    // use a unique name
    std::string aName(ConceptuallyStaticInstances::instance()->getUniversalNameCreator().makeName());
    Symbol& theSymbol(addSymbol(aName,
				aKind,
				aType,
				aShape,
				anActiveTypeFlag,
				true));
    theSymbol.setAnnotation("");
    return theSymbol; 
  } // end SymbolTable::addUniqueAuxSymbol()

  Symbol&  
  SymbolTable::addUniqueSymbol(const NameCreator& aNameCreator,
                               const SymbolKind::SymbolKind_E& aKind,
                               const SymbolType::SymbolType_E& aType,
                               const SymbolShape::SymbolShape_E& aShape,
                               bool anActiveTypeFlag) {
    Symbol& theSymbol (addSymbol(aNameCreator.makeName(),
                                 aKind,
                                 aType,
                                 aShape,
                                 anActiveTypeFlag,
                                 true));
    theSymbol.setAnnotation("");
    return theSymbol; 
  } // end SymbolTable::addUniqueSymbol()

  std::string SymbolTable::debug() const {
    std::ostringstream out;
    out << "SymbolTable[" << this 
	<< "," << myHashTableSymbolP.debug() 
	<< "]" << std::ends; 
    return out.str();
  } // end of SymbolTable::debug
  
  void SymbolTable::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<" 
       << ourXAIFName 
       << ">" 
       << std::endl; 
    for(HashTableSymbolP::InternalHashMapType::const_iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++)
      (*myHashMap_iterator).second->printXMLHierarchy(os);       
    os << pm.indent() 
       << "</"
       << ourXAIFName
       << ">" << std::endl;
    pm.releaseInstance();
  } 

  const Symbol& SymbolTable::getSymbol(const std::string& aName) const { 
    return *(myHashTableSymbolP.getElement(aName));
  } 

  std::list<Symbol*> SymbolTable::getActiveSymbols() {
    std::list<Symbol*> active_symbols;
    for(HashTableSymbolP::InternalHashMapType::iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++) {
      if ((*myHashMap_iterator).second->getActiveTypeFlag()) {
	// add to list
	active_symbols.push_back((*myHashMap_iterator).second);
      }
    }
    // return list
    return active_symbols;
  }

  void SymbolTable::forcedPassivation() { 
    for(HashTableSymbolP::InternalHashMapType::iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++)
      (*myHashMap_iterator).second->forcedPassivation();
  } 

  bool SymbolTable::hasElement(const Symbol& aSymbol) const { 
    for(HashTableSymbolP::InternalHashMapType::const_iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++)
      if ((*myHashMap_iterator).second==&aSymbol)
	return true;
    return false;
  } 

  bool SymbolTable::hasSymbolWithPlainName(const std::string& aPlainName) const { 
    for(HashTableSymbolP::InternalHashMapType::const_iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++) { 
      if ((*myHashMap_iterator).second->samePlainName(aPlainName)) 
	return true;
    }
    return false;
  } 

  const Symbol& SymbolTable::getSymbolWithPlainName(const std::string& aPlainName) const { 
    bool foundOne=false;
    const Symbol* aSymbol_p(0);
    for(HashTableSymbolP::InternalHashMapType::const_iterator myHashMap_iterator=
	  myHashTableSymbolP.getInternalHashMap().begin();
	myHashMap_iterator!=myHashTableSymbolP.getInternalHashMap().end();
	myHashMap_iterator++) { 
      if ((*myHashMap_iterator).second->samePlainName(aPlainName)) { 
	if (!foundOne) { 
	  foundOne=true;
	  aSymbol_p=(*myHashMap_iterator).second;
	}
	else { 
	  THROW_LOGICEXCEPTION_MACRO("SymbolTable::getSymbolWithPlainName: name "
				     << aPlainName.c_str()
				     << " has ambiguous matches "
				     << aSymbol_p->getId().c_str()
				     << " and " 
				     << (*myHashMap_iterator).second->getId());
	}
      }
    }
    if (!foundOne) 
      THROW_LOGICEXCEPTION_MACRO("SymbolTable::getSymbolWithPlainName: no symbol matching plain name "
				 << aPlainName.c_str());
    return *aSymbol_p;
  } 

} 
