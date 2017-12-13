#ifndef _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPH_INCLUDE_
#define _XAIFBOOSTERCROSSCOUNTRYINTERFACE_ACCUMULATIONGRAPH_INCLUDE_
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

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/AccumulationGraphEdge.hpp"

using namespace xaifBooster;

namespace xaifBoosterCrossCountryInterface {

  /**
   * Class that implements a graph that represents a flattening of JAEs.
   * Use of this graph allows easier constant folding for Jacobian acumulation
   * and allows us to create expressions for preaccumulation that involve more than one operation.
   * NOTE: building the accumulation graph from the list of JAEs should not be part of eliminate in the future.
   */
  class AccumulationGraph : public GraphWrapper<AccumulationGraphVertex,
						AccumulationGraphEdge>,
			    public virtual Debuggable {

  public:

    AccumulationGraph(); 
    ~AccumulationGraph();

    std::string debug() const;

    /// calls GraphVizDisplay::show
    void
    show(const std::string&) const;

  }; // end of class AccumulationGraph

  class AccumulationGraphPropertiesWriter {
  public:
    AccumulationGraphPropertiesWriter(const AccumulationGraph& g) : myG(g) {};

    void operator()(std::ostream& out) const {
      out << "rankdir=BT;" << std::endl;
      AccumulationGraph::ConstVertexIteratorPair aAGViPair(myG.vertices());
      for (AccumulationGraph::ConstVertexIterator aAGVi(aAGViPair.first); aAGVi != aAGViPair.second; ++aAGVi)
        if (myG.numOutEdgesOf(*aAGVi) == 0)
          out << "{rank=max; " << (*aAGVi).getDescriptor() << ";}" << std::endl;
        else if (myG.numInEdgesOf(*aAGVi) == 0)
          out << "{rank=min; " << (*aAGVi).getDescriptor() << ";}" << std::endl;
    }

    const AccumulationGraph& myG;

  }; // end class AccumulationGraphPropertiesWriter

  class AccumulationGraphVertexLabelWriter {
  public:
    AccumulationGraphVertexLabelWriter(const AccumulationGraph& g) : myG(g) {};
    template <class BoostIntenalVertexDescriptor>
    void operator()(std::ostream& out, 
		    const BoostIntenalVertexDescriptor& v) const {
      const AccumulationGraphVertex* theAccumulationGraphVertex_p =
	dynamic_cast<const AccumulationGraphVertex*>(boost::get(boost::get(BoostVertexContentType(),
									   myG.getInternalBoostGraph()),
								v));
      const AccumulationGraphVertex& theAGV(
	dynamic_cast<const AccumulationGraphVertex&>(
          *boost::get(boost::get(BoostVertexContentType(),
	                         myG.getInternalBoostGraph()),
                      v)
        )
      );
      // set groupname and shape
      std::string theGroupname, theShape;
      if (theAccumulationGraphVertex_p->hasRemainderGraphEdge()) {
	theGroupname = "RemainderGraphEdges";
	theShape = "box";
      }
      else if (myG.numInEdgesOf(*theAccumulationGraphVertex_p)) {
	theGroupname = "Intermediate ops";
	theShape = "box";//"circle";
      }
      else { // leaves
	theShape = "egg";
	theGroupname = "leaves";
      }

      // mark outputs
      std::string styleString("solid,rounded");
      std::string fontcolorString("black");
      if (theAGV.hasRemainderGraphEdge()) {
        styleString = "filled,rounded";
        fontcolorString = "white";
      }

      out << "["
          << "group=\"" << theGroupname.c_str() << "\""
	  << ",fixedsize=false"
          << ",shape=" << theShape.c_str()
          << ",color=" << theAGV.getColorString().c_str()
          << ",style=\"" << styleString.c_str() << "\""
          << ",fontcolor=" << fontcolorString.c_str()
          << ",fontsize=14" // (default)
	  << ",label=\"" << theAGV.getLabelString().c_str() << "\""
          << ",tooltip=\"" << theAGV.debug().c_str() << "\""
          << "]";
    }

    const AccumulationGraph& myG;

  }; // end class AccumulationGraphVertexLabelWriter

  class AccumulationGraphEdgeLabelWriter {
  public:
    AccumulationGraphEdgeLabelWriter(const AccumulationGraph& g) : myG(g) {};
    template <class BoostIntenalEdgeDescriptor>
    void operator()(std::ostream& out,
		    const BoostIntenalEdgeDescriptor& v) const {
    }
    const AccumulationGraph& myG;
  }; // end class AccumulationGraphEdgeLabelWriter

} // end namespace xaifBoosterCrossCountryInterface

#endif

