// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/SymbolAlgFactory.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

namespace xaifBooster { 

  const std::string Symbol::ourXAIFName("xaif:Symbol");

  const std::string Symbol::our_myId_XAIFName("symbol_id");

  const std::string Symbol::our_myActiveTypeFlag_XAIFName("active");

  const std::string Symbol::our_myTempFlag_XAIFName("temp");

  HashTable<Symbol::SymbolPassivation> Symbol::ourPassivatedSymbolHashTable;

  bool Symbol::ourCaseSensitiveFlag=false; 

  FrontEndDecorations::FrontEndDecorations_E Symbol::ourFrontEndDecorations=FrontEndDecorations::OPEN64_STYLE;

  Symbol::Symbol(const std::string& aName, 
		 const SymbolKind::SymbolKind_E& aKind,
		 const SymbolType::SymbolType_E& aType,
		 const SymbolShape::SymbolShape_E& aShape,
		 bool anActiveTypeFlag,
		 bool aTempFlag,
		 bool makeAlgorithm) : 
    myKind(aKind),
    myType(aType),
    myShape(aShape),
    myActiveTypeFlag(anActiveTypeFlag),
    myTempFlag(aTempFlag),
    mySymbolAlgBase_p(0) {
    setId(aName);
    if (makeAlgorithm)
      mySymbolAlgBase_p=SymbolAlgFactory::instance()->makeNewAlg(*this);
  }

  Symbol::~Symbol() {
    if (mySymbolAlgBase_p) delete mySymbolAlgBase_p;
  }

  void Symbol::printXMLHierarchy(std::ostream& os) const {
    if (mySymbolAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getSymbolAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of Symbol::printXMLHierarchy


  void
  Symbol::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << SymbolKind::our_attribute_XAIFName.c_str() 
       << "=\""
       << SymbolKind::toString(myKind).c_str()
       << "\" " 
       << SymbolType::our_attribute_XAIFName.c_str() 
       << "=\""
       << SymbolType::toString(myType).c_str()
       << "\" " 
       << FrontEndType::our_attribute_XAIFName.c_str() 
       << "=\""
       << myFrontEndType.c_str()
       << "\" " 
       << SymbolShape::our_attribute_XAIFName.c_str() 
       << "=\""
       << SymbolShape::toString(myShape).c_str()
       << "\" " 
       << ObjectWithAnnotation::our_myAnnotation_XAIFName.c_str() 
       << "=\""
       << getAnnotation().c_str()
       << "\" " 
       << our_myActiveTypeFlag_XAIFName.c_str() 
       << "=\""
       << myActiveTypeFlag
       << "\" " 
       << our_myTempFlag_XAIFName.c_str() 
       << "=\""
       << myTempFlag
       << "\">" 
       << std::endl; 
    for (DimensionBoundsPList::const_iterator li=myDimensionBoundsPList.begin();
	 li!=myDimensionBoundsPList.end();
	 ++li)
      (*li)->printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl; 
    pm.releaseInstance();
  } // end 
  
  std::string Symbol::debug () const { 
    std::ostringstream out;
    out << "Symbol[" << this 
	<< ",myName=" << getId().c_str() 
	<< ",myKind=" << SymbolKind::toString(myKind).c_str() 
	<< ",myType=" << SymbolType::toString(myType).c_str()
	<< ",myShape=" << SymbolShape::toString(myShape).c_str()
	<< ",myActiveTypeFlag=" << myActiveTypeFlag
	<< ",myTempFlag=" << myTempFlag
	<< "]" << std::ends;  
    return out.str();
  } // end of Symbol::debug

  const SymbolKind::SymbolKind_E& Symbol::getSymbolKind() const { 
    return myKind; 
  }

  const SymbolType::SymbolType_E& Symbol::getSymbolType() const { 
    return myType; 
  }

  const SymbolShape::SymbolShape_E& Symbol::getSymbolShape() const {
    return myShape;
  }

  bool Symbol::getActiveTypeFlag() const { 
    return myActiveTypeFlag;
  }

  void Symbol::addDimensionBounds(int aLower, 
				  int anUpper) { 
    myDimensionBoundsPList.push_back(new DimensionBounds(aLower,anUpper));
  }

  const Symbol::DimensionBoundsPList& Symbol::getDimensionBoundsPList() const { 
    if (myShape==SymbolShape::SCALAR)
      THROW_LOGICEXCEPTION_MACRO("Symbol::getDimensionBoundsPList: no dimensions for scalar symbols");
    return myDimensionBoundsPList;
  } 

  bool Symbol::hasDimensionBounds()const { 
    return ((myShape!=SymbolShape::SCALAR) && !myDimensionBoundsPList.empty() ? true:false);
  } 

  SymbolAlgBase&
  Symbol::getSymbolAlgBase() const {
    if (!mySymbolAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Symbol::getSymbolAlgBase: not set");
    return *mySymbolAlgBase_p;
  }
                                                                                
  void Symbol::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    getSymbolAlgBase().genericTraversal(anAction_c);
  }

  Symbol::SymbolPassivation::SymbolPassivation() :
    mySymbolp(0),
    myPassivateFlag(false),
    myWarnedFlag(false) {
  }

  Symbol::SymbolPassivation::SymbolPassivation(const std::string& aCommandLineName) :
    myCommandLineName(aCommandLineName),
    mySymbolp(0),
    myPassivateFlag(false),
    myWarnedFlag(false) {
  }

  bool Symbol::SymbolPassivation::hasPassivatedSymbol() const { 
    return ((mySymbolp)?true:false);
  }

  std::string Symbol::SymbolPassivation::getCommandLineName() const { 
    return myCommandLineName;
  }

  Symbol& Symbol::SymbolPassivation::getSymbol() const {
    if (!hasPassivatedSymbol()) 
      THROW_LOGICEXCEPTION_MACRO("Symbol::SymbolPassivation::getSymbol: no symbol set");
    return *mySymbolp;
  }

  void Symbol::SymbolPassivation::passivate(const std::string& anXaifName,
					    Symbol& aSymbol) { 
    if (hasPassivatedSymbol()) { 
      // check if  the new symbol's scope encloses the old symbol's scope
      switch(ConceptuallyStaticInstances::instance()->
	     getCallGraph().getScopeTree().onScopePath(aSymbol,*mySymbolp)) { 
      case Scopes::CHILD_PARENT:
	// the old scope encloses the new one,
	if (!myWarnedFlag) { 
	  // issue a warning
	  DBG_MACRO(DbgGroup::WARNING, "Symbol::SymbolPassivation::passivate: encountered sub scope with symbol "
		    << myCommandLineName.c_str()
		    << " which will not be passivated (warning once only)");
	  myWarnedFlag=true;
	}
	break; 
      case Scopes::PARENT_CHILD: 
	// the new scope encloses the old one
	// move up the reference
	if (myPassivateFlag) 
	  // the old reference was actually passivated, undo ot 
	  mySymbolp->myActiveTypeFlag=true;
	if (!myWarnedFlag) { 
	  // issue a warning
	  DBG_MACRO(DbgGroup::WARNING, "Symbol::SymbolPassivation::passivate: encountered sub scope with symbol "
		    << myCommandLineName.c_str()
		    << " which will not be passivated (warning once only)");
	  myWarnedFlag=true;
	}
	break; 
      case Scopes::NO_PATH:
	THROW_LOGICEXCEPTION_MACRO("Symbol::SymbolPassivation::passivate: symbol "
				   << myCommandLineName.c_str()
				   << " occurs at least in two unrelated scopes");
	break;
      default: 
	THROW_LOGICEXCEPTION_MACRO("Symbol::SymbolPassivation::passivate: no logic to handle Scopes::onPath return value "
				   << ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().onScopePath(aSymbol,*mySymbolp));
	break;
      }
    }
    mySymbolp=&aSymbol;
    myXaifName=anXaifName;
    // note the passivation
    myPassivateFlag=mySymbolp->myActiveTypeFlag;
    // passivate
    mySymbolp->myActiveTypeFlag=false;
  }

  void Symbol::forcedPassivation() { 
    if (!myTempFlag
	&& 
	!ourPassivatedSymbolHashTable.getInternalHashMap().empty()) { 
      // find a matching element
      // we have 2 issues, 
      // first case sensitivity
      std::string theSymbolName(getId());
      if (!ourCaseSensitiveFlag)
	std::transform(theSymbolName.begin(),
		       theSymbolName.end(), 
		       theSymbolName.begin(), 
		       toupper);
      // second the decorations added by the 
      // fortran front end.
      std::string theSymbolNameStripped(stripFrontEndDecorations(theSymbolName,(getSymbolKind()==SymbolKind::SUBROUTINE)));
      if (ourPassivatedSymbolHashTable.hasElement(theSymbolNameStripped))
	ourPassivatedSymbolHashTable.getElement(theSymbolNameStripped).passivate(getId(), 
									       *this);
    } 
  }

  void Symbol::addSymbolNamesToPassivate(const std::string& theSymbolNamesSeparatedBySpaces) { 
    std::string allUpperCase(theSymbolNamesSeparatedBySpaces);
    if (!ourCaseSensitiveFlag)
      std::transform(allUpperCase.begin(),
		     allUpperCase.end(), 
		     allUpperCase.begin(), 
		     toupper);
    std::string::size_type startPosition=0,endPosition=1; // end pos. should at least be 1 or we have an empty string
    std::string::size_type totalSize(allUpperCase.size());
    while (startPosition<=totalSize && endPosition<=totalSize) { 
      startPosition=allUpperCase.find_first_not_of(' ',startPosition);
      endPosition=allUpperCase.find_first_of(' ',startPosition);
      ourPassivatedSymbolHashTable.addElement(allUpperCase.substr(startPosition,
								  endPosition-startPosition),
					      SymbolPassivation(allUpperCase.substr(startPosition,
										    endPosition-startPosition)));
      startPosition=endPosition;
    } 
  }

  void Symbol::setFrontEndDecorations(const FrontEndDecorations::FrontEndDecorations_E& aStyle) { 
    ourFrontEndDecorations=aStyle;
  } 

  std::string Symbol::stripFrontEndDecorations(const std::string& aDecoratedName,
					       bool isSubroutineName) { 
    std::string plainName(aDecoratedName);
    switch (ourFrontEndDecorations) { 
      case FrontEndDecorations::OPEN64_STYLE : {
	// strip the trailing _[0-9]* appended by mfef90 from the variable Name
	std::string open64PlainName(aDecoratedName,0,aDecoratedName.find_last_of('_'));
	std::string aDecoration(aDecoratedName,aDecoratedName.find_last_of('_'),aDecoratedName.size());
	unsigned int position=0;
	while(position<aDecoration.size() && aDecoration[position]=='_')
	  position++;
	while(position<aDecoration.size() && std::isdigit(aDecoration[position]))
	  position++;
	if (position!=aDecoration.size()) {
	  // this doesn't have the proper appendix
	  DBG_MACRO(DbgGroup::ERROR, "Symbol::stripFrontEndDecorations: unexpected decoration string "
		    << aDecoration.c_str() 
		    << " in " 
		    << aDecoratedName.c_str()
		    << " but we continue");
	  return aDecoratedName;
	}
	if (isSubroutineName && open64PlainName[open64PlainName.size()-1]=='_')
	  // only user defined subroutine have an extra _
	  open64PlainName.erase(open64PlainName.size()-1);
	plainName=open64PlainName;
      }
	break;
      case FrontEndDecorations::NO_STYLE :
	break;
      default:
	THROW_LOGICEXCEPTION_MACRO("Symbol::stripFrontEndDecorations: no logic for "<< FrontEndDecorations::toString(ourFrontEndDecorations).c_str());
	break;
    }
    return plainName;
  }
    
  std::string Symbol::plainName() const { 
    if (myTempFlag)
      // temporaries have been created here, not by the front-end
      return getId();
    return stripFrontEndDecorations(getId(), (getSymbolKind()==SymbolKind::SUBROUTINE));
  }

  bool Symbol::samePlainName(const std::string& aPlainName) const { 
    std::string aPlainNameCaseified(aPlainName);
    if (!ourCaseSensitiveFlag)
      std::transform(aPlainNameCaseified.begin(),
		     aPlainNameCaseified.end(), 
		     aPlainNameCaseified.begin(), 
		     toupper);
    std::string theSymbolPlainName(plainName());
    if (!ourCaseSensitiveFlag)
      std::transform(theSymbolPlainName.begin(),
		     theSymbolPlainName.end(), 
		     theSymbolPlainName.begin(), 
		     toupper);
    return (aPlainNameCaseified.compare(theSymbolPlainName)==0);
  }

  void Symbol::setCaseSensitive() { 
    ourCaseSensitiveFlag=true;
  } 

  const FrontEndType& Symbol::getFrontEndType() const { 
    return myFrontEndType;
  }
  
  void Symbol::setFrontEndType(const FrontEndType& aFrontEndType) {
    if (!myFrontEndType.empty())
      THROW_LOGICEXCEPTION_MACRO("Symbol::setFrontEndType: already set");
    myFrontEndType=aFrontEndType;
  }

  bool Symbol::isTemporary()const { 
    return myTempFlag;
  } 

} // end of namespace xaifBooster 
