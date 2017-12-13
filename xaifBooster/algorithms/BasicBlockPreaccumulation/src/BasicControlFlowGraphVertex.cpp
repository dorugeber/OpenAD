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

#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/ForLoop.hpp"
#include "xaifBooster/system/inc/BooleanOperation.hpp"

#include "xaifBooster/algorithms/AdjointUtils/inc/BasicBlockPrintVersion.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicControlFlowGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  BasicControlFlowGraphVertex::BasicControlFlowGraphVertex() :
    original(false), 
    myOriginalVertex_p(0), 
    myNewVertex_p(0),
    myIndex(0) {
  }

  BasicControlFlowGraphVertex::BasicControlFlowGraphVertex(const ControlFlowGraphVertex* theOriginal) : 
    original(true),
    myOriginalVertex_p(theOriginal),
    myNewVertex_p(0),
    myIndex(0) {
  }

  BasicControlFlowGraphVertex::~BasicControlFlowGraphVertex() {
    if (myNewVertex_p)
      delete myNewVertex_p;
  }

  const ControlFlowGraphVertexAlgBase&
  BasicControlFlowGraphVertex::getOriginalControlFlowGraphVertexAlg() const {
    return dynamic_cast<const ControlFlowGraphVertexAlgBase&>(myOriginalVertex_p->getControlFlowGraphVertexAlgBase());
  }

  ControlFlowGraphVertexAlgBase&
  BasicControlFlowGraphVertex::getOriginalControlFlowGraphVertexAlg() {
    return dynamic_cast<ControlFlowGraphVertexAlgBase&>(myOriginalVertex_p->getControlFlowGraphVertexAlgBase());
  }

  const ControlFlowGraphVertexAlgBase&
  BasicControlFlowGraphVertex::getNewControlFlowGraphVertexAlg() const {
    if (!myNewVertex_p)
      THROW_LOGICEXCEPTION_MACRO("BasicControlFlowGraphVertex::getNewControlFlowGraphVertexAlg: not set");
    return dynamic_cast<const ControlFlowGraphVertexAlgBase&>(myNewVertex_p->getControlFlowGraphVertexAlgBase());
  }

  void 
  BasicControlFlowGraphVertex::supplyAndAddNewVertex(ControlFlowGraphVertex& theNewVertex) { 
    if (myNewVertex_p)
      THROW_LOGICEXCEPTION_MACRO("BasicControlFlowGraphVertex::supplyAndAddNewVertex: already set");
    myNewVertex_p=&theNewVertex; 
  }

  void
  BasicControlFlowGraphVertex::printXMLHierarchy(std::ostream& os) const {
    if (!original) { 
      xaifBooster::PrintVersion::PrintVersion_E aPrintVersion(xaifBooster::ConceptuallyStaticInstances::instance()->
							      getPrintVersion());
      xaifBooster::ConceptuallyStaticInstances::instance()->setPrintVersion(xaifBooster::PrintVersion::SYSTEM_ONLY);
      myNewVertex_p->printXMLHierarchy(os);
      xaifBooster::ConceptuallyStaticInstances::instance()->setPrintVersion(aPrintVersion);
    }
    else {
      myOriginalVertex_p->printXMLHierarchy(os);
    }
  }

  std::string
  BasicControlFlowGraphVertex::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::BasicControlFlowGraphVertex["
	<< this
	<< ",original="
	<< original
	<< ",myIndex="
	<< myIndex
	<< ",getKind():"
	<< BasicControlFlowGraphVertex::getKind()
	<< "]"
	<< "]" << std::ends;
    return out.str();
  }

  void BasicControlFlowGraphVertex::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

  void BasicControlFlowGraphVertex::setIndex(int i) {
    myIndex=i;
  } 

  int BasicControlFlowGraphVertex::getIndex() const {
    return myIndex;
  }
                                                                    
  ControlFlowGraphVertexKind::ControlFlowGraphVertexKind_E 
  BasicControlFlowGraphVertex::getKind() const {
    if (original)
      return myOriginalVertex_p->getKind();
    else
      return myNewVertex_p->getKind();
  }

} // end of namespace

