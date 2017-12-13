// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/GraphVizDisplay.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraph.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface {  

  AccumulationGraph::AccumulationGraph() {}
  AccumulationGraph::~AccumulationGraph() {} 

  std::string AccumulationGraph::debug() const {
    std::ostringstream out;
    out << "AccumulationGraph[" << this
        << "]" << std::ends;
    return out.str();
  } // end AccumulationGraph::debug()

  void
  AccumulationGraph::show(const std::string& namePrefix) const {
    std::ostringstream nameStream;
    nameStream << namePrefix << this;
    GraphVizDisplay::show(*this,
                          nameStream.str(),
                          AccumulationGraphVertexLabelWriter(*this),
                          AccumulationGraphEdgeLabelWriter(*this),
                          AccumulationGraphPropertiesWriter(*this));
  }

} // end namespace xaifBoosterCrossCountryInterface

