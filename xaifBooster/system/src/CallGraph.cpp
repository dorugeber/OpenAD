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
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/CallGraphAlgFactory.hpp"
#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"

namespace xaifBooster { 

  const std::string CallGraph::ourXAIFName("xaif:CallGraph");
  const std::string CallGraph::our_mySchemaInstance_XAIFName("xmlns:xsi");
  const std::string CallGraph::our_myXAIFInstance_XAIFName("xmlns:xaif");
  const std::string CallGraph::our_mySchemaLocation_XAIFName("xsi:schemaLocation");
  const std::string CallGraph::our_myProgramName_XAIFName("program_name");
  const std::string CallGraph::our_myPrefix_XAIFName("prefix");

  CallGraph::CallGraph(const std::string& aSchemaInstance,
		       const std::string& anXAIFInstance,
		       const std::string& aSchemaLocation,
		       const std::string& aPrefix) : 
    mySchemaInstance(aSchemaInstance),
    myXAIFInstance(anXAIFInstance),
    mySchemaLocation(aSchemaLocation),
    myPrefix(aPrefix) {
    myCallGraphAlgBase_p=CallGraphAlgFactory::instance()->makeNewAlg(*this);
  }
                                                                                
  CallGraph::~CallGraph() {
    if (myCallGraphAlgBase_p) delete myCallGraphAlgBase_p;
  }

  void
  CallGraph::printXMLHierarchy(std::ostream& os) const {
    if (myCallGraphAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getCallGraphAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of CallGraph::printXMLHierarchy


  void
  CallGraph::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str() 
       << " " 
       << our_mySchemaInstance_XAIFName.c_str() 
       << "=\"" 
       << mySchemaInstance.c_str()
       << "\" " 
       << our_myXAIFInstance_XAIFName.c_str() 
       << "=\""
       << myXAIFInstance.c_str()
       << "\" " 
       << our_mySchemaLocation_XAIFName.c_str() 
       << "=\""
       << mySchemaLocation.c_str()
       << "\" " 
       << our_myProgramName_XAIFName.c_str() 
       << "=\""
       << myProgramName.c_str()
       << "\" " 
       << our_myPrefix_XAIFName.c_str() 
       << "=\""
       << myPrefix.c_str()
       << "\">" 
       << std::endl; 
    myScopeTree.printXMLHierarchy(os);
    myAliasMap.printXMLHierarchy(os);
    myDuUdMap.printXMLHierarchy(os);
    myDoMap.printXMLHierarchy(os);
    CallGraph::ConstVertexIteratorPair p(vertices());
    CallGraph::ConstVertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt)
      (*beginIt).printXMLHierarchy(os);
    CallGraph::ConstEdgeIteratorPair pe=edges();
    CallGraph::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
    for (;beginIte!=endIte ;++beginIte) 
      (*beginIte).printXMLHierarchy(os,
				    *this);
    os << pm.indent()
       << "</"
       << ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of CallGraph::printXMLHierarchy

  std::string CallGraph::debug () const { 
    std::ostringstream out;
    out << "CallGraph[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of CallGraph::debug

  Scopes& CallGraph::getScopeTree() { 
    return myScopeTree;
  }

  AliasMap& CallGraph::getAliasMap() { 
    return myAliasMap;
  }

  const AliasMap& CallGraph::getAliasMap() const { 
    return myAliasMap;
  }
  
  DuUdMap& CallGraph::getDuUdMap() { 
    return myDuUdMap;
  }

  const DuUdMap& CallGraph::getDuUdMap() const { 
    return myDuUdMap;
  }
 
  DoMap& CallGraph::getDoMap() { 
    return myDoMap;
  }

  const DoMap& CallGraph::getDoMap() const { 
    return myDoMap;
  }
  
  void CallGraph::setProgramName(const std::string& aProgramName) { 
    myProgramName=aProgramName;
  } 

  const std::string& CallGraph::getSchemaLocation() const { 
    return mySchemaLocation;
  } 

  void CallGraph::resetSchemaLocation(const std::string& aNewLocation) { 
    mySchemaLocation=aNewLocation;
  } 

  const std::string& CallGraph::getPrefix() const { 
    return myPrefix;
  }

  CallGraphAlgBase&
  CallGraph::getCallGraphAlgBase() {
    if (!myCallGraphAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraph::getCallGraphAlgBase: not set");
    return *myCallGraphAlgBase_p;
  }
                                                                                
  const CallGraphAlgBase&
  CallGraph::getCallGraphAlgBase() const {
    if (!myCallGraphAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraph::getCallGraphAlgBase: not set");
    return *myCallGraphAlgBase_p;
  }
                                                                                
  void CallGraph::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    getCallGraphAlgBase().genericTraversal(anAction_c);
    GraphWrapperTraversable<CallGraphVertex,CallGraphEdge>::traverseToChildren(anAction_c);
  }
  
  const CallGraphVertex& CallGraph::getSubroutineBySymbolReference(const SymbolReference& aSymbolReference) const { 
    typedef std::list<const SymbolReference*> SymbolReferencePList;
    static SymbolReferencePList undefinedSubroutineList; // for reporting purposes
    CallGraph::ConstVertexIteratorPair p(vertices());
    CallGraph::ConstVertexIterator beginIt(p.first),endIt(p.second);
    for (;beginIt!=endIt ;++beginIt) { 
      if ((*beginIt).getControlFlowGraph().getSymbolReference().refersToSameSymbolAs(aSymbolReference)) { 
	return (*beginIt);
      }
    }
    throw SubroutineNotFoundException(aSymbolReference);
    // to appease the compiler
    return (*beginIt);
  }

  const CallGraphVertex& CallGraph::getSubroutineByPlainName(const std::string& aPlainName) const {
    Scopes::ConstVertexIteratorPair p=myScopeTree.vertices();
    Scopes::ConstVertexIterator anIt(p.first),endIt(p.second);
    const Symbol *aFirstSymbol_p(0),*aSecondSymbol_p(0);
    const Scope *aFirstScope_p(0),*aSecondScope_p(0);
    const CallGraphVertex *aFirstCallGraphVertex_p(0),*aSecondCallGraphVertex_p(0);
    for (; anIt!=endIt ;++anIt) { 
      if ((*anIt).getSymbolTable().hasSymbolWithPlainName(aPlainName)) { 
	aSecondSymbol_p=&((*anIt).getSymbolTable().getSymbolWithPlainName(aPlainName));
	aSecondScope_p=&(*anIt);
	if (aSecondSymbol_p->getSymbolKind()==SymbolKind::SUBROUTINE) { 
	  try { 
	    aSecondCallGraphVertex_p=&(getSubroutineBySymbolReference(SymbolReference(*aSecondSymbol_p,* aSecondScope_p)));
	  } 
	  catch (SubroutineNotFoundException& e) { 
	    // do nothing here, some symboltables just keep duplicate symbols (with different decorations)
	    // but there is no actual subrotuine definition associated with them
	  }
	}
	if (aFirstCallGraphVertex_p && aSecondCallGraphVertex_p) { 
	  THROW_LOGICEXCEPTION_MACRO("CallGraph::getSubroutineByPlainName: name "
				     << aPlainName.c_str()
				     << " is ambiguously matched by "
				     << aSecondSymbol_p->getId().c_str()
				     << " and " 
				     << aFirstSymbol_p->getId().c_str()
				     << " plain names are "
				     << aSecondSymbol_p->plainName().c_str()
				     << " and "
				     << aFirstSymbol_p->plainName().c_str()
				     << " resp.");
	} 
	if (aSecondCallGraphVertex_p) { 
  	  aFirstSymbol_p=aSecondSymbol_p;
	  aFirstScope_p=aSecondScope_p;
          aFirstCallGraphVertex_p=aSecondCallGraphVertex_p;
	  aSecondCallGraphVertex_p=0;
        }
      }
    }
    if (!aFirstCallGraphVertex_p)
      THROW_LOGICEXCEPTION_MACRO("CallGraph::getSubroutineByPlainName: no subroutine named "
				 << aPlainName.c_str());
    return *aFirstCallGraphVertex_p;
  } 

} // end of namespace xaifBooster 
