#ifndef _PRIVATELINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
#define _PRIVATELINEARIZEDCOMPUTATIONALGRAPH_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/HashTable.hpp"
#include "xaifBooster/utils/inc/GuardedMap.hpp"

#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/DuUdMapUseResult.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphVertex.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * the default representation for PrivateLinearizedComputationalGraph
   * the edges will be instances of a subclass of LinearizedComputationalGraphEdge, and
   * the vertices will be instances of a subclass of LinearizedComputationalGraphVertex
   */
  class PrivateLinearizedComputationalGraph : public xaifBoosterCrossCountryInterface::LinearizedComputationalGraph,
					      public virtual Debuggable {

  public:

    PrivateLinearizedComputationalGraph(); 

    virtual ~PrivateLinearizedComputationalGraph(){};
    
    /**
     * \todo JU implementation incomplete
     */
    std::string debug() const { return std::string("PrivateLinearizedComputationalGraph");};

    /// calls GraphvizDisplay::show
    void
    show(const std::string& outputName) const;

    void
    mapAssignmentLHS2PLCGV(const Assignment& aAssignment,
                           const PrivateLinearizedComputationalGraphVertex& thePLCGV);

    const PrivateLinearizedComputationalGraphVertex&
    getLCGVertexForAssignmentLHS(const Assignment& aAssignment) const;

    void addToIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndependentVertex);

    void addToDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDependentVertex,
			    const ObjectWithId::Id& aStatementId);

    void removeFromIndependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theIndependentVertex);

    void removeFromDependentList(const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex& theDependentVertex);

    const DuUdMapUseResult::StatementIdLists getStatementIdLists()const;

    void addToPassiveStatementIdList(const ObjectWithId::Id& aStatementId);

  private: 

    /** 
     * this is the list of statementIds for the active statements in which  
     * the resp. dependent is the right hand side
     * myDependentStatementIdList and myPassiveStatementIdList are supposed to be disjoint
     * the list initially contains all active assignments but later all assignments 
     * where the LHS variables are not dependent variables are removed.
     */
    StatementIdList myDependentStatementIdList;
    
    /** 
     * this is the list of statementIds for the passive statements 
     * we need this for determination of dependents with du chains
     * since a variable may be used in a statement determined to be passive
     * and therefore not show up in myDependentStatementIdList
     * myDependentStatementIdList and myPassiveStatementIdList are supposed to be disjoint
     */
    StatementIdList myPassiveStatementIdList;

    /** 
     * all statements to establish order between the elements of myPassiveStatementIdList
     * and myDependentStatementIdList
     */
    StatementIdList myStatementIdList;

    typedef GuardedMap<const Assignment*,
                     const PrivateLinearizedComputationalGraphVertex*> CAssignmentP2CPLCGVertexPMap;
    /** 
     * keep track of correspondence between each (active) assignment and
     * the PLCG vertex that corresponds to the LHS
     */
    CAssignmentP2CPLCGVertexPMap myAssignmentLHS2LCGVertexPMap;
    
  }; // end of class PrivateLinearizedComputationalGraph 

  class PrivateLinearizedComputationalGraphPropertiesWriter {
  public:
    PrivateLinearizedComputationalGraphPropertiesWriter(const PrivateLinearizedComputationalGraph& g) : myG(g) {};

    void operator()(std::ostream& out) const {
      out << "rankdir=LR;" << std::endl;
      // fix rank for sources/sinks
      PrivateLinearizedComputationalGraph::ConstVertexIteratorPair aPLCGVpair(myG.vertices());
      for (PrivateLinearizedComputationalGraph::ConstVertexIterator aPLCGVi(aPLCGVpair.first), aPLCGViend(aPLCGVpair.second); aPLCGVi != aPLCGViend; ++aPLCGVi)
        if (myG.numOutEdgesOf(*aPLCGVi) == 0)
          out << "{rank=max; " << (*aPLCGVi).getDescriptor() << ";}" << std::endl;
        else if (myG.numInEdgesOf(*aPLCGVi) == 0)
          out << "{rank=min; " << (*aPLCGVi).getDescriptor() << ";}" << std::endl;
    }

    const PrivateLinearizedComputationalGraph& myG;

  };

} 
                                                                     
#endif
