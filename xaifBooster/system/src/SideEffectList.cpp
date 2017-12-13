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
#include "xaifBooster/system/inc/SideEffectList.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

namespace xaifBooster { 

  const std::string SideEffectList::our_SideEffectReference_XAIFName("xaif:SideEffectReference");

  SideEffectList::SideEffectList(){}

  SideEffectList::~SideEffectList(){ 
    for (VariablePList::iterator i=myVariablePList.begin();
	 i!=myVariablePList.end();
	 ++i){ 
      if (*i)
	delete *i;
    }
  } 

  void
  SideEffectList::printXMLHierarchy(const std::string& aListName,
				    std::ostream& os) const { 
    if (myVariablePList.empty())
      return;
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << aListName.c_str() 
       << ">"
       << std::endl;
    for (VariablePList::const_iterator i=myVariablePList.begin();
	 i!=myVariablePList.end();
	 ++i){ 
      PrintManager& pm1=PrintManager::getInstance();
      os << pm1.indent() 
	 << "<"
	 << our_SideEffectReference_XAIFName.c_str() 
	 << " ";
      (*i)->printVariableReferenceXMLAttributes(os);
      os << ">" 
	 << std::endl;
      (*i)->printXMLHierarchy(os);
      os << pm1.indent()
	 << "</"
	 << our_SideEffectReference_XAIFName.c_str()
	 << ">"
	 << std::endl;
      pm1.releaseInstance();
    } // end for  
    os << pm.indent() 
       << "</"
       << aListName.c_str() 
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of SideEffectList::printXMLHierarchy

  std::string SideEffectList::debug () const { 
    std::ostringstream out;
    out << "SideEffectList[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of SideEffectList::debug

  const SideEffectList::VariablePList& SideEffectList::getVariablePList() const { 
    return myVariablePList;
  }
  
  Variable& SideEffectList::addSideEffectReference() { 
    Variable* theVariable_p=new Variable;
    myVariablePList.push_back(theVariable_p);
    return *theVariable_p;
  } 

  bool SideEffectList::hasElement(const Variable& aVariable) const {
    DBG_MACRO(DbgGroup::DATA,"SideEffectList::hasElement: checking for " 
	      << aVariable.getVariableSymbolReference().getSymbol().getId().c_str());
    for (VariablePList::const_iterator i=myVariablePList.begin();
	 i!=myVariablePList.end();
	 ++i){ 
      if ((*i)->getVariableSymbolReference().refersToSameSymbolAs(aVariable.getVariableSymbolReference())) 
	return true;
    }
    return false; 
  } 

} // end of namespace xaifBooster 
