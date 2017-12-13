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
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/SubroutineCallAlgBase.hpp"
#include "xaifBooster/system/inc/SubroutineCallAlgFactory.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/ControlFlowGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"

namespace xaifBooster { 

  const std::string SubroutineCall::ourXAIFName("xaif:SubroutineCall");
  const std::string SubroutineCall::our_myId_XAIFName("statement_id");
  const std::string SubroutineCall::our_symbolId_XAIFName("symbol_id");
  const std::string SubroutineCall::our_scopeId_XAIFName("scope_id");
  const std::string SubroutineCall::our_myFormalArgCount_XAIFName("formalArgCount");
  bool SubroutineCall::ourBlackBoxOptimism(true);

  SubroutineCall::SubroutineCall (const Symbol& theSymbol,
				  const Scope& theScope,
				  const ActiveUseType::ActiveUseType_E activeUse,
				  unsigned short formalArgCount,
				  bool makeAlgorithm) :
    mySymbolReference(theSymbol,theScope),
    myActiveUse(activeUse),
    myActiveUseSetFlag(false),
    myFormalArgCount(formalArgCount) { 
    if (makeAlgorithm)
      myBasicBlockElementAlgBase_p=SubroutineCallAlgFactory::instance()->makeNewAlg(*this); 
  } 

  SubroutineCall::~SubroutineCall() { 
    for (ConcreteArgumentPList::iterator i=myConcreteArgumentPList.begin();
	 i!=myConcreteArgumentPList.end();
	 ++i) { 
      if (*i)
	delete *i;
    }
  } 

  SubroutineCallAlgBase& 
  SubroutineCall::getSubroutineCallAlgBase() const { 
    if (!myBasicBlockElementAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("SubroutineCall::getSubroutineCallAlgBase: not set");
    return dynamic_cast<SubroutineCallAlgBase&>(*myBasicBlockElementAlgBase_p);
  }

  void
  SubroutineCall::printXMLHierarchy(std::ostream& os) const { 
    if (myBasicBlockElementAlgBase_p
// 	&& 
// 	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY
	)
      getSubroutineCallAlgBase().printXMLHierarchy(os);
    else 
      printXMLHierarchyImpl(os);
  }

  void
  SubroutineCall::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\" " 
       << our_symbolId_XAIFName.c_str() 
       << "=\"" 
       << mySymbolReference.getSymbol().getId().c_str()
       << "\" " 
       << our_scopeId_XAIFName.c_str() 
       << "=\"" 
       << mySymbolReference.getScope().getId().c_str()
       << "\" " 
       << ActiveUseType::our_attribute_XAIFName.c_str() 
       << "=\"" 
       << ActiveUseType::toString(myActiveUse).c_str()
       << "\" " 
       << our_myFormalArgCount_XAIFName.c_str() 
       << "=\"" 
       << myFormalArgCount
       << "\">" 
       << std::endl;
    for (ConcreteArgumentPList::const_iterator i=myConcreteArgumentPList.begin();
	 i!=myConcreteArgumentPList.end();
	 ++i)
      (*i)->printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } 

  std::string SubroutineCall::debug () const { 
    std::ostringstream out;
    out << "SubroutineCall[" 
	<< this 
	<< BasicBlockElement::debug().c_str()
	<< "myActiveUse"
	<< "="
	<< ActiveUseType::toString(myActiveUse).c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of SubroutineCall::debug

  void SubroutineCall::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
    getSubroutineCallAlgBase().genericTraversal(anAction_c);
  } 

  SubroutineCall::ConcreteArgumentPList& 
  SubroutineCall::getConcreteArgumentPList() { 
    return myConcreteArgumentPList;
  } 
  
  const SubroutineCall::ConcreteArgumentPList& 
  SubroutineCall::getConcreteArgumentPList() const { 
    return myConcreteArgumentPList;
  } 

  const ConcreteArgument& SubroutineCall::getConcreteArgument(unsigned int aPosition) const { 
    for(ConcreteArgumentPList::const_iterator i = myConcreteArgumentPList.begin();
	i!=myConcreteArgumentPList.end();
	++i) { 
      if ((*i)->getPosition()==aPosition)
	return **i;
    }
    THROW_LOGICEXCEPTION_MACRO("SubroutineCall::getConcreteArgument: invalid position" << aPosition);
  } 
  
  const SymbolReference& 
  SubroutineCall::getSymbolReference() const { 
    return mySymbolReference;
  } 

  bool SubroutineCall::getActiveType() const { 
    return mySymbolReference.getSymbol().getActiveTypeFlag();
  }

  bool SubroutineCall::getActiveFlag() const { 
    bool theAnswer(true);
    switch (myActiveUse) {
    case ActiveUseType::ACTIVEUSE: 
      theAnswer=true;
      break;
    case ActiveUseType::PASSIVEUSE:
      theAnswer=false;
      break;
    case ActiveUseType::UNDEFINEDUSE:
      { 
	if ((theAnswer=getActiveType())) // The assignment here is on purpose and not a typo
	  myActiveUse=ActiveUseType::ACTIVEUSE;
	else 
	  myActiveUse=ActiveUseType::PASSIVEUSE;
	myActiveUseSetFlag=true;
	break;
      }
    default: 
      THROW_LOGICEXCEPTION_MACRO("SubroutineCall::getActiveFlag: unknown value of myActiveUse:"
				 << myActiveUse);
      break;
    }
    return theAnswer;
  } 

  void SubroutineCall::setActiveUse(ActiveUseType::ActiveUseType_E anActiveUse) { 
    if (myActiveUseSetFlag)
      THROW_LOGICEXCEPTION_MACRO("SubroutineCall::setActiveUseType: already set, cannot reset");
    myActiveUse=anActiveUse;
    myActiveUseSetFlag=true;
  } 

  ActiveUseType::ActiveUseType_E SubroutineCall::getActiveUse() const { 
    return myActiveUse;
  } 

  bool SubroutineCall::overwrites(const Variable& aVariable) const { 
    // this is a hack until we have better analysis
    // see if it matches anything in the subroutine's mod list: 
    try { 
      const ControlFlowGraph& theSubroutine(ConceptuallyStaticInstances::instance()->getCallGraph().
 					  getSubroutineBySymbolReference(getSymbolReference()).getControlFlowGraph());
      if (theSubroutine.overwrites(aVariable.getVariableSymbolReference()))
	return true;
      // check if it is in the argument list: 
      for (ConcreteArgumentPList::const_iterator i=myConcreteArgumentPList.begin();
	   i!=myConcreteArgumentPList.end();
	   ++i) { 
	if((*i)->isArgument() 
	   && 
	   (*i)->getArgument().getVariable().equivalentTo(aVariable)) { 
	  // is an argument, check the mod-list for the corresponding formal 
	  // find the corresponding formal: 
	  const ArgumentList::ArgumentSymbolReferencePList& theFormalList(theSubroutine.getArgumentList().getArgumentSymbolReferencePList());
	  for (ArgumentList::ArgumentSymbolReferencePList::const_iterator formalIt=theFormalList.begin();
	       formalIt!=theFormalList.end();
	       ++formalIt) { 
	    if ((*formalIt)->getPosition()==(*i)->getPosition()
		&& 
		theSubroutine.overwrites(**formalIt)) { 
	      return true;
	    }
	  }
	}
      }
      return false;
    }
    catch (SubroutineNotFoundException e) { 
      if (ourBlackBoxOptimism)
	return false; 
      else
	return true; 
    } 
  } 

  bool SubroutineCall::getBlackBoxOptimism() { 
    return ourBlackBoxOptimism; 
  } 
  
  void SubroutineCall::noBlackBoxOptimism() { 
    ourBlackBoxOptimism=false;
  } 

  unsigned short SubroutineCall::getFormalArgCount() const { 
    return myFormalArgCount;
  } 
  
  bool
  SubroutineCall::hasExpression(const Expression& anExpression) const {
    for (ConcreteArgumentPList::const_iterator argI = myConcreteArgumentPList.begin();
         argI != myConcreteArgumentPList.end(); ++argI)
      if ((*argI)->getArgument().getVariable().hasExpression(anExpression))
        return true;
    if (myBasicBlockElementAlgBase_p)
      return myBasicBlockElementAlgBase_p->hasExpression(anExpression);
    else
    return false;
  } // end SubroutineCall::hasExpression()
} // end of namespace xaifBooster 
