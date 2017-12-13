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

#include "xaifBooster/algorithms/CodeReplacement/inc/ReplacementList.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/Replacement.hpp"

namespace xaifBoosterCodeReplacement { 

  const std::string ReplacementList::ourXAIFName("xaif:ReplacementList");
  const std::string ReplacementList::our_myTemplateName_XAIFName("template_name");

  ReplacementList::ReplacementList (const Symbol& theSymbol,
				    const Scope& theScope,
				    const Scope& theCFGScope,
				    const std::string& aTemplateName,
				    const ArgumentList& anArgumentList) : 
    ControlFlowGraphCommonAttributes(theSymbol, theScope, theCFGScope),
    myTemplateName(aTemplateName),
    myArgumentList(anArgumentList) { 
  } 

  ReplacementList::~ReplacementList(){
    for (ReplacementPList::iterator i=myReplacementPList.begin();
	 i!=myReplacementPList.end();
	 ++i) 
      if (*i)
	delete (*i);
  }

  void
  ReplacementList::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str()
       << " ";
    printAttributes(os,mySymbolReference);
    os << " "
       << our_myTemplateName_XAIFName.c_str() 
       << "=\"" 
       << myTemplateName.c_str()
       << "\""
       << ">"
       << std::endl; 
    myArgumentList.printXMLHierarchy(os);
    for (ReplacementPList::const_iterator i=myReplacementPList.begin();
	 i!=myReplacementPList.end();
	 ++i) 
      (*i)->printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of ReplacementList::printXMLHierarchy

  std::string ReplacementList::debug () const { 
    std::ostringstream out;
    out << "ReplacementList[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of ReplacementList::debug

  const std::string& ReplacementList::getTemplateName() const {
    return myTemplateName; 
  }

  Replacement& ReplacementList::addReplacement(unsigned int aPlaceHolder) { 
    for (ReplacementPList::iterator i=myReplacementPList.begin();
	 i!=myReplacementPList.end();
	 ++i) 
      if (aPlaceHolder==(*i)->getPlaceHolder())
	THROW_LOGICEXCEPTION_MACRO("ReplacementList::addReplacement: place holder "
				   << aPlaceHolder
				   << " already in use");
    Replacement* aReplacement_p=new Replacement(aPlaceHolder);
    myReplacementPList.push_back(aReplacement_p);
    return *aReplacement_p;
  } 

  const ArgumentList& ReplacementList::getArgumentList() const { 
    return myArgumentList;
  } 

} 
