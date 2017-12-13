/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/
#ifdef USEXAIFBOOSTER

#include "angel/include/angel_tools.hpp"
#include "angel/include/eliminations.hpp"
#include "angel/include/reroutings.hpp"

using namespace std;
using namespace boost;
using namespace xaifBoosterCrossCountryInterface;

namespace angel {

void reroutable_edges (c_graph_t& angelLCG,
                       vector<edge_reroute_t>& erv) {
  erv.clear();
  set<c_graph_t::vertex_t> downset, upset;
  c_graph_t::edge_t decrement_e;
  bool found_decrement_e;
  c_graph_t::ei_t ei, e_end;
  c_graph_t::iei_t iei, ie_end;
  c_graph_t::oei_t oei, oe_end;
  property_map<pure_c_graph_t, VertexVisited>::type visited = get(VertexVisited(), angelLCG);
  c_graph_t::vi_t cleari, clear_end;

  // iterate over all edges; consider each to be pre-routed and post-routed
  for (tie (ei, e_end) = edges (angelLCG); ei != e_end; ++ei) {
    c_graph_t::edge_t e = *ei;

    // check for preroutability
    if (in_degree (target (e, angelLCG), angelLCG) > 1) {
      // iterate over possible pivots (inedges of tgt(e))
      for (tie (iei, ie_end) = in_edges (target (*ei, angelLCG), angelLCG); iei != ie_end; ++iei) {
	c_graph_t::edge_t pivot_e = *iei;
	// skip the edge we're considering for rerouting
	if (source (pivot_e, angelLCG) == source (e, angelLCG)) continue;
	// the source of the pivot edge can't be an independent (we add an edge into it)
	if (in_degree (source (pivot_e, angelLCG), angelLCG) == 0) continue;
	// ensure that no decrement fill-in is created
	for (tie(oei, oe_end) = out_edges(source(pivot_e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	  if (*oei == pivot_e) continue; // skip the pivot edge
	  tie(decrement_e, found_decrement_e) = edge(source(e, angelLCG), target(*oei, angelLCG), angelLCG);
	  if (!found_decrement_e) break; // decrement fill-in has been found (not allowed)
	}
        if (oei != oe_end) continue; // this will be true iff some decrement fill is detected

	// ensure that we cant reach src(e) from src(pivot_e) (would create cycle)
	// first clear visited list
	for (tie(cleari, clear_end) = vertices(angelLCG); cleari != clear_end; ++cleari) visited[*cleari] = false;
	if (reachable (source(pivot_e, angelLCG), source(e, angelLCG), angelLCG)) continue;
	erv.push_back (edge_reroute_t (e, pivot_e, true));

      } // end all pivot candidates
    } // end check for pre-routability

    // check for postroutability
    if (out_degree (source (e, angelLCG), angelLCG) > 1) {
      // iterate over possible pivots (outedges of src(e))
      for (tie (oei, oe_end) = out_edges (source (e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	c_graph_t::edge_t pivot_e = *oei;
	// skip the edge we're considering for rerouting
	if (target (pivot_e, angelLCG) == target (e, angelLCG)) continue;
	// tgt(pivot_e) can't be a dependent vertex (we add an edge out of it)
	if (out_degree (target (pivot_e, angelLCG), angelLCG) == 0) continue;
	// ensure that no decrement fill-in is created
	for (tie(iei, ie_end) = in_edges(target(pivot_e, angelLCG), angelLCG); iei != ie_end; ++iei) {
	  if (*iei == pivot_e) continue; // skip the pivot edge
	  tie(decrement_e, found_decrement_e) = edge(source(*iei, angelLCG), target(e, angelLCG), angelLCG);
	  if (!found_decrement_e) break; // decrement fill-in has been found (not allowed)
	}
        if (iei != ie_end) continue; // this will be true iff some decrement fill is detected

	// ensure that we cant reach tgt(pivot_e) from tgt(e) (would create cycle)
	// first clear visited list
	for (tie(cleari, clear_end) = vertices(angelLCG); cleari != clear_end; ++cleari) visited[*cleari] = false;
	if (reachable (target(e, angelLCG), target(pivot_e, angelLCG), angelLCG)) continue;
	erv.push_back (edge_reroute_t (e, pivot_e, false));

      } // end all pivot candidates
    } // end check for postroutability
    
  } // end all edges in angelLCG

#ifndef NDEBUG
  cout << "	There are " << erv.size() << " reroutings in G" << endl;
#endif
  
} // end reroutable_edges()

unsigned int reroutable_edges(c_graph_t& angelLCG,
                              vector<Rerouting>& allReroutingsV) {
  allReroutingsV.clear();
  vector <edge_reroute_t> tempRerouteTV;
  reroutable_edges(angelLCG, tempRerouteTV);
  if (tempRerouteTV.empty())
    return 0;

  for (size_t i = 0; i < tempRerouteTV.size(); i++)
   allReroutingsV.push_back(Rerouting (tempRerouteTV[i], angelLCG));
  return allReroutingsV.size();
} // end reroutable_edges()

int reroute_effect (const edge_reroute_t er,
		    const c_graph_t& angelLCG,
		    const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
		    bool& incrementIsTrivial) {

  c_graph_t::edge_t e = er.e;
  c_graph_t::edge_t pe = er.pivot_e;
  c_graph_t::iei_t iei, ie_end;
  c_graph_t::oei_t oei, oe_end;
  boost::property_map<c_graph_t, EdgeType>::const_type eType = get(EdgeType(), angelLCG);
  c_graph_t::edge_t increment_e, decrement_e;
  bool found_increment_e, found_decrement_e;

  int nontrivial_edge_change = 0;

  // consider the loss of the rerouted edge
  if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS
  || (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[e] != UNIT_EDGE)
  || (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[e] == VARIABLE_EDGE)) nontrivial_edge_change--;

  if (er.isPre) { // pre-routing
    // DECREMENT EDGES: No fill-in, but we allow when a decrement edge goes from trivial to nontrivial
    for (tie(oei, oe_end) = out_edges(source(pe, angelLCG), angelLCG); oei != oe_end; ++oei) {
      if (*oei == pe) continue; // skip the pivot edge
      tie (decrement_e, found_decrement_e) = edge(source(e, angelLCG), target(*oei, angelLCG), angelLCG);
      THROW_EXCEPT_MACRO (!found_decrement_e, consistency_exception, "Pre-routing passed to filter causes decrement fill-in");	
      // no awareness: no effect
      // unit awareness:
      if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[decrement_e] == UNIT_EDGE) nontrivial_edge_change++;
      // constant awareness:
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[decrement_e] != VARIABLE_EDGE)
	if (eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE) nontrivial_edge_change++;
    } // end all outedges of src(pivot_e)

    // INCREMENT EDGE: change occurs only when increment edge was nontrivial to begin with
    tie(increment_e, found_increment_e) = edge(target(pe, angelLCG), target(e, angelLCG), angelLCG);
    if (found_increment_e) { // increment absorption
      if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) incrementIsTrivial = false;
      else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS) {
	incrementIsTrivial = false; // because of absorption (addition) it will be nontrivial no matter what
	if (eType[increment_e] == UNIT_EDGE) nontrivial_edge_change++;
      } // end unit awareness
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS) {
	// if ANY involved edge is variable, then increment edge is nontrivial
	if (eType[increment_e] == VARIABLE_EDGE || eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE) incrementIsTrivial = false;
	else incrementIsTrivial = true;
	// if the result is nontrivial, but the increment edge WAS trivial to begin with, our nontriv count goes up
	if (eType[increment_e] != VARIABLE_EDGE && !incrementIsTrivial) nontrivial_edge_change++;
      } // end constant awareness
    } // end increment absorption
    else { // increment fill-in
      if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) {
	nontrivial_edge_change++;
	incrementIsTrivial = false;
      } // end no awareness
      else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS) {
	if (eType[e] != UNIT_EDGE || eType[pe] != UNIT_EDGE) {
	  nontrivial_edge_change++;
	  incrementIsTrivial = false;
	}
	else incrementIsTrivial = true;
      } // end unit awareness
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS) {
	if (eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE) {
	  nontrivial_edge_change++;
	  incrementIsTrivial = false;
	}
	else incrementIsTrivial = true;
      } // end constant awareness
    } // end increment fill-in

  } // end pre-routing
  else { // post-routing

    // DECREMENT EDGES: No fill-in, but we allow when a decrement edge goes from trivial to nontrivial
    for (tie(iei, ie_end) = in_edges(target(pe, angelLCG), angelLCG); iei != ie_end; ++iei) {
      if (*iei == pe) continue; // skip the pivot edge
      tie (decrement_e, found_decrement_e) = edge (source(*iei, angelLCG), target(e, angelLCG), angelLCG);
      THROW_EXCEPT_MACRO (!found_decrement_e, consistency_exception, "Post-routing passed to filter causes decrement fill-in");	
      // no awareness: no effect
      // unit awareness:
      if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[decrement_e] == UNIT_EDGE) nontrivial_edge_change++;
      // constant awareness:
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[decrement_e] != VARIABLE_EDGE)
	if (eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE) nontrivial_edge_change++;
    } // end all outedges of src(pivot_e)

    // INCREMENT EDGE: change occurs only when increment edge was nontrivial to begin with
    tie(increment_e, found_increment_e) = edge(target(pe, angelLCG), target(e, angelLCG), angelLCG);
    if (found_increment_e) { // increment absorption
      if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) incrementIsTrivial = false;
      else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS) { // increase iff edge was trivial to begin with
	incrementIsTrivial = false; // because of absorption (addition) it will be nontrivial no matter what
	if (eType[increment_e] == UNIT_EDGE) nontrivial_edge_change++;
      } // end unit awareness
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS) { // if ANY involved edge is variable, then increment edge is nontrivial
	if (eType[increment_e] == VARIABLE_EDGE || eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE) incrementIsTrivial = false;
	else incrementIsTrivial = true;
	if (eType[increment_e] != VARIABLE_EDGE && !incrementIsTrivial) nontrivial_edge_change++;
      } // end constant awareness
    } // end increment absorption
    else { // increment fill-in
      if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) {
	nontrivial_edge_change++;
	incrementIsTrivial = false;
      } // end no awareness
      else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS) {
	if (eType[e] != UNIT_EDGE || eType[pe] != UNIT_EDGE) {
	  nontrivial_edge_change++;
	  incrementIsTrivial = false;
	}
	else incrementIsTrivial = true;
      } // end unit awareness
      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS) {
	if (eType[e] == VARIABLE_EDGE || eType[pe] == VARIABLE_EDGE) {
	  nontrivial_edge_change++;
	  incrementIsTrivial = false;
	}
	else incrementIsTrivial = true;
      } // end constant awareness
    } // end increment fill-in

  } // end post-routing

  return nontrivial_edge_change;
} // end reroute_effect()

// pre-/post-routing an edge cannot isolate a vertex
unsigned int preroute_edge_directly (edge_reroute_t er,
				     c_graph_t& angelLCG,
				     list<EdgeRef_t>& edge_ref_list,
				     JacobianAccumulationExpressionList& jae_list) {
  #ifndef NDEBUG
    cout << "prerouting edge " << er.e << " about pivot edge " << er.pivot_e << "\t(and creating the corresponding JAEs)" << endl;
  #endif

  unsigned int cost = 0;
  boost::property_map<c_graph_t, EdgeType>::type eType = get(EdgeType(), angelLCG);
  c_graph_t::iei_t iei, ie_end; 
  c_graph_t::oei_t oei, oe_end; 

  JacobianAccumulationExpression& new_jae = jae_list.addExpression();
  // Increment the edge from the source of e to to v by the quotient e/pivot_e (create it if it doesnt exist)
  JacobianAccumulationExpressionVertex& jaev_divide = new_jae.addVertex();
  //jaev_divide.setOperation (JacobianAccumulationExpressionVertex::DIVIDE_OP);
  jaev_divide.setOperation (JacobianAccumulationExpressionVertex::ADD_OP);
  JacobianAccumulationExpressionVertex& jaev_e = new_jae.addVertex();
  JacobianAccumulationExpressionVertex& jaev_pivot_e = new_jae.addVertex();
  setJaevRef (er.e, jaev_e, angelLCG, edge_ref_list);
  setJaevRef (er.pivot_e, jaev_pivot_e, angelLCG, edge_ref_list);
  new_jae.addEdge(jaev_e, jaev_divide);
  new_jae.addEdge(jaev_pivot_e, jaev_divide);

  // INCREMENT EDGE
  bool found_increment_e;
  c_graph_t::edge_t increment_e;
  tie (increment_e, found_increment_e) = edge (source (er.e, angelLCG), source (er.pivot_e, angelLCG), angelLCG);
  if (found_increment_e) { // increment absorption
    JacobianAccumulationExpressionVertex& jaev_increment_e = new_jae.addVertex();
    setJaevRef (increment_e, jaev_increment_e, angelLCG, edge_ref_list);
    JacobianAccumulationExpressionVertex& jaev_add = new_jae.addVertex();
    jaev_add.setOperation (JacobianAccumulationExpressionVertex::ADD_OP);
    new_jae.addEdge(jaev_increment_e, jaev_add);
    new_jae.addEdge(jaev_divide, jaev_add);

    //point increment_e at the top of the new JAE
    removeRef (increment_e, angelLCG, edge_ref_list);
    EdgeRef_t new_increment_e_ref (increment_e, &jaev_add);
    edge_ref_list.push_back(new_increment_e_ref);

    //set edge type for absorption increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[increment_e] != VARIABLE_EDGE)				eType[increment_e] = CONSTANT_EDGE;
  } // end increment absorption
  else { //no increment edge was already present (fill-in)
    tie (increment_e, found_increment_e) = add_edge (source (er.e, angelLCG), source (er.pivot_e, angelLCG), angelLCG.next_edge_number++, angelLCG);

    // point the new edge at the divide operation in the new JAE
    EdgeRef_t new_increment_e_ref (increment_e, &jaev_divide);
    edge_ref_list.push_back(new_increment_e_ref);

    //set edge type for fill-in increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE)	eType[increment_e] = UNIT_EDGE;
    else									eType[increment_e] = CONSTANT_EDGE;
  }

  // determine cost of creating increment edge (we divide e/pivot_e, so trivial iff pivot_e is unit)
  if (eType[er.pivot_e] != UNIT_EDGE)
    cost++;

  // DECREMENT OPERATIONS

  // for all successors of v (except the target of e), perform decrement operations on edges from src_of_e to 
  for (tie (oei, oe_end) = out_edges (source (er.pivot_e, angelLCG), angelLCG); oei != oe_end; oei++) {
    if (target (*oei, angelLCG) == target (er.e, angelLCG)) continue;
    JacobianAccumulationExpression& new_jae = jae_list.addExpression();
    JacobianAccumulationExpressionVertex& jaev_divide = new_jae.addVertex();
    //jaev_divide.setOperation (JacobianAccumulationExpressionVertex::DIVIDE_OP);
    jaev_divide.setOperation (JacobianAccumulationExpressionVertex::MULT_OP);
    JacobianAccumulationExpressionVertex& jaev_e = new_jae.addVertex();
    JacobianAccumulationExpressionVertex& jaev_pivot_e = new_jae.addVertex();
    setJaevRef (er.e, jaev_e, angelLCG, edge_ref_list);
    setJaevRef (er.pivot_e, jaev_pivot_e, angelLCG, edge_ref_list);
    new_jae.addEdge(jaev_e, jaev_divide);
    new_jae.addEdge(jaev_pivot_e, jaev_divide);
    // create mult vertex and connect it up
    JacobianAccumulationExpressionVertex& jaev_mult = new_jae.addVertex();
    jaev_mult.setOperation (JacobianAccumulationExpressionVertex::MULT_OP);
    new_jae.addEdge(jaev_divide, jaev_mult);
    JacobianAccumulationExpressionVertex& jaev_vout_e = new_jae.addVertex();
    setJaevRef (*oei, jaev_vout_e, angelLCG, edge_ref_list);
    new_jae.addEdge(jaev_vout_e, jaev_mult);

    bool found_decrement_e;
    c_graph_t::edge_t decrement_e;
    tie (decrement_e, found_decrement_e) = edge (source (er.e, angelLCG), target (*oei, angelLCG), angelLCG);

    if (found_decrement_e) { // decrement absorption
      JacobianAccumulationExpressionVertex& jaev_decrement_e = new_jae.addVertex();
      JacobianAccumulationExpressionVertex& jaev_subtract = new_jae.addVertex();
      //jaev_subtract.setOperation (JacobianAccumulationExpressionVertex::SUBTRACT_OP);
      jaev_subtract.setOperation (JacobianAccumulationExpressionVertex::ADD_OP);
      new_jae.addEdge(jaev_mult, jaev_subtract);
      new_jae.addEdge(jaev_decrement_e, jaev_subtract);

      // point the decrement edge at the divide operation in the new JAE
      removeRef (decrement_e, angelLCG, edge_ref_list);
      EdgeRef_t new_decrement_e_ref (decrement_e, &jaev_subtract);
      edge_ref_list.push_back(new_decrement_e_ref);

      //set edge type for absorption decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)
	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[decrement_e] != VARIABLE_EDGE)
	eType[decrement_e] = CONSTANT_EDGE;
    } // end decrement absorption
    else { // decrement fill-in
      tie (decrement_e, found_decrement_e) = add_edge (source (er.e, angelLCG), target (*oei, angelLCG), angelLCG.next_edge_number++, angelLCG);

      // point the new edge at the divide operation in the new JAE
      EdgeRef_t new_decrement_e_ref (decrement_e, &jaev_divide);
      edge_ref_list.push_back(new_decrement_e_ref);

      //set edge type for fill-in decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)
	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE && eType[*oei] == UNIT_EDGE)
	eType[decrement_e] = UNIT_EDGE;
      else
	eType[decrement_e] = CONSTANT_EDGE;
    }

    // eval cost for decrements: trivial if both e and pivot_e are unit or if decrement is unit
    if (!((eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE) || eType[*oei] == UNIT_EDGE))
      cost++;
  } // end all decrements

  remove_edge (er.e, angelLCG);

  return cost;
} // end preroute_edge_directly()

unsigned int postroute_edge_directly (edge_reroute_t er,
				      c_graph_t& angelLCG,
				      list<EdgeRef_t>& edge_ref_list,
				      JacobianAccumulationExpressionList& jae_list) {
  #ifndef NDEBUG
    cout << "postrouting edge " << er.e << " about pivot edge " << er.pivot_e << "\t(and creating the corresponding JAEs)" << endl;
  #endif

  unsigned int cost = 0;
  boost::property_map<c_graph_t, EdgeType>::type eType = get(EdgeType(), angelLCG);
  c_graph_t::iei_t iei, ie_end; 
  c_graph_t::oei_t oei, oe_end; 

  // Increment the edge from the source of e to to v by the quotient e/pivot_e (create it if it doesnt exist)
  JacobianAccumulationExpression& new_jae = jae_list.addExpression();
  JacobianAccumulationExpressionVertex& jaev_divide = new_jae.addVertex();
  //jaev_divide.setOperation (JacobianAccumulationExpressionVertex::DIVIDE_OP);
  jaev_divide.setOperation (JacobianAccumulationExpressionVertex::ADD_OP);
  JacobianAccumulationExpressionVertex& jaev_e = new_jae.addVertex();
  JacobianAccumulationExpressionVertex& jaev_pivot_e = new_jae.addVertex();
  setJaevRef (er.e, jaev_e, angelLCG, edge_ref_list);
  setJaevRef (er.pivot_e, jaev_pivot_e, angelLCG, edge_ref_list);
  new_jae.addEdge(jaev_e, jaev_divide);
  new_jae.addEdge(jaev_pivot_e, jaev_divide);

  // INCREMENT EDGE
  bool found_increment_e;
  c_graph_t::edge_t increment_e;
  tie (increment_e, found_increment_e) = edge (target (er.pivot_e, angelLCG), target (er.e, angelLCG), angelLCG);
  if (found_increment_e) { // increment absorption
    JacobianAccumulationExpressionVertex& jaev_increment_e = new_jae.addVertex();
    setJaevRef (increment_e, jaev_increment_e, angelLCG, edge_ref_list);
    JacobianAccumulationExpressionVertex& jaev_add = new_jae.addVertex();
    jaev_add.setOperation (JacobianAccumulationExpressionVertex::ADD_OP);
    new_jae.addEdge(jaev_increment_e, jaev_add);
    new_jae.addEdge(jaev_divide, jaev_add);

    //point increment_e at the top of the new JAE
    removeRef (increment_e, angelLCG, edge_ref_list);
    EdgeRef_t new_increment_e_ref (increment_e, &jaev_add);
    edge_ref_list.push_back(new_increment_e_ref);

    //set edge type for absorption increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[increment_e] != VARIABLE_EDGE)				eType[increment_e] = CONSTANT_EDGE;
  } // end increment absorption
  else { //no increment edge was already present (fill-in)
    tie (increment_e, found_increment_e) = add_edge (target (er.pivot_e, angelLCG), target (er.e, angelLCG), angelLCG.next_edge_number++, angelLCG);

    // point the new edge at the divide operation in the new JAE
    EdgeRef_t new_increment_e_ref (increment_e, &jaev_divide);
    edge_ref_list.push_back(new_increment_e_ref);

    //set edge type for fill-in increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE)	eType[increment_e] = UNIT_EDGE;
    else									eType[increment_e] = CONSTANT_EDGE;
  }

  // determine cost of creating increment edge (we divide e/pivot_e, so trivial iff pivot_e is unit)
  if (eType[er.pivot_e] != UNIT_EDGE)
    cost++;

  // DECREMENT OPERATIONS

  // for all predecessors of tgt(pivot_e) (except src(e)), perform decrement operations on edges to tgt(e) 
  for (tie (iei, ie_end) = in_edges (target (er.pivot_e, angelLCG), angelLCG); iei != ie_end; iei++) {
    if (source (*iei, angelLCG) == source (er.pivot_e, angelLCG)) continue;
    JacobianAccumulationExpression& new_jae = jae_list.addExpression(); 
    JacobianAccumulationExpressionVertex& jaev_divide = new_jae.addVertex();
    //jaev_divide.setOperation (JacobianAccumulationExpressionVertex::DIVIDE_OP);
    jaev_divide.setOperation(JacobianAccumulationExpressionVertex::MULT_OP);
    JacobianAccumulationExpressionVertex& jaev_e = new_jae.addVertex();
    JacobianAccumulationExpressionVertex& jaev_pivot_e = new_jae.addVertex();
    setJaevRef (er.e, jaev_e, angelLCG, edge_ref_list);
    setJaevRef (er.pivot_e, jaev_pivot_e, angelLCG, edge_ref_list);
    new_jae.addEdge(jaev_e, jaev_divide);
    new_jae.addEdge(jaev_pivot_e, jaev_divide);
    // create mult vertex and connect it up
    JacobianAccumulationExpressionVertex& jaev_mult = new_jae.addVertex();
    jaev_mult.setOperation (JacobianAccumulationExpressionVertex::MULT_OP);
    new_jae.addEdge(jaev_divide, jaev_mult);
    JacobianAccumulationExpressionVertex& jaev_vin_e = new_jae.addVertex();
    setJaevRef (*iei, jaev_vin_e, angelLCG, edge_ref_list);
    new_jae.addEdge(jaev_vin_e, jaev_mult);

    bool found_decrement_e;
    c_graph_t::edge_t decrement_e;
    tie (decrement_e, found_decrement_e) = edge (source (*iei, angelLCG), target (er.e, angelLCG), angelLCG);
    if (found_decrement_e) { // decrement absorption
      JacobianAccumulationExpressionVertex& jaev_decrement_e = new_jae.addVertex();
      JacobianAccumulationExpressionVertex& jaev_subtract = new_jae.addVertex();
      //jaev_subtract.setOperation (JacobianAccumulationExpressionVertex::SUBTRACT_OP);
      jaev_subtract.setOperation(JacobianAccumulationExpressionVertex::ADD_OP);
      new_jae.addEdge(jaev_mult, jaev_subtract);
      new_jae.addEdge(jaev_decrement_e, jaev_subtract);

      // point the decrement edge at the divide operation in the new JAE
      removeRef (decrement_e, angelLCG, edge_ref_list);
      EdgeRef_t new_decrement_e_ref (decrement_e, &jaev_subtract);
      edge_ref_list.push_back(new_decrement_e_ref);

      //set edge type for absorption decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)
	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[decrement_e] != VARIABLE_EDGE)
	eType[decrement_e] = CONSTANT_EDGE;
    } // end decrement absorption
    else { // decrement fill-in
      tie (decrement_e, found_decrement_e) = add_edge (source (*iei, angelLCG), target (er.e, angelLCG), angelLCG.next_edge_number++, angelLCG);

      // point the new edge at the divide operation in the new JAE
      EdgeRef_t new_decrement_e_ref (decrement_e, &jaev_divide);
      edge_ref_list.push_back(new_decrement_e_ref);

      //set edge type for fill-in decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)
	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE && eType[*iei] == UNIT_EDGE)
	eType[decrement_e] = UNIT_EDGE;
      else
	eType[decrement_e] = CONSTANT_EDGE;
    }

    // eval cost for decrements: trivial if both e and pivot_e are unit or if decrement is unit
    if (!((eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE) || eType[*iei] == UNIT_EDGE))
      cost++;
  } // end all decrements

  remove_edge (er.e, angelLCG);

  return cost;
} // end postroute_edge_directly()

unsigned int prerouteEdge_noJAE (edge_reroute_t er,
                                 c_graph_t& angelLCG) {
  #ifndef NDEBUG
    cout << "prerouting edge " << er.e << " about pivot edge " << er.pivot_e << "\t(without creating any JAEs)" << endl;
  #endif

  unsigned int cost = 0;
  boost::property_map<c_graph_t, EdgeType>::type eType = get(EdgeType(), angelLCG);
  c_graph_t::iei_t iei, ie_end; 
  c_graph_t::oei_t oei, oe_end; 

  // INCREMENT EDGE
  bool found_increment_e;
  c_graph_t::edge_t increment_e;
  tie (increment_e, found_increment_e) = edge (source (er.e, angelLCG), source (er.pivot_e, angelLCG), angelLCG);
  if (found_increment_e) { // increment absorption
    //set edge type for absorption increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[increment_e] != VARIABLE_EDGE)				eType[increment_e] = CONSTANT_EDGE;
  } // end increment absorption
  else { //no increment edge was already present (fill-in)
    tie (increment_e, found_increment_e) = add_edge (source (er.e, angelLCG), source (er.pivot_e, angelLCG), angelLCG.next_edge_number++, angelLCG);
    //set edge type for fill-in increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE)	eType[increment_e] = UNIT_EDGE;
    else									eType[increment_e] = CONSTANT_EDGE;
  }
  // determine cost of creating increment edge (we divide e/pivot_e, so trivial iff pivot_e is unit)
  if (eType[er.pivot_e] != UNIT_EDGE)
    cost++;

  // DECREMENT EDGES
  for (tie (oei, oe_end) = out_edges (source (er.pivot_e, angelLCG), angelLCG); oei != oe_end; oei++) {
    if (target (*oei, angelLCG) == target (er.e, angelLCG)) continue;
    bool found_decrement_e;
    c_graph_t::edge_t decrement_e;
    tie (decrement_e, found_decrement_e) = edge (source (er.e, angelLCG), target (*oei, angelLCG), angelLCG);
    if (found_decrement_e) { // decrement absorption
      //set edge type for absorption decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[decrement_e] != VARIABLE_EDGE)								eType[decrement_e] = CONSTANT_EDGE;
    } // end decrement absorption
    else { // decrement fill-in
      tie (decrement_e, found_decrement_e) = add_edge (source (er.e, angelLCG), target (*oei, angelLCG), angelLCG.next_edge_number++, angelLCG);
      //set edge type for fill-in decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE && eType[*oei] == UNIT_EDGE)		eType[decrement_e] = UNIT_EDGE;
      else													eType[decrement_e] = CONSTANT_EDGE;
    }
    // eval cost for decrements: trivial if both e and pivot_e are unit or if decrement is unit
    if (!((eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE) || eType[*iei] == UNIT_EDGE))
      cost++;
  } // end all decrements
  remove_edge (er.e, angelLCG);
  return cost;
} // end prerouteEdge_noJAE()

unsigned int postrouteEdge_noJAE (edge_reroute_t er,
                                  c_graph_t& angelLCG) {
  #ifndef NDEBUG
    cout << "postrouting edge " << er.e << " about pivot edge " << er.pivot_e << "\t(without creating any JAEs)" << endl;
  #endif

  unsigned int cost = 0;
  boost::property_map<c_graph_t, EdgeType>::type eType = get(EdgeType(), angelLCG);
  c_graph_t::iei_t iei, ie_end; 
  c_graph_t::oei_t oei, oe_end; 

  // INCREMENT EDGE
  bool found_increment_e;
  c_graph_t::edge_t increment_e;
  tie (increment_e, found_increment_e) = edge (target (er.pivot_e, angelLCG), target (er.e, angelLCG), angelLCG);
  if (found_increment_e) { // increment absorption
    //set edge type for absorption increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[increment_e] != VARIABLE_EDGE)				eType[increment_e] = CONSTANT_EDGE;
  } // end increment absorption
  else { //no increment edge was already present (fill-in)
    tie (increment_e, found_increment_e) = add_edge (target (er.pivot_e, angelLCG), target (er.e, angelLCG), angelLCG.next_edge_number++, angelLCG);
    //set edge type for fill-in increment edge
    if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE)	eType[increment_e] = VARIABLE_EDGE;
    else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE)	eType[increment_e] = UNIT_EDGE;
    else									eType[increment_e] = CONSTANT_EDGE;
  }
  // determine cost of creating increment edge (we divide e/pivot_e, so trivial iff pivot_e is unit)
  if (eType[er.pivot_e] != UNIT_EDGE)
    cost++;

  // DECREMENT EDGES
  for (tie (iei, ie_end) = in_edges (target (er.pivot_e, angelLCG), angelLCG); iei != ie_end; iei++) {
    if (source (*iei, angelLCG) == source (er.pivot_e, angelLCG)) continue;
    bool found_decrement_e;
    c_graph_t::edge_t decrement_e;
    tie (decrement_e, found_decrement_e) = edge (source (*iei, angelLCG), target (er.e, angelLCG), angelLCG);
    if (found_decrement_e) { // decrement absorption
      //set edge type for absorption decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[decrement_e] != VARIABLE_EDGE)								eType[decrement_e] = CONSTANT_EDGE;
    } // end decrement absorption
    else { // decrement fill-in
      tie (decrement_e, found_decrement_e) = add_edge (source (*iei, angelLCG), target (er.e, angelLCG), angelLCG.next_edge_number++, angelLCG);
      //set edge type for fill-in decrement edge
      if (eType[er.e] == VARIABLE_EDGE || eType[er.pivot_e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)	eType[decrement_e] = VARIABLE_EDGE;
      else if (eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE && eType[*iei] == UNIT_EDGE)		eType[decrement_e] = UNIT_EDGE;
      else													eType[decrement_e] = CONSTANT_EDGE;
    }
    // eval cost for decrements: trivial if both e and pivot_e are unit or if decrement is unit
    if (!((eType[er.e] == UNIT_EDGE && eType[er.pivot_e] == UNIT_EDGE) || eType[*iei] == UNIT_EDGE))
      cost++;
  } // end all decrements
  remove_edge (er.e, angelLCG);
  return cost;
} // end postrouteEdge_noJAE()

} // namespace angel

#endif // USEXAIFBOOSTER

