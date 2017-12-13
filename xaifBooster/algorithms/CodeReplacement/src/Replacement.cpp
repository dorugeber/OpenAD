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

#include "xaifBooster/algorithms/CodeReplacement/inc/Replacement.hpp"
#include "xaifBooster/algorithms/CodeReplacement/inc/ConceptuallyStaticInstances.hpp"

namespace xaifBoosterCodeReplacement { 

  const std::string Replacement::ourXAIFName("xaif:Replacement");
  const std::string Replacement::our_myPlaceHolder_XAIFName("placeholder");

  Replacement::Replacement (unsigned aPlaceHolder) : 
    myPlaceHolder(aPlaceHolder),
    myControlFlowGraphBase_p(0),
    myReversibleControlFlowGraph_p(0),
    myPrintVersion(PrintVersion::ORIGINAL) { 
  } 

  Replacement::~Replacement(){
  }

  void Replacement::setControlFlowGraphBase(const ControlFlowGraphBase& theOtherControlFlowGraphBase) { 
    if (myControlFlowGraphBase_p || myReversibleControlFlowGraph_p)
      THROW_LOGICEXCEPTION_MACRO("Replacement::setControlFlowGraphBase: already got one");
    myControlFlowGraphBase_p=&theOtherControlFlowGraphBase;
  } 

  const ControlFlowGraphBase& Replacement::getControlFlowGraphBase() const { 
    if (!myControlFlowGraphBase_p)
      THROW_LOGICEXCEPTION_MACRO("Replacement::setControlFlowGraphBase: don't have one");
    return *myControlFlowGraphBase_p;
  } 

  void Replacement::setReversibleControlFlowGraph(const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& theOtherReversibleControlFlowGraph) { 
    if (myControlFlowGraphBase_p || myReversibleControlFlowGraph_p)
      THROW_LOGICEXCEPTION_MACRO("Replacement::setReversibleControlFlowGraph: already got one");
    myReversibleControlFlowGraph_p=&theOtherReversibleControlFlowGraph;
  } 

  const xaifBoosterControlFlowReversal::ReversibleControlFlowGraph& Replacement::getReversibleControlFlowGraph() const { 
    if (!myReversibleControlFlowGraph_p)
      THROW_LOGICEXCEPTION_MACRO("Replacement::setReversibleControlFlowGraph: don't have one");
    return *myReversibleControlFlowGraph_p;
  } 

  void
  Replacement::printXMLHierarchy(std::ostream& os) const { 
    ConceptuallyStaticInstances::instance()->setPrintVersion(getPrintVersion());
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str()
       << " "
       << our_myPlaceHolder_XAIFName.c_str() 
       << "=\"" 
       << myPlaceHolder
       << "\"";
    if ((myReversibleControlFlowGraph_p && !myReversibleControlFlowGraph_p->isStructured())
	|| 
	(myControlFlowGraphBase_p && !myControlFlowGraphBase_p->isStructured())) { 
      os << " "
	 << ControlFlowGraph::our_myStructuredFlag_XAIFName.c_str() 
	 << "=\"" 
	 << false
	 << "\"";
    }
    os << ">" 
       << std::endl;
    if (myControlFlowGraphBase_p) { 
      ControlFlowGraphBase::ConstVertexIteratorPair p(getControlFlowGraphBase().vertices());
      ControlFlowGraphBase::ConstVertexIterator beginIt(p.first),endIt(p.second);
      for (;beginIt!=endIt ;++beginIt)
	(*beginIt).printXMLHierarchy(os);
      ControlFlowGraphBase::ConstEdgeIteratorPair pe(getControlFlowGraphBase().edges());
      ControlFlowGraphBase::ConstEdgeIterator beginIte(pe.first),endIte(pe.second);
      for (;beginIte!=endIte ;++beginIte)
	(*beginIte).printXMLHierarchy(os,
				      getControlFlowGraphBase());
    }
    else if (myReversibleControlFlowGraph_p) 
      myReversibleControlFlowGraph_p->printXMLHierarchy(os);
    else { 
//      THROW_LOGICEXCEPTION_MACRO("Replacement::printXMLHierarchy: no CFG set");
    } 
    os << pm.indent()
       << "</"
       << ourXAIFName.c_str()
       << ">"
       << std::endl;
    pm.releaseInstance();
    ConceptuallyStaticInstances::instance()->setPrintVersion(PrintVersion::ORIGINAL);
  } // end of Replacement::printXMLHierarchy

  std::string Replacement::debug () const { 
    std::ostringstream out;
    out << "Replacement[" << this 
	<< "]" << std::ends;  
    return out.str();
  } // end of Replacement::debug

  const unsigned int Replacement::getPlaceHolder() const {
    return myPlaceHolder; 
  }

  void Replacement::setPrintVersion(PrintVersion::PrintVersion_E aPrintVersion) { 
    myPrintVersion=aPrintVersion;
  } 

  PrintVersion::PrintVersion_E Replacement::getPrintVersion() const { 
    return myPrintVersion;
  }

} 
