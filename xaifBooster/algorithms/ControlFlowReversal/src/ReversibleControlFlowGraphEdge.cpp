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
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraph.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraphEdge.hpp"
#include "xaifBooster/algorithms/ControlFlowReversal/inc/ReversibleControlFlowGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 

  ReversibleControlFlowGraphEdge::ReversibleControlFlowGraphEdge() :
    myConditionValueFlag(false),
    myConditionValue(0),
    myRevConditionValueFlag(false),
    myRevConditionValue(0),
    myOriginalEdge_p(0) {
  }

  ReversibleControlFlowGraphEdge::ReversibleControlFlowGraphEdge(const ControlFlowGraphEdge* theOriginal) : 
    myConditionValueFlag(false),
    myConditionValue(0),
    myRevConditionValueFlag(false),
    myRevConditionValue(0),
    myOriginalEdge_p(theOriginal) {
    if (theOriginal->hasConditionValue())
      setConditionValue(theOriginal->getConditionValue());
  }

  ReversibleControlFlowGraphEdge::~ReversibleControlFlowGraphEdge() {}

  bool ReversibleControlFlowGraphEdge::leadsToLoopBody() const {
    return (myConditionValueFlag && myConditionValue==1);
  }

  bool ReversibleControlFlowGraphEdge::isOriginal() const {
    return (myOriginalEdge_p!=0);
  }

  const ControlFlowGraphEdge& ReversibleControlFlowGraphEdge::getOriginalEdge() const {
    if (!myOriginalEdge_p)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphEdge::getOriginalEdge: not set");
    return *myOriginalEdge_p;
  }

  bool ReversibleControlFlowGraphEdge::hasConditionValue() const {
    return myConditionValueFlag;
  }

  void ReversibleControlFlowGraphEdge::setConditionValue(int cv) {
    if (myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphEdge::setConditionValue: already set");
    myConditionValue=cv;
    myConditionValueFlag=true;
  }

  int ReversibleControlFlowGraphEdge::getConditionValue() const {
    if (!myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphEdge::getConditionValue: not set");
    return myConditionValue;
  }

  bool ReversibleControlFlowGraphEdge::hasRevConditionValue() const {
    return myRevConditionValueFlag;
  }

  void ReversibleControlFlowGraphEdge::setRevConditionValue(int cv) {
    if (myRevConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphEdge::setRevConditionValue: already set");
    myRevConditionValue=cv;
    myRevConditionValueFlag=true;
  }

  int ReversibleControlFlowGraphEdge::getRevConditionValue() const {
    if (!myRevConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("ReversibleControlFlowGraphEdge::getRevConditionValue: not set");
    return myRevConditionValue;
  }

  void ReversibleControlFlowGraphEdge::printXMLHierarchy(std::ostream& os, 
							 const ReversibleControlFlowGraph& theGraph) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ControlFlowGraphEdge::ourXAIFName.c_str()
       << " "
       << ControlFlowGraphEdge::our_myId_XAIFName.c_str()
       << "=\""
       << getId().c_str()
       << "\" "
       << ControlFlowGraphEdge::our_source_XAIFName.c_str()
       << "=\"";
    const ReversibleControlFlowGraphVertex& src=theGraph.getSourceOf(*this);
    if (src.isOriginal())
      os << src.getOriginalVertex().getId().c_str();
    else
      os << src.getNewVertex().getId().c_str();
    os << "\" " 
       << ControlFlowGraphEdge::our_target_XAIFName.c_str() 
       << "=\"";
    const ReversibleControlFlowGraphVertex& tgt = theGraph.getTargetOf(*this);
    if (tgt.isOriginal())
      os << tgt.getOriginalVertex().getId().c_str();
    else
      os << tgt.getNewVertex().getId().c_str();
    os << "\" "
       << ControlFlowGraphEdge::our_myConditionValueFlag_XAIFName.c_str()
       << "=\""
       << myConditionValueFlag
       << "\"";
    if (myConditionValueFlag) { 
      os << " "
	 << ControlFlowGraphEdge::our_myConditionValue_XAIFName.c_str()
	 << "=\""
	 << myConditionValue
	 << "\"";
    }
    os << "/>" << std::endl;
    pm.releaseInstance();
  } // end of ReversibleControlFlowGraphEdge::printXMLHierarchy


  std::string
  ReversibleControlFlowGraphEdge::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::ReversibleControlFlowGraphEdge["
        << this
        << "]" << std::ends;
    return out.str();
  }

  void ReversibleControlFlowGraphEdge::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

} // end of namespace

