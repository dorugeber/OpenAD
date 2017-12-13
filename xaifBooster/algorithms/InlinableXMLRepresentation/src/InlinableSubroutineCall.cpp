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

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

using namespace xaifBooster;

namespace xaifBoosterInlinableXMLRepresentation { 

  const std::string InlinableSubroutineCall::ourXAIFName("xaif:InlinableSubroutineCall");

  const std::string InlinableSubroutineCall::our_mySubroutineName_XAIFName("subroutine_name");

  const std::string InlinableSubroutineCall::our_myId_XAIFName("statement_id");

  InlinableSubroutineCall::InlinableSubroutineCall(const std::string& aSubroutineName) : 
    mySubroutineName(aSubroutineName) { 
  } 

  InlinableSubroutineCall::~InlinableSubroutineCall(){
    for (SubroutineCall::ConcreteArgumentPList::iterator i=myConcreteArgumentPList.begin();
	 i!=myConcreteArgumentPList.end();
	 ++i)
      if (*i)
	delete *i;
  }

  void
  InlinableSubroutineCall::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str()
       << " "
       << our_mySubroutineName_XAIFName.c_str() 
       << "=\"" 
       << mySubroutineName.c_str()
       << "\""
       << " "
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\""
       << ">"
       << std::endl;
    for (SubroutineCall::ConcreteArgumentPList::const_iterator i=myConcreteArgumentPList.begin();
	 i!=myConcreteArgumentPList.end();
	 ++i)
      (*i)->printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of InlinableSubroutineCall::printXMLHierarchy

  std::string InlinableSubroutineCall::debug () const { 
    std::ostringstream out;
    out << "InlinableSubroutineCall[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of InlinableSubroutineCall::debug

  ConcreteArgument& InlinableSubroutineCall::addConcreteArgument(unsigned int aPosition) { 
    for (SubroutineCall::ConcreteArgumentPList::iterator i=myConcreteArgumentPList.begin();
	 i!=myConcreteArgumentPList.end();
	 ++i)
      if ((*i)->getPosition()==aPosition)
	THROW_LOGICEXCEPTION_MACRO("InlinableSubroutineCall::addConcreteArgument: position " 
				   << aPosition
				   << " already in use");
    ConcreteArgument* newConcreteArgument_p=new ConcreteArgument(aPosition);
    myConcreteArgumentPList.push_back(newConcreteArgument_p);
    return * newConcreteArgument_p;
  } 

  const SubroutineCall::ConcreteArgumentPList&  
  InlinableSubroutineCall::getArgumentList() const { 
    return myConcreteArgumentPList;
  } 

  const std::string& 
  InlinableSubroutineCall::getSubroutineName() const { 
    return mySubroutineName;
  }

  void 
  InlinableSubroutineCall::appendSuffix(const std::string& aSuffix) { 
    mySubroutineName+=aSuffix;
  } 

} 
