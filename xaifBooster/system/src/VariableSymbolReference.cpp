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
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

namespace xaifBooster { 

  const std::string VariableSymbolReference::ourXAIFName("xaif:SymbolReference");

  const std::string VariableSymbolReference::our_scopeId_XAIFName("scope_id");

  const std::string VariableSymbolReference::our_myId_XAIFName("vertex_id");

  VariableSymbolReference::VariableSymbolReference (const Symbol& theSymbol,
						    const Scope& theScope) :
    SymbolReference(theSymbol,theScope){
  }

  VariableVertex& 
  VariableSymbolReference::createCopyOfMyself() const { 
    VariableSymbolReference* aNewVariableSymbolReference_p=new VariableSymbolReference(mySymbol_r,
										       myScope_r);
    aNewVariableSymbolReference_p->setId(getId());
    aNewVariableSymbolReference_p->setAnnotation(getAnnotation());
    return *aNewVariableSymbolReference_p;
  }

  std::string VariableSymbolReference::equivalenceSignature() const { 
    std::ostringstream oss;
    oss << "VariableSymbolReference::" 
	<< &mySymbol_r 
	<< "_" 
	<< mySymbol_r.getId().c_str()
	<< "_"
	<< myScope_r.getId().c_str()
	<< std::ends;
    return std::string(oss.str());
  } 

  void VariableSymbolReference::printXMLHierarchy(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName 
       << " "
       << our_myId_XAIFName
       << "=\""
       << getId().c_str()
       << "\" " 
       << Symbol::our_myId_XAIFName
       << "=\"" 
       << mySymbol_r.getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\" " 
       << our_scopeId_XAIFName 
       << "=\"" 
       << myScope_r.getId().c_str()
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end if VariableSymbolReference::printXMLHierarchy
  
  std::string VariableSymbolReference::debug () const { 
    std::ostringstream out;
    out << "VariableSymbolReference[" << this 
	<< " mySymbol=" << mySymbol_r.getId().c_str()
	<< " myScope=" << myScope_r.getId().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of VariableSymbolReference::debug

  bool VariableSymbolReference::isSymbol() const { 
    return true;
  } 

} // end of namespace xaifBooster 
