/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#ifdef USEXAIFBOOSTER

#ifndef 	_reroutings_include_
#define 	_reroutings_include_

#include "angel_types.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/AwarenessLevel.hpp"

using std::list;
using std::vector;
using std::cout;
using boost::tie;

namespace angel {

/** Populates a list of all viable edge reroutings in \p angelLCG.

    \param angelLCG the c_graph_t (passed by reference) that the operation is performed on.
    \param erv empty list that will hold all pre-routings and post-routings in \p angelLCG.
    \return List of edge reroutings \p erv (by reference).
 */
void reroutable_edges (c_graph_t& angelLCG,
		       vector<edge_reroute_t>& erv);

unsigned int reroutable_edges(c_graph_t& angelLCG,
                              vector<Rerouting>& allReroutingsV);

/** Calculates the change in nontrivial edge count from \p er
    without actually performing it.  In addition,
    incrementIsTrivial is returned by reference
 */
int reroute_effect (const edge_reroute_t er,
		    const c_graph_t& angelLCG,
                    const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
		    bool& incrementIsTrivial);

unsigned int preroute_edge_directly (edge_reroute_t er,
				     c_graph_t& angelLCG,
				     list<EdgeRef_t>& edge_ref_list,
                                     xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& jae_list);

unsigned int postroute_edge_directly (edge_reroute_t er,
				      c_graph_t& angelLCG,
				      list<EdgeRef_t>& edge_ref_list,
                                      xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& jae_list);

unsigned int prerouteEdge_noJAE (edge_reroute_t er,
                                 c_graph_t& angelLCG);

unsigned int postrouteEdge_noJAE (edge_reroute_t er,
                                  c_graph_t& angelLCG);

} // namespace angel

#endif // _reroutings_include_

#endif  // USEXAIFBOOSTER

