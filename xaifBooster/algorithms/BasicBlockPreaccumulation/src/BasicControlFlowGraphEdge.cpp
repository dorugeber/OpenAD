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

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraph.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  BasicControlFlowGraphEdge::BasicControlFlowGraphEdge() :
    myConditionValueFlag(false),
    myConditionValue(0),
    myOriginalEdge_p(0) {
  }

  BasicControlFlowGraphEdge::BasicControlFlowGraphEdge(const ControlFlowGraphEdge* theOriginal) : 
    myConditionValueFlag(false),
    myConditionValue(0),
    myOriginalEdge_p(theOriginal) {
    if (theOriginal->hasConditionValue())
      setConditionValue(theOriginal->getConditionValue());
  }

  BasicControlFlowGraphEdge::~BasicControlFlowGraphEdge() {}

  bool BasicControlFlowGraphEdge::isOriginal() const {
    return (myOriginalEdge_p!=0);
  }

  const ControlFlowGraphEdge& BasicControlFlowGraphEdge::getOriginalEdge() const {
    if (!myOriginalEdge_p)
      THROW_LOGICEXCEPTION_MACRO("BasicControlFlowGraphEdge::getOriginalEdge: not set");
    return *myOriginalEdge_p;
  }

  bool BasicControlFlowGraphEdge::hasConditionValue() const {
    return myConditionValueFlag;
  }

  void BasicControlFlowGraphEdge::setConditionValue(int cv) {
    if (myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("BasicControlFlowGraphEdge::setConditionValue: already set");
    myConditionValue=cv;
    myConditionValueFlag=true;
  }

  int BasicControlFlowGraphEdge::getConditionValue() const {
    if (!myConditionValueFlag)
      THROW_LOGICEXCEPTION_MACRO("BasicControlFlowGraphEdge::getConditionValue: not set");
    return myConditionValue;
  }

  void BasicControlFlowGraphEdge::printXMLHierarchy(std::ostream& os, 
						    const BasicControlFlowGraph& theGraph) const {
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
    const BasicControlFlowGraphVertex& src=theGraph.getSourceOf(*this);
    if (src.isOriginal())
      os << src.getOriginalVertex().getId().c_str();
    else
      os << src.getNewVertex().getId().c_str();
    os << "\" " 
       << ControlFlowGraphEdge::our_target_XAIFName.c_str() 
       << "=\"";
    const BasicControlFlowGraphVertex& tgt = theGraph.getTargetOf(*this);
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
  } // end of BasicControlFlowGraphEdge::printXMLHierarchy


  std::string
  BasicControlFlowGraphEdge::debug() const {
    std::ostringstream out;
    out << "xaifBoosterBasicBlockPreaccumulation::BasicControlFlowGraphEdge["
        << this
        << "]" << std::ends;
    return out.str();
  }

  void BasicControlFlowGraphEdge::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

} // end of namespace

