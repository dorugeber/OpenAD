// $Id: xaif_interface.hpp,v 1.7 2004/10/16 04:18:17 jean_utke Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifndef         _xaif_interface_include_
#define         _xaif_interface_include_

#ifdef USEXAIFBOOSTER

#include<list>

#include "angel_types.hpp"

#include "xaifBooster/algorithms/CrossCountryInterface/inc/EliminationException.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/Elimination.hpp"

namespace angel {

  /// simply returns true if anf only if \p e is considered trivial with respect to \p ourAwarenessLevel
  bool isTrivialEdge(const c_graph_t::edge_t& e,
                     c_graph_t& theAngelLCG,
                     const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel);

  /// theAngelLCG is only not const so that we can access eType
  void assessPairElim(const c_graph_t::edge_t& e1,
                      const c_graph_t::edge_t& e2,
                      c_graph_t& theAngelLCG,
                      const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                      unsigned int& cost,
                      int& totalEdgecountChange,
                      int& nontrivialEdgecountChange);

  /// theAngelLCG is only not const so that we can access eType
  void assessFrontEdgeElim(const c_graph_t::edge_t& e,
                           c_graph_t& theAngelLCG,
                           const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                           unsigned int& cost,
                           int& totalEdgecountChange,
                           int& nontrivialEdgecountChange);

  /// theAngelLCG is only not const so that we can access eType
  void assessBackEdgeElim(const c_graph_t::edge_t& e,
                          c_graph_t& theAngelLCG,
                          const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                          unsigned int& cost,
                          int& totalEdgecountChange,
                          int& nontrivialEdgecountChange);

  /**
   * Determine the effect of performing \p anEdgeElim in \p theAngelLCG, but without actually doing anything.
   * The results are returned in \p cost, \p totalEdgecountChange, and \p nontrivialEdgecountChange (all passed by reference).
   * theAngelLCG is only not const so that we can access eType. 
   */
  void assessEdgeElim(const EdgeElim& anEdgeElim,
                      c_graph_t& theAngelLCG,
                      const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                      unsigned int& cost,
                      int& totalEdgecountChange,
                      int& nontrivialEdgecountChange);

  /**
   * This function greedily performs edge eliminations that have no computational cost and decrease the total number
   * of edges without increasing the number of nontrivial edges.  This is intended as a post-processing step to scarcity heuristics.
   * \todo Currently, it does not perform eliminations that decrease the nontrivial edge count.
   * The idea is that this should be handled by the heuristic itself.
   */
  void postProcessRemainderGraph(c_graph_t& theAngelLCG,
                                 xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& theJAEList,
                                 std::list<EdgeRef_t>& edge_ref_list,
                                 const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel);

} // end namespace angel

#endif // USEXAIFBOOSTER

#endif  // _xaif_interface_include_

