// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/SymbolShape.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"


#include "xaifBooster/algorithms/TypeChange/inc/SymbolAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"


namespace xaifBoosterTypeChange {  

  SymbolAlg::SymbolAlg(const Symbol& theContainingSymbol) : 
    SymbolAlgBase(theContainingSymbol),
    myIsExternalFlag(false),
    myHasHandCodedWrapperFlag(false),
    myHandCodeWrapperSymbolReference_p(0) { 
  }

  SymbolAlg::~SymbolAlg() { 
    if (myHandCodeWrapperSymbolReference_p)
      delete myHandCodeWrapperSymbolReference_p;
  } 

  void SymbolAlg::printXMLHierarchy(std::ostream& os) const { 
    getContaining().printXMLHierarchyImpl(os);
  }

  std::string 
  SymbolAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterTypeChange::SymbolAlg["
	<< this 
	<< ","
 	<< SymbolAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void SymbolAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  bool SymbolAlg::isExternal() const { 
    return myIsExternalFlag;
  }

  void SymbolAlg::setExternal() { 
    if (myIsExternalFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::setExternal: already set");
    myIsExternalFlag=true;
  }

  void SymbolAlg::internalRename(const SymbolReference& theOriginalSymbolReference) { 
    if (myIsExternalFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::internalRename: set to external");
    if (myHandCodeWrapperSymbolReference_p)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::internalRename: already renamed");
    makeReplacementSymbol(theOriginalSymbolReference);
  }

  bool SymbolAlg::hasHandCodedWrapper() const { 
    if (!myIsExternalFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::hasHandCodedWrapper: is not external");
    return myHasHandCodedWrapperFlag;
  } 

  void SymbolAlg::makeReplacementSymbol(const SymbolReference& theOriginalSymbolReference) { 
    // make the replacement symbol:
    const std::string theNewName(ConceptuallyStaticInstances::instance()->getCallGraph().getPrefix()+
			   theOriginalSymbolReference.getSymbol().getId());
    Symbol& theNewSymbol(ConceptuallyStaticInstances::instance()->
			 getCallGraph().getScopeTree().getScopeById(theOriginalSymbolReference.getScope().getId()).
			 getSymbolTable().
			 addSymbol(theNewName,
				   theOriginalSymbolReference.getSymbol().getSymbolKind(),
				   theOriginalSymbolReference.getSymbol().getSymbolType(),
				   theOriginalSymbolReference.getSymbol().getSymbolShape(),
				   true,
				   true));
    theNewSymbol.setAnnotation("SymbolAlg::makeReplacementSymbol");
    myHandCodeWrapperSymbolReference_p=new SymbolReference(theNewSymbol,theOriginalSymbolReference.getScope());
  } 

  void SymbolAlg::setHandCodedWrapper(const SymbolReference& theOriginalSymbolReference) { 
    if (!myIsExternalFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::setHandCodedWrapper: is not external");
    if (myHasHandCodedWrapperFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::setHandCodedWrapper: already set");
    myHasHandCodedWrapperFlag=true;
    makeReplacementSymbol(theOriginalSymbolReference);
  }
    
  const SignaturePattern& SymbolAlg::getActivityPattern() const { 
    if (!myHasHandCodedWrapperFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::getActivityPattern: has no hand coded wrapper");
    return myActivityPattern;
  } 

  SignaturePattern& SymbolAlg::getActivityPattern() { 
    if (!myHasHandCodedWrapperFlag)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::getActivityPattern: has no hand coded wrapper");
    return myActivityPattern;
  } 

  const SymbolReference& SymbolAlg::getReplacementSymbolReference() const { 
    if (!myHandCodeWrapperSymbolReference_p)
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::getReplacementSymbolReference: has no replacement");
    return *myHandCodeWrapperSymbolReference_p;
  }

  bool SymbolAlg::hasReplacementSymbolReference() const { 
    return (myHandCodeWrapperSymbolReference_p)?true:false;
  }
  
  bool SymbolAlg::needsAllocation() const { 
    if (!getContaining().isTemporary()) { 
      THROW_LOGICEXCEPTION_MACRO("SymbolAlg::needsAllocation: can be called only for temporary variables");
    }
    if (getContaining().getSymbolShape()==SymbolShape::SCALAR)
      return false;
    return !(getContaining().hasDimensionBounds());
  } 
    
} // end of namespace 
