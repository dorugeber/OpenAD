/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#ifndef 	_heuristics_include_
#define 	_heuristics_include_

#include <vector>

#include "angel_types.hpp"
#include "angel_io.hpp"
#include "eliminations.hpp"

#ifdef USE_MPI
#include "gmpi.hpp"
#include "angel_comm.hpp"
#endif // USE_MPI

#ifdef USEXAIFBOOSTER
#include "reroutings.hpp"
#endif // USEXAIFBOOSTER

namespace angel {

  using std::vector;
  
// =====================================================
// Basic class for heuristics
// =====================================================

template <class Objective_t= int>
class base_heuristic_t {
protected:
  Objective_t   my_objective;
  bool          is_set;           // whether my_object is set
  bool          my_maximize;      // if objective value is maximized
public:
  typedef Objective_t    objective_t;
  base_heuristic_t (bool _m) : is_set (false), my_maximize (_m) {}
  Objective_t objective() const {
    THROW_DEBUG_EXCEPT_MACRO (!is_set, consistency_exception, "objective not set"); return my_objective;}
  void set_objective (Objective_t o) {
    is_set= true; my_objective= o; }
  void set_empty_objective () {
    is_set= true; 
    my_objective= my_maximize ? std::numeric_limits<Objective_t>::min() 
                              : std::numeric_limits<Objective_t>::max(); }
  bool to_maximize() const {return my_maximize;}
};

template <class Object_t, class Ad_graph_t, class Op_t, class Objective_t>
int standard_heuristic_op (const vector<Object_t>& v1, const Ad_graph_t& adg,
			   vector<Object_t>& v2, Op_t op, base_heuristic_t<Objective_t>& h);

// =====================================================
// for vertex elimination
// =====================================================

/** \brief Operator class for forward mode in vertex elimination
    
    Since operator() behaves like forward_mode_vertex objects of this class
    can be called like the function.
    For instance, forward_mode_vertex_t fm; fm (v1, cg, v2); is identical with
    forward_mode_vertex (v1, cg, v2);.
    However, the object fm can be more easyly used in templates.
    \sa forward_mode_vertex 
*/
class forward_mode_vertex_t : public base_heuristic_t<int> {
public:
  forward_mode_vertex_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Forward mode in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Result vector of vertices, contains vertex with lowest number
    \return Size of vv2, always 1 (if vv1 is not empty)
*/
extern forward_mode_vertex_t forward_mode_vertex;

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

/** \brief Operator class for reverse mode in vertex elimination

    operator() is equivalent to reverse_mode_vertex
    \sa reverse_mode_vertex
    \sa forward_mode_vertex_t
*/
class reverse_mode_vertex_t : public base_heuristic_t<int> {
public:
  reverse_mode_vertex_t () : base_heuristic_t<int> (true) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Reverse mode in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Result vector of vertices, contains vertex with highest number
    \return Size of vv2, always 1 (if vv1 is not empty)
*/
extern reverse_mode_vertex_t reverse_mode_vertex;

// -------------------------------------------------------------------------
// Lowest Markowitz
// -------------------------------------------------------------------------

/** \brief Operator class for lowest Markowitz in vertex elimination

    operator() is equivalent to lowest_markowitz_vertex
    \sa lowest_markowitz_vertex
    \sa forward_mode_vertex_t
*/
class lowest_markowitz_vertex_t : public base_heuristic_t<int> {
public:
  lowest_markowitz_vertex_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Lowest Markowitz degree first in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices with lowest Markowitz degree
    \return Size of vv2
*/
extern lowest_markowitz_vertex_t lowest_markowitz_vertex;
  
// -------------------------------------------------------------------------
// Lowest relative Markowitz
// -------------------------------------------------------------------------

/** \brief Operator class for relative lowest Markowitz in vertex elimination

    operator() is equivalent to lowest_relative_markowitz_vertex
    \sa lowest_relative_markowitz_vertex
    \sa forward_mode_vertex_t
*/
class lowest_relative_markowitz_vertex_t : public base_heuristic_t<int> {
public:
  lowest_relative_markowitz_vertex_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Lowest relative Markowitz degree first in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices with lowest relative Markowitz degree
    \return Size of vv2
*/
extern lowest_relative_markowitz_vertex_t lowest_relative_markowitz_vertex;

// -------------------------------------------------------------------------
// Lowest fill-in
// -------------------------------------------------------------------------

/** \brief Operator class for lowest fill-in in vertex elimination

    operator() is equivalent to lowest_fill_in_vertex
    \sa lowest_fill_in_vertex
    \sa forward_mode_vertex_t
*/
class lowest_fill_in_vertex_t : public base_heuristic_t<int> {
public:
  lowest_fill_in_vertex_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Lowest fill-in in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices whose elimination induces miniminal fill-in
    \return Size of vv2
*/
extern lowest_fill_in_vertex_t lowest_fill_in_vertex;

// -------------------------------------------------------------------------
// LMMD
// -------------------------------------------------------------------------

/** \brief Class for lowest Markowitz with minimal damage in vertex elimination

    It searches for vertices with minimal sum of Markowitz degree and weighted damaged.
    The damage of a vertex's elimination is defined as the increase of Markowitz degrees of
    all other vertices.
*/
class lmmd_vertex_t : public base_heuristic_t<int> {
  double weight;
public:
  /// Weight is set in the constructor
  lmmd_vertex_t (double w) : base_heuristic_t<int> (false), weight (w) {}
  /** Heuristic computed in operator
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices with lowest lmmd-value
    \return Size of vv2
  */
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};


/** \brief Predefined object of lmmd_vertex_t with weight=1.0

    This object is introduced for syntactical coherence with other heuristics since
    lmmd_vertex can be called like a function.
*/
extern lmmd_vertex_t lmmd_vertex;

// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

/** \brief Operator class for maximal overall Markowitz degree reduction in vertex elimination
    \sa momr_vertex
    \sa forward_mode_vertex_t
*/
class momr_vertex_t : public base_heuristic_t<int> {
public:
  momr_vertex_t () : base_heuristic_t<int> (true) {}
/** \brief Maximal overall Markowitz degree reduction in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices with maximal overall Markowitz degree reduction
    \return Size of vv2
*/
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/// Instance of momr_vertex_t, can be used as a function and an argument
extern momr_vertex_t momr_vertex;

// -------------------------------------------------------------------------
// Maximal overall path length reduction
// -------------------------------------------------------------------------

/** \brief Operator class for maximal overall path length reduction in vertex elimination

    operator() is equivalent to moplr_vertex
    \sa moplr_vertex
    \sa forward_mode_vertex_t
*/
class moplr_vertex_t : public base_heuristic_t<int> {
public:
  moplr_vertex_t () : base_heuristic_t<int> (true) {}
  int operator() (const vector<c_graph_t::vertex_t>& vv1,
		  const c_graph_t& cg, 
		  vector<c_graph_t::vertex_t>& vv2);
};

/** \brief Maximal overall path length reduction in vertex elimination
    \param vv1 Set of vertices that can be eliminated
    \param cg c-graph
    \param vv2 Set of vertices with maximal overall path length reduction
    \return Size of vv2
*/
extern moplr_vertex_t moplr_vertex;

// =====================================================
// for edge elimination (in c-graph)
// =====================================================

/** \brief Forward mode in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    int forward_mode_edge (const vector<pair<c_graph_t::edge_t,bool> >& ev1,
                           const c_graph_t& cg,
			   vector<pair<c_graph_t::edge_t,bool> >& ev2).
*/
int forward_mode_edge_f (const vector<c_graph_t::edge_t>& ev1,
			 bool front,
			 const c_graph_t& cg,
			 vector<c_graph_t::edge_t>& ev2);

/** \brief Forward mode in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    Edges in ev1 that cannot be front eliminated are ignored.
    It is equivalent to forward mode in vertex and face elimination
    when forward mode is used as sole criterion.
*/
inline int forward_mode_edge_front (const vector<c_graph_t::edge_t>& ev1,
				    const c_graph_t& cg,
				    vector<c_graph_t::edge_t>& ev2) {
  return forward_mode_edge_f (ev1, true, cg, ev2);
}

/** \brief Forward mode in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    Edges in ev1 that cannot be back eliminated are ignored.
    It is NOT equivalent to forward mode in vertex and face elimination.
*/
inline int forward_mode_edge_back (const vector<c_graph_t::edge_t>& ev1,
				   const c_graph_t& cg,
				   vector<c_graph_t::edge_t>& ev2) {
  return forward_mode_edge_f (ev1, false, cg, ev2);
}

/** \brief Operator class for mixed forward edge elimination

    operator() is equivalent to forward_mode_edge (
                 const vector<pair<c_graph_t::edge_t,bool> >& ev1,
			const c_graph_t& cg,
			vector<pair<c_graph_t::edge_t,bool> >& ev2)
    \sa forward_mode_vertex_t
*/
class forward_mode_edge_t : public base_heuristic_t<double> {
public:
  forward_mode_edge_t () : base_heuristic_t<double> (false) {}
  int operator() (const vector<edge_bool_t>& ev1,
		  const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Forward mode in edge elimination (mixed front and back elimination)
    \param ev1 Set of edges that can be eliminated and how
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    If lowest edge appears twice in ev1 than front elimination is used. 
    Mixed edge forward mode is realised such that the same eliminations are effectively done
    as in vertex and face elimination
    when forward mode is used as sole criterion.
*/
extern forward_mode_edge_t forward_mode_edge;

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

/** \brief Reverse mode in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges, contains highest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    int reverse_mode_edge (const vector<pair<c_graph_t::edge_t,bool> >& ev1,
                           const c_graph_t& cg,
			   vector<pair<c_graph_t::edge_t,bool> >& ev2).
*/
int reverse_mode_edge_f (const vector<c_graph_t::edge_t>& ev1,
			 bool front,
			 const c_graph_t& cg,
			 vector<c_graph_t::edge_t>& ev2);

/** \brief Reverse mode in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    Edges in ev1 that cannot be front eliminated are ignored.
*/
inline int reverse_mode_edge_front (const vector<c_graph_t::edge_t>& ev1,
				    const c_graph_t& cg,
				    vector<c_graph_t::edge_t>& ev2) {
  return reverse_mode_edge_f (ev1, true, cg, ev2);
}

/** \brief Reverse mode in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    Edges in ev1 that cannot be back eliminated are ignored.
*/
inline int reverse_mode_edge_back (const vector<c_graph_t::edge_t>& ev1,
				   const c_graph_t& cg,
				   vector<c_graph_t::edge_t>& ev2) {
  return reverse_mode_edge_f (ev1, false, cg, ev2);
}

/** \brief Operator class for mixed reverse edge elimination

    operator() is equivalent to reverse_mode_edge (
                 const vector<pair<c_graph_t::edge_t,bool> >& ev1,
			const c_graph_t& cg,
			vector<pair<c_graph_t::edge_t,bool> >& ev2)
    \sa forward_mode_vertex_t
*/
class reverse_mode_edge_t : public base_heuristic_t<double> {
public:
  reverse_mode_edge_t () : base_heuristic_t<double> (false) {}
  int operator() (const vector<edge_bool_t>& ev1,
		  const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Reverse mode in edge elimination (mixed front and back elimination)
    \param ev1 Set of edges that can be eliminated and how
    \param cg c-graph
    \param ev2 Result vector of edges, contains lowest edge in lexicographical order
    \return Size of ev2, always 1 (if ev1 is not empty)

    If lowest edge appears twice in ev1 than front elimination is used. 
    Mixed edge reverse mode is realised such that the same eliminations are effectively done
    as in vertex and face elimination
    when reverse mode is used as sole criterion.
*/
extern reverse_mode_edge_t reverse_mode_edge;

// -------------------------------------------------------------------------
// Lowest Markowitz
// -------------------------------------------------------------------------

/** \brief Lowest Markowitz in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges with lowest Markowitz degree
    \return Size of ev2

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    int lowest_markowitz_edge (const vector<edge_bool_t>& ev1,
                           const c_graph_t& cg,
			   vector<edge_bool_t>& ev2).
    Note that Markowitz degree of an edge can be different for front and back elimination.
*/
int lowest_markowitz_edge_f (const vector<c_graph_t::edge_t>& ev1,
			     bool front,
			     const c_graph_t& cg,
			     vector<c_graph_t::edge_t>& ev2);


/** \brief Lowest Markowitz in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest Markowitz degree
    \return Size of ev2

    Edges in ev1 that cannot be front eliminated are ignored.
*/
inline int lowest_markowitz_edge_front (const vector<c_graph_t::edge_t>& ev1,
					const c_graph_t& cg,
					vector<c_graph_t::edge_t>& ev2) {
  return lowest_markowitz_edge_f (ev1, true, cg, ev2);
}


/** \brief Lowest Markowitz in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest Markowitz degree
    \return Size of ev2

    Edges in ev1 that cannot be back eliminated are ignored.
*/
inline int lowest_markowitz_edge_back (const vector<c_graph_t::edge_t>& ev1,
				       const c_graph_t& cg,
				       vector<c_graph_t::edge_t>& ev2) {
  return lowest_markowitz_edge_f (ev1, false, cg, ev2);
}

/** \brief Operator class for lowest Markowitz in mixed edge elimination

    operator() is equivalent to lowest_markowitz_edge (
                 const vector<edge_bool_t>& ev1,
			const c_graph_t& cg,
			vector<edge_bool_t>& ev2)
    \sa forward_mode_vertex_t
*/
class lowest_markowitz_edge_t : public base_heuristic_t<int> {
public:
  lowest_markowitz_edge_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<edge_bool_t>& ev1,
		  const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Lowest Markowitz in edge elimination (mixed front and back elimination)
    \param ev1 Set of edges that can be eliminated and how
    \param cg c-graph
    \param ev2 Result vector of edges with lowest Markowitz degree
    \return Size of ev2
*/
extern lowest_markowitz_edge_t lowest_markowitz_edge;

// -------------------------------------------------------------------------
// Lowest relative Markowitz
// -------------------------------------------------------------------------

/** \brief Lowest relative Markowitz in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges with lowest relative Markowitz degree
    \return Size of ev2

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    int lowest_relative_markowitz_edge (const vector<edge_bool_t>& ev1,
                           const c_graph_t& cg,
			   vector<edge_bool_t>& ev2).
    Note that the relative Markowitz degree of an edge can be different for front and back elimination.
*/
int lowest_relative_markowitz_edge_f (const vector<c_graph_t::edge_t>& ev1,
				      bool front,
				      const c_graph_t& cg,
				      vector<c_graph_t::edge_t>& ev2);

/** \brief Lowest relative Markowitz in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest relative Markowitz degree
    \return Size of ev2

    Edges in ev1 that cannot be front eliminated are ignored.
*/
inline int lowest_relative_markowitz_edge_front (const vector<c_graph_t::edge_t>& ev1,
						 const c_graph_t& cg,
						 vector<c_graph_t::edge_t>& ev2) {
  return lowest_relative_markowitz_edge_f (ev1, true, cg, ev2);
}

/** \brief Lowest relative Markowitz in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest relative Markowitz degree
    \return Size of ev2

    Edges in ev1 that cannot be back eliminated are ignored.
*/
inline int lowest_relative_markowitz_edge_back (const vector<c_graph_t::edge_t>& ev1,
						const c_graph_t& cg,
						vector<c_graph_t::edge_t>& ev2) {
  return lowest_relative_markowitz_edge_f (ev1, false, cg, ev2);
}


/** \brief Operator class for lowest relative Markowitz in mixed edge elimination

    operator() is equivalent to lowest_relative_markowitz_edge (
                 const vector<edge_bool_t>& ev1,
			const c_graph_t& cg,
			vector<edge_bool_t>& ev2)
    \sa forward_mode_vertex_t
*/
class lowest_relative_markowitz_edge_t : public base_heuristic_t<int> {
public:
  lowest_relative_markowitz_edge_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<edge_bool_t>& ev1,
		  const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Lowest relative Markowitz in edge elimination (mixed front and back elimination)
    \param ev1 Set of edges that can be eliminated and how
    \param cg c-graph
    \param ev2 Result vector of edges with lowest relative Markowitz degree
    \return Size of ev2
*/
extern lowest_relative_markowitz_edge_t lowest_relative_markowitz_edge;

// -------------------------------------------------------------------------
// Lowest fill-in
// -------------------------------------------------------------------------

/** \brief Lowest Fill-in in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges with lowest fill-in
    \return Size of ev2

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    Note that fill-in by an edge's elimination can be different for front and back elimination.
*/
int lowest_fill_in_edge_f (const vector<c_graph_t::edge_t>& ev1,
			   bool front,
			   const c_graph_t& cg,
			   vector<c_graph_t::edge_t>& ev2);

/** \brief Lowest fill-in in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest fill-in
    \return Size of ev2

    Edges in ev1 that cannot be front eliminated are ignored.
*/
inline int lowest_fill_in_edge_front (const vector<c_graph_t::edge_t>& ev1,
				      const c_graph_t& cg,
				      vector<c_graph_t::edge_t>& ev2) {
  return lowest_fill_in_edge_f (ev1, true, cg, ev2);
}

/** \brief Lowest fill-in in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with lowest fill-in
    \return Size of ev2

    Edges in ev1 that cannot be back eliminated are ignored.
*/
inline int lowest_fill_in_edge_back (const vector<c_graph_t::edge_t>& ev1,
				     const c_graph_t& cg,
				     vector<c_graph_t::edge_t>& ev2) {
  return lowest_fill_in_edge_f (ev1, false, cg, ev2);
}

// -------------------------------------------------------------------------
// LMMD
// -------------------------------------------------------------------------

// here the class is the basic part
/** \brief Class for lowest Markowitz with minimal damage in mixed edge elimination

    It searches for edges with minimal sum of Markowitz degree and weighted damaged.
    The damage of a edge's elimination is defined as the increase of Markowitz degrees of
    all other edges.
    Note that this values can be different for front and back elimination
*/
class lmmd_edge_t : public base_heuristic_t<int> {
  double weight;
public:
  /// Weight is set in the constructor
  lmmd_edge_t (double w) : base_heuristic_t<int> (false), weight (w) {}
  /** Heuristic computed in operator
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Set of edges with lowest lmmd-value
    \return Size of vv2
  */
  int operator() (const vector<edge_bool_t>& ev1,
		  const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Predefined object of lmmd_edge_t with weight=1.0

    This object is introduced for syntactical coherence with other heuristics since
    lmmd_edge can be called like a function.
*/
extern lmmd_edge_t lmmd_edge;

// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

/** \brief Maximal overall Markowitz reduction in edge elimination
    \param ev1 Set of edges that can be eliminated
    \param front Used for front elimination if true, otherwise for back elimination
    \param cg c-graph
    \param ev2 Result vector of edges with maximal overall Markowitz reduction
    \return Size of ev2

    This function is intended for elimination sequences that are either completely
    front or completely back eliminations. For mixed sequences use
    int momr_edge (const vector<edge_bool_t>& ev1,
                           const c_graph_t& cg,
			   vector<edge_bool_t>& ev2).
    Note that Markowitz degree reduction due to edge elimination can be different 
    for front and back elimination.
*/
int momr_edge_f (const vector<c_graph_t::edge_t>& ev1,
		 bool front,
		 const c_graph_t& cg,
		 vector<c_graph_t::edge_t>& ev2);

/** \brief Maximal overall Markowitz reduction in front edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with maximal overall Markowitz reduction
    \return Size of ev2

    Edges in ev1 that cannot be front eliminated are ignored.
*/
inline int momr_edge_front (const vector<c_graph_t::edge_t>& ev1,
			    const c_graph_t& cg,
			    vector<c_graph_t::edge_t>& ev2) {
  return momr_edge_f (ev1, true, cg, ev2);
}

/** \brief Maximal overall Markowitz reduction in back edge elimination
    \param ev1 Set of edges that can be eliminated
    \param cg c-graph
    \param ev2 Result vector of edges with maximal overall Markowitz reduction
    \return Size of ev2

    Edges in ev1 that cannot be back eliminated are ignored.
*/
inline int momr_edge_back (const vector<c_graph_t::edge_t>& ev1,
			   const c_graph_t& cg,
			   vector<c_graph_t::edge_t>& ev2) {
  return momr_edge_f (ev1, false, cg, ev2);
}
  
/** \brief Operator class for lowest Markowitz in mixed edge elimination

    operator() is equivalent to momr_edge (
                 const vector<edge_bool_t>& ev1,
			const c_graph_t& cg,
			vector<edge_bool_t>& ev2)
    \sa forward_mode_vertex_t
*/
class momr_edge_t : public base_heuristic_t<int> {
public:
  momr_edge_t () : base_heuristic_t<int> (true) {}
  int operator() (const vector<edge_bool_t>& ev1, const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

/** \brief Maximal overall Markowitz reduction in mixed edge elimination 
    \param ev1 Set of edges that can be eliminated and how
    \param cg c-graph
    \param ev2 Result vector of edges with maximal overall Markowitz reduction
    \return Size of ev2
*/
extern momr_edge_t momr_edge;

// -------------------------------------------------------------------------
// Minimal distance 
// -------------------------------------------------------------------------

/** \brief Minimizes the maximal distance of vertices involved in an edge elimination
    The motivation is that for small distances it is not very probable to re-insert one 
    of new edges later.
*/
class minimal_distance_edge_t : public base_heuristic_t<int> {
public:
  minimal_distance_edge_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<edge_bool_t>& ev1, const c_graph_t& cg,
		  vector<edge_bool_t>& ev2);
};

// -------------------------------------------------------------------------
// Maximal overall path length reduction
// -------------------------------------------------------------------------

/** \brief Maximal overall path length reduction in mixed edge elimination 
    \param ev1 Set of edges that can be eliminated and how
    \param front indicates if the edge is to be front eliminated 
    \param cg c-graph
    \param ev2 Result vector of edges with maximal path length reduction
    \return Size of ev2
*/
int moplr_edge (const vector<c_graph_t::edge_t>& ev1,
		bool front,
		const c_graph_t& cg,
		vector<c_graph_t::edge_t>& ev2);

// =====================================================
// for edge elimination (in line graph)
// =====================================================


// =====================================================
// for face elimination
// =====================================================


/** \brief Operator class for forward mode in face elimination

    operator() is equivalent to forward_mode_face
    \sa forward_mode_face
    \sa forward_mode_vertex_t
*/
class forward_mode_face_t : public base_heuristic_t<double> {
public:
  forward_mode_face_t () : base_heuristic_t<double> (false) {}
  int operator() (const vector<line_graph_t::face_t>& fv1,
		  const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};
  
/** \brief Forward mode in face elimination
    \param fv1 Set of faces that can be eliminated
    \param lg Line graph
    \param fv2 Result vector of faces, contains face with lowest number (see description)
    \return Size of fv2, always 1 (if fv1 is not empty)

    In terms of vertex numbers, each face has a representation (i, j, k) (whereby several 
    faces may have the same triplet).
    Faces are compared lexicographically with j as first criterion followed by i and k.
    It is equivalent to forward mode in vertex  elimination and edge elimination (with front
    elimination only or mixed eliminations)
    when forward mode is used as sole criterion.
*/
extern forward_mode_face_t forward_mode_face;

// -------------------------------------------------------------------------
// reverse mode
// -------------------------------------------------------------------------

/** \brief Operator class for reverse mode in vertex elimination

    operator() is equivalent to reverse_mode_face
    \sa reverse_mode_face
    \sa forward_mode_vertex_t
*/
class reverse_mode_face_t : public base_heuristic_t<double> {
public:
  reverse_mode_face_t () : base_heuristic_t<double> (true) {}
  int operator() (const vector<line_graph_t::face_t>& fv1,
		  const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};

/** \brief Reverse mode in face elimination
    \param fv1 Set of faces that can be eliminated
    \param lg Line graph
    \param fv2 Result vector of faces, contains face with highest number (see description)
    \return Size of fv2, always 1 (if fv1 is not empty)

    In terms of vertex numbers, each face has a representation (i, j, k) (whereby several 
    faces may have the same triplet).
    Faces are compared lexicographically with j as first criterion followed by i and k.
    It is equivalent to reverse mode in vertex  elimination and edge elimination (with front
    elimination only or mixed eliminations)
    when reverse mode is used as sole criterion.
*/
extern reverse_mode_face_t reverse_mode_face;

/** \brief Operator class for reverse_mode_face_whole_vertex

    operator() is equivalent to reverse_mode_face_whole_vertex
    \sa reverse_mode_face_whole_vertex
    \sa forward_mode_vertex_t
*/
class reverse_mode_face_whole_vertex_t : base_heuristic_t<int> {
public:
  reverse_mode_face_whole_vertex_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<line_graph_t::face_t>& fv1,
		  const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};
  
/** \brief Reverse mode emulating vertex elimination with face eliminations
    \param fv1 Set of faces that can be eliminated
    \param lg Line graph
    \param fv2 Set of faces that belong to vertex with the highest number (see description)
    \return Size of fv2

    In terms of vertex numbers, each face has a representation (i, j, k) (whereby several 
    faces may have the same triplet).
    All faces with highest j are returned.
*/
extern reverse_mode_face_whole_vertex_t reverse_mode_face_whole_vertex;

// -------------------------------------------------------------------------
// Lowest Markowitz
// -------------------------------------------------------------------------

class lowest_markowitz_face_t : public base_heuristic_t<int> {
public:
  lowest_markowitz_face_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<line_graph_t::face_t>& fv1,
		const line_graph_t& lg,
		vector<line_graph_t::face_t>& fv2);
};
  
/** \brief Lowest Markowitz for face elimination
    \param fv1 Set of faces that can be eliminated
    \param lg Line graph
    \param fv2 Set of faces  with the lowest 
               Markowitz degree (see description)
    \return Size of fv2

    In terms of vertex numbers, each face has a representation (i, j, k) (whereby several 
    faces may have the same triplet).
    With this representation the definition of Markowitz degree can be generalized
    to line graphs.
    The  Markowitz degree of vertex j in line graph is the number of faces with j as
    second value, Markowitz(j) = |{(*, j, *)}|
    For all vertices j where some face like (*, j, *) exist in fv1,
    the Markowitz degrees are computed and their minimum determined.
    Returned are all faces from fv1 where j has minimal Markowitz degree.
*/
extern lowest_markowitz_face_t lowest_markowitz_face;

// -------------------------------------------------------------------------
// Lowest Markowitz complete elimination
// -------------------------------------------------------------------------

//
// searches for faces where j from the c-graph vertex 
// triplet representation (i,j,k) has minimal Markowitz degree
// 
// from the set of faces a tie-breaker chose a subset that belong 
// to one vertex, e.g. faces (*,j,*) with maximal j -> tie-breaker=reverse mode
//
// vertex j is stored and in following calls of the operator only
// faces (*,j,*) are returned as long as there some
// when no faces are found anymore then a new vertex is searched
//

// declaration needed in next function
void markowitz_on_line_graph (const line_graph_t& lg, vector<int>& mdegree);

/** \brief Lowest Markowitz for face elimination with completion of vertex elimination

    Computation of Markowitz degree as in lowest_markowitz_face().
    Tie-breaker heuristic used to find unique vertex j to eliminate.
    Multiple faces can be returned by the tie-breaker but they belong to
    the same vertex, e.g. like reverse_mode_face_whole_vertex().
    The vertex j is saved and all following calls return faces belonging to j
    as long as their are any in fv1.
    If all faces belonging to j are contained in fv1, the vertex is completely
    eliminated.
*/
template <class Heuristic_t>
class lowest_markowitz_face_complete_t : public base_heuristic_t<int> {
  int lastv; 
  Heuristic_t tiebreaker;
public:
  /// Constructor with tie-breaker heuristic
  lowest_markowitz_face_complete_t (Heuristic_t t) : 
    base_heuristic_t<int> (false), lastv (-1), tiebreaker (t) {}
  
  /** \brief Heuristic computed in operator
      \param fv1 Set of faces that can be eliminated
      \param lg Line graph
      \param fv2 Set of faces  with the lowest Markowitz degree (see description)
      \return Size of fv2
  */
  int operator() (const vector<line_graph_t::face_t>& fv1,
		  const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};
  
// -------------------------------------------------------------------------
// MOMR
// -------------------------------------------------------------------------

/** \brief Operator class for maximal overall Markowitz degree reduction in face elimination

    operator() is equivalent to momr_face
    \sa momr_face
    \sa forward_mode_vertex_t
*/
class momr_face_t : public base_heuristic_t<int> {
public:
  momr_face_t () : base_heuristic_t<int> (true) {}
  int operator() (const vector<line_graph_t::face_t>& fv1,
		  const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};
  
/** \brief Maximal overall Markowitz degree reduction in face elimination
    \param fv1 Set of faces that can be eliminated
    \param lg line graph
    \param fv2 Set of faces with maximal overall Markowitz degree reduction
    \return Size of fv2

    Implemenation rests on an old definition of face elimination.
    It is not yet tested whether it works properly .
    In face elimination MOMR and LMMD are identical.
*/
extern momr_face_t momr_face;

// -------------------------------------------------------------------------
// Minimal distance 
// -------------------------------------------------------------------------

/** \brief Minimal distance for face elimination
    \param fv1 Set of faces that can be eliminated
    \param lg Line graph
    \param fv2 Set of faces  with minimal distance (see description)
    \return Size of fv2
    
    In terms of vertex numbers, each face has a representation (i, j, k) (whereby several 
    faces may have the same triplet).
    Faces where k - i is  minimal are returned.
    The motivation is that for small k-i it is not very probable to re-insert this face later.
*/
class minimal_distance_face_t : public base_heuristic_t<int> {
public:
  minimal_distance_face_t () : base_heuristic_t<int> (false) {}
  int operator() (const vector<line_graph_t::face_t>& fv1, const line_graph_t& lg,
		  vector<line_graph_t::face_t>& fv2);
};

// =====================================================
// edge_ij_elim_t heuristics (derived from edge elimination heuristics)
// =====================================================

/// Creates a heuristic for (i,j,front) type from a heuristic for (edge,front).
template <class Edge_heuristic_t>
class edge_ij_elim_heuristic_t {
  Edge_heuristic_t  h;
public:
  /// Constructor stores the face-based heuristic to be used
  edge_ij_elim_heuristic_t (Edge_heuristic_t _h) : h (_h) {}
  int operator() (const vector<edge_ij_elim_t>& eev1,
		  const c_graph_t& cg,
		  vector<edge_ij_elim_t>& eev2) {
    vector<edge_bool_t>   ebv1, ebv2;
    for (size_t c= 0; c < eev1.size(); c++) {
      c_graph_t::edge_t edge; bool found;
      tie (edge, found)= angel::edge (eev1[c].i, eev1[c].j, cg);
      if (found) ebv1.push_back (std::make_pair (edge, eev1[c].front)); }
    h (ebv1, cg, ebv2);
    eev2.resize (0);
    for (size_t c= 0; c < ebv2.size(); c++) {
      edge_ij_elim_t ee (source (ebv2[c].first, cg), 
			 target (ebv2[c].first, cg), ebv2[c].second);
      eev2.push_back (ee); }
    return eev2.size();
  }
};


// =====================================================
// triplet_t heuristics (derived from face elimination heuristics)
// =====================================================

/** \brief Creates a heuristic for triplet type from a heuristic for faces.

    Third entry in each triplet is ignored but it will be kept.
*/
template <class Face_heuristic_t>
class triplet_heuristic_t {
  Face_heuristic_t  h;
public:
  /// Constructor stores the face-based heuristic to be used
  triplet_heuristic_t (Face_heuristic_t _h) : h (_h) {}
  int operator() (const vector<triplet_t>& tv1,
		  const line_graph_t& lg,
		  vector<triplet_t>& tv2) {
    vector<line_graph_t::face_t> fv1, fv2;
    for (size_t c= 0; c < tv1.size(); c++) {
      line_graph_t::face_t face; bool found;
      tie (face, found)= angel::edge (tv1[c].i, tv1[c].j, lg);
      if (found) fv1.push_back (face); }
    h (fv1, lg, fv2);
    tv2.resize (0);
    for (size_t c= 0, tc= 0; c < fv2.size(); c++) {
      int s= source (fv2[c], lg), t= target (fv2[c], lg);
      for (; s != tv1[tc].i || t != tv1[tc].j; tc++);
      tv2.push_back (tv1[tc]); }
    return tv2.size();
  }
};

/** \brief Simulates vertex elimination heuristics with edge eliminations

    Egde eliminations belonging to one vertex elimination are executed 
    successively. Can be applied to c-graphs containing dependents with
    out-degree > 0. If multiple vertex heuristics are used it is better to
    combine them first and to derive and edge heuristic from it than
    combining the derived edge heuristics.
*/
template <class Vertex_heuristic_t>
class emulated_vertex_heuristic_t {
  Vertex_heuristic_t  h;
  c_graph_t::vertex_t last_vertex;
public:
  /// Constructor stores the vertex-based heuristic to be used
  emulated_vertex_heuristic_t (Vertex_heuristic_t _h) : h (_h), last_vertex (0) {}
  int operator() (const vector<edge_ij_elim_t>& tv1, const c_graph_t& cg,
		  vector<edge_ij_elim_t>& tv2);
};


// =====================================================
// combining heuristics
// =====================================================

/** \brief Make a pair of heuristics

    Two heuristics are semantically and syntactically combined to one heuristic.
    Objects of this class can passed where a heuristic is used in a template
    or can be called like heuristic functions.
    For instance, heuristic_pair_t<lowest_markowitz_vertex_t, reverse_mode_vertex_t> lmrm;
    lmrm (v1, cg, v2); is identical with lowest_markowitz_vertex (v1, cg, vtmp); 
    reverse_mode_vertex (vtmp, cg, v2).
    \sa use_heuristic
*/
template <class Heuristic1_t, class Heuristic2_t>
class heuristic_pair_t {
private:
  Heuristic1_t h1;
  Heuristic2_t h2;
public:
  /// Constructor with 
  heuristic_pair_t (Heuristic1_t _h1, Heuristic2_t _h2):
    h1 (_h1), h2 (_h2) {}
  /// Operator that behaves like one heuristic function
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2) {
    Vector_t vt; h1 (v1, adg, vt); return h2 (vt, adg, v2); }
};

/** \brief Make a pair of heuristics

    Three heuristics are semantically and syntactically combined to one heuristic.
    \sa use_heuristic
    \sa heuristic_pair_t
*/
template <class Heuristic1_t, class Heuristic2_t, class Heuristic3_t>
class heuristic_triplet_t {
private:
  Heuristic1_t h1;
  Heuristic2_t h2;
  Heuristic3_t h3;
public:
  heuristic_triplet_t (Heuristic1_t _h1, Heuristic2_t _h2, Heuristic3_t _h3):
    h1 (_h1), h2 (_h2), h3 (_h3) {}

  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2) {
    Vector_t vt1, vt2; h1 (v1, adg, vt1); h2 (vt1, adg, vt2); 
    return h3 (vt2, adg, v2);}
};

// =====================================================
// apply heuristic to some graph
// =====================================================

/** \brief Use heuristic to transform c-/line graph into bi-/tri-partite graph
    \param adg c-graph or line graph
    \param el_seq Obtained elimination sequence
    \param h Heuristic or combination of heuristics
    \return Elimination costs

    At first graph adg is copied. The type of elimination is determined by the
    element type of vector el_seq.
    Then all
    objects (vertex, edge, face) chosen by \p h are eliminated from the graph copy.
    \sa eliminatable_objects
    \sa eliminate()
    \sa heuristic_pair_t
*/
template <class Object_t, class Ad_graph_t, class Heuristic_t>
inline int use_heuristic (const Ad_graph_t& adg, vector<Object_t>& el_seq,
			  Heuristic_t h) {
  Ad_graph_t adg_copy (adg);
  vector<Object_t> v1;
  eliminatable_objects (adg_copy, v1);
  int costs= 0;
  el_seq.resize (0); // clean elimination sequence

  while (!v1.empty()) {
    vector<Object_t> v2;
    h (v1, adg_copy, v2);
    Object_t o= v2[0];
    costs+= eliminate (o, adg_copy);
    el_seq.push_back (o);
    eliminatable_objects (adg_copy, v1);
  }
  return costs;
}

/** \brief Use heuristic to transform c-/line graph into bi-/tri-partite graph
    \param adg c-graph or line graph
    \param el_seq Obtained elimination sequence
    \param h Heuristic or combination of heuristics
    \return Elimination costs

    Same as use_heuristic but the graph is changed not copied.
    \sa use_heuristic
    \sa eliminatable_objects
    \sa eliminate()
    \sa heuristic_pair_t
*/
template <class Object_t, class Ad_graph_t, class Heuristic_t>
inline int use_heuristic_noconst (Ad_graph_t& adg, vector<Object_t>& el_seq,
				   Heuristic_t h) {
  Ad_graph_t& adg_copy (adg);
  vector<Object_t> v1;
  eliminatable_objects (adg_copy, v1);
  int costs= 0;
  el_seq.resize (0); // clean elimination sequence

  while (!v1.empty()) {
    vector<Object_t> v2;
    h (v1, adg_copy, v2);
    Object_t o= v2[0];
    costs+= eliminate (o, adg_copy);
    el_seq.push_back (o);
    eliminatable_objects (adg_copy, v1);
  }
  return costs;
}

/** \brief Debugging version of use_heuristic, several outputs.
    \param adg c-graph or line graph
    \param el_seq Obtained elimination sequence
    \param h Heuristic or combination of heuristics
    \return Elimination costs
    \sa use_heuristic
    \sa eliminatable_objects
    \sa eliminate()
    \sa heuristic_pair_t
*/
template <class Object_t, class Ad_graph_t, class Heuristic_t>
inline int use_heuristic_debug (const Ad_graph_t& adg, vector<Object_t>& el_seq,
				 Heuristic_t h) {
  // Ad_graph_t adg_copy (adg);
  Ad_graph_t adg_copy; 
  write_graph ("\nuse_heuristic_debug: passed graph", adg);
  cout << "edges are ";
  typename Ad_graph_t::edge_iterator ei, e_end;
  for (tie (ei, e_end)= edges (adg); ei != e_end; ++ei)
    cout << '(' << source (*ei, adg) << ", " << target (*ei, adg) << "), ";
  cout << "\n";

  write_graph ("\nuse_heuristic_debug: copied graph", adg_copy);
  adg_copy= adg;
  write_graph ("\nuse_heuristic_debug: copied graph", adg_copy);
  vector<Object_t> v1;
  eliminatable_objects (adg_copy, v1);
  int costs= 0;
  el_seq.resize (0); // clean elimination sequence

  while (!v1.empty()) {
    write_vector ("use_heuristic_debug: eliminatable objects", v1);
    vector<Object_t> v2;
    h (v1, adg_copy, v2);
    write_vector ("use_heuristic_debug: selected objects", v2);
    Object_t o= v2[0];
    costs+= eliminate (o, adg_copy);
    write_graph ("use_heuristic_debug: resulting graph", adg_copy);
    el_seq.push_back (o);
    eliminatable_objects (adg_copy, v1);
  }
  return costs;
}


/** \brief Tracing version of use_heuristic, writes costs for every elimination.
    \param adg c-graph or line graph
    \param el_seq Obtained elimination sequence
    \param h Heuristic or combination of heuristics
    \param output 
    \return Elimination costs
    \sa use_heuristic
    \sa eliminatable_objects
    \sa eliminate
    \sa heuristic_pair_t
*/
template <class Object_t, class Ad_graph_t, class Heuristic_t, class Output_t>
inline int use_heuristic_trace (const Ad_graph_t& adg, vector<Object_t>& el_seq,
				Heuristic_t h, Output_t output) {
  Ad_graph_t adg_copy (adg);
  vector<Object_t> v1;
  eliminatable_objects (adg_copy, v1);
  int costs= 0;
  el_seq.resize (0); // clean elimination sequence

  while (!v1.empty()) {
    vector<Object_t> v2;
    h (v1, adg_copy, v2);
    Object_t o= v2[0];
    int ocosts= eliminate (o, adg_copy);
    costs+= ocosts;
    output (cout, o); 
    cout << " costs " << ocosts << " --> overall " << costs << endl;
    el_seq.push_back (o);
    eliminatable_objects (adg_copy, v1);
  }
  return costs;
}

/** \brief Applies heuristic and uses output visitor write costs and graphs for every elimination.
    \param adg c-graph or line graph
    \param el_seq Obtained elimination sequence
    \param h Heuristic or combination of heuristics
    \param output Visitor to where written is
    \return Elimination costs
    \sa use_heuristic
    \sa eliminatable_objects
    \sa eliminate
    \sa heuristic_pair_t
    \sa no_output_t
    \sa string_stream_output_t
*/
template <class Object_t, class Ad_graph_t, class Heuristic_t, class Output_t>
inline int apply_heuristic (const Ad_graph_t& adg, vector<Object_t>& el_seq,
			    Heuristic_t h, Output_t output) {
  Ad_graph_t adg_copy (adg);
  vector<Object_t> v1;
  eliminatable_objects (adg_copy, v1);
  output.write_graph ("Original graph", adg_copy);
  int costs= 0, iteration= 0;
  el_seq.resize (0); // clean elimination sequence

  while (!v1.empty()) {
    vector<Object_t> v2;
    h (v1, adg_copy, v2);
    Object_t o= v2[0];
    int ocosts= eliminate (o, adg_copy);
    costs+= ocosts;
    output << "Elimination of " << o << " costs " << ocosts << " --> overall " << costs << '\n';
    std::ostringstream gtext; gtext << "Graph after " << ++iteration << "iterations\n";
    output.write_graph (gtext.str(), adg_copy);
    el_seq.push_back (o);
    eliminatable_objects (adg_copy, v1);
  }
  return costs;
}

/// Applies 5 heuristics to \p adg and returns the elimination sequence of the cheapest one
template <class Object_t, class Ad_graph_t, class Heuristic1_t, class Heuristic2_t, 
          class Heuristic3_t, class Heuristic4_t, class Heuristic5_t>
inline int best_heuristic (const Ad_graph_t& adg, vector<Object_t>& el_seq,
			   Heuristic1_t h1, Heuristic2_t h2, Heuristic3_t h3, 
			   Heuristic4_t h4, Heuristic5_t h5);

// =====================================================
// find subset of v1 that is best w.r.t. op
// =====================================================

/// Find best subset of \p v1 w.r.t. \p op, skeleton for new heuristics
template <class Object_t, class Ad_graph_t, class Op_t>
int find_best_subset (const vector<Object_t>& v1, const Ad_graph_t& adg,
		      vector<Object_t>& v2, Op_t op, bool maximize) {
  v2.resize (0);

  if (v1.size() == 0) return 0;
  int best= maximize ? -op (v1[0], adg) : op (v1[0], adg);
  v2.push_back (v1[0]);

  for (size_t c= 1; c < v1.size(); c++) {
    Object_t o= v1[c];
    int value= maximize ? -op (o, adg) : op (o, adg);
    if (value < best) v2.resize (0);
    if (value <= best) {
      v2.push_back (o); best= value;}
  }
  return v2.size();
}

#ifdef USEXAIFBOOSTER

// =====================================================
// scarcity preserving eliminations
// =====================================================

/** \brief Determines the effect, in terms of nontrivial edge count, of performing edge elimination \p be

    \param be edge elimination target under consideration
    \param angelLCG c-graph
    \param ourAwarenessLevel setting such as unit aware, constant aware, or no awareness
    \return net effect on nontrivial edge count
*/
int edge_elim_effect (const edge_bool_t be,
		      const c_graph_t& angelLCG,
                      const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel);

/** \brief Determines the effect, in terms of nontrivial edge count, of performing edge elimination \p ee

    \param ee edge elimination target under consideration
    \param angelLCG The linearized computational graph
    \param ourAwarenessLevel setting such as unit aware, constant aware, or no awareness
    \return net effect on nontrivial edge count
*/
int edge_elim_effect(const EdgeElim ee,
		     const c_graph_t& angelLCG,
                     const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel);

/** \brief Filter that selects edge elimination targets that don't increase the nontrivial edge count

    \param bev1 set of edges that can be eliminated
    \param angelLCG c-graph
    \param ourAwarenessLevel needed to assess costs of eliminations
    \param bev2 set of edge elims that don't increase the nontrivial edge count
    \return size of bev2
*/
bool maintaining_edge_eliminations(const vector<EdgeElim>& bev1,
                                   const c_graph_t& angelLCG,
                                   const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                   vector<EdgeElim>& bev2);

/** \brief Filter that selects edge elimination targets that decrease the nontrivial edge count

    \param bev1 set of edges that can be eliminated
    \param angelLCG c-graph
    \param ourAwarenessLevel needed to assess costs of eliminations
    \param bev2 set of edge elims that decrease the nontrivial edge count
    \return size of bev2
*/
bool reducing_edge_eliminations(const vector<EdgeElim>& bev1,
                                const c_graph_t& angelLCG,
                                const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                vector<EdgeElim>& bev2);

/** \brief Filter that selects edge elimination targets whose refill dependences (a possibly empty set of vertices)
 * have been met (meaning that there is no alternate path for the edge through the vertex).

    \param bev1 set of edges that can be eliminated
    \param angelLCG c-graph
    \param refillDependences partial map of edges to a set of vertices that lie on paths from the edge sources
	   to the edge targets, used to anticipate refill.
    \param bev2 set of edge elims that dont violate refill dependences (returned by reference)
    \return size of bev2
*/
bool refill_avoiding_edge_eliminations(const vector<EdgeElim>& bev1,
                                       c_graph_t& angelLCG,
                                       const refillDependenceMap_t refillDependences,
                                       vector<EdgeElim>& bev2);

/** Filter for selecting those edge eliminations that don't undo a rerouting
 * (a front-elimination can undo a pre-routing, and a back-elimination can undo a post-routing)
 */
bool rerouting_considerate_edge_eliminations(const vector<EdgeElim>& bev,
                                             const c_graph_t& angelLCG,
                                             const std::vector<Transformation>& transformationsPerformedV,
                                             vector<EdgeElim>& reroutingConsiderateEdgeElimsV);

unsigned int lowestMarkowitzEdgeElim(const vector<EdgeElim>& inEEV,
                                     const c_graph_t& angelLCG,
                                     vector<EdgeElim>& outEEV);

bool reverseModeEdgeElim(const vector<EdgeElim>& inEEV,
                         const c_graph_t& angelLCG,
                         vector<EdgeElim>& outEEV);

// ==============================================================================
// |			FILTERS FOR REROUTINGS					|
// ==============================================================================

/** Filter that populates \p noncyclicReroutingsV with (strictly) those reroutings
 * that have not already been performed, based on \p transformationsPerformedV
 */
size_t noncyclicReroutings(const vector<Rerouting>& erv,
			   const std::vector<Transformation>& transformationsPerformedV,
			   const c_graph_t& angelLCG,
			   vector<Rerouting>& noncyclicReroutingsV);
/*
bool maintaining_reroutings (const vector<edge_reroute_t>& erv,
			     const c_graph_t& angelLCG,
			     const AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			     vector<edge_reroute_t>& maintainReroutingsV);
*/

/** Filter that populates /p reducingReroutingsV with those reroutings that can be followed
 * by an edge elimination with an overall reduction in the nontrivial edgecount.
 */
bool reducing_reroutings (const vector<Rerouting>& erv,
			  const c_graph_t& angelLCG,
                          const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
			  vector<Rerouting>& reducingReroutingsV);

// ==============================================================================
// |		FILTERS FOR ELIMINATIONS AND REROUTINGS	(TRANSFORMATIONS)	|
// ==============================================================================

/** Assesses the change in nontrivial edge count that results from applying the transformation \p t 
 * 
 */
int transformation_effect(const Transformation t,
			  const c_graph_t& angelLCG,
                          const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel);

/** Filter that populates \p allViableTransformationsV with all possible edge eliminations and
 * all possible reroutings that haven't already been performed (so-called noncyclic reroutings).
 */
bool all_viable_transformations(c_graph_t& angelLCG,
				const std::vector<Transformation>& transformationsPerformedV,
				vector<Transformation>& allViableTransformationsV);

/** Filter that populates /p maintainingTransformationsV with those edge eliminations and
 * reroutings that maintain the nontrivial edge count (in particular, this includes all reroutings).
 */
bool maintaining_transformations(const vector<Transformation>& tv,
                                 const c_graph_t& angelLCG,
                                 const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                 vector<Transformation>& maintainingTransformationsV);

/** Filter that populates /p reducingTransformationsV with edge eliminations that reduce
 * the nontrivial edge count and reroutings that can be followed by an edge elimination
 * with an overall reduction in the nontrivial edgecount.
 */
bool reducing_transformations(const vector<Transformation>& tv,
                              const c_graph_t& angelLCG,
                              const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                              vector<Transformation>& reducingTransformationsV);

/** Filter that populates /p refillAvoidingTransformationsV with edge eliminations that
 * aren't known to be refillable in the future.  Any reroutings are passed straight through.
 */
bool refill_avoiding_transformations(const vector<Transformation>& tv,
                                     c_graph_t& angelLCG,
                                     const xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel,
                                     const refillDependenceMap_t& refillDependences,
                                     vector<Transformation>& refillAvoidingTransformationsV);

/** Filter that populates /p reroutingConsiderateTransformationsV with edge eliminations
 * that don't undo reroutings.  Any reroutings are passed straight through.
 */
bool rerouting_considerate_transformations(const vector<Transformation>& tv,
					   const c_graph_t& angelLCG,
					   const std::vector<Transformation>& transformationsPerformedV,
					   vector<Transformation>& reroutingConsiderateTransformationsV); 

/** Filter that populates /p lowestMarkowitzTransformationsV with those edge eliminations
 * that have the lowest markowitz degree.  Any reroutings are passed straight through.
 */
bool lowest_markowitz_transformations(const vector<Transformation>& tv,
                                      const c_graph_t& angelLCG,
                                      vector<Transformation>& lowestMarkowitzTransformationsV);

/** Filter that populates /p lowestMarkowitzTransformationsV with an edge elimination
 * that is chosen by reverse topological order.  Any reroutings are passed straight through.
 */
bool reverse_mode_transformations (const vector<Transformation>& tv,
				   const c_graph_t& angelLCG,
				   vector<Transformation>& reverseModeTransformationsV);

#endif // USEXAIFBOOSTER

#ifdef USE_MPI
/// Build a parallel heuristic out of a sequential 
template <class Heuristic_t, class Comm_t>
class par_heuristic_t {
  Heuristic_t       h;
  Comm_t            comm;
public:
  /// Constructor with sequential heuristic
  par_heuristic_t (Heuristic_t _h, Comm_t _comm) : h (_h), comm (_comm) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2); 
};

template <class Comm_t>
class split_vector_t {
  Comm_t            comm;
public:
  split_vector_t (Comm_t _comm) : comm (_comm) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t&, Vector_t& v2) {
    v2.resize (0);
    int me= comm.Get_rank(), np= comm.Get_size();
    size_t first= block_begin (me, np, v1.size()), last= block_begin (me + 1, np, v1.size()) - 1;
    for (; first <= last; first++) v2.push_back (v1[first]);
    return v2.size(); }
};

typedef split_vector_t<GMPI::Intracomm> std_split_vector_t;

template <class Comm_t>
class bcast_best_t {
  Comm_t            comm;
public:
  bcast_best_t (Comm_t _comm) : comm (_comm) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t&, Vector_t& v2);
};

typedef bcast_best_t<GMPI::Intracomm> std_bcast_best_t;

template <class Heuristic_t, class Comm_t>
class par_frame_t {
private:
  split_vector_t<Comm_t>   my_split_vector;
  Heuristic_t              h;
  bcast_best_t<Comm_t>     my_bcast_best;
  heuristic_triplet_t<split_vector_t<Comm_t>,Heuristic_t,bcast_best_t<Comm_t> > my_triplet;
public:
  par_frame_t (Heuristic_t _h, Comm_t _comm) : my_split_vector (_comm), h (_h), 
                       my_bcast_best (_comm), my_triplet (my_split_vector, h, my_bcast_best) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2) {
    return my_triplet (v1, adg, v2); }
};

template <class Heuristic1_t, class Heuristic2_t, class Comm_t>
class par_heuristic_pair_t {
private:
  typedef par_heuristic_t<Heuristic1_t,Comm_t>   ph1_t;
  typedef par_heuristic_t<Heuristic2_t,Comm_t>   ph2_t;
  typedef heuristic_pair_t<ph1_t,ph2_t>          pair_t;
  typedef par_frame_t<pair_t,Comm_t>             par_heuristic_t;
  ph1_t              ph1;
  ph2_t              ph2;
  pair_t             pair;
  par_heuristic_t    par_heuristic;
public:
  par_heuristic_pair_t (Heuristic1_t _h1, Heuristic2_t _h2, Comm_t _comm) :
    ph1 (_h1, _comm), ph2 (_h2, _comm), pair (ph1, ph2), par_heuristic (pair, _comm) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2) {
    return par_heuristic (v1, adg, v2); }
};

template <class Heuristic1_t, class Heuristic2_t, class Heuristic3_t, class Comm_t>
class par_heuristic_triplet_t {
private:
  typedef par_heuristic_t<Heuristic1_t,Comm_t>   ph1_t;
  typedef par_heuristic_t<Heuristic2_t,Comm_t>   ph2_t;
  typedef par_heuristic_t<Heuristic3_t,Comm_t>   ph3_t;
  typedef heuristic_triplet_t<ph1_t,ph2_t,ph3_t> triplet_t;
  typedef par_frame_t<triplet_t,Comm_t>          par_heuristic_t;
  ph1_t              ph1;
  ph2_t              ph2;
  ph3_t              ph3;
  triplet_t          triplet;
  par_heuristic_t    par_heuristic;
public:
  par_heuristic_triplet_t (Heuristic1_t _h1, Heuristic2_t _h2, Heuristic3_t _h3, Comm_t _comm) :
    ph1 (_h1, _comm), ph2 (_h2, _comm), ph3 (_h3, _comm), triplet (ph1, ph2, ph3), 
    par_heuristic (triplet, _comm) {}
  template <class Vector_t, class Ad_graph_t>
  int operator() (const Vector_t& v1, const Ad_graph_t& adg, Vector_t& v2) {
    return par_heuristic (v1, adg, v2); }
};

#endif // USE_MPI

} // namespace angel

#include "heuristics_impl.hpp"

#endif  // _heuristics_include_

