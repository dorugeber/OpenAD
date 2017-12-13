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
#include "xaifBooster/system/inc/TraversalStack.hpp"

namespace xaifBooster {

  TraversalStack::TraversalStack() :
    myCurrentCallGraphVertexInstance_p(NULL),
    myCurrentBasicBlockInstance_p(NULL) {
  }

  TraversalStack::~TraversalStack() {
  }

  std::string TraversalStack::debug () const {
    std::ostringstream out;
    out << "TraversalStack[" << this
        << ",myCurrentCallGraphVertexInstance_p=" << myCurrentCallGraphVertexInstance_p
        << ",myCurrentBasicBlockInstance_p=" << myCurrentBasicBlockInstance_p
	<< "]" << std::ends;
    return out.str();
  } // end of TraversalStack::debug

  void TraversalStack::setCurrentCallGraphVertexInstance(CallGraphVertex& aCallGraphVertex) {
    if (myCurrentCallGraphVertexInstance_p)
      THROW_LOGICEXCEPTION_MACRO("TraversalStack::setCurrentCallGraphVertexInstance: already set");
    myCurrentCallGraphVertexInstance_p = &aCallGraphVertex;
  } // end TraversalStack::setCurrentCallGraphVertexInstance()

  void TraversalStack::resetCurrentCallGraphVertexInstance() {
    myCurrentCallGraphVertexInstance_p = NULL;
  } // end TraversalStack::setCurrentCallGraphVertexInstance()

  const CallGraphVertex& TraversalStack::getCurrentCallGraphVertexInstance() const {
    if (!myCurrentCallGraphVertexInstance_p)
      THROW_LOGICEXCEPTION_MACRO("TraversalStack::getCurrentCallGraphVertexInstance: not set");
    return *myCurrentCallGraphVertexInstance_p;
  } // end TraversalStack::getCurrentCallGraphVertexInstance()

  void TraversalStack::setCurrentBasicBlockInstance(BasicBlock& aBasicBlock) {
    if (myCurrentBasicBlockInstance_p)
      THROW_LOGICEXCEPTION_MACRO("TraversalStack::setCurrentBasicBlockInstance: already set");
    myCurrentBasicBlockInstance_p = &aBasicBlock;
  } // end TraversalStack::setCurrentBasicBlockInstance()

  void TraversalStack::resetCurrentBasicBlockInstance() {
    myCurrentBasicBlockInstance_p = NULL;
  } // end TraversalStack::setCurrentBasicBlockInstance()

  const BasicBlock& TraversalStack::getCurrentBasicBlockInstance() const {
    if (!myCurrentBasicBlockInstance_p)
      THROW_LOGICEXCEPTION_MACRO("TraversalStack::getCurrentBasicBlockInstance: not set");
    return *myCurrentBasicBlockInstance_p;
  } // end TraversalStack::getCurrentBasicBlockInstance()

} // end namespace xaifBooster 

