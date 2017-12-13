// $Id: heuristics.cpp,v 1.11 2008/02/28 14:57:33 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#include <limits.h>
#include <algorithm>

#include "angel/include/heuristics.hpp"
#include "angel/include/angel_exceptions.hpp"
#include "angel/include/angel_tools.hpp"
#ifdef USEXAIFBOOSTER
#include "angel/include/reroutings.hpp"
using namespace xaifBoosterCrossCountryInterface;
#endif

namespace angel {

using namespace std;
using namespace boost;

// =====================================================
// for vertex elimination
// =====================================================

int forward_mode_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
				       const c_graph_t& cg, 
				       vector<c_graph_t::vertex_t>& vv2) {
  vv2.resize (0);
  if (vv1.size() == 0) {set_empty_objective (); return 0; }
  vv2.push_back (vv1[0]);

  for (size_t c= 1; c < vv1.size(); c++) 
    if (vv1[c] < vv2[0]) vv2[0]= vv1[c];
  set_objective (vv2[0]);
  return 1;
}

forward_mode_vertex_t forward_mode_vertex;

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

int reverse_mode_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
				       const c_graph_t& cg, 
				       vector<c_graph_t::vertex_t>& vv2) {
  vv2.resize (0);
  if (vv1.size() == 0) {set_empty_objective (); return 0; }
  vv2.push_back (vv1[0]);

  for (size_t c= 1; c < vv1.size(); c++)
    if (vv1[c] > vv2[0]) vv2[0]= vv1[c];
  set_objective (vv2[0]);
  return 1;
}

reverse_mode_vertex_t reverse_mode_vertex;

// -------------------------------------------------------------------------
// Lowest Markowitz 
// -------------------------------------------------------------------------

// operator for lowest Markowitz on vertices 
struct lmv_op_t {
  int operator() (c_graph_t::vertex_t v, const c_graph_t& cg) {
    return in_degree (v, cg) * out_degree (v, cg); }
};

int lowest_markowitz_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
					   const c_graph_t& cg, 
					   vector<c_graph_t::vertex_t>& vv2) {
  lmv_op_t   lmv_op;
  return standard_heuristic_op (vv1, cg, vv2, lmv_op, *this);
}

lowest_markowitz_vertex_t lowest_markowitz_vertex;

// -------------------------------------------------------------------------
// Lowest relative Markowitz
// -------------------------------------------------------------------------

// operator for lowest relative Markowitz on vertices and edges
struct lrm_op_t {
  vector<int> indep, dep;
  lrm_op_t (const c_graph_t& cg) : indep (num_vertices (cg)), dep (num_vertices (cg)) {
    number_independent_vertices (cg, indep); number_dependent_vertices (cg, dep); }
  int operator() (bool front, c_graph_t::edge_t edge, const c_graph_t& cg) {
    return front ? out_degree (target (edge, cg), cg) - dep[target (edge, cg)]
                 : in_degree (source (edge, cg), cg) - indep[source (edge, cg)]; }
  int operator() (edge_bool_t eb, const c_graph_t& cg) {
    return eb.second ? out_degree (target (eb.first, cg), cg) - dep[target (eb.first, cg)]
                     : in_degree (source (eb.first, cg), cg) - indep[source (eb.first, cg)]; }
  int operator() (c_graph_t::vertex_t v, const c_graph_t& cg) {
    return in_degree (v, cg) * out_degree (v, cg) - indep[v] * dep[v]; }
};

int lowest_relative_markowitz_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
						    const c_graph_t& cg, 
						    vector<c_graph_t::vertex_t>& vv2) {
  lrm_op_t   lrm_op (cg);
  return standard_heuristic_op (vv1, cg, vv2, lrm_op, *this);
}

lowest_relative_markowitz_vertex_t lowest_relative_markowitz_vertex;

// -------------------------------------------------------------------------
// subroutine of Lowest fill-in
// -------------------------------------------------------------------------

// how many new in_edges has j:target(e) by back-eliminating e
int new_in_edges (c_graph_t::edge_t e, const c_graph_t& cg) {

  typedef c_graph_t::vertex_t        vertex_t;
  typedef c_graph_t::iei_t           iei_t;

  iei_t    siei, sie_end, tiei, tie_end;
  tie (siei, sie_end)= in_edges (source (e, cg), cg);
  tie (tiei, tie_end)= in_edges (target (e, cg), cg);
  int new_edges= 0;

  vector<vertex_t> nsl; // list of new sources to check double insertions

  for (; siei != sie_end; ++siei) {
    vertex_t ss= source (*siei, cg);
    iei_t i= tiei;
    for (; i != tie_end; ++i)
      if (source (*i, cg) == ss) break;
    if (i == tie_end
	&& find (nsl.begin(), nsl.end(), ss) == nsl.end()) {
      nsl.push_back (ss); new_edges++; }
  }

  return new_edges;
}

// how many new out_edges has j:=source(e) by front-eliminating e
int new_out_edges (c_graph_t::edge_t e, const c_graph_t& cg) {

  typedef c_graph_t::vertex_t        vertex_t;
  typedef c_graph_t::oei_t           oei_t;

  oei_t    soei, soe_end, toei, toe_end;
  tie (soei, soe_end)= out_edges (source (e, cg), cg);
  tie (toei, toe_end)= out_edges (target (e, cg), cg);
  int new_edges= 0;

  vector<vertex_t> ntl; // list of new targets to check double insertions

  for (; toei != toe_end; ++toei) {
    vertex_t tt= target (*toei, cg);
    oei_t i= soei;
    for (; i != soe_end; ++i)
      if (target (*i, cg) == tt) break;
    if (i == soe_end
	&& find (ntl.begin(), ntl.end(), tt) == ntl.end()) {
      ntl.push_back (tt); new_edges++; }
  }

  return new_edges;
}


// -------------------------------------------------------------------------
// Lowest fill-in
// -------------------------------------------------------------------------


// operator for lowest fill-in on vertices 
struct fiv_op_t {
  int operator() (c_graph_t::vertex_t v, const c_graph_t& cg) {
    int                  fill_ins= 0;
    c_graph_t::iei_t     iei, ie_end;
    for (tie (iei, ie_end)= in_edges (v, cg); iei != ie_end; ++iei)
      fill_ins+= new_out_edges (*iei, cg);
    return fill_ins; }
};

int lowest_fill_in_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
					 const c_graph_t& cg, 
					 vector<c_graph_t::vertex_t>& vv2) {
  fiv_op_t   fiv_op;
  return standard_heuristic_op (vv1, cg, vv2, fiv_op, *this);
}

lowest_fill_in_vertex_t lowest_fill_in_vertex;

// -------------------------------------------------------------------------
// subroutines of LMMD MOMR
// -------------------------------------------------------------------------

// how much the markowitz degree of j:=source(e) is enlarged by front-eliminating e
// this is in_degree(j) * #new out_edges(j) 
int markowitz_enlargement_front (c_graph_t::edge_t e, const c_graph_t& cg,
				 bool eliminate_parallel_edges= false) {
  int ee= 0; // number of eliminated edges
  c_graph_t::vertex_t s= source (e, cg), t= target (e, cg);
  if (vertex_type (t, cg) == intermediate){ // if dependent edges are not eliminated
    if (eliminate_parallel_edges) {
      c_graph_t::oei_t soei, soe_end;
      for (tie (soei, soe_end)= out_edges (s, cg); soei != soe_end; soei++)
	ee+= target (*soei, cg) == t;
    } else ee= 1;
  }
  return in_degree (source (e, cg), cg) * (new_out_edges (e, cg) - ee);
}

// how much is the markowitz degree of j:=target(e2) enlarged by front-eliminating e
int markowitz_enlargement_front (c_graph_t::edge_t e, c_graph_t::edge_t e2, 
				 const c_graph_t& cg) {

  THROW_DEBUG_EXCEPT_MACRO (target (e, cg) != source (e2, cg), consistency_exception,
			 "e and e2 does not match"); 

  c_graph_t::vertex_t j= target (e2, cg), se= source (e, cg), te= target (e, cg);

  // if e is last in_edge of te than e2 will be eliminated -> j has one in_edge less
  // int in_edge_change= in_degree (te, cg) == 1 ? -1 : 0; 

  // if e is last in_edge of te than e2 and its parallel edges will be eliminated 
  // -> j has one or more in_edges less
  int in_edge_change= in_degree (te, cg) == 1 ? - count_parallel_edges (e2, cg) : 0; 

  // if source(e) is not source of an in_edge of j -> j has one in_edge more
  c_graph_t::iei_t  iei, ie_end;
  for (tie (iei, ie_end)= in_edges (j, cg); iei != ie_end; ++iei)
    if (source (*iei, cg) == se) break;
  if (iei == ie_end) in_edge_change++;

  return in_edge_change * out_degree (j, cg);
}

// how much the markowitz degree of j:=target(e) is enlarged by back-eliminating e
// this is out_degree(j) * #new in_edges(j) 
int markowitz_enlargement_back (c_graph_t::edge_t e, const c_graph_t& cg,
				bool eliminate_parallel_edges= false) {

  int ee= 0; // number of eliminated edges
  if (eliminate_parallel_edges) {
    c_graph_t::vertex_t s= source (e, cg), t= target (e, cg);
    c_graph_t::iei_t tiei, tie_end;
    for (tie (tiei, tie_end)= in_edges (t, cg); tiei != tie_end; ++tiei)
      ee+= source (*tiei, cg) == s;
  } else ee= 1;

  return out_degree (target (e, cg), cg) * (new_in_edges (e, cg) - ee);
}

// how much is the markowitz degree of j:=source(e2) enlarged by back-eliminating e
int markowitz_enlargement_back (c_graph_t::edge_t e, c_graph_t::edge_t e2, 
				const c_graph_t& cg) {

  // assert (source (e, cg) == target (e2, cg));
  THROW_DEBUG_EXCEPT_MACRO (source (e, cg) != target (e2, cg), consistency_exception,
			 "e and e2 does not match"); 

  c_graph_t::vertex_t j= source (e2, cg), se= source (e, cg), te= target (e, cg);

  // if e is last out_edge of se than e2 will be eliminated -> j has one out_edge less
  int out_edge_change= out_degree (se, cg) == 1 ? -1 : 0; 

  // if target(e) is not target of an out_edge of j -> j has one out_edge more
  c_graph_t::oei_t    oei, oe_end;
  for (tie (oei, oe_end)= out_edges (j, cg); oei != oe_end; ++oei)
    if (target (*oei, cg) == te) break;
  if (oei == oe_end) out_edge_change++;

  return out_edge_change * in_degree (j, cg);
}

// how much is the markowitz degree of all neighbors of v increased by its elimination
// multiply occurred neighbors are counted only once
int markowitz_enlargement_all_neighbors (c_graph_t::vertex_t v, const c_graph_t& cg) {

  using namespace std;

  typedef c_graph_t::vertex_t              vertex_t;

  int enl= 0;

  // parallel edges does not cause multiple Markowitz changes
  vector<vertex_t> cv; // already checked vertices
  cv.reserve (10);     // reduce mallocs

  c_graph_t::iei_t     iei, ie_end;
  tie (iei, ie_end)= in_edges (v, cg);
  for (; iei != ie_end; ++iei) {
    vertex_t v= source (*iei, cg);
    if (find (cv.begin(), cv.end(), v) == cv.end()) {
      enl+= markowitz_enlargement_front (*iei, cg, true);
      cv.push_back (v); } }

  cv.resize (0);             // reduce search space
  c_graph_t::oei_t     oei, oe_end;
  tie (oei, oe_end)= out_edges (v, cg);
  for (; oei != oe_end; ++oei) {
    vertex_t v= target (*oei, cg);    
    if (find (cv.begin(), cv.end(), v) == cv.end()) {
      enl+= markowitz_enlargement_back (*oei, cg, true);
      cv.push_back (v); } }
  return enl;
}

struct markowitz_enlargement_front_t {
  c_graph_t::edge_t _e;                  // first edge is stored
  markowitz_enlargement_front_t (c_graph_t::edge_t e) : _e (e) {}
  int operator() (c_graph_t::edge_t e2, const c_graph_t& cg) const {
    return markowitz_enlargement_front (_e, e2, cg); }
};

struct markowitz_enlargement_back_t {
  c_graph_t::edge_t _e;                  // first edge is stored
  markowitz_enlargement_back_t (c_graph_t::edge_t e) : _e (e) {}
  int operator() (c_graph_t::edge_t e2, const c_graph_t& cg) const {
    return markowitz_enlargement_back (_e, e2, cg); }
};


// -------------------------------------------------------------------------
// LMMD
// -------------------------------------------------------------------------

// operator that computes LMMD value for a single vertex
struct lmmdv_op_t {
  double w; // weight
  lmmdv_op_t (double _w) : w (_w) {}
  int operator() (c_graph_t::vertex_t v, const c_graph_t& cg) {
    int markowitz= in_degree (v, cg) * out_degree (v, cg),
      damage= markowitz_enlargement_all_neighbors (v, cg);
    return int (double (markowitz) + w * double (damage)); }
};

int lmmd_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
			       const c_graph_t& cg, 
			       vector<c_graph_t::vertex_t>& vv2) {
  lmmdv_op_t      lmmdv_op (weight);
  return standard_heuristic_op (vv1, cg, vv2, lmmdv_op, *this);
}
  
lmmd_vertex_t lmmd_vertex (1.0);

// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

// operator that computes MOMR value for a single vertex
struct momrv_op_t {
  int operator() (c_graph_t::vertex_t v, const c_graph_t& cg) {
    int momr= in_degree (v, cg) * out_degree (v, cg)
              - markowitz_enlargement_all_neighbors (v, cg);
#ifndef NDEBUG
    c_graph_t         gtmp (cg);  vertex_elimination (v, gtmp);
    THROW_DEBUG_EXCEPT_MACRO (overall_markowitz_degree (cg) - overall_markowitz_degree (gtmp) != momr, 
			   consistency_exception, "momr not correct"); 
#endif
    return momr; }
};

int momr_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
			       const c_graph_t& cg, 
			       vector<c_graph_t::vertex_t>& vv2) {
  momrv_op_t      momrv_op;
  return standard_heuristic_op (vv1, cg, vv2, momrv_op, *this);
}

momr_vertex_t momr_vertex;

// -------------------------------------------------------------------------
// subroutines of Maximal overall path length reduction
// -------------------------------------------------------------------------

// overall path length reduction of face (e1, e2)
inline int oplr_face (c_graph_t::edge_t e1, c_graph_t::edge_t e2,
		      const vector<int>& vni, const vector<int>& vli, 
		      const vector<int>& vno, const vector<int>& vlo, 
		      const c_graph_t& cg) {

  // assert (target (e1, cg) == source (e2, cg));
  THROW_DEBUG_EXCEPT_MACRO (target (e1, cg) != source (e2, cg), consistency_exception,
			 "e1 and e2 does not match"); 

  c_graph_t::vertex_t p= source (e1, cg), s= target (e2, cg);

  c_graph_t::edge_t e;
  bool found;
  boost::tie (e, found)= edge (p, s, cg);

  return found ? vno[s] * (vni[p] + vli[p]) + vni[p] * (vno[s] + vlo[s])
               : vni[p] * vno[s];
}

int oplr_edge_front (c_graph_t::edge_t e,
		     const vector<int>& vni, const vector<int>& vli, 
		     const vector<int>& vno, const vector<int>& vlo, 
		     const c_graph_t& cg) {

  int oplr= 0;
  graph_traits<c_graph_t>::out_edge_iterator    oei, oe_end;
  tie (oei, oe_end)= out_edges (target (e, cg), cg);

  for (; oei != oe_end; ++oei)
    oplr+= oplr_face (e, *oei, vni, vli, vno, vlo, cg);

  return oplr;
}

int oplr_edge_back (c_graph_t::edge_t e,
		    const vector<int>& vni, const vector<int>& vli, 
		    const vector<int>& vno, const vector<int>& vlo, 
		    const c_graph_t& cg) {

  int oplr= 0;
  graph_traits<c_graph_t>::in_edge_iterator    iei, ie_end;
  tie (iei, ie_end)= in_edges (source (e, cg), cg);

  for (; iei != ie_end; ++iei)
    oplr+= oplr_face (*iei, e, vni, vli, vno, vlo, cg);

  return oplr;
}

// -------------------------------------------------------------------------
// Maximal overall path length reduction
// -------------------------------------------------------------------------

// operator that computes path length reduction for a single vertex
struct oplrv_op_t {
  vector<int> vni, vli, vno, vlo;
  oplrv_op_t (const c_graph_t& cg) {
    in_out_path_lengths (cg, vni, vli, vno, vlo); }
  int operator () (c_graph_t::vertex_t v, const c_graph_t& cg) {
    int oplr= 0;
    c_graph_t::iei_t    iei, ie_end;
    for (tie (iei, ie_end)= in_edges (v, cg); iei != ie_end; ++iei)
      oplr+= oplr_edge_front (*iei, vni, vli, vno, vlo, cg);
    return oplr; }
};

int moplr_vertex_t::operator() (const vector<c_graph_t::vertex_t>& vv1,
				const c_graph_t& cg, 
				vector<c_graph_t::vertex_t>& vv2) {
  oplrv_op_t oplrv_op (cg);
  return standard_heuristic_op (vv1, cg, vv2, oplrv_op, *this);
}

moplr_vertex_t moplr_vertex;

// =====================================================
// for edge elimination (in c-graph)
// =====================================================

int forward_mode_edge_f (const vector<c_graph_t::edge_t>& ev1,
			 bool front,
			 const c_graph_t& cg,
			 vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);

  if (ev1.size() == 0) return 0;
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++)
    if (front ? inv_lex_less (ev1[c], ev2[0], cg) : lex_less (ev1[c], ev2[0], cg)) 
      ev2[0]= ev1[c];

  return 1;
}

// objective function for forward mode in edge elimination
// works up to 47 million vertices in IEEE arithmetic
inline double fme_obj (edge_bool_t eb, const c_graph_t& cg) {
  c_graph_t::edge_t   edge= eb.first;
  // front is prefered -> add something if not front because we minimize
  int high, low, notfront;
  if (eb.second) high= target (edge, cg), low= source (edge, cg), notfront= 0;
  else high= source (edge, cg), low= target (edge, cg), notfront= 1;
  return (2 * high + notfront) * double (cg.x()) + low;
}

int forward_mode_edge_t::operator() (const vector<edge_bool_t>& ev1,
				     const c_graph_t& cg,
				     vector<edge_bool_t>& ev2) {
  ev2.resize (0);
  if (ev1.size() == 0) {set_empty_objective (); return 0; }
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
//    THROW_DEBUG_EXCEPT_MACRO (fme_obj (ev1[c], cg) < fme_obj (ev2[0], cg) != lex_less (ev1[c], ev2[0], cg),
//			   consistency_exception, "objective function and comparator does not match");
    if (lex_less (ev1[c], ev2[0], cg)) ev2[0]= ev1[c]; }
  set_objective (fme_obj (ev2[0], cg));
  return 1;
}

forward_mode_edge_t forward_mode_edge;

// remark fm: if all eliminatable edges are considered than only front elimination is used
//            this way one can force same sequences in vertex, edge and face elimination
//            when forward mode is used as sole criterion

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

int reverse_mode_edge_f (const vector<c_graph_t::edge_t>& ev1,
			 bool front,
			 const c_graph_t& cg,
			 vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);

  if (ev1.size() == 0) return 0;
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++)
    if (front ? inv_lex_greater (ev1[c], ev2[0], cg) : lex_greater (ev1[c], ev2[0], cg)) 
      ev2[0]= ev1[c];

  return 1;
}

// objective function for reverse mode in edge elimination
// works up to 47 million vertices in IEEE arithmetic
inline double rme_obj (edge_bool_t eb, const c_graph_t& cg) {
  c_graph_t::edge_t   edge= eb.first;
  // front is prefered -> add something if front because we maximize
  int high, low, front;
  if (eb.second) high= target (edge, cg), low= source (edge, cg), front= 1;
  else high= source (edge, cg), low= target (edge, cg), front= 0;
  return (2 * high + front) * double (cg.x()) + low;
}

int reverse_mode_edge_t::operator() (const vector<edge_bool_t>& ev1,
				     const c_graph_t& cg,
				     vector<edge_bool_t>& ev2) {
  ev2.resize (0);

  if (ev1.size() == 0) {set_empty_objective (); return 0; }
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
//    THROW_DEBUG_EXCEPT_MACRO ((rme_obj (ev1[c], cg) > rme_obj (ev2[0], cg)) != lex_greater (ev1[c], ev2[0], cg),
//			   consistency_exception, "objective function and comparator does not match");
    if (lex_greater (ev1[c], ev2[0], cg)) ev2[0]= ev1[c]; }
    set_objective (rme_obj (ev2[0], cg));
  return 1;
}

reverse_mode_edge_t reverse_mode_edge;

// remark rm: if all eliminatable edges are considered than only front elimination is used
//            this way one can force same sequences in vertex, edge and face elimination
//            when reverse mode is used as sole criterion

// -------------------------------------------------------------------------
// Lowest Markowitz
// -------------------------------------------------------------------------

int lowest_markowitz_edge_f (const vector<c_graph_t::edge_t>& ev1,
			     bool front,
			     const c_graph_t& cg,
			     vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);

  if (ev1.size() == 0) return 0;
  int min_degree= front ? out_degree (target (ev1[0], cg), cg)
                        : in_degree (source (ev1[0], cg), cg);
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
    c_graph_t::edge_t e= ev1[c];
    int degree= front ? out_degree (target (e, cg), cg)
                      : in_degree (source (e, cg), cg);
    if (degree < min_degree) ev2.resize (0);
    if (degree <= min_degree) {
      ev2.push_back (e); min_degree= degree;}
  }
  return ev2.size();
}

// operator that computes the Markowitz degree for a single edge elimination
struct lme_op_t {
  int operator() (edge_bool_t eb, const c_graph_t& cg) {
    return eb.second ? out_degree (target (eb.first, cg), cg)
                     : in_degree (source (eb.first, cg), cg); }
};

int lowest_markowitz_edge_t::operator() (const vector<edge_bool_t>& ev1,
					 const c_graph_t& cg,
					 vector<edge_bool_t>& ev2) {
  lme_op_t      lme_op;
  return standard_heuristic_op (ev1, cg, ev2, lme_op, *this);
}

lowest_markowitz_edge_t lowest_markowitz_edge;

// -------------------------------------------------------------------------
// Lowest relative Markowitz
// -------------------------------------------------------------------------

int lowest_relative_markowitz_edge_f (const vector<c_graph_t::edge_t>& ev1,
				      bool front,
				      const c_graph_t& cg,
				      vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);
  if (ev1.size() == 0) return 0;
  lrm_op_t   lrm_op (cg);
  int min_degree= lrm_op (front, ev1[0], cg);
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
    int degree= lrm_op (front, ev1[c], cg); 
    if (degree < min_degree) ev2.resize (0);
    if (degree <= min_degree) {
      ev2.push_back (ev1[c]); min_degree= degree;} }
  return ev2.size();
}

int lowest_relative_markowitz_edge_t::operator() (const vector<edge_bool_t>& ev1,
						  const c_graph_t& cg,
						  vector<edge_bool_t>& ev2) {
  lrm_op_t   lrm_op (cg);
  return standard_heuristic_op (ev1, cg, ev2, lrm_op, *this);
}

lowest_relative_markowitz_edge_t lowest_relative_markowitz_edge;

// -------------------------------------------------------------------------
// Lowest fill-in
// -------------------------------------------------------------------------

inline int fill_in_front (c_graph_t::edge_t e, const c_graph_t& cg) {
  return new_out_edges (e, cg);
}

inline int fill_in_back (c_graph_t::edge_t e, const c_graph_t& cg) {
  return new_in_edges (e, cg);
}


int lowest_fill_in_edge_f (const vector<c_graph_t::edge_t>& ev1,
			   bool front,
			   const c_graph_t& cg,
			   vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);

  if (ev1.size() == 0) return 0;
  int min_degree= front ? fill_in_front (ev1[0], cg)
                        : fill_in_back (ev1[0], cg);
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
    c_graph_t::edge_t e= ev1[c];
    int degree= front ? fill_in_front (e, cg)
                      : fill_in_back (e, cg);
    if (degree < min_degree) ev2.resize (0);
    if (degree <= min_degree) {
      ev2.push_back (e); min_degree= degree;}
  }

  return ev2.size();
}


// -------------------------------------------------------------------------
// LMMD
// -------------------------------------------------------------------------

lmmd_edge_t lmmd_edge (1.0);

inline int lmmd_edge_front (c_graph_t::edge_t e, double w, const c_graph_t& cg) {
  int markowitz= out_degree (target (e, cg), cg); 
  markowitz_enlargement_front_t me (e);
  int damage= markowitz_enlargement_front (e, cg)
              + sum_over_all_out_edges (target (e, cg), cg, me);
  return int (double (markowitz) + w * double (damage));
}

inline int lmmd_edge_back (c_graph_t::edge_t e, double w, const c_graph_t& cg) {
  int markowitz= in_degree (source (e, cg), cg);
  markowitz_enlargement_back_t me (e);
  int damage= markowitz_enlargement_back (e, cg)
              + sum_over_all_in_edges (source (e, cg), cg, me);
  return int (double (markowitz) + w * double (damage));
}

struct lmmde_op_t {
  double w; // weight
  lmmde_op_t (double _w) : w (_w) {}
  int operator() (edge_bool_t eb, const c_graph_t& cg) {
    return eb.second ? lmmd_edge_front (eb.first, w, cg)
                     : lmmd_edge_back (eb.first, w, cg); }
};

int lmmd_edge_t::operator() (const vector<edge_bool_t>& ev1,
			     const c_graph_t& cg,
			     vector<edge_bool_t>& ev2) {
  lmmde_op_t      lmmde_op (weight);
  return standard_heuristic_op (ev1, cg, ev2, lmmde_op, *this);
}


// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

inline int momr_edge_front (c_graph_t::edge_t e, const c_graph_t& cg) {

  markowitz_enlargement_front_t me (e);
  int momr= out_degree (target (e, cg), cg) - markowitz_enlargement_front (e, cg)
           - sum_over_all_out_edges (target (e, cg), cg, me);

#ifndef NDEBUG
  c_graph_t         gtmp (cg);

  // eliminating e in gtmp does not work -> edge_descriptor from gtmp needed
  c_graph_t::edge_t   etmp;
  c_graph_t::ei_t     ei, e_end;
  c_graph_t::vertex_t s= source (e, cg), t= target (e, cg);
  for (boost::tie (ei, e_end)= edges (gtmp); ei != e_end; ++ei)
    if (source (*ei, gtmp) == s && target (*ei, gtmp) == t) {
      etmp= *ei; break; }
  // assert (ei != e_end); // otherwise edge not found
  THROW_DEBUG_EXCEPT_MACRO (ei == e_end, consistency_exception, "No matching edge found"); 

  front_edge_elimination (etmp, gtmp);
  // assert (overall_markowitz_degree (cg) - overall_markowitz_degree (gtmp) == momr);
  THROW_DEBUG_EXCEPT_MACRO (overall_markowitz_degree (cg) - overall_markowitz_degree (gtmp) != momr, 
			 consistency_exception, "momr not correct"); 
#endif
  return momr;
}

inline int momr_edge_back (c_graph_t::edge_t e, const c_graph_t& cg) {
  // reduction of markowitz degree in vertex source (e)
  int momr= in_degree (source (e, cg), cg);
  // target (e) can get a larger markowitz degree due to new in_edges
  momr-= markowitz_enlargement_back (e, cg);

  // the predecessors of source (e) can get a larger markowitz degree
  // due to a new out_edge to target (e)
  c_graph_t::iei_t    iei, ie_end;
  tie (iei, ie_end)= in_edges (source (e, cg), cg);
  for (; iei != ie_end; ++iei)
    momr-= markowitz_enlargement_back (e, *iei, cg);
#ifndef NDEBUG
  markowitz_enlargement_back_t me (e);
  int momr2= in_degree (source (e, cg), cg) - markowitz_enlargement_back (e, cg)
            - sum_over_all_in_edges (source (e, cg), cg, me);
  // assert (momr == momr2);
  THROW_DEBUG_EXCEPT_MACRO (momr2 != momr, consistency_exception, "momr not correct"); 

  c_graph_t         gtmp (cg);
  c_graph_t::vi_t   vi, v_end;
  int old_overall_markowitz= 0, new_overall_markowitz= 0;
  for (boost::tie (vi, v_end)= vertices (gtmp); vi != v_end; ++vi)
    old_overall_markowitz+= in_degree (*vi, gtmp) * out_degree (*vi, gtmp);

  // eliminating e in gtmp does not work -> edge_descriptor from gtmp needed
  c_graph_t::edge_t   etmp;
  c_graph_t::ei_t     ei, e_end;
  c_graph_t::vertex_t s= source (e, cg), t= target (e, cg);
  for (boost::tie (ei, e_end)= edges (gtmp); ei != e_end; ++ei)
    if (source (*ei, gtmp) == s && target (*ei, gtmp) == t) {
      etmp= *ei; break; }
  // assert (ei != e_end); // otherwise edge not found
  THROW_DEBUG_EXCEPT_MACRO (ei == e_end, consistency_exception, "No matching edge found"); 

  back_edge_elimination (etmp, gtmp);
  for (boost::tie (vi, v_end)= vertices (gtmp); vi != v_end; ++vi)
    new_overall_markowitz+= in_degree (*vi, gtmp) * out_degree (*vi, gtmp);
  // assert (old_overall_markowitz-new_overall_markowitz == momr);
  THROW_DEBUG_EXCEPT_MACRO (old_overall_markowitz-new_overall_markowitz != momr, 
			 consistency_exception, "momr not correct"); 
#endif
  return momr;
}

// operator for MOMR on edges
struct momre_op_t {
  int operator() (bool front, c_graph_t::edge_t edge, const c_graph_t& cg) {
    return front ? momr_edge_front (edge, cg) : momr_edge_back (edge, cg); }
  int operator() (edge_bool_t eb, const c_graph_t& cg) {
    return eb.second ? momr_edge_front (eb.first, cg) : momr_edge_back (eb.first, cg); }
};

int momr_edge_f (const vector<c_graph_t::edge_t>& ev1,
		 bool front,
		 const c_graph_t& cg,
		 vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);
  if (ev1.size() == 0) return 0;
  momre_op_t momre_op;
  int max_momr= momre_op (front, ev1[0], cg);
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
    int momr= momre_op (front, ev1[c], cg); 
    if (momr > max_momr) ev2.resize (0);
    if (momr >= max_momr) {
      ev2.push_back (ev1[c]); max_momr= momr;}
  }
  return ev2.size();
}

int momr_edge_t::operator() (const vector<edge_bool_t>& ev1, const c_graph_t& cg,
			     vector<edge_bool_t>& ev2) {
  momre_op_t   momre_op;
  return standard_heuristic_op (ev1, cg, ev2, momre_op, *this);
}

momr_edge_t momr_edge;

// -------------------------------------------------------------------------
// Minimal distance 
// -------------------------------------------------------------------------

// operator for minimal distances on faces
struct diste_op_t {
  int operator() (edge_bool_t eb, const c_graph_t& cg) {
    c_graph_t::vertex_t           i= source (eb.first, cg), j= target (eb.first, cg), dist= 0;
    vector<c_graph_t::vertex_t>   nb;
    if (eb.second) {
      successor_set (j, cg, nb);
      for (size_t c= 0; c < nb.size(); c++)
	if (nb[c] - i > dist) dist= nb[c] - i;
    } else {
      predecessor_set (j, cg, nb);
      for (size_t c= 0; c < nb.size(); c++)
	if (j - nb[c] > dist) dist= j - nb[c]; }
    THROW_DEBUG_EXCEPT_MACRO (dist <= 0, consistency_exception, "Wrong distance in edge elimination");
    return dist; }
};

int minimal_distance_edge_t::operator() (const vector<edge_bool_t>& ev1, const c_graph_t& cg,
					 vector<edge_bool_t>& ev2) {
  return standard_heuristic_op (ev1, cg, ev2, diste_op_t(), *this);
}

minimal_distance_edge_t minimal_distance_edge;

// -------------------------------------------------------------------------
// Maximal overall path length reduction
// -------------------------------------------------------------------------

int moplr_edge (const vector<c_graph_t::edge_t>& ev1,
		bool front,
		const c_graph_t& cg,
		vector<c_graph_t::edge_t>& ev2) {
  ev2.resize (0);
  if (ev1.size() == 0) return 0;

  vector<int> vni, vli, vno, vlo;
  in_out_path_lengths (cg, vni, vli, vno, vlo);

  int max_oplr= front ? oplr_edge_front (ev1[0], vni, vli, vno, vlo, cg)
                      : oplr_edge_back (ev1[0], vni, vli, vno, vlo, cg);
  ev2.push_back (ev1[0]);

  for (size_t c= 1; c < ev1.size(); c++) {
    c_graph_t::edge_t e= ev1[c];
    int oplr= front ? oplr_edge_front (e, vni, vli, vno, vlo, cg)
                    : oplr_edge_back (e, vni, vli, vno, vlo, cg);
    if (oplr > max_oplr) ev2.resize (0);
    if (oplr >= max_oplr) {
      ev2.push_back (e); max_oplr= oplr;}
  }

  return ev2.size();
}



// =====================================================
// for face elimination
// =====================================================

// objective function for forward and reverse mode in face elimination
// works up to 165 thousands vertices in IEEE arithmetic
inline double fmf_obj (line_graph_t::face_t f, const line_graph_t& lg) {
  int i, j, k, x= lg.x();
  face_vertex_name (f, lg, i, j, k);
  return ((double (j) * double (x)) + double (i)) * double (x) + double (k);
}

int forward_mode_face_t::operator() (const vector<line_graph_t::face_t>& fv1,
				     const line_graph_t& lg,
				     vector<line_graph_t::face_t>& fv2) {
  fv2.resize (0);
  if (fv1.size() == 0) {set_empty_objective (); return 0; }
  fv2.push_back (fv1[0]);

  for (size_t c= 1; c < fv1.size(); c++) {
//    THROW_DEBUG_EXCEPT_MACRO (fmf_obj (fv1[c], lg) < fmf_obj (fv2[0], lg) != lex_less_face (fv1[c], fv2[0], lg),
//			   consistency_exception, "objective function and comparator does not match");
    if (lex_less_face (fv1[c], fv2[0], lg)) fv2[0]= fv1[c]; }
  set_objective (fmf_obj (fv2[0], lg));
  return 1;
}

forward_mode_face_t forward_mode_face;

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

int reverse_mode_face_t::operator() (const vector<line_graph_t::face_t>& fv1,
				     const line_graph_t& lg,
				     vector<line_graph_t::face_t>& fv2) {
  fv2.resize (0);
  if (fv1.size() == 0) {set_empty_objective (); return 0; }
  fv2.push_back (fv1[0]);

  for (size_t c= 1; c < fv1.size(); c++) {
//    THROW_DEBUG_EXCEPT_MACRO (fmf_obj (fv1[c], lg) < fmf_obj (fv2[0], lg) != lex_less_face (fv1[c], fv2[0], lg),
//			   consistency_exception, "objective function and comparator does not match");
    if (!lex_less_face (fv1[c], fv2[0], lg)) fv2[0]= fv1[c]; }
  set_objective (fmf_obj (fv2[0], lg));
  return 1;
}

reverse_mode_face_t reverse_mode_face;

int reverse_mode_face_whole_vertex_t::operator() (const vector<line_graph_t::face_t>& fv1,
						  const line_graph_t& lg,
						  vector<line_graph_t::face_t>& fv2) {
  fv2.resize (0);
  if (fv1.size() == 0) return 0;
  fv2.push_back (fv1[0]);
  line_graph_t::const_evn_t evn= get(boost::vertex_name, lg);

  int maxv= evn[source(fv1[0], lg)].second;

  for (size_t c= 1; c < fv1.size(); c++) {
    line_graph_t::face_t f= fv1[c];
    int v= evn[source(f, lg)].second;
    if (v > maxv) {fv2.resize (0); maxv= v;}
    if (v == maxv) fv2.push_back (f); }

  return fv2.size();
}

reverse_mode_face_whole_vertex_t reverse_mode_face_whole_vertex;

// -------------------------------------------------------------------------
// subroutine of Lowest Markowitz on line graph
// -------------------------------------------------------------------------

void markowitz_on_line_graph (const line_graph_t& lg,
			      vector<int>& mdegree) {

  line_graph_t::const_evn_t   evn= get(vertex_name, lg);
  int                         max_vertex_cg= 0;
  line_graph_t::ei_t          ei, e_end;
  for (boost::tie (ei, e_end)= vertices (lg); ei != e_end; ++ei)
    if (max_vertex_cg < evn[*ei].second) max_vertex_cg= evn[*ei].second;
    
  // number of faces in which vertex j occurs in the center
  // filter out independent and dependent vertices (i,i,k) or (i,k,k)
  mdegree.resize (0); mdegree.resize (max_vertex_cg+1);
  line_graph_t::fi_t          fi, f_end;
  for (boost::tie (fi, f_end)= edges (lg); fi != f_end; ++fi) {
    line_graph_t::edge_t s= source (*fi, lg), t= target (*fi, lg);
    if (evn[s].first != evn[s].second && evn[t].first != evn[t].second)
	mdegree[evn[s].second]++; }
}



// -------------------------------------------------------------------------
// Lowest Markowitz
// -------------------------------------------------------------------------

// operator for lowest Markowitz on faces
struct lmf_op_t {
  vector<int> mdegree;
  lmf_op_t (const line_graph_t& lg) {
    markowitz_on_line_graph (lg, mdegree); }
  int operator() (line_graph_t::face_t f, const line_graph_t& lg) {
    int i, j, k; face_vertex_name (f, lg, i, j, k);
    THROW_DEBUG_EXCEPT_MACRO (mdegree[j] == 0, consistency_exception, "Un-eliminatable face in fv1"); 
    return mdegree[j]; }
};

int lowest_markowitz_face_t::operator() (const vector<line_graph_t::face_t>& fv1,
					 const line_graph_t& lg,
					 vector<line_graph_t::face_t>& fv2) {
  lmf_op_t      lmf_op (lg);
  return standard_heuristic_op (fv1, lg, fv2, lmf_op, *this);
}


// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

// will a face (p,i,k) from (i,j)'s predecessor (p,i) to (i,k) be inserted ?
// or does it already exist
class new_pik_t {
  int i, k;
public:
  new_pik_t (int _i, int _k) : i (_i), k (_k) {}
  int operator() (line_graph_t::face_t pij, const line_graph_t& lg) const {
    line_graph_t::edge_t pi= source (pij, lg);
    // for independent edges, new faces doesn't affect Mark. -> stop
    if (vertex_type (pi, lg) == independent) return 0; 
    line_graph_t::ofi_t ofi, of_end;
    for (tie (ofi, of_end)= out_edges (pi, lg); ofi != of_end; ++ofi) {
      int v1, v2;
      edge_vertex_name (target (*ofi, lg), lg, v1, v2); 
      // assert (v1 == i);
      THROW_DEBUG_EXCEPT_MACRO (v1 != i, consistency_exception, "Adjacency corrupted in line graph"); 
      if (v2 == k) return 0; } // (p,i,k) found
    return 1;
  }
};

struct source_not_independent_t {
  int operator() (line_graph_t::face_t f, const line_graph_t& lg) const {
    return (vertex_type (source (f, lg), lg) != independent); }
};

// will a face (i,k,s) to (j,k)'s successor (k,s) from (i,k) be inserted ?
// or does it already exist
class new_iks_t {
  int i, k;
public:
  new_iks_t (int _i, int _k) : i (_i), k (_k) {}
  int operator() (line_graph_t::face_t jks, const line_graph_t& lg) const {
    line_graph_t::edge_t ks= target (jks, lg);
    // for dependent edges, new faces doesn't affect Mark. -> stop
    if (vertex_type (ks, lg) == dependent) return 0; 
    line_graph_t::ifi_t ifi, if_end;
    for (tie (ifi, if_end)= in_edges (ks, lg); ifi != if_end; ++ifi) {
      int v1, v2;
      edge_vertex_name (source (*ifi, lg), lg, v1, v2);
      // assert (v2 == k);
      THROW_DEBUG_EXCEPT_MACRO (v2 != k, consistency_exception, "Adjacency corrupted in line graph"); 
      if (v1 == i) return 0; } // (i,k,s) found
    return 1;
  }
};

struct target_not_dependent_t {
  int operator() (line_graph_t::face_t f, const line_graph_t& lg) const {
    return (vertex_type (target (f, lg), lg) != dependent); }
};

// operator for MOMR on faces
struct momrf_op_t {
  int operator() (line_graph_t::face_t f, const line_graph_t& lg) {
    int momr= 1; // the face itself
    int i, j, k; // f's vertices w.r.t. c-graph
    face_vertex_name (f, lg, i, j, k);
    line_graph_t::edge_t ij= source (f, lg), jk= target (f, lg);

    new_pik_t new_pik (i, k);
    momr-= sum_over_all_in_edges (ij, lg, new_pik);
    if (out_degree (ij, lg) == 1) 
      momr+= sum_over_all_in_edges (ij, lg, source_not_independent_t());

    new_iks_t new_iks (i, k);
    momr-= sum_over_all_out_edges (jk, lg, new_iks);
    if (in_degree (jk, lg) == 1) 
      momr+= sum_over_all_out_edges (jk, lg, target_not_dependent_t());
#ifndef NDEBUG
    line_graph_t         gtmp (lg);

    // eliminating f in gtmp does not work -> edge_descriptor from gtmp needed
    line_graph_t::face_t   ftmp;
    ftmp= *same_edge (f, lg, gtmp);
    face_elimination (ftmp, gtmp);

    int old_overall_markowitz= overall_markowitz_degree (lg);
    int new_overall_markowitz= overall_markowitz_degree (gtmp);
    if (old_overall_markowitz - new_overall_markowitz != momr) {
      write_graph ("AD edge before elimination", lg);
      line_graph_t::const_evn_t                 evn= get(vertex_name, lg);
      write_vertex_property (cout, "vertices of this edge graph", evn, lg);
      cout << "overall_markowitz_degree is " << old_overall_markowitz << "\n"; 
      cout << "elimination of face: ";
      write_face_op_t wf (gtmp); wf (cout, ftmp); 
      cout << endl;
      write_graph ("AD edge after elimination", gtmp);
      line_graph_t::evn_t                 evntmp= get(vertex_name, gtmp);
      write_vertex_property (cout, "vertices of this edge graph", evntmp, lg);
      cout << "overall_markowitz_degree is " << new_overall_markowitz 
	   << " momr is " << momr << "\n"; 
      line_graph_t         gtmp2 (lg);
      ftmp= *same_edge (f, lg, gtmp2);
      face_elimination (ftmp, gtmp2);
    }
    // assert (overall_markowitz_degree (lg) - overall_markowitz_degree (gtmp) == momr);
    THROW_DEBUG_EXCEPT_MACRO (overall_markowitz_degree (lg) - overall_markowitz_degree (gtmp) != momr, 
			   consistency_exception, "momr not correct"); 
#endif
    return momr; }
};

int momr_face_t::operator() (const vector<line_graph_t::face_t>& fv1,
			     const line_graph_t& lg,
			     vector<line_graph_t::face_t>& fv2) {
  momrf_op_t   momrf_op;
  return standard_heuristic_op (fv1, lg, fv2, momrf_op, *this);
}

momr_face_t momr_face;

// -------------------------------------------------------------------------
// Minimal distance 
// -------------------------------------------------------------------------

// operator for minimal distances on faces
struct distf_op_t {
  int operator() (line_graph_t::face_t f, const line_graph_t& lg) {
    int i, j, k; face_vertex_name (f, lg, i, j, k);
    return k - i; }
};

int minimal_distance_face_t::operator() (const vector<line_graph_t::face_t>& fv1,
					 const line_graph_t& lg, vector<line_graph_t::face_t>& fv2) {
  return standard_heuristic_op (fv1, lg, fv2, distf_op_t(), *this);
}

#ifdef USEXAIFBOOSTER

// -------------------------------------------------------------------------
// Scarcity preserving edge eliminations
// -------------------------------------------------------------------------
int edge_elim_effect (const edge_bool_t be,
		      const c_graph_t& angelLCG,
		      const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel) {
  
  boost::property_map<c_graph_t, EdgeType>::const_type eType = get(EdgeType(), angelLCG);
  c_graph_t::oei_t oei, oe_end;
  c_graph_t::iei_t iei, ie_end;
  c_graph_t::edge_t absorb_e;
  bool found_absorb;

  c_graph_t::edge_t e = be.first;
  bool isFront = be.second;
  int nontrivialEdgeChange = 0;

  // No awareness: removal of e decreases edge count
  if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) nontrivialEdgeChange--;
  // unit awareness: e must be nonunit for its removal to decrease nontrivial edges
  else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[e] != UNIT_EDGE) nontrivialEdgeChange--;
  // constant awareness: e must be variable for its removal to decrease nontrivial edges
  else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[e] == VARIABLE_EDGE) nontrivialEdgeChange--;

  if (isFront) { // front-elimination
    // if tgt(e) is isolated by the elimination
    if (in_degree (target (e, angelLCG), angelLCG) == 1) {
      for (tie (oei, oe_end) = out_edges (target (e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	// all the outedges of tgt(e) will go away.  we need to see how this affects nontrivial edge count
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) nontrivialEdgeChange--;
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*oei] != UNIT_EDGE) nontrivialEdgeChange--;
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*oei] == VARIABLE_EDGE) nontrivialEdgeChange--;
      } // end all outedges of tgt(e)
    }

    // determine effect of absorption/fill-in
    for (tie (oei, oe_end) = out_edges (target (e, angelLCG), angelLCG); oei != oe_end; ++oei) {
      tie (absorb_e, found_absorb) = edge (source (e, angelLCG), target (*oei, angelLCG), angelLCG);
      if (found_absorb) { // absorption
	//no awareness: no increase in edge count
	//unit awareness: absorb_e will be nonunit afterwards.  increase only if absorb_e was previously unit 
	if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE) nontrivialEdgeChange++;
	// constant awareness: increase if absorb edge is nonvariable and either e or *oei is variable
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	  if (eType[e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE) nontrivialEdgeChange++;
      }
      else { // fill-in
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) nontrivialEdgeChange++;
	// unit awareness: if either is nonunit, the fill-in is nonunit
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[e] != UNIT_EDGE || eType[*oei] != UNIT_EDGE)) nontrivialEdgeChange++;
	// constant awareness: if either is variable, the fill-in is variable
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[e] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)) nontrivialEdgeChange++;
      }
    } // end all successors of tgt(e)

  } // end front-elimination
  else { // back-elimination

    // consider in-edges lost due to isolating src(e) (requires src(e) is not dependent)
    if (out_degree (source (e, angelLCG), angelLCG) == 1 && vertex_type (source (e, angelLCG), angelLCG) != dependent) {
      for (tie (iei, ie_end) = in_edges (source (e, angelLCG), angelLCG); iei != ie_end; ++iei) {
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) nontrivialEdgeChange--;
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*iei] != UNIT_EDGE) nontrivialEdgeChange--;
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*iei] == VARIABLE_EDGE) nontrivialEdgeChange--;
      } // end all inedges of src(e)
    }
	      
    // determine effect of absorption/fill-in
    for (tie (iei, ie_end) = in_edges (source (e, angelLCG), angelLCG); iei != ie_end; ++iei) {
      tie (absorb_e, found_absorb) = edge (source (*iei, angelLCG), target (e, angelLCG), angelLCG);
      if (found_absorb) { // absorption
	//no awareness: no increase in edge count
	//unit awareness: absorb_e will be nonunit afterwards.  increase only if absorb_e was previously unit
	if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE) nontrivialEdgeChange++;
	// constant awareness: increase if absorb edge is nonvariable and either e or *oei is variable
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	  if (eType[e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE) nontrivialEdgeChange++;
      }
      else { // fill-in
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS) nontrivialEdgeChange++;
	 // unit awareness: if either is nonunit, the fill-in is nonunit
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[e] != UNIT_EDGE || eType[*iei] != UNIT_EDGE)) nontrivialEdgeChange++;
	// constant awareness: if either is variable, the fill-in is variable
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[e] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)) nontrivialEdgeChange++;
      }
    } // end all predecessors of src(e)
  } // end back-elimination

  return nontrivialEdgeChange;
}

int edge_elim_effect(const EdgeElim ee,
		     const c_graph_t& angelLCG,
		     const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel) {
  return edge_elim_effect(make_pair(ee.getE(angelLCG), ee.isFront()), angelLCG, ourAwarenessLevel);
} // end edge_elim_effect()

bool maintaining_edge_eliminations(const vector<EdgeElim>& bev1,
                                   const c_graph_t& angelLCG,
                                   const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                   vector<EdgeElim>& bev2) {
  bev2.clear();
  if (bev1.empty()) return 0;

  for (size_t c = 0; c < bev1.size(); c++)
    if (edge_elim_effect (bev1[c], angelLCG, ourAwarenessLevel) <= 0)
      bev2.push_back (bev1[c]);

#ifndef NDEBUG
  cout << "	Of " << bev1.size() << " edge eliminations passed to maintaining_edge_eliminations(), " << bev2.size() << " maintain the nontrivial edge count" << endl;
#endif

  if (bev2.empty()) {
    bev2 = bev1;
    return false;
  }
  else return true;
} // end count_maintain_edge_eliminations()

bool reducing_edge_eliminations(const vector<EdgeElim>& bev1,
                                const c_graph_t& angelLCG,
                                const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                vector<EdgeElim>& bev2) {
  bev2.clear();
  if (bev1.empty()) return 0;

  for (size_t c = 0; c < bev1.size(); c++)
    if (edge_elim_effect (bev1[c], angelLCG, ourAwarenessLevel) < 0)
      bev2.push_back (bev1[c]);

  #ifndef NDEBUG
  cout << "	Of " << bev1.size() << " edge eliminations passed to reducing_edge_eliminations()"
       << ", " << bev2.size() << " reduce the nontrivial edge count" << endl;
  #endif

  if(bev2.empty()) {
    bev2 = bev1;
    return false;
  }
  else return true;
} // end count_maintain_edge_eliminations()

bool refill_avoiding_edge_eliminations(const vector<EdgeElim>& bev1,
                                       c_graph_t& angelLCG,
                                       const refillDependenceMap_t refillDependences,
                                       vector<EdgeElim>& bev2) {
  bev2.clear();
  if (bev1.empty())
    return 0;

  c_graph_t::iei_t iei, ie_end;
  c_graph_t::oei_t oei, oe_end;
  refillDependenceMap_t::const_iterator depMap_i;
  vertex_set_t::const_iterator vDepSet_i;
  property_map<pure_c_graph_t, VertexVisited>::type visited = get(VertexVisited(), angelLCG);
  c_graph_t::vi_t cleari, clear_end;

  for (size_t c = 0; c < bev1.size(); c++) {
    c_graph_t::edge_t e = bev1[c].getE(angelLCG); // the direction of elimination (front/back) doesn't matter

    depMap_i = refillDependences.find(make_pair(source (e, angelLCG), target(e, angelLCG)));
    if (depMap_i != refillDependences.end()) { // we have refill dependences to consider for e
#ifndef NDEBUG
      cout << "edge " << e << " has some refill dependences. Checking them..." << endl;
#endif
      vertex_set_t vDepSet = depMap_i->second; // extract the vertex dependence set for e

      // check all vertices that this edge depends on
      // the dependence vertex can't be both below tgt(e) and above src(e)
      for (vDepSet_i = vDepSet.begin(); vDepSet_i != vDepSet.end(); vDepSet_i++) {
	// clear visited property for all vertices
	for (tie(cleari, clear_end) = vertices(angelLCG); cleari != clear_end; ++cleari) visited[*cleari] = false;
	if (reachable (source (e, angelLCG), *vDepSet_i, angelLCG)) {
	  // clear visited property for all vertices (again)
	  for (tie(cleari, clear_end) = vertices(angelLCG); cleari != clear_end; ++cleari) visited[*cleari] = false;
	  if (reachable (*vDepSet_i, target (e, angelLCG), angelLCG)) {
#ifndef NDEBUG
	    cout << "edge " << e << " has an unmet refill dependence on vertex " << *vDepSet_i << endl;
#endif
	    break;
	  } // end if vertex dependency is not met
	}
      } // end all vertex dependences

      // all vertex dependences for this edge have been met
      if (vDepSet_i == vDepSet.end())
        bev2.push_back(bev1[c]);
    } // end if vertex dependences exist
    else bev2.push_back(bev1[c]);

  } // end iterate over bev1

#ifndef NDEBUG
  cout << "	Of " << bev1.size() << " edge eliminations passed to refill_avoiding_edge_eliminations(), " << bev2.size() << " don't violate refill dependences" << endl;
#endif

  if (bev2.empty()) {
    bev2 = bev1;
    return false;
  }
  else return true;
} // end refill_avoiding_edge_eliminations()

bool rerouting_considerate_edge_eliminations(const vector<EdgeElim>& bev,
                                             const c_graph_t& angelLCG,
                                             const std::vector<Transformation>& transformationsPerformedV,
                                             vector<EdgeElim>& reroutingConsiderateEdgeElimsV) {
  reroutingConsiderateEdgeElimsV.clear();
  if (bev.empty())
    return false;

  // Check every for every possible edge elimination
  for (size_t i = 0; i < bev.size(); i++) {
    size_t j;

    if (bev[i].isFront()) { // front-elimination
      // check all previously performed reroutings to see if the edge elim undoes it
      for (j = 0; j < transformationsPerformedV.size(); j++) {
	if (transformationsPerformedV[j].isRerouting()
         && transformationsPerformedV[j].getRerouting().isPre()
	 && source(transformationsPerformedV[j].getRerouting().getE(angelLCG), angelLCG) == bev[i].getSource()
	 && source(transformationsPerformedV[j].getRerouting().getPivotE(angelLCG), angelLCG) == bev[i].getTarget())
	    break;
      }
    } // end front-elimination

    else { // back-elimination
      for (j = 0; j < transformationsPerformedV.size(); j++) {
	if (transformationsPerformedV[j].isRerouting()
         && !transformationsPerformedV[j].getRerouting().isPre()
	 && target(transformationsPerformedV[j].getRerouting().getE(angelLCG), angelLCG) == bev[i].getTarget()
	 && target(transformationsPerformedV[j].getRerouting().getPivotE(angelLCG), angelLCG) == bev[i].getSource())
	    break;
      } // end all transformations
    } // end back-elimination

    if (j == transformationsPerformedV.size())
      reroutingConsiderateEdgeElimsV.push_back(bev[i]);
  } // end iterate over bev

#ifndef NDEBUG
  cout << "	Of " << bev.size() << " edge eliminations passed to rerouting_considerate_edge_eliminations(), " << reroutingConsiderateEdgeElimsV.size() << " don't undo a rerouting" << endl;
#endif

  if (reroutingConsiderateEdgeElimsV.empty()) {
    reroutingConsiderateEdgeElimsV = bev;
    return false;
  }
  else return true;
} // end rerouting_considerate_edge_eliminations()

unsigned int lowestMarkowitzEdgeElim(const vector<EdgeElim>& inEEV,
                                     const c_graph_t& angelLCG,
                                     vector<EdgeElim>& outEEV) {
  outEEV.clear();
  if (inEEV.empty())
    return 0;

  vector<edge_bool_t> inBEV, outBEV;

  for (size_t i = 0; i < inEEV.size(); i++)
    inBEV.push_back(inEEV[i].getBool(angelLCG));

  lowest_markowitz_edge(inBEV, angelLCG, outBEV);

  for (size_t i = 0; i < outBEV.size(); i++)
    outEEV.push_back(EdgeElim (outBEV[i], angelLCG));

  return outEEV.size();
} // end lowestMarkowitzEdgeElim()

bool reverseModeEdgeElim(const vector<EdgeElim>& inEEV,
                         const c_graph_t& angelLCG,
                         vector<EdgeElim>& outEEV) {
  outEEV.clear();
  if (inEEV.empty())
    return false;

  vector<edge_bool_t> inBEV, outBEV;

  for (size_t i = 0; i < inEEV.size(); i++)
    inBEV.push_back(inEEV[i].getBool(angelLCG));

  reverse_mode_edge(inBEV, angelLCG, outBEV);

  for (size_t i = 0; i < outBEV.size(); i++)
    outEEV.push_back(EdgeElim (outBEV[i], angelLCG));

  return true;
} // end reverseModeEdgeElim()

// ==============================================================================
// |                    FILTERS FOR REROUTINGS                                  |
// ==============================================================================

size_t noncyclicReroutings (const vector<Rerouting>& erv,
			    const std::vector<Transformation>& transformationsPerformedV,
			    const c_graph_t& angelLCG,
			    vector<Rerouting>& noncyclicReroutingsV) {
  noncyclicReroutingsV.clear();
  if (erv.empty())
    return 0;
  size_t j;
  // check each rerouting in erv to see whether it has already been performed
  for (size_t i = 0; i < erv.size(); i++) {
    // go through the history 
    for (j = 0; j < transformationsPerformedV.size(); j++)
      if (transformationsPerformedV[j].isRerouting()
       && transformationsPerformedV[j].getRerouting() == erv[i]);
	  break;
    if (j == transformationsPerformedV.size()) // if it made it all the way through, the rerouting hasn't already been performed
      noncyclicReroutingsV.push_back(erv[i]);
  } // end iterate over erv
  return noncyclicReroutingsV.size();
} // end noncyclicReroutings()

/*
bool maintaining_reroutings (const vector<edge_reroute_t>& erv,
			     const c_graph_t& angelLCG,
			     const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			     vector<edge_reroute_t>& maintainingReroutingsV) {
  maintainingReroutingsV.clear();
  if (erv.empty()) return 0;

  bool dummyBool;

  for (size_t i = 0; i < erv.size(); i++) {
    cout << "reroute_effect = " << reroute_effect(erv[i], angelLCG, ourAwarenessLevel, dummyBool) << endl;
    if (reroute_effect(erv[i], angelLCG, ourAwarenessLevel, dummyBool) <= 0)
      maintainingReroutingsV.push_back(erv[i]);
  }
  cout << "Of " << erv.size() << " reroutings passed to maintaining_reroutings(), " << maintainingReroutingsV.size() << " maintain the nontrivial edge count" << endl;

  if (maintainingReroutingsV.empty()) {
    maintainingReroutingsV = erv;
    return false;
  }
  else return true;
} // end maintaining_reroutings()
*/

/*
bool reducing_reroutings (const vector<edge_reroute_t>& erv,
			  const c_graph_t& angelLCG,
			  const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			  vector<edge_reroute_t>& reducingReroutingsV) {
  reducingReroutingsV.clear();
  if (erv.empty()) return 0;
  size_t i;

  vector<edge_reroute_t> reducingReroutingstypes12V, reducingReroutingstype3V;

  if (edge_reducing_rerouteElims_types12 (erv, angelLCG, ourAwarenessLevel, false, reducingReroutingstypes12V))
    for (i = 0; i < reducingReroutingstypes12V.size(); i++)
      reducingReroutingsV.push_back(reducingReroutingstypes12V[i]);

  if (edge_reducing_rerouteElims_type3 (erv, angelLCG, ourAwarenessLevel, false, reducingReroutingstype3V))
    for (i = 0; i < reducingReroutingstype3V.size(); i++)
      reducingReroutingsV.push_back(reducingReroutingstype3V[i]);
#ifndef NDEBUG
  cout << "	Of " << erv.size() << " reroutings passed to reducing_reroutings(), " << reducingReroutingsV.size() << " reduce the nontrivial edge count when followed by elimination" << endl;
#endif
  if (reducingReroutingsV.empty()) {
    //reducingReroutingsV = erv;
    return false;
  }
  else return true;
} // end reducing_reroutings()
*/

bool reducing_reroutings (const vector<Rerouting>& erv,
			  const c_graph_t& angelLCG,
			  const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			  vector<Rerouting>& reducingReroutingsV) {
  reducingReroutingsV.clear();
  if (erv.empty())
    return 0;

  boost::property_map<c_graph_t, EdgeType>::const_type eType = get(EdgeType(), angelLCG);
  c_graph_t::iei_t iei, ie_end;
  c_graph_t::oei_t oei, oe_end;
  c_graph_t::edge_t absorb_e, increment_absorb_e, decrement_absorb_e;
  bool found_absorb_e;

  for (size_t i = 0; i < erv.size(); i++) {
    edge_reroute_t er = erv[i].getER(angelLCG);
    // first record effect of the rerouting itself
    bool incrementIsTrivial;
    int nontrivialEdgeChange_rerouting = reroute_effect (er, angelLCG, ourAwarenessLevel, incrementIsTrivial);

    c_graph_t::edge_t e = er.e;
    c_graph_t::edge_t pe = er.pivot_e;
    er.pivot_eliminatable = false;
    er.increment_eliminatable = false;
    er.type3EdgeElimVector.clear();

    int nontrivialEdgeChange_elimIncrement = 0;
    int nontrivialEdgeChange_elimPivot = 0;

    if (er.isPre) { // pre-routing
      //---------------------------------------------------------------------------------------------------------------------------
      // determine effect of back-eliminating the increment edge on the nontrivial edge count (nontrivialEdgeChange_elimIncrement)
      //---------------------------------------------------------------------------------------------------------------------------

      // cannot back-eliminate the increment edge if src(e) is an independent
      if (in_degree (source (e, angelLCG), angelLCG) > 0) {
	// determine effect of removing the increment edge
	if (!incrementIsTrivial) nontrivialEdgeChange_elimIncrement--;

	// examine effect of back-eliminating increment edge
	for (tie(iei, ie_end) = in_edges(source(e, angelLCG), angelLCG); iei != ie_end; ++iei) {
	  tie(absorb_e, found_absorb_e) = edge(source(*iei, angelLCG), source(pe, angelLCG), angelLCG);
	  if (found_absorb_e) { // absorption: count when the absorb_e goes from trivial to nontrivial
	    if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)		nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	      if (eType[*iei] == VARIABLE_EDGE || !incrementIsTrivial)						nontrivialEdgeChange_elimIncrement++;
	  } // end absorption
	  else { // fill-in: is the fill-in trivial or not?
	    if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)										nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[*iei] != UNIT_EDGE || !incrementIsTrivial))		nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[*iei] == VARIABLE_EDGE || !incrementIsTrivial))	nontrivialEdgeChange_elimIncrement++;
	  } // end fill-in
        } // end all preds of src(e)
	if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_elimIncrement < 0) er.increment_eliminatable = true;
      } // end if increment edge can be back-eliminated
      
      //------------------------------------------------------------------------------------------------------------------------------------------------
      // determine effect of front-eliminating the pivot edge on the nontrivial edge count (nontrivialEdgeChange_elimPivot)
      //------------------------------------------------------------------------------------------------------------------------------------------------

      // front-elimination of pivot edge MUST isolate the target
      if (in_degree(target(pe, angelLCG), angelLCG) == 2 && vertex_type(target(pe, angelLCG), angelLCG) != dependent) {

	// determine effect of eliminating the pivot edge
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)						nontrivialEdgeChange_elimPivot--;
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[pe] != UNIT_EDGE)		nontrivialEdgeChange_elimPivot--;
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[pe] == VARIABLE_EDGE)	nontrivialEdgeChange_elimPivot--;

	// iterate over successors of tgt(pe); the fill/absorb edges will have the same source as the pivot edge
	for (tie (oei, oe_end) = out_edges(target(pe, angelLCG), angelLCG); oei != oe_end; ++oei) {
	  // determine effect of removing *oei
	  if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)							nontrivialEdgeChange_elimPivot--;
	  else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*oei] != UNIT_EDGE)		nontrivialEdgeChange_elimPivot--;
	  else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*oei] == VARIABLE_EDGE)	nontrivialEdgeChange_elimPivot--;

	  tie (absorb_e, found_absorb_e) = edge (source(pe, angelLCG), target(*oei, angelLCG), angelLCG);
	  if (found_absorb_e) { // absorption: we need to detect of it goes from trivial to nontrivial
	    if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)		nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	      if (eType[pe] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE)					nontrivialEdgeChange_elimPivot++;
	  } // end absorption case
	  else { // fill-in
	    if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)											nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[pe] != UNIT_EDGE || eType[*oei] != UNIT_EDGE))			nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[pe] == VARIABLE_EDGE || eType[*oei] == VARIABLE_EDGE))	nontrivialEdgeChange_elimPivot++;
	  } // end fill-in case

	} // end all successors of tgt(e)=tgt(pe)
	if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_elimPivot < 0) er.pivot_eliminatable = true;
      } // end determine nontrivialEdgeChange_elimPivot

      //------------------------------------------------------------------------------------------------------------------------------------------------
      // determine effect of back-eliminating (type 3) 
      //------------------------------------------------------------------------------------------------------------------------------------------------

      // iterate over outedges of tgt(e), consider back-elimination of *oei
      for (tie(oei, oe_end) = out_edges(target(e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	int nontrivialEdgeChange_backElimination = 0;

	// consider loss of *oei
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS
	|| (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*oei] != UNIT_EDGE)
	|| (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*oei] == VARIABLE_EDGE)) nontrivialEdgeChange_backElimination--;

	// consider fill/absorb effect of back-eliminating *oei
	for (tie(iei, ie_end) = in_edges(target(e, angelLCG), angelLCG); iei != ie_end; ++iei) {
	  if (*iei == e) continue; // skip the rerouted edge
	  tie(absorb_e, found_absorb_e) = edge(source(*iei, angelLCG), target(*oei, angelLCG), angelLCG);
	  if (found_absorb_e) { // absorption: only counts if it goes from trivial to nontrivial
	    if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)		nontrivialEdgeChange_backElimination++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	      if (eType[*oei] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)					nontrivialEdgeChange_backElimination++;
	  }
	  else { // fill-in
	    if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)											nontrivialEdgeChange_backElimination++;
	    else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[*oei] != UNIT_EDGE || eType[*iei] != UNIT_EDGE))		nontrivialEdgeChange_backElimination++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[*oei] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE))	nontrivialEdgeChange_backElimination++;
	  }
	} // end all inedges of tgt(e)
	if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_backElimination < 0) er.type3EdgeElimVector.push_back(target(*oei, angelLCG));
      } // end all outedges of tgt(e) (end type 3)

    } // end pre-routing
    else { // post-routing

      //------------------------------------------------------------------------------------------------------------------------------------------------
      // determine effect of front-eliminating the increment edge on the nontrivial edge count
      //------------------------------------------------------------------------------------------------------------------------------------------------

      // cannot front-eliminate the increment edge if tgt(e) is a dependent
      if (vertex_type(target(e, angelLCG), angelLCG) != dependent)  {
	// determine effect of removing the increment edge
	if (!incrementIsTrivial) nontrivialEdgeChange_elimIncrement--;

	// examine effect of front-eliminating increment edge
	for (tie (oei, oe_end) = out_edges(target(e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	  tie (absorb_e, found_absorb_e) = edge(target(pe, angelLCG), target(*oei, angelLCG), angelLCG);
	  if (found_absorb_e) { // absorption: count when the absorb_e goes from trivial to nontrivial
	    if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)		nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	      if (eType[*oei] == VARIABLE_EDGE || !incrementIsTrivial)						nontrivialEdgeChange_elimIncrement++;
	  } // end absorption
	  else { // fill-in: is the fill-in trivial or not?
	    if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)										nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[*oei] != UNIT_EDGE || !incrementIsTrivial))		nontrivialEdgeChange_elimIncrement++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[*oei] == VARIABLE_EDGE || !incrementIsTrivial))	nontrivialEdgeChange_elimIncrement++;
	  } // end fill-in
        } // end all preds of src(e)
	if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_elimIncrement < 0) er.increment_eliminatable = true;
      } // end if increment edge can be front-eliminated

      //------------------------------------------------------------------------------------------------------------------------------------------------
      // determine effect of back-eliminating the pivot edge on the nontrivial edge count
      //------------------------------------------------------------------------------------------------------------------------------------------------

      // front-elimination of pivot edge MUST isolate the target
      if (out_degree (source(pe, angelLCG), angelLCG) == 2 && in_degree (source(pe, angelLCG), angelLCG) > 0) {

	// determine effect of eliminating the pivot edge
	if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)						nontrivialEdgeChange_elimPivot--;
	else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[pe] != UNIT_EDGE)		nontrivialEdgeChange_elimPivot--;
	else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[pe] == VARIABLE_EDGE)	nontrivialEdgeChange_elimPivot--;

	// iterate over predecessors of src(pe)
	// the fill/absorb edges will have the same target as the pivot edge
	for (tie (iei, ie_end) = in_edges(source(pe, angelLCG), angelLCG); iei != ie_end; ++iei) {
	  // determine effect of removing the outedge
	  if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)							nontrivialEdgeChange_elimPivot--;
	  else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*iei] != UNIT_EDGE)		nontrivialEdgeChange_elimPivot--;
	  else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*iei] == VARIABLE_EDGE)	nontrivialEdgeChange_elimPivot--;

	  tie (absorb_e, found_absorb_e) = edge (source(*iei, angelLCG), target(pe, angelLCG), angelLCG);
	  if (found_absorb_e) { // absorption: we need to detect of it goes from trivial to nontrivial
	    if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)		nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
	      if (eType[pe] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)					nontrivialEdgeChange_elimPivot++;
	  } // end absorption case
	  else { // fill-in
	    if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)											nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[pe] != UNIT_EDGE || eType[*iei] != UNIT_EDGE))			nontrivialEdgeChange_elimPivot++;
	    else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[pe] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE))	nontrivialEdgeChange_elimPivot++;
	  } // end fill-in case

	} // end all successors of tgt(e)=tgt(pe)
	if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_elimPivot < 0) er.pivot_eliminatable = true;
      } // end determine nontrivialEdgeChange_elimPivot

      //------------------------------------------------------------------------------------------------------------------------------------------------
      // determine effect of front-eliminating (type 3) 
      //------------------------------------------------------------------------------------------------------------------------------------------------

      // iterate over inedges of src(e), consider front-elimination of *iei
      if (vertex_type(source(e, angelLCG), angelLCG) != dependent) {
	for (tie(iei, ie_end) = in_edges(source(e, angelLCG), angelLCG); iei != ie_end; ++iei) {
	  int nontrivialEdgeChange_frontElimination = 0;

	  // consider loss of *iei
	  if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS
	  || (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[*iei] != UNIT_EDGE)
	  || (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[*iei] == VARIABLE_EDGE)) nontrivialEdgeChange_frontElimination--;

	  // consider fill/absorb effect of front-eliminating *iei
	  for (tie(oei, oe_end) = out_edges(source(e, angelLCG), angelLCG); oei != oe_end; ++oei) {
	    if (*oei == e) continue; // skip the rerouted edge
	    tie(absorb_e, found_absorb_e) = edge(source(*iei, angelLCG), target(*oei, angelLCG), angelLCG);
	    if (found_absorb_e) { // absorption: only counts if it goes from trivial to nontrivial
	      if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && eType[absorb_e] == UNIT_EDGE)			nontrivialEdgeChange_frontElimination++;
	      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && eType[absorb_e] != VARIABLE_EDGE)
		if (eType[*oei] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE)					nontrivialEdgeChange_frontElimination++;
	    }
	    else { // fill-in
	      if (ourAwarenessLevel == AwarenessLevel::NO_AWARENESS)										nontrivialEdgeChange_frontElimination++;
	      else if (ourAwarenessLevel == AwarenessLevel::UNIT_AWARENESS && (eType[*oei] != UNIT_EDGE || eType[*iei] != UNIT_EDGE))		nontrivialEdgeChange_frontElimination++;
	      else if (ourAwarenessLevel == AwarenessLevel::CONSTANT_AWARENESS && (eType[*oei] == VARIABLE_EDGE || eType[*iei] == VARIABLE_EDGE))	nontrivialEdgeChange_frontElimination++;
	    } // end fill-in
	  } // end all outedges of src(e)
	  if (nontrivialEdgeChange_rerouting + nontrivialEdgeChange_frontElimination < 0) er.type3EdgeElimVector.push_back(source(*iei, angelLCG));
	} // end all inedges of src(e)
      } // end type 3
    } // end post-routing

    if (er.pivot_eliminatable || er.increment_eliminatable || !er.type3EdgeElimVector.empty())
      reducingReroutingsV.push_back(Rerouting (er, angelLCG));

  } // end iterate through erv

#ifndef NDEBUG
  cout << "	Of " << erv.size() << " reroutings passed to reducing_reroutings(), " << reducingReroutingsV.size() << " reduce the nontrivial edge count when followed by elimination" << endl;
#endif

  return !reducingReroutingsV.empty();
} // end reducing_reroutings()

// ==============================================================================
// |            FILTERS FOR ELIMINATIONS AND REROUTINGS (TRANSFORMATIONS)       |
// ==============================================================================

int transformation_effect(const Transformation t,
			  const c_graph_t& angelLCG,
			  const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel) {
  int effect;
  if (t.isRerouting()) {
    bool dummy_incrementIsTrivial;
    effect = reroute_effect(t.getRerouting().getER(angelLCG), angelLCG, ourAwarenessLevel, dummy_incrementIsTrivial);
  }
  else
    effect = edge_elim_effect(t.getEdgeElim(), angelLCG, ourAwarenessLevel);
  return effect;
} // end transformation_effect()

bool all_viable_transformations (c_graph_t& angelLCG,
				 const std::vector<Transformation>& transformationsPerformedV,
				 vector<Transformation>& allViableTransformationsV) {
  allViableTransformationsV.clear();
  // find all eliminatable edges
  vector<EdgeElim> allEdgeElimsV;
  eliminatable_edges(angelLCG, allEdgeElimsV);
  for (size_t i = 0; i < allEdgeElimsV.size(); i++)
    allViableTransformationsV.push_back(Transformation (allEdgeElimsV[i]));
  // find viable reroutings
  vector<Rerouting> allReroutingsV, noncyclicReroutingsV;
  reroutable_edges (angelLCG, allReroutingsV);
  noncyclicReroutings (allReroutingsV, transformationsPerformedV, angelLCG, noncyclicReroutingsV);
  for (size_t i = 0; i < noncyclicReroutingsV.size(); i++)
    allViableTransformationsV.push_back(Transformation (noncyclicReroutingsV[i]));
  #ifndef NDEBUG
  cout << "\tThere are " << allEdgeElimsV.size() << " viable Edge eliminations in G" << endl;
  cout << "\tOf " << allReroutingsV.size() << " possible reroutings, " << noncyclicReroutingsV.size() << " are noncyclic" << endl;
  cout << "\t\tThere are " << allViableTransformationsV.size() << " viable transformations in G" << endl;
  #endif
  return !allViableTransformationsV.empty();
} // end all_viable_transformations()

bool maintaining_transformations(const vector<Transformation>& tv,
				 const c_graph_t& angelLCG,
				 const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
				 vector<Transformation>& maintainingTransformationsV) {
  maintainingTransformationsV.clear();
  if (tv.empty())
    return false;

  vector<Rerouting> tempReroutingsV;
  vector<EdgeElim> tempEdgeElimsV, tempMaintainingEdgeElimsV;

  // create temporary lists
  for (size_t i = 0; i < tv.size(); i++)
    tv[i].isRerouting() ? tempReroutingsV.push_back(tv[i].getRerouting())
                        : tempEdgeElimsV.push_back(tv[i].getEdgeElim());

  // if there are edge elims, push them to the transformation vector
  if (maintaining_edge_eliminations(tempEdgeElimsV, angelLCG, ourAwarenessLevel, tempMaintainingEdgeElimsV)) 
    for (size_t i = 0; i < tempMaintainingEdgeElimsV.size(); i++)
      maintainingTransformationsV.push_back(Transformation (tempMaintainingEdgeElimsV[i]));

  // push all reroutings to the transformation vector
  for (size_t i = 0; i < tempReroutingsV.size(); i++)
    maintainingTransformationsV.push_back(Transformation (tempReroutingsV[i]));

  #ifndef NDEBUG
  cout << "Of " << tv.size() << " transformations passed to maintaining_transformations()"
       << ", " << maintainingTransformationsV.size() << " maintain the nontrivial edge count" << endl;
  #endif

  // if there are neither edge elims nor reroutings, return the transformation vector we were passed
  if (maintainingTransformationsV.empty()) {
    maintainingTransformationsV = tv;
    return false;
  }
  else return true;
} // end count_maintaining_transformations()

bool reducing_transformations(const vector<Transformation>& tv,
			      const c_graph_t& angelLCG,
			      const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			      vector<Transformation>& reducingTransformationsV) {
  reducingTransformationsV.clear();
  if (tv.empty())
    return 0;

  vector<Rerouting> tempReroutingsV, tempReducingReroutingsV;
  vector<EdgeElim> tempEdgeElimsV, tempReducingEdgeElimsV;

  // split tv into separate temporary lists
  for (size_t i = 0; i < tv.size(); i++)
    tv[i].isRerouting() ? tempReroutingsV.push_back(tv[i].getRerouting())
                        : tempEdgeElimsV.push_back(tv[i].getEdgeElim());

  // if there are edge elims, push them to the transformation vector
  if (reducing_edge_eliminations(tempEdgeElimsV, angelLCG, ourAwarenessLevel, tempReducingEdgeElimsV)) 
    for (size_t i = 0; i < tempReducingEdgeElimsV.size(); i++)
      reducingTransformationsV.push_back(Transformation (tempReducingEdgeElimsV[i]));

  // if there are reroutings, push them to the transformation vector
  if (reducing_reroutings(tempReroutingsV, angelLCG, ourAwarenessLevel, tempReducingReroutingsV))
    for (size_t i = 0; i < tempReducingReroutingsV.size(); i++)
      reducingTransformationsV.push_back(Transformation (tempReducingReroutingsV[i]));

#ifndef NDEBUG
  cout << "Of " << tv.size() << " transformations passed to reducing_transformations(), " << reducingTransformationsV.size() << " reduce the nontrivial edge count" << endl;
#endif

  // if there are neither edge elims nor reroutings, return only the edge elims
  if (reducingTransformationsV.empty()) {
    for (size_t i = 0; i < tempEdgeElimsV.size(); i++) // push back all the edge elims
      reducingTransformationsV.push_back(Transformation (tempEdgeElimsV[i]));
    // if there are no edge elims that maintain or reduce, and no reroutings that reduce: push back all edge elims
    if (reducingTransformationsV.empty()) {
      vector<EdgeElim> allEdgeElimsV;
      eliminatable_edges(angelLCG, allEdgeElimsV);
      for (size_t j = 0; j < allEdgeElimsV.size(); j++)
        reducingTransformationsV.push_back(Transformation (allEdgeElimsV[j]));
    }
    return false;
  }    
/*
  // if there are neither edge elims nor reroutings, return the transformation vector we were passed
  if (reducingTransformationsV.empty()) {
    reducingTransformationsV = tv;
    return false;
  } */
  else return true;

} // end count_reducing_transformations()

bool refill_avoiding_transformations(const vector<Transformation>& tv,
				     c_graph_t& angelLCG,
				     const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
				     const refillDependenceMap_t& refillDependences,
				     vector<Transformation>& refillAvoidingTransformationsV) {
  refillAvoidingTransformationsV.clear();
  if (tv.empty())
    return false;

  vector<Rerouting> tempReroutingsV;
  vector<EdgeElim> tempEdgeElimsV, tempRefillAvoidingEdgeElimsV;

  // create temporary edge elim and rerouting vectors
  for (size_t i = 0; i < tv.size(); i++)
    tv[i].isRerouting() ? tempReroutingsV.push_back(tv[i].getRerouting())
                        : tempEdgeElimsV.push_back(tv[i].getEdgeElim());

  // run edge elims through the refill filter
  if (refill_avoiding_edge_eliminations(tempEdgeElimsV, angelLCG, refillDependences, tempRefillAvoidingEdgeElimsV)) {
    // push refill avoiding edge elims to the transformation vector
    for (size_t i = 0; i < tempRefillAvoidingEdgeElimsV.size(); i++)
      refillAvoidingTransformationsV.push_back(Transformation (tempRefillAvoidingEdgeElimsV[i]));
    // push all reroutings to the transformation vector
    for (size_t i = 0; i < tempReroutingsV.size(); i++)
      refillAvoidingTransformationsV.push_back(Transformation (tempReroutingsV[i]));
    return true;
  } // end if refill avoiders were found

  else { // none of the edge elims avoid refill
    refillAvoidingTransformationsV = tv;
    return false;
  }
} // end refill_avoiding_transformations()

bool rerouting_considerate_transformations(const vector<Transformation>& tv,
					   const c_graph_t& angelLCG,
					   const std::vector<Transformation>& transformationsPerformedV,
					   vector<Transformation>& reroutingConsiderateTransformationsV) {
  reroutingConsiderateTransformationsV.clear();
  if (tv.empty())
    return false;

  vector<Transformation> tempReroutingsV;
  vector<EdgeElim> tempEdgeElimsV, tempReroutingConsiderateEdgeElimsV;

  // create temporary edge elim and rerouting vectors
  for (size_t i = 0; i < tv.size(); i++)
    tv[i].isRerouting() ? tempReroutingsV.push_back(tv[i].getRerouting())
                        : tempEdgeElimsV.push_back(tv[i].getEdgeElim());

  // pass all edge elims through the edge elim filter
  if (rerouting_considerate_edge_eliminations(tempEdgeElimsV, angelLCG, transformationsPerformedV, tempReroutingConsiderateEdgeElimsV)) {
    // add preferred edge elims to the transformations vector to be returned
    for (size_t i = 0; i < tempReroutingConsiderateEdgeElimsV.size(); i++)
      reroutingConsiderateTransformationsV.push_back(Transformation (tempReroutingConsiderateEdgeElimsV[i]));
    // push all reroutings to the transformation vector
    for (size_t i = 0; i < tempReroutingsV.size(); i++)
      reroutingConsiderateTransformationsV.push_back(Transformation (tempReroutingsV[i]));
    return true;
  } // end if rerouting considerate edge elims were found

  else { // none of the edge elims are rerouting considerate
    reroutingConsiderateTransformationsV = tv;
    return false;
  }
} // end rerouting_considerate_transformations ()

bool lowest_markowitz_transformations(const vector<Transformation>& tv,
                                      const c_graph_t& angelLCG,
                                      vector<Transformation>& lowestMarkowitzTransformationsV) {
  lowestMarkowitzTransformationsV.clear();
  if (tv.empty())
    return false;
  // create temporary edge elim vector
  vector<EdgeElim> tempEdgeElimsV, tempLowestMarkowitzEdgeElimsV;
  for (size_t i = 0; i < tv.size(); i++)
    if (!tv[i].isRerouting())
      tempEdgeElimsV.push_back(tv[i].getEdgeElim());
  // if no edge elims, return exactly what we got
  if (tempEdgeElimsV.empty()) {
    lowestMarkowitzTransformationsV = tv;
    return false;
  }
  lowestMarkowitzEdgeElim(tempEdgeElimsV, angelLCG, tempLowestMarkowitzEdgeElimsV);
  // add preferred edge elims to the transformations vector to be returned
  for (size_t i = 0; i < tempLowestMarkowitzEdgeElimsV.size(); i++)
    lowestMarkowitzTransformationsV.push_back(Transformation (tempLowestMarkowitzEdgeElimsV[i]));
  return true;
} // end lowest_markowitz_transformations()

bool reverse_mode_transformations(const vector<Transformation>& tv,
                                  const c_graph_t& angelLCG,
                                  vector<Transformation>& reverseModeTransformationsV) {
  reverseModeTransformationsV.clear();
  if (tv.empty())
    return false;
  // create temporary edge elim vector
  vector<EdgeElim> tempEdgeElimsV, tempReverseModeEdgeElimsV;
  for (size_t i = 0; i < tv.size(); i++)
    if (!tv[i].isRerouting())
      tempEdgeElimsV.push_back(tv[i].getEdgeElim());
  // if no edge elims, return exactly what we got
  if (tempEdgeElimsV.empty()) {
    reverseModeTransformationsV = tv;
    return false;
  }
  reverseModeEdgeElim(tempEdgeElimsV, angelLCG, tempReverseModeEdgeElimsV);
  // add preferred edge elims to the transformations vector to be returned
  for (size_t i = 0; i < tempReverseModeEdgeElimsV.size(); i++)
    reverseModeTransformationsV.push_back(Transformation (tempReverseModeEdgeElimsV[i]));
  return true;
} // end reverse_mode_transformations()

#endif // USEXAIFBOOSTER

} // namespace angel

// to do: some names are confusing, e.g. ev for a face vector
