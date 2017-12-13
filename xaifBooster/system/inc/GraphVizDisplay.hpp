#ifndef _GRAPHVIZDISPLAY_INCLUDE_
#define _GRAPHVIZDISPLAY_INCLUDE_
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

namespace xaifBooster { 

  /** 
   * a wrapper for the graphviz output, 
   * uses graphviz's dot and 'xv' to display 
   * graph structures
   */
  class GraphVizDisplay { 
    
  public: 
    
    /** 
     * labels vertices with the boost descriptors, 
     * i.e. either integers for vecS containers 
     * or hex addresses for listS containers
     */
    class DefaultVertexLabelWriter { 
    public: 
      DefaultVertexLabelWriter(){};
      template <class BoostInternalVertexDescriptor>
      void operator()(std::ostream& out, const BoostInternalVertexDescriptor& v) const { 
	out << "[label=\"V_" << v << "\"]"; // the 'dot' utility cannot parse hex addresses as labels
      }
    };
    
    /**
     * labels edges with nothing 
     */
    class DefaultEdgeLabelWriter { 
    public: 
      DefaultEdgeLabelWriter(){};
      template <class BoostInternalEdgeDescriptor>
      void operator()(std::ostream& out, const BoostInternalEdgeDescriptor& v) const { 
	//	out << "[label=\"" << v << "\"]"; // we don't want to clutter the picture
      }
    };

    /**
     * labels the graph with nothing
     */
    class DefaultGraphPropertiesWriter {
    public:
      DefaultGraphPropertiesWriter(){};
      void operator()(std::ostream& out) const { 
      }
    }; // end class DefaultGraphPropertiesWriter

//     
//   // examples: 
//     
//   class EdgeLabelWriter {
//   public:
//     EdgeLabelWriter(const Expression& e) : myE(e) {};
//     template <class BoostIntenalEdgeDescriptor>
//     void operator()(std::ostream& out, const BoostIntenalEdgeDescriptor& v) const {
//       out << "[label=\"" << PartialDerivativeKind::toString(boost::get(boost::get(BoostEdgeContentType(),
//                                                                                   myE.getInternalBoostGraph()),
//                                                                        v)->getPartialDerivativeKind()).c_str() << "\"]";
//     };
//     const Expression& myE;
//   };


//   class VertexLabelWriter {
//   public:
//     VertexLabelWriter(const Expression& e) : myE(e) {};
//     template <class BoostIntenalVertexDescriptor>
//     void operator()(std::ostream& out, const BoostIntenalVertexDescriptor& v) const {
//       out << "[label=\"" << boost::get(boost::get(BoostVertexContentType(),
//                                                   myE.getInternalBoostGraph()),
//                                        v)->equivalenceSignature().c_str() << "\"]";
//     };
//     const Expression& myE;
//   };

//   // usage: 
//   if (DbgLoggerManager::instance()->isSelected(DbgGroup::GRAPHICS))
//     GraphVizDisplay::show(getRHS(),"analyzed",
//                           VertexLabelWriter(getRHS()),
//                           EdgeLabelWriter(getRHS()));


    /** 
     * the name supplied here is the base name of a file to be created in /tmp
     */
    template <class Graph>
    static void show(const Graph& g, 
		     const std::string& aFileName);

    /** 
     * the name supplied here is the base name of a file to be created in /tmp
     * the label writer can be like the default label writer used in this implementation
     */
    template <class Graph, class VertexLabelWriter>
    static void show(const Graph& g,
		     const std::string& aFileName,
		     const VertexLabelWriter& aVertexLabelWriter);

    /** 
     * the name supplied here is the base name of a file to be created in /tmp
     * the label writer can be like the default label writer used in this implementation
     */
    template <class Graph, class VertexLabelWriter, class EdgeLabelWriter>
    static void show(const Graph& g,
		     const std::string& aFileName,
		     const VertexLabelWriter& aVertexLabelWriter,
		     const EdgeLabelWriter& anEdgeLabelWriter);

    /** 
     * the name supplied here is the base name of a file to be created in /tmp
     * the label writer can be like the default label writer used in this implementation
     */
    template <class Graph, class VertexLabelWriter, class EdgeLabelWriter, class GraphPropertiesWriter>
    static void show(const Graph& g,
		     const std::string& aFileName,
		     const VertexLabelWriter& aVertexLabelWriter,
		     const EdgeLabelWriter& anEdgeLabelWriter,
		     const GraphPropertiesWriter& aGraphPropertiesWriter);

  }; 

} 

#include "xaifBooster/system/inc/TemplateImpl/GraphVizDisplay.cpp"

#endif 
