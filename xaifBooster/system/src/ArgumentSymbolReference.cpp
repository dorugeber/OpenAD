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
#include "xaifBooster/system/inc/ArgumentSymbolReference.hpp"
#include "xaifBooster/system/inc/ArgumentSymbolReferenceAlgFactory.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"

namespace xaifBooster { 

  const std::string ArgumentSymbolReference::ourXAIFName("xaif:ArgumentSymbolReference");
  const std::string ArgumentSymbolReference::our_myPosition_XAIFName("position");
  const std::string ArgumentSymbolReference::our_symbolId_XAIFName("symbol_id");
  const std::string ArgumentSymbolReference::our_scopeId_XAIFName("scope_id");
  const std::string ArgumentSymbolReference::our_myIntent_XAIFName("intent");

  ArgumentSymbolReference::ArgumentSymbolReference (const Symbol& theSymbol,
						    const Scope& theScope,
						    unsigned int thePosition,
						    IntentType::IntentType_E theIntent,
						    bool makeAlgorithm) :
    SymbolReference(theSymbol,theScope),
    myPosition(thePosition),
    myIntent(theIntent) {
    if (makeAlgorithm)
      myArgumentSymbolReferenceAlgBase_p=
	ArgumentSymbolReferenceAlgFactory::instance()->makeNewAlg(*this); 
  }

  ArgumentSymbolReference::~ArgumentSymbolReference() {
    if (myArgumentSymbolReferenceAlgBase_p)
      delete myArgumentSymbolReferenceAlgBase_p;
  } 

  ArgumentSymbolReferenceAlgBase& 
  ArgumentSymbolReference::getArgumentSymbolReferenceAlgBase() const { 
    if (!myArgumentSymbolReferenceAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("ArgumentSymbolReference::getArgumentSymbolReferenceAlgBase: not set");
    return *myArgumentSymbolReferenceAlgBase_p;
  }

  void
  ArgumentSymbolReference::printXMLHierarchy(std::ostream& os) const { 
    if (myArgumentSymbolReferenceAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getArgumentSymbolReferenceAlgBase().printXMLHierarchy(os);
    else 
      printXMLHierarchyImpl(os);
  }

  void ArgumentSymbolReference::printXMLHierarchyImpl(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " "
       << our_myPosition_XAIFName.c_str()
       << "=\""
       << myPosition
       << "\" " 
       << our_symbolId_XAIFName.c_str()
       << "=\"" 
       << mySymbol_r.getId().c_str()
       << "\" " 
       << our_scopeId_XAIFName.c_str() 
       << "=\"" 
       << myScope_r.getId().c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\" " 
       << our_myIntent_XAIFName.c_str() 
       << "=\"" 
       << IntentType::toString(myIntent).c_str()
       << "\"/>" 
       << std::endl; 
    pm.releaseInstance();
  } // end if ArgumentSymbolReference::printXMLHierarchy
  
  std::string ArgumentSymbolReference::debug () const { 
    std::ostringstream out;
    out << "ArgumentSymbolReference[" 
	<< this 
	<< ","
	<< "myArgumentSymbolReferenceAlgBase_p"
	<< "="
	<< myArgumentSymbolReferenceAlgBase_p
	<< "]" << std::ends;  
    return out.str();
  } // end of ArgumentSymbolReference::debug

  unsigned int ArgumentSymbolReference::getPosition() const { 
    return myPosition;
  } 
    
  IntentType::IntentType_E ArgumentSymbolReference::getIntent() const { 
    return myIntent;
  } 

  void ArgumentSymbolReference::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
    getArgumentSymbolReferenceAlgBase().genericTraversal(anAction_c);
  } 


} // end of namespace xaifBooster 
