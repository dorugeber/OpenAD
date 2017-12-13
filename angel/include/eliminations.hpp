/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#ifndef 	_eliminations_include_
#define 	_eliminations_include_

#include "angel_types.hpp"
#include "angel_io.hpp"

#ifdef USEXAIFBOOSTER
  #include "xaifBooster/algorithms/CrossCountryInterface/inc/AwarenessLevel.hpp"
  using std::list;
#endif

  namespace angel {

  using std::vector;
  using std::cout;
  using boost::tie;

// =========================================================================
// eliminations in c-graph
// =========================================================================

// -------------------------------------------------------------------------
// elimination of a single vertex in compute graph
// -------------------------------------------------------------------------

/**
    Elimination of vertex \p v from c-graph \p cg
    \return The costs (number of operation)
*/
int vertex_elimination (c_graph_t::vertex_t v, c_graph_t& cg);	

/**
    Elimination of vertex with number \p j from c-graph \p cg
    \return The costs (number of operation)
*/ 
inline int vertex_elimination (int j, c_graph_t& cg) {
  return vertex_elimination (vertex (j, cg), cg); }

// -------------------------------------------------------------------------
// elimination of a single edge in compute graph
// -------------------------------------------------------------------------

/**
   Front elimination of edge \p e from c-graph \p cg
    \return The costs (number of operation)
*/ 
int front_edge_elimination (c_graph_t::edge_t e,
			    c_graph_t& cg);

/**
   Front elimination of edge from vertex \p i to vertex \p j from c-graph \p cg
    \return The costs (number of operation)
*/ 
inline int front_edge_elimination (c_graph_t::vertex_t i,
				   c_graph_t::vertex_t j,
				   c_graph_t& cg) {
  bool                          found_ij;
  c_graph_t::edge_t   edge_ij;
  tie (edge_ij, found_ij)= edge (i, j, cg);
  return found_ij ? front_edge_elimination (edge_ij, cg) : 0;
}

/**
   Front elimination of edge from vertex with number \p i to vertex with number
 \p j from c-graph \p cg
    \return The costs (number of operation)
*/ 
inline int front_edge_elimination (int i, int j, c_graph_t& cg) {
  return front_edge_elimination (vertex (i, cg), vertex (j, cg), cg); }


/**
   Back elimination of edge \p e from c-graph \p cg
    \return The costs (number of operation)
*/ 
int back_edge_elimination (c_graph_t::edge_t e,
			   c_graph_t& cg);

/** Back elimination of edge from vertex \p i to vertex \p j from c-graph \p cg
    \return The costs (number of operation)
*/ 
inline int back_edge_elimination (c_graph_t::vertex_t i, 
				  c_graph_t::vertex_t j,
				  c_graph_t& cg) {
  bool                          found_ij;
  c_graph_t::edge_t   edge_ij;
  tie (edge_ij, found_ij)= edge (i, j, cg);
  return found_ij ? back_edge_elimination (edge_ij, cg) : 0;
}

/** Back elimination of edge from vertex with number \p i to vertex with number
    \p j from c-graph \p cg
    \return The costs (number of operation)
*/ 
inline int back_edge_elimination (int i, int j, c_graph_t& cg) {
  return back_edge_elimination (vertex (i, cg), vertex (j, cg), cg); }

/** Front elimination of edge \p e from c-graph \p cg if \p front=true otherwise back eliminination
    \return The costs (number of operation)
*/ 
inline int edge_elimination (c_graph_t::edge_t e, bool front,
			     c_graph_t& cg) {
  return front ? front_edge_elimination (e, cg)
               : back_edge_elimination (e, cg);
}

/** Front elimination of edge \p e.first from c-graph \p cg if \p e.second=true otherwise back eliminination
    \return The costs (number of operation)
*/ 
inline int edge_elimination (edge_bool_t e, c_graph_t& cg) {
  return e.second ? front_edge_elimination (e.first, cg)
                  : back_edge_elimination (e.first, cg);
}

/** Front elimination of edge from vertex  \p i to vertex 
 \p j from c-graph \p cg if \p front=true otherwise back eliminination
    \return The costs (number of operation)
*/ 
inline int edge_elimination (c_graph_t::vertex_t i,
			     c_graph_t::vertex_t j,
			     bool front, c_graph_t& cg) {
  return front ? front_edge_elimination (i, j, cg)
               : back_edge_elimination (i, j, cg);
}

/** Front elimination of edge from vertex with number \p i to vertex with number
    \p j from c-graph \p cg if \p front=true otherwise back eliminination
    \return The costs (number of operation)
*/ 
inline int edge_elimination (int i, int j, bool front, c_graph_t& cg) {
  return front ? front_edge_elimination (i, j, cg)
               : back_edge_elimination (i, j, cg);
}

/// Edge elimination specified by \p ee
inline int edge_elimination (edge_ij_elim_t ee, c_graph_t& cg) {
  return edge_elimination (ee.i, ee.j, ee.front, cg); }

// -------------------------------------------------------------------------
// elimination sequences of vertices in compute graph
// -------------------------------------------------------------------------

/**
    Elimination of vertices in sequence \p seq from c-graph \p cg
    \return The costs (number of operation)
*/
int vertex_elimination (const vector<int>& seq, c_graph_t& cg);


/** Eliminate sequence \p seq of edges from c-graph \p cg
    \return The costs (number of operation)
*/
inline int edge_elimination (const edge_elim_seq_t& seq, c_graph_t& cg) {
  int costs= 0;
  for (size_t i= 0; i < seq.size(); i++)
    costs+= edge_elimination (seq[i].edge, seq[i].front, cg);
  return costs;
}
  
/** Eliminate sequence \p seq of edges from c-graph \p cg
    \return The costs (number of operation)
*/
inline int edge_elimination (const edge_pair_elim_seq_t& seq, c_graph_t& cg) {
  int costs= 0;
  for (size_t k= 0; k < seq.size(); k++)
    costs+= edge_elimination (seq[k].i, seq[k].j, seq[k].front, cg);
  return costs;
}
  
/** Eliminate sequence \p seq of edges from c-graph \p cg
    \return The costs (number of operation)
*/
inline int edge_elimination (const edge_ij_elim_seq_t& seq, c_graph_t& cg) {
  int costs= 0;
  for (size_t k= 0; k < seq.size(); k++)
    costs+= edge_elimination (seq[k].i, seq[k].j, seq[k].front, cg);
  return costs;
}
  
// =========================================================================
// eliminations in line graph
// =========================================================================

// -------------------------------------------------------------------------
// elimination of a single vertex in line graph
// -------------------------------------------------------------------------

/**
   Eliminate vertex with number \p j in line graph \p lg

   All faces (*,j,*) are eliminated 
*/
int vertex_elimination (int j, line_graph_t& lg);

// -------------------------------------------------------------------------
// elimination of a single edge in line graph
// -------------------------------------------------------------------------

/**
   Front eliminate edge from vertex with number \p i to vertex number \p j in line graph \p lg

   All faces (i,j,*) are eliminated 
*/
int front_edge_elimination (int i, int j, line_graph_t& lg);

/**
   Front eliminate edge \p vij in line graph \p lg

   All faces (i,j,*) are eliminated 
*/
int front_edge_elimination (line_graph_t::edge_t vij, line_graph_t& lg);

/**
   Back eliminate edge from vertex with number \p i to vertex number \p j in line graph \p lg

   All faces (*,i,j) are eliminated 
*/
int back_edge_elimination (int i, int j, line_graph_t& lg);

/**
   Back eliminate edge \p vij in line graph \p lg

   All faces (*,i,j) are eliminated 
*/
int back_edge_elimination (line_graph_t::edge_t vij, line_graph_t& lg);

/// Front elimination of edge from vertex  \p i to vertex 
/// \p j from line graph \p lg if \p front=true otherwise back eliminination
inline int edge_elimination (int i, int j, bool front, line_graph_t& lg) {
  return front ? front_edge_elimination (i, j, lg)
               : back_edge_elimination (i, j, lg);
}

/// Front elimination of edge \p vij from line graph \p lg if \p front=true otherwise back eliminination
inline int edge_elimination (line_graph_t::edge_t vij,
			     bool front, line_graph_t& lg) {
  return front ? front_edge_elimination (vij, lg)
               : back_edge_elimination (vij, lg);
}

// -------------------------------------------------------------------------
// elimination sequences of edges in line graph
// -------------------------------------------------------------------------

// additional data structures
// --------------------------

/// Edge \p edge (as node of line graph) to eliminate from c-graph and whether 
/// it should be front or back eliminated
struct edge_vertex_elim_t {
  line_graph_t::edge_t    edge;
  bool                    front;
};

/// sequences of edges as nodes from line graph
typedef vector<edge_vertex_elim_t>       edge_vertex_elim_seq_t;

/// Eliminate sequence \p seq of edges from line graph \p lg
inline int edge_elimination (const edge_vertex_elim_seq_t& seq, line_graph_t& lg) {
  int costs= 0;
  for (size_t k= 0; k < seq.size(); k++)
    costs+= edge_elimination (seq[k].edge, seq[k].front, lg);
  return costs;
}

// -------------------------------------------------------------------------
// elimination of a single face in line graph
// -------------------------------------------------------------------------

/** Eliminate face \p f from line graph \p lg
    \param f the face
    \param kr is a request for the number of a new node or the number of the 
    absorbing the face, i.e. if face elimination inserts a new node
    into \p lg it should be number with \p kr and
    if a new node is immediately absorbed by some node \c k then it should be 
    \c k = \p kr.
    If the request cannot be satisfied the face is not eliminated.
    \p kr = -1 means no request.
    \param lg the line graph
    \param ac is a container for graphs representing the accumulation code
    \return The number of node inserted or where the absorption took place.
 */
int face_elimination (line_graph_t::face_t f, int kr, line_graph_t& lg, accu_graph_t& ac);

/** Eliminate face \p f from line graph \p lg
    \param f the face
    \param kr is a request for the number of a new node or the number of the node 
    absorbing the face, i.e. if face elimination inserts a new node
    into \p lg it should be number with \p kr and
    if a new node is immediately absorbed by some node \c k then it should be 
    \c k = \p kr.
    If the request cannot be satisfied the face is not eliminated.
    \p kr = -1 means no request.
    \param lg the line graph
    \return The number of node inserted or where the absorption took place.
 */
inline int face_elimination (line_graph_t::face_t f, int kr, line_graph_t& lg) {
  accu_graph_t dummy (*lg.cgp, lg);
  return face_elimination (f, kr, lg, dummy); }

/** Eliminate face \p f from line graph \p lg
    \return The number of node inserted or where the absorption took place.
 */
inline int face_elimination (line_graph_t::face_t f, line_graph_t& lg) {
  return face_elimination (f, -1, lg); }

/** Eliminate face from line graph
    \param i node number of the source of the face
    \param j node number of the source of the face
    \param lg the line graph
    \return The number of node inserted or where the absorption took place.
 */
inline int face_elimination (int i, int j, line_graph_t& lg) {
  line_graph_t::face_t f; bool found;
  tie (f, found)= edge (i, j, lg);
  return found ? face_elimination (f, lg) : -1;
}

/** Eliminate face from line graph
    \param i node number of the source of the face
    \param j node number of the source of the face
    \param kr is a request for the number of a new node or the number of the node 
    absorbing the face, i.e. if face elimination inserts a new node
    into \p lg it should be number with \p kr and
    if a new node is immediately absorbed by some node \c k then it should be 
    \c k = \p kr.
    If the request cannot be satisfied the face is not eliminated.
    \p kr = -1 means no request.
    \param lg the line graph
    \return The number of node inserted or where the absorption took place.
 */
inline int face_elimination (int i, int j, int kr, line_graph_t& lg) {
  line_graph_t::face_t f; bool found;
  tie (f, found)= edge (i, j, lg);
  return found ? face_elimination (f, kr, lg) : -1;
}

inline int face_elimination (int i, int j, int kr, line_graph_t& lg, accu_graph_t& ac) {
  line_graph_t::face_t f; bool found;
  tie (f, found)= edge (i, j, lg);
  return found ? face_elimination (f, kr, lg, ac) : -1;
}

/** Eliminate face from line graph
    \param t a triplet of node number (i, j, kr), for meaning of kr see parameter lists 
    of other face elimination functions 
    \param lg the line graph
    \return Whether a face was eliminated, i.e. the elimination costs.

    The third parameter of \p t is overwritten if the elimination was successful.
    So the information on both the resulting node and the elimination costs are provided.
 */
inline int face_elimination (triplet_t& t, line_graph_t& lg) {
  int k= face_elimination (t.i, t.j, t.k, lg);
  if (k != -1) t.k= k; return k != -1;
}

/** Eliminate face from line graph
    \param t a triplet of node number (i, j, kr), for meaning of kr see parameter lists 
    of other face elimination functions 
    \param lg the line graph
    \param ac is a container for graphs representing the accumulation code
    \return Whether a face was eliminated, i.e. the elimination costs.

    The third parameter of \p t is overwritten if the elimination was successful.
    So the information on both the resulting node and the elimination costs are provided.
    We found that this version is the most convenient
    for general-purpose usage, e.g. stochastic algorithms.
 */
inline int face_elimination (triplet_t& t, line_graph_t& lg, accu_graph_t& ac) {
  int k= face_elimination (t.i, t.j, t.k, lg, ac);
  if (k != -1) t.k= k; return k != -1;
}

/** Returns whether face elimination 
    induces an operation during Jacobian accumulation
    \param i node number of the source of the face
    \param j node number of the source of the face
    \param k is the number of a new node or the number of the node 
    absorbing the face
    \param lg the line graph
    \return 1 if elimination induces operation (+, *, or fused multiply add) 
    otherwise 0
 */
inline int was_non_trivial_elimination (int i, int j, int k, const line_graph_t& lg) {
  line_graph_t::const_ed_t   el= get(boost::vertex_degree, lg);  // edge label
  return ((k != -1 && el[i] != 1 && el[j] != 1) || k < lg.v()-1); // absorption -> a+= b in accumulation
}

/** Returns whether face elimination 
    induces an operation during Jacobian accumulation
    \param f the face
    \param k is the number of a new node or the number of the node 
    absorbing the face
    \param lg the line graph
    \return 1 if elimination induces operation (+, *, or fused multiply add) 
    otherwise 0
 */
inline int was_non_trivial_elimination (line_graph_t::face_t f, int k, const line_graph_t& lg) {
  line_graph_t::edge_t  i= source (f, lg), j= target (f, lg);
  return was_non_trivial_elimination (i, j, k, lg);
}

// -------------------------------------------------------------------------
// elimination sequences of faces
// -------------------------------------------------------------------------


// =========================================================================
// overloaded elimination functions for templates
// =========================================================================

/// Overloaded elimination for templates, here vertex elimination in c-graph
inline int eliminate (c_graph_t::vertex_t v, c_graph_t& cg) {
  return vertex_elimination (v, cg); }

/// Overloaded elimination for templates, here vertex elimination in c-graph
inline int eliminate (int j, c_graph_t& cg) {
  return vertex_elimination (j, cg); }

/// Overloaded elimination for templates, here vertex elimination in line graph
inline int eliminate (int j, line_graph_t& lg) {
  return vertex_elimination (j, lg); }

/// Overloaded elimination for templates, here vertex elimination in c-graph
inline int eliminate (edge_bool_t e, c_graph_t& cg) {
  return edge_elimination (e, cg);
}

/// Overloaded elimination for templates, here vertex elimination in c-graph
inline int eliminate (edge_ij_elim_t ee, c_graph_t& cg) {
  return edge_elimination (ee, cg);
}

/** Overloaded elimination for templates, here face elimination in line graph
   \returns The costs, not the resulting line graph node.

   Therefore, it can be used in template use_heuristic.
*/
inline int eliminate (line_graph_t::face_t f, line_graph_t& lg) {
  int k= face_elimination (f, -1, lg); 
  return was_non_trivial_elimination (f, k, lg);
}

/** Overloaded elimination for templates, here face elimination in line graph
   \returns The elimination costs.

   Therefore, it can be used in template use_heuristic.
   Since the inserted/absorbing node is overwritten in \p t it can be used in
   elimination_history_t too.
*/
inline int eliminate (triplet_t& t, line_graph_t& lg) {
  return face_elimination (t, lg);
}

// =========================================================================
// which vertices, edges or faces can be eliminated
// =========================================================================

/// Returns a set of vertices that can be eliminated from c-graph \p cg
int eliminatable_vertices (const c_graph_t& cg, 
			   vector<c_graph_t::vertex_t>& vv);

/** \brief Returns a set of vertices that can be eliminated from c-graph \p cg by edge elimination

    Besides intermediate vertices, dependent vertices with outgoing edges are included.
*/
int semi_eliminatable_vertices (const c_graph_t& cg, vector<c_graph_t::vertex_t>& vv);

/** \brief Returns a set of edges that can be eliminated from c-graph \p cg 

    In fact it only copies the edges into a vector for better treatment.
*/
int eliminatable_edges (const c_graph_t& cg, 
			vector<c_graph_t::edge_t>& ev);

/// Returns a set of edges that can be front eliminated from c-graph \p cg
int front_eliminatable_edges (const c_graph_t& cg, 
			      vector<c_graph_t::edge_t>& ev);

/// Returns a set of edges that can be back eliminated from c-graph \p cg
int back_eliminatable_edges (const c_graph_t& cg, 
			     vector<c_graph_t::edge_t>& ev);

/** \brief Returns a set of edges that can be eliminated from c-graph \p cg and how

    It copies the edges into a vector and distiguishes which edge can be front,
    (i.e. pair(e,true)) and which can be back-eliminated, i.e. pair(e,true).
    Edges can appear twice in the vector.
*/
int eliminatable_edges (const c_graph_t& cg,
			vector<edge_bool_t>& ev);

/** \brief Returns a set of edges that can be eliminated from c-graph \p cg and how

    It copies the edges into a vector and distiguishes which edge can be front,
    (i.e. (i,j,true)) and which can be back-eliminated, i.e. (i,j,true).
    Edges can appear twice in the vector.
*/
int eliminatable_edges (const c_graph_t& cg,
			vector<edge_ij_elim_t>& ev);

/// Returns a set of edges that can be eliminated from c-graph \p cg
unsigned int eliminatable_edges(const c_graph_t& cg, 
				vector<EdgeElim>& ev);

/// Returns a set of faces that can be eliminated from line graph \p lg
int eliminatable_faces (const line_graph_t& lg, 
			vector<line_graph_t::face_t>& fv);

/** \brief Returns a set of eliminatable faces as triplets \p tv from line graph \p lg

    The first and second value of each triplet gives the face and the third is -1.
    Thus there is no more information than eliminatable_faces 
    but vectors of triplets can be used in situations where vectors of faces cannot.
*/
inline int eliminatable_triplets (const line_graph_t& lg, vector<triplet_t>& tv) {
  vector<line_graph_t::face_t> fv;
  eliminatable_faces (lg, fv);
  tv.resize (0);
  for (size_t c= 0; c < fv.size(); c++) {
    int s= source (fv[c], lg), t= target (fv[c], lg);
    triplet_t tr (s, t, -1); tv.push_back (tr); }
  return tv.size();
}

// =========================================================================
// which vertices, edges or faces can be eliminated (overloaded versions)
// =========================================================================

/// Synonym of eliminatable_vertices for usage in templates \sa use_heuristics
inline int eliminatable_objects (const c_graph_t& cg, 
				 vector<c_graph_t::vertex_t>& vv) {
  return eliminatable_vertices (cg, vv);
}

/// Synonym of eliminatable_edges for usage in templates \sa use_heuristics
inline int eliminatable_objects (const c_graph_t& cg, 
				 vector<c_graph_t::edge_t>& ev) {
  return eliminatable_edges (cg, ev);
}

/// Synonym of eliminatable_edges for usage in templates \sa use_heuristics
inline int eliminatable_objects (const c_graph_t& cg,
				 vector<edge_bool_t>& ev) {
  return eliminatable_edges (cg, ev);
}

/// Synonym of eliminatable_edges for usage in templates \sa use_heuristics
inline int eliminatable_objects (const c_graph_t& cg,
				 vector<edge_ij_elim_t>& ev) {
  return eliminatable_edges (cg, ev);
}

/// Synonym of eliminatable_faces for usage in templates \sa use_heuristics
inline int eliminatable_objects (const line_graph_t& lg, 
				 vector<line_graph_t::face_t>& fv) {
  return eliminatable_faces (lg, fv);
}

/// Synonym of eliminatable_triplets for usage in templates \sa use_heuristics
inline int eliminatable_objects (const line_graph_t& lg, 
				 vector<triplet_t>& tv) {
  return eliminatable_triplets (lg, tv);
}



/** \brief Elimination history

    Stores a c- or line graph and an elimination sequence, where the
    application of the sequence to the original graph shall result 
    in this graph.
    This class is introduced to use vertex, edge, and face elimination in LSA
    (see sa.hpp).
*/
template <typename Ad_graph_t,
          typename El_spec_t>
class elimination_history_t {
private:

public:
  const Ad_graph_t             og;     ///< The original graph
  vector<El_spec_t>       seq;    ///< Elimination sequence
  Ad_graph_t                   cg;     ///< Current graph
  int                          ccosts; ///< Current costs (og -> cg)

  typedef Ad_graph_t  ad_graph_t;
  typedef El_spec_t   el_spec_t;

  /** \brief Default constructor

      Only for completeness, better not use.
  */
  elimination_history_t () : og () {}

  /** \brief Constructor with empty sequence
      \param _cg Graph, which is copied into \c og and \c cg
  */
  elimination_history_t (const Ad_graph_t& _cg) : 
    og (_cg), cg (_cg), ccosts (0) {
    THROW_EXCEPT_MACRO (!check (), consistency_exception, "Inconsistent input graph");}

  /** \brief Constructor
      \param _og Original line graph
      \param _seq Elimination sequence
      \param _cg Current line graph
      \param _ccosts Current costs
      In debug mode it is checked if application of \p _seq to \p _og yields \p _cg
  */
  elimination_history_t (const Ad_graph_t& _og, const vector<El_spec_t>& _seq,
			 const Ad_graph_t& _cg, int _ccosts= 0) :
    og (_og), seq (_seq), cg (_cg), ccosts (_ccosts) {
    THROW_EXCEPT_MACRO (!check (), consistency_exception, "Inconsistent input graphs");}

  /** \brief Constructor
      \param _og Original line graph
      \param _seq Elimination sequence

      Current line graph \p _cg is built by application of \p _seq to \p _og 
  */
  elimination_history_t (const Ad_graph_t& _og, const vector<El_spec_t>& _seq) :
    og (_og), seq (_seq) {
    THROW_EXCEPT_MACRO (!og.check (), consistency_exception, "Inconsistent input graph");
    bool seq_ok= rebuild_graph (); 
    THROW_EXCEPT_MACRO (!seq_ok, consistency_exception, "Inconsistent elimination sequence");
  }

  /// Copy constructor
  elimination_history_t (const elimination_history_t& eh) :
    og (eh.og), seq (eh.seq), cg (eh.cg), ccosts (eh.ccosts) {
    THROW_EXCEPT_MACRO (!og.check (), consistency_exception, "Inconsistent start graph");
    THROW_EXCEPT_MACRO (!cg.check (), consistency_exception, "Inconsistent current graph");
    THROW_EXCEPT_MACRO (!check_sequence(), consistency_exception, "Inconsistent elimination sequence");
  }

  /// Assign operator 
  elimination_history_t& operator= (const elimination_history_t& eh) {
    (Ad_graph_t&) og= eh.og; seq= eh.seq; cg= eh.cg; ccosts= eh.ccosts; 
    THROW_DEBUG_EXCEPT_MACRO (!check (), consistency_exception, "Inconsistent graphs after assignment");
    return *this; }

  /// Swap
  void swap (elimination_history_t& eh) {
    ((Ad_graph_t&) og).swap ((Ad_graph_t&) eh.og); 
    seq.swap (eh.seq); cg.swap (eh.cg); std::swap (ccosts, eh.ccosts); 
    THROW_DEBUG_EXCEPT_MACRO (!check (), consistency_exception, "Inconsistent graphs after swapping");}

  /** \brief Eliminate \p el from cg
      \sa eliminate_object

      It calls eliminate_objects (el, cg). The first parameter of eliminate_objects
      may be a non-const reference, which is for instance useful for triplet_t to return
      the line graph node inserted or where the absorption took place. The return value
      of eliminate_objects (el, cg) must be the elimination costs. Costs greater than zero
      are assumed as successful elimination (trivial eliminations are not considered at
      this point) and \p el is appended to seq. If \p el is passed as non-const reference
      its value after the elimination is appended.
  */
  int elimination (El_spec_t el) {
    int costs= eliminate (el, cg); 
    if (costs > 0) {seq.push_back (el); ccosts+= costs; }
    return costs; }

  /// Checks if og --seq--> cg
  bool check_sequence () const {
    Ad_graph_t og_copy (og);
    int costs= 0;
    for (size_t c= 0; c < seq.size(); c++) {
      El_spec_t el= seq[c]; // copied from seq because some eliminations change el
      int el_costs= eliminate (el, og_copy); 
      if (el_costs == 0) {      
	cout << "check_sequence failed";
	write_vector (" with elimination sequence", seq);
	cout << " at " << c << "th entry, which is " << seq[c] << std::endl;
	write_graph ("at this moment the graph is", og_copy);
	cout << "it is " << (og_copy.check() ? "" : "not ") << "consistent\n";
	write_vector ("complete elimination sequence is", seq);
	return false; }
      else costs+= el_costs; }
    if (cg != og_copy) {
      cout << "check_sequence failed because of different resulting graphs.\n";
      write_graph ("current graph is", cg);
      write_graph ("seq applied to og results in", og_copy); 
      write_graph ("original graph was", og);
      write_vector ("elimination sequence is", seq);
      return false; }
    if (ccosts != costs) {
      cout << "check_sequence failed because of different elimination costs.\n";
      cout << "current costs are " << ccosts << std::endl;
      cout << "seq applied to og requires " << costs << std::endl;
      write_graph ("original graph was", og);
      write_vector ("elimination sequence is", seq);
      write_graph ("current graph is", cg);
      return false; }
    return true; 
  }

  /// Checks if og --seq--> cg and consistency of both graphs
  bool check () const {
    if (!og.check ()) return false; // original graph inconsistent
    if (!cg.check ()) return false; // current graph inconsistent
    return check_sequence (); }

  /// Rebuild cg from og and seq
  bool rebuild_graph () {
    Ad_graph_t og_copy (og);
    int        costs= 0;
    for (size_t c= 0; c < seq.size(); c++) {
      int el_costs= eliminate (seq[c], og_copy);
      if (el_costs == 0) return false; else costs+= el_costs; }
    cg= og_copy; ccosts= costs; return true; }

  /** \brief Complete sequence by heuristic \p h

      In SA applications \p h must be the same heuristic used cost operator
      (ANGEL cannot check this).
  */
  template <typename Heuristic_t>
  void complete_sequence (Heuristic_t h) {
    vector<El_spec_t>    v1, v2;
    for (eliminatable_objects (cg, v1); v1.size() > 0; eliminatable_objects (cg, v1)) {
      v2.resize (0); h (v1, cg, v2); elimination (v2[0]); }}
};

/** \brief Vertex elimination history for LSA usage.
    \sa elimination_history_t
*/
typedef elimination_history_t<c_graph_t, c_graph_t::vertex_t> vertex_elimination_history_t;
/** \brief Edge elimination history for LSA usage.
    \sa elimination_history_t
*/
typedef elimination_history_t<c_graph_t, edge_ij_elim_t>      edge_elimination_history_t;
/** \brief Face elimination history for LSA usage.
    \sa elimination_history_t
*/
typedef elimination_history_t<line_graph_t, triplet_t>        face_elimination_history_t;

/// Converts vertex elimination sequence into (mixed) edge elimination sequence
bool convert_elimination_sequence (const vector<c_graph_t::vertex_t>& vv, 
				   const c_graph_t& cg,
				   vector<edge_ij_elim_t>& ev);

/// Converts (mixed) edge elimination sequence into face elimination sequence
bool convert_elimination_sequence (const vector<edge_ij_elim_t>& ev,
				   const line_graph_t& lg,
				   vector<triplet_t>& tv);

#ifdef USEXAIFBOOSTER

EdgeRefType_E getRefType (const c_graph_t::edge_t e,
			  const c_graph_t& angelLCG,
			  const list<EdgeRef_t>& edge_ref_list);

const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge* getLCG_p (const c_graph_t::edge_t e,
                                                                                    const c_graph_t& angelLCG,
                                                                                    const list<EdgeRef_t>& edge_ref_list);

xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex* getJAE_p (const c_graph_t::edge_t e,
                                                                                  const c_graph_t& angelLCG,
                                                                                  const list<EdgeRef_t>& edge_ref_list);

void setJaevRef (const c_graph_t::edge_t e,
                 xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex& jaev,
		 const c_graph_t& angelLCG,
		 const list<EdgeRef_t>& edge_ref_list);
	
void removeRef (const c_graph_t::edge_t e,
		const c_graph_t& angelLCG,
		list<EdgeRef_t>& edge_ref_list);

/** Multiply a contiguous pair of edges in an angel c_graph_t and create a new
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpression.

    \param e1 the first edge. 
    \param e2 the second edge (its source is the target of \p e1). 
    \param angelLCG the c_graph_t (passed by reference) that the operation is performed on.
    \param edge_ref_list the stl list container that keeps track of the reference
           (LCG pointer or JAE pointer) for each edge in angelLCG.
    \param jae_list the xaifBooster JAE list that we construct incrementally
           as we perform eliminations.
    \return The cost (in terms of multiplications) of the elimination.

    If there's fill-in, a new edge is created and a new edge reference points it to the new JAE.
    If there's absorption, the existing edge has its reference updated to point to the new JAE.
 */
unsigned int multiply_edge_pair_directly (const c_graph_t::edge_t e1,
					  const c_graph_t::edge_t e2,
					  c_graph_t& angelLCG,
					  list<EdgeRef_t>& edge_ref_list,
                                          xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& jae_list);

/** Front eliminate an edge from an angel c_graph_t but go directly to a
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpression, rather
    than the internal angel accumulation graph type.

    \param e the edge that will be front eliminated. 
    \param angelLCG the c_graph_t (passed by reference) that the elimination is performed on.
    \param edge_ref_list the stl list container that keeps track of the reference
           (LCG pointer or JAE pointer) for each edge in angelLCG.
    \param jae_list the xaifBooster JAE list that we construct incrementally
           as we perform eliminations.
    \return The cost (in terms of multiplications) of the elimination.

    This function is also aware of unit and constant edges.  This entails labeling
    new edges with the appropriate type, as well as determining the cost appropriately.
 */
unsigned int front_eliminate_edge_directly (c_graph_t::edge_t e,
					    c_graph_t& angelLCG,
					    list<EdgeRef_t>& edge_ref_list,
                                            xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& jae_list);

/** Back eliminate an edge from an angel c_graph_t but go directly to a
    xaifBoosterCrossCountryInterface::JacobianAccumulationExpression, rather
    than the internal angel accumulation graph type.

    \param e the edge that will be back eliminated.
    \param angelLCG the c_graph_t (passed by reference) that the elimination is performed on.
    \param edge_ref_list the stl list container that keeps track of the reference
           (LCG pointer or JAE pointer) for each edge in angelLCG.
    \param jae_list the xaifBooster JAE list that we construct incrementally
           as we perform eliminations.
    \return The cost (in terms of multiplications) of the elimination.

    This function is also aware of unit and constant edges.  This entails labeling
    new edges with the appropriate type, as well as determining the cost appropriately.
 */
unsigned int back_eliminate_edge_directly (c_graph_t::edge_t e,
					   c_graph_t& angelLCG,
					   list<EdgeRef_t>& edge_ref_list,
                                           xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionList& jae_list);

unsigned int pair_elim (c_graph_t::edge_t e1,
			c_graph_t::edge_t e2,
			c_graph_t& angelLCG,
			const elimSeq_cost_t& currentElimSeq,
			refillDependenceMap_t& refillDependences);

unsigned int front_elim (c_graph_t::edge_t e,
			 c_graph_t& angelLCG,
			 const elimSeq_cost_t& currentElimSeq,
			 refillDependenceMap_t& refillDependences);

unsigned int back_elim (c_graph_t::edge_t e,
			c_graph_t& angelLCG,
			const elimSeq_cost_t& currentElimSeq,
			refillDependenceMap_t& refillDependences);

unsigned int pairElim_noJAE (c_graph_t::edge_t e1,
			     c_graph_t::edge_t e2,
			     c_graph_t& angelLCG,
			     const transformationSeq_cost_t* currentTransformationSequence,
			     refillDependenceMap_t& refillDependences);

unsigned int frontEdgeElimination_noJAE (c_graph_t::edge_t e,
					 c_graph_t& angelLCG,
					 const transformationSeq_cost_t* currentTransformationSequence,
					 refillDependenceMap_t& refillDependences);

unsigned int backEdgeElimination_noJAE (c_graph_t::edge_t e,
					c_graph_t& angelLCG,
					const transformationSeq_cost_t* currentTransformationSequence,
					refillDependenceMap_t& refillDependences);

#endif // USEXAIFBOOSTER

} // namespace angel

#endif  // _eliminations_include_

