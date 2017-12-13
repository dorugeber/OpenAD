#ifndef _COMBINEDGRAPH_INCLUDE_
#define _COMBINEDGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/boostWrapper/inc/GraphWrapper.hpp"

#include "xaifBooster/system/inc/Variable.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraph.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"

#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphVertex.hpp"
#include "xaifBooster/algorithms/PushPop/inc/CombinedGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop {

  /**
   * the default representation for the CombinedGraph
   */
  class CombinedGraph : public GraphWrapper<CombinedGraphVertex,
                                            CombinedGraphEdge>{
  public:

    /**
     * \todo AML implementation incomplete
     */
    std::string debug() const { return std::string("CombinedGraph");};

    typedef std::map<const xaifBoosterBasicBlockPreaccumulation::RemainderGraphVertex*,
                     const CombinedGraphVertex*> CRGVp2CCGVpMap;

    typedef std::map<const xaifBoosterBasicBlockPreaccumulation::RemainderGraphEdge*,
                     const CombinedGraphVertex*> CRGEp2CCGVpMap;

  };

  class CombinedGraphVertexLabelWriter {
  public:
    CombinedGraphVertexLabelWriter(const CombinedGraph& g) : myG(g) {};

    template <class BoostInternalVertexDescriptor>
    void
    operator()(std::ostream& out, 
               const BoostInternalVertexDescriptor& v) const {
      const CombinedGraphVertex& theCGVertex(dynamic_cast<const CombinedGraphVertex&>(*boost::get(boost::get(BoostVertexContentType(),
                                                                                                             myG.getInternalBoostGraph()),
                                                                                                  v)));
      std::string penWidthString = theCGVertex.hasComputationalGraphVertex() ? "4.0"
                                                                             : "2.0";
      // set style and font color
      std::string styleString("solid");//diagonals");
      std::string fontColorString("black");
      if (theCGVertex.hasRemainderGraphEdge()) {
        styleString = "filled";
        fontColorString = "white";
      }
      out << "["
          << "penwidth=" << penWidthString.c_str()
          << ",fontsize=14" // (default)
          << ",height=0.25,width=0.25" // this gives a minimum
          << ",shape=" << theCGVertex.getShapeString().c_str()
          << ",color=" << theCGVertex.getColorString().c_str()
          << ",fontcolor=" << fontColorString.c_str()
          << ",style=\"" << styleString.c_str() << "\""
          << ",label=\"" << theCGVertex.getLabelString().c_str() << "\""
          << ",tooltip=\"" << theCGVertex.debug().c_str() << "\""
          << "]";
    }

    const CombinedGraph& myG;

  };

  class CombinedGraphEdgeLabelWriter {
  public:
    CombinedGraphEdgeLabelWriter(const CombinedGraph& g) : myG(g) {};

    template <class BoostInternalEdgeDescriptor>
    void operator()(std::ostream& out, const BoostInternalEdgeDescriptor& v) const {
      const CombinedGraphEdge& theCGEdge(
       dynamic_cast<const CombinedGraphEdge&>(*boost::get(boost::get(BoostEdgeContentType(),
                                                                     myG.getInternalBoostGraph()),
                                                          v))
      );
      std::string penwidthString = (theCGEdge.getOriginType() == CombinedGraphEdge::ORIGINAL_EDGE) ? "3.0"
                                                                                                   : "1.0";
      // common properties
      out << "["
          << "labelfloat=false"
          << ",fontsize=14" // (default)
          << ",style=" << theCGEdge.getStyleString().c_str()
          << ",color=" << theCGEdge.getColorString().c_str()
          << ",penwidth=" << penwidthString
          << ",edgetooltip=\"" << theCGEdge.debug().c_str() << "\""
          << "]";
    }

    const CombinedGraph& myG;

  };

  class CombinedGraphPropertiesWriter {
  public:
    CombinedGraphPropertiesWriter(const CombinedGraph& g) : myG(g) {
    };

    void operator()(std::ostream& out) const {
      out << "rankdir=LR;" << std::endl;
      CombinedGraph::ConstVertexIteratorPair aCGVpair(myG.vertices());
      for (CombinedGraph::ConstVertexIterator aCGVi(aCGVpair.first); aCGVi != aCGVpair.second; ++aCGVi)
        if ((myG.numOutEdgesOf(*aCGVi) == 0) && (*aCGVi).hasRemainderGraphEdge())
          out << "{rank=max; " << (*aCGVi).getDescriptor() << ";}" << std::endl;
        else if (myG.numInEdgesOf(*aCGVi) == 0)
          out << "{rank=min; " << (*aCGVi).getDescriptor() << ";}" << std::endl;
    }

    const CombinedGraph& myG;

  };

}

#endif
