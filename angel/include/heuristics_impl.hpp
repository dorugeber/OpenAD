// $Id: heuristics_impl.hpp,v 1.5 2004/02/22 18:44:46 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#include <set>
// #include <limits>
#include <limits.h>
#include "angel_exceptions.hpp"

namespace angel {
  using std::vector;

template <typename Heuristic_t>
int lowest_markowitz_face_complete_t<Heuristic_t>::operator() (const vector<line_graph_t::face_t>& fv1,
							       const line_graph_t& lg,
							       vector<line_graph_t::face_t>& fv2) {
    fv2.resize(0);
    line_graph_t::const_evn_t evn= get(boost::vertex_name, lg);

    // if there are still faces from the vertex with 
    // the lowest Markowitz degree chosen at last return these faces
    typedef line_graph_t::face_t face_t;
    if (lastv != -1) {
      for (size_t c= 0; c < fv1.size(); c++) {   
	face_t f= fv1[c];
	if (lastv == evn[source(f, lg)].second) fv2.push_back (f); }
      if (fv2.size() != 0) return fv2.size();
    }

    // otherwise search new vertex (vertices)
    vector<int> mdegree;
    markowitz_on_line_graph (lg, mdegree);
    
    vector<face_t> fvlm; // faces via vertices with miminal Markowitz
    fvlm.push_back (fv1[0]);
    int minm= mdegree[evn[source(fv1[0], lg)].second]; // minimal Markowitz
    
    for (size_t c= 1; c < fv1.size(); c++) {
      face_t f= fv1[c];
      int m= mdegree[evn[source(f, lg)].second];
      if (m < minm) {fvlm.resize (0); minm= m;}
      if (m == minm) fvlm.push_back (f); }
    
    tiebreaker (fvlm, lg, fv2);
    THROW_DEBUG_EXCEPT_MACRO (fv2.size() == 0, consistency_exception, "Tiebreaker returned empty vector");
    lastv= evn[source(fv2[0], lg)].second;
    
    // test if all returned faces belong to the same vertex
#ifndef NDEBUG
    for (size_t c= 1; c < fv2.size(); c++)
      THROW_EXCEPT_MACRO (lastv != evn[source(fv2[c], lg)].second, consistency_exception, 
		       "Returned faces does not belong to the same vertex");
#endif

    return fv2.size();
} // end of operator



template <class Vertex_heuristic_t>
int emulated_vertex_heuristic_t<Vertex_heuristic_t>::operator() (const vector<edge_ij_elim_t>& eev1,
								 const c_graph_t& cg,
								 vector<edge_ij_elim_t>& eev2) {
  eev2.resize (0);

  // looking for egde eliminations belonging to last vertex elimination
  // and which other vertex eliminations could be performed
  std::set<c_graph_t::vertex_t>   vs;
  for (size_t c= 0; c < eev1.size(); c++) {
    c_graph_t::vertex_t v= eev1[c].front ? eev1[c].j : eev1[c].i;
    if (v == last_vertex) eev2.push_back (eev1[c]); 
    vs.insert (v);}

  if (eev2.size() > 0) return eev2.size(); // belonging to last vertex elimination

  vector<c_graph_t::vertex_t> vv1 (vs.begin(), vs.end()), vv2;
  h (vv1, cg, vv2);
  for (size_t c= 0; c < eev1.size(); c++) {
    c_graph_t::vertex_t v= eev1[c].front ? eev1[c].j : eev1[c].i;
    if (find (vv2.begin(), vv2.end(), v) != vv2.end()) eev2.push_back (eev1[c]); }
  return eev2.size();
}

template <class Object_t, class Ad_graph_t, class Heuristic1_t, class Heuristic2_t, 
          class Heuristic3_t, class Heuristic4_t, class Heuristic5_t>
inline int best_heuristic (const Ad_graph_t& adg, vector<Object_t>& el_seq,
			   Heuristic1_t h1, Heuristic2_t h2, Heuristic3_t h3, 
			   Heuristic4_t h4, Heuristic5_t h5) {
  vector<std::pair<int, vector<Object_t> > > results (5);
  results[0].first= use_heuristic (adg, results[0].second, h1);
  results[1].first= use_heuristic (adg, results[1].second, h2);
  results[2].first= use_heuristic (adg, results[2].second, h3);
  results[3].first= use_heuristic (adg, results[3].second, h4);
  results[4].first= use_heuristic (adg, results[4].second, h5);

  size_t bestIndex = 0;
  int bestCost = results[0].first;
  for (size_t c = 1; c < 5; c++) {
    if (results[c].first < bestCost) {
      bestIndex = c;
      bestCost = results[c].first;
    }
  }
  el_seq = results[bestIndex].second;
  return bestCost;
}

#ifdef USE_MPI
template <class Heuristic_t, class Comm_t>
template <class Vector_t, class Ad_graph_t>
int par_heuristic_t<Heuristic_t, Comm_t>::operator() (const Vector_t& v1, const Ad_graph_t& adg, 
						      Vector_t& v2) {

  // best local objects 
  typedef typename Heuristic_t::objective_t   objective_t;
  h (v1, adg, v2);
  objective_t  my_objective= h.objective(), best_objective;

  // find best global objective value
  MPI::Datatype     datatype= GMPI::which_mpi_t (my_objective);
  MPI::Op           op= h.to_maximize() ? MPI::MAX : MPI::MIN;
  comm.Allreduce (&my_objective, &best_objective, 1, datatype, op);

  // if there are better objectives elsewhere --> throw my objects away
  if (my_objective != best_objective) v2.resize (0);
  return v2.size();
}

template <class Comm_t>
template <class Vector_t, class Ad_graph_t>
int bcast_best_t<Comm_t>::operator() (const Vector_t& v1, const Ad_graph_t&, Vector_t& v2) {
  int my_pe_size[2], sum_pe_size[2];
  my_pe_size[1]= v1.size(); my_pe_size[0]= v1.size() == 0 ? 0 : comm.Get_rank(); 
  comm.Allreduce (my_pe_size, sum_pe_size, 2, MPI::INT, MPI::SUM);
  THROW_EXCEPT_MACRO (sum_pe_size[1] != 1, consistency_exception, "v1 must contain 1 element overall!");
  v2= v1;
  GMPI::comm_ref_t<int, Vector_t> comm_ref (v2); // reference used in communication
  comm.Bcast (comm_ref, sum_pe_size[0]);
  return v2.size(); 
}

#endif // USE_MPI

/// Find best subset of \p v1 w.r.t. \p op, skeleton for new heuristics
template <class Object_t, class Ad_graph_t, class Op_t, class Objective_t>
int standard_heuristic_op (const vector<Object_t>& v1, const Ad_graph_t& adg,
			   vector<Object_t>& v2, Op_t op, base_heuristic_t<Objective_t>& h) {
  v2.resize (0);
  Objective_t best= h.to_maximize() ? std::numeric_limits<Objective_t>::min() : 
                                      std::numeric_limits<Objective_t>::max();

  for (size_t c= 0; c < v1.size(); c++) {
    Object_t o= v1[c];
    Objective_t value= op (o, adg);
    if (h.to_maximize()) {
      if (value > best) v2.resize (0);
      if (value >= best) { v2.push_back (o); best= value; }
    } else {
      if (value < best) v2.resize (0);
      if (value <= best) { v2.push_back (o); best= value;} } }
  h.set_objective (best);
  return v2.size();
}


} // namespace angel
