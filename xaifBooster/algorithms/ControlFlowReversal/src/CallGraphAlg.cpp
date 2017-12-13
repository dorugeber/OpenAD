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

#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/ControlFlowReversal/inc/CallGraphAlg.hpp"


using namespace xaifBooster;

namespace xaifBoosterControlFlowReversal { 
 
 std::string CallGraphAlg::myAlgorithmSignature(std::string("_cfr_"));


  CallGraphAlg::CallGraphAlg(CallGraph& theContaining) : CallGraphAlgBase(theContaining) {
  }

  CallGraphAlg::~CallGraphAlg() {
  }

  const std::string&
  CallGraphAlg::getAlgorithmSignature() const {
    return myAlgorithmSignature;
  }

  class CallGraphVertexLabelWriter {
  public:
    CallGraphVertexLabelWriter(const CallGraph& g) : myG(g) {}
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
      out << "[label=\"" << boost::get(boost::get(BoostVertexContentType(),
                                                  myG.getInternalBoostGraph()),
                                       v)->getSubroutineName() << "\"]";
    }
    const CallGraph& myG;
  };

  void CallGraphAlg::algorithm_action_4() {
    DBG_MACRO(DbgGroup::CALLSTACK,
              "xaifBoosterControlFlowReversal::CallGraphAlg::algorithm_action_4(reverse control flow) called for: "
              << debug().c_str());
    if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS) && DbgLoggerManager::instance()->wantTag("callGraph")) {     
       GraphVizDisplay::show(getContaining(),"call_graph",
 			    CallGraphVertexLabelWriter(getContaining()));
    }
  } // end CallGraphAlg::algorithm_action_4() 

  void
  CallGraphAlg::printXMLHierarchy(std::ostream& os) const {
    getContaining().printXMLHierarchyImpl(os);
  }

  std::string
  CallGraphAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterControlFlowReversal::CallGraphAlg["
        << this
        << "]" << std::ends;
    return out.str();
  }

  void CallGraphAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  }

} // end of namespace

