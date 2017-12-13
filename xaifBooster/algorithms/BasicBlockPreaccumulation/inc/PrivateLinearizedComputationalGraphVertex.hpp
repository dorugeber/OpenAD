#ifndef _PRIVATELINEARIZEDCOMPUTATIONALGRAPHVERTEX_INCLUDE_
#define _PRIVATELINEARIZEDCOMPUTATIONALGRAPHVERTEX_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/ObjectWithId.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"

using namespace xaifBooster; 

namespace xaifBoosterBasicBlockPreaccumulation { 
 
  /**
   * Vertices in the linearized computational graph maintain pointers to
   * the corresponding expression vertices in the linearized RHS and
   * the corresponding LHSVariable (if there is one)
   */
  class PrivateLinearizedComputationalGraphVertex : public xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex {

  public:

    PrivateLinearizedComputationalGraphVertex();
    ~PrivateLinearizedComputationalGraphVertex(){};

    virtual std::string debug() const;

    void
    setOrigin(const ExpressionVertex& aExpressionVertex,
              const Assignment& aAssignment);

    /**
     * to be used when this LCG vertex corresp. to the LHS of \p aAssignment
     * (currently exclusively for direct copy vertices)
     */
    void
    setOrigin(const Assignment& aAssignment);

    const ExpressionVertex&
    getOriginatingExpressionVertex() const;

    void associateExpressionVertex(const ExpressionVertex&);

    const CExpressionVertexPSet& getAssociatedExpressionVertexPSet() const;

    bool hasOriginalVariable() const;

    const Variable& getOriginalVariable() const;

    const ObjectWithId::Id& getStatementId() const;

    bool hasAuxiliaryVariable() const;

    const Variable& getAuxiliaryVariable() const;

    /// for GraphViz
    std::string getLabelString() const;

  private: 

    const ExpressionVertex* myOriginatingExpressionVertex_p;

    const Assignment* myOriginatingAssignment_p;

    /**
     * we do not own these expression vertices
     */
    CExpressionVertexPSet myOriginalExpressionVertexPSet;

    /**
     * Pointer to the variable that originally corresponds to this vertex, in the case where one exists.
     * The variable is not owned by this class.
     */
    const Variable* myOriginalVariable_p;

    /// (may not be set) points to the corresp. auxiliary variable if one was created during Linearization
    const Variable* myAuxiliaryVariable_p;

  }; 
 
  class PrivateLinearizedComputationalGraphVertexLabelWriter {
  public:
    PrivateLinearizedComputationalGraphVertexLabelWriter(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph& g) : myG(g) {};

    template <class BoostInternalVertexDescriptor>
    void operator()(std::ostream& out, 
                    const BoostInternalVertexDescriptor& v) const {
      const PrivateLinearizedComputationalGraphVertex& thePLCGV(
       dynamic_cast<const PrivateLinearizedComputationalGraphVertex&>(
        *boost::get(boost::get(BoostVertexContentType(),
                               myG.getInternalBoostGraph()),
                    v))
      );
      std::string theVertexShape("ellipse");
      std::string orientationString("0");
      std::string theVertexGroupname("intermediates");
      std::string vertexFixedSize("false");
      std::string labelloc("c");
      const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& theDepVertexPList(myG.getDependentList());
      for (xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList::const_iterator aDepVertexPListI(theDepVertexPList.begin());
           aDepVertexPListI!=theDepVertexPList.end();
           ++aDepVertexPListI) { 
        if (&thePLCGV == *(aDepVertexPListI)) {
          theVertexShape = "invtriangle";
          orientationString = "270";
          theVertexGroupname = "dependents";
          vertexFixedSize = "true";
          labelloc = "t";
          break;
        }
      }
      const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList& theIndepVertexPList(myG.getIndependentList());
      for (xaifBoosterCrossCountryInterface::LinearizedComputationalGraph::VertexPointerList::const_iterator aIndepVertexPListI(theIndepVertexPList.begin());
           aIndepVertexPListI!=theIndepVertexPList.end();
           ++aIndepVertexPListI) { 
        if (&thePLCGV == *(aIndepVertexPListI)) {
          theVertexShape = "triangle";
          orientationString = "270";
          theVertexGroupname = "independents";
          vertexFixedSize = "true";
          labelloc = "b";
          break;
        }
      }

      out << "["
          << "penwidth=3.0"
          << ",fontsize=14" // (default)
        //<< ",fixedsize=" << vertexFixedSize.c_str()
          << ",group=\"" << theVertexGroupname.c_str() << "\""
          << ",shape=" << theVertexShape.c_str()
          << ",orientation=" << orientationString.c_str()
          << ",label=\"" << thePLCGV.getLabelString().c_str() << "\""
        //<< ",labelloc=\"" << labelloc.c_str() << "\""
          << ",tooltip=\"" << thePLCGV.debug().c_str() << "\""
          << "]";
    }

    const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph& myG;

  };

} 
                                                                     
#endif

