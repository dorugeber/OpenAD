// $Id: angel_types.hpp,v 1.26 2008/02/28 16:21:08 gottschling Exp $ 
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#ifndef 	_angel_types_include_
#define 	_angel_types_include_

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

#define BOOST_NO_HASH // gets rid of hash_set deprecation warnings until boost fixes its code
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/property_map/property_map.hpp"

#include <map>
#include <set>
#ifdef USEXAIFBOOSTER
#include "xaifBooster/algorithms/CrossCountryInterface/inc/LinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/JacobianAccumulationExpressionList.hpp"
#include "xaifBooster/algorithms/CrossCountryInterface/inc/GraphCorrelations.hpp"
#endif // USEXAIFBOOSTER

#include "angel_exceptions.hpp"

// =====================================================
// c-graph
// =====================================================

namespace angel {

/// Vertex type for vertex_t in c_graph_t and edge_t in line_graph_t
enum vertex_type_t {independent,   ///< Independent vertex
		    intermediate,  ///< Non-empty vertex neither independent nor dependent
		    dependent,     ///< Independent vertex
		    dead_vertex,   ///< Empty vertex, should not be dependent or independent
		    undefined_vertex ///< Undefined, e.g. out of range
};

enum Edge_Type_E { VARIABLE_EDGE,
		   UNIT_EDGE,
		   CONSTANT_EDGE };

struct EdgeType { 
  enum { num = 129 };
  typedef boost::edge_property_tag kind;
}; // end struct

// edge properties
typedef boost::property<boost::edge_weight_t, int>			edge_weight_property;
typedef boost::property<boost::edge_index_t, int, edge_weight_property>	edge_index_weight_property;
typedef boost::property<EdgeType, int, edge_index_weight_property>	edge_type_index_weight_property;

/// Pure BGL type definition of c-graph
//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, 
//			boost::no_property, edge_type_index_weight_property> pure_c_graph_t;

struct VertexVisited { 
  //enum { num = 128 };
  typedef boost::vertex_property_tag kind;
}; // end struct

// vertex visited property (for reachability queries)
typedef boost::property<VertexVisited, bool>				vertex_visited_property;

/// Pure BGL type definition of c-graph
typedef boost::adjacency_list<boost::vecS, 			// OutEdgeList
			      boost::vecS,			// VertexList
			      boost::bidirectionalS, 		// Directed
			      vertex_visited_property,		// VertexProperties
			      edge_type_index_weight_property>	// EdgeProperties
  pure_c_graph_t;

// some forward declarations
class graph_package_t; 
class accu_graph_t; 
class edge_address_t; 

/// C-graph type
class c_graph_t: public pure_c_graph_t	{
private:
    int           X;   // number of independent variables
public:
  /// Type of basic type from BGL
  typedef pure_c_graph_t                                                  pure_graph_t;
  /// Vertex type 
  typedef pure_c_graph_t::vertex_descriptor                               vertex_t;
  /// Edge type 
  typedef pure_c_graph_t::edge_descriptor                                 edge_t;
  /// Vertex iterator type 
  typedef boost::graph_traits<pure_c_graph_t>::vertex_iterator            vi_t;
  /// Edge iterator type 
  typedef boost::graph_traits<pure_c_graph_t>::edge_iterator              ei_t;
  /// Iterator type of a vertex's incoming edges
  typedef boost::graph_traits<pure_c_graph_t>::in_edge_iterator           iei_t;
  /// Iterator type of a vertex's outgoing edges
  typedef boost::graph_traits<pure_c_graph_t>::out_edge_iterator          oei_t;
  /// Type of property edge label for const c_graph_t
  typedef boost::property_map<pure_c_graph_t, boost::edge_weight_t>::const_type	const_ew_t;
  /// Type of property edge label for non-const c_graph_t
  typedef boost::property_map<pure_c_graph_t, boost::edge_weight_t>::type	ew_t;
  /// Type of property edge index for const c_graph_t
  typedef boost::property_map<pure_c_graph_t, boost::edge_index_t>::const_type	const_eind_t;
  /// Type of property edge index for non-const c_graph_t
  typedef boost::property_map<pure_c_graph_t, boost::edge_index_t>::type	eind_t;
  /// Type of property edge isUnit for const c_graph_t
  typedef boost::property_map<pure_c_graph_t, EdgeType>::const_type		const_etype_t;
  /// Type of property edge isUnit for non-const c_graph_t
  typedef boost::property_map<pure_c_graph_t, EdgeType>::type			etype_t;

  int          next_edge_number;   ///< useful for insertion of new edges

  std::vector<vertex_t>   dependents;   ///< set of dependent vertices

  /// Default constructor
  c_graph_t () : 
    pure_c_graph_t (), X (0), next_edge_number (0) {}

  /** \brief Constructor with vertex specification
      \param V_ number of all vertices
      \param X_ number of independent vertices
      \param deps set of dependent vertices
  */
  c_graph_t (int V_, int X_, const std::vector<vertex_t>& deps) :
      pure_c_graph_t (V_), X (X_), next_edge_number (0), dependents (deps) {
    // rem. in basic blocks vertex may be both dependent and independent
    #ifndef NDEBUG
      // assert (X >= 0 && X < V_); // X==0 is usefull in graph construction
      THROW_EXCEPT_MACRO (X < 0 && X > V_, consistency_exception, "X inconsistent");
      for (size_t c= 0; c < dependents.size(); c++)
	// assert (dependents[c] < (vertex_t) V_);
	THROW_EXCEPT_MACRO (dependents[c] >= (vertex_t) V_, consistency_exception, "dependents inconsistent");
    #endif
  }

  /** \brief Constructor with vertex specification
      \param X_ number of independent vertices
      \param Z_ number of intermediated vertices
      \param Y_ number of dependent vertices, will be located at the end
  */
  c_graph_t (int X_, int Z_, int Y_) : 
       pure_c_graph_t (X_+Y_+Z_), X (X_), next_edge_number (0) {
    // last Y vertices are dependent if nothing else is specified
    vi_t      vi, v_end;
    tie(vi, v_end)= vertices(*this);
    for (int c= 0; c < X_+Z_; c++, ++vi);
    for (; vi != v_end; ++vi)
      dependents.push_back (*vi);
  }

  /// Copy constructor
  c_graph_t (const c_graph_t& _g) : 
    pure_c_graph_t (_g), X (_g.X), 
    next_edge_number (_g.next_edge_number), dependents (_g.dependents) {}
  
  /// Assign operator 
  c_graph_t& operator= (const c_graph_t& _g) { 
    clear_edges ();
    pure_c_graph_t::operator= (_g); X= _g.X;
    next_edge_number= _g.next_edge_number;
    dependents= _g.dependents;
    return *this; }

  /// Swap
  void swap (c_graph_t& _g) {
    pure_c_graph_t::swap (_g);
    std::swap (X, _g.X); 
    std::swap (next_edge_number, _g.next_edge_number); dependents.swap (_g.dependents); }

  int x () const {return X;}                           ///< Number of independent variables
  void x (int x) { X=x;}                               ///< Number of independent variables
  int y () const {return (int) dependents.size();}     ///< Number of dependent variables
  int v () const {return (int) num_vertices(*this);}   ///< Number of variables 
  int z () const {return v()-x()-y();}                 ///< Number of intermediate vertices

  /// Vertex type of \p v
  enum vertex_type_t vertex_type (vertex_t ve) const {
    if (int (ve) >= v()) return undefined_vertex;
    if (ve < (vertex_t) X) return independent;
    if (std::find (dependents.begin(), dependents.end(), ve) != dependents.end()) return dependent;
    return in_degree (ve, *this) == 0 && out_degree (ve, *this) == 0 ? dead_vertex : intermediate; }

  /// Check consistency of graph
  bool check () const;
  /// Check consistency of initial graph
  bool check_initial () const;
  /// Change dependent vertices with successor into intermediate vertices
  void remove_dependents_with_successors ();

  /** \brief Removes edges explicitly
   *  \note It seems that BGL does not remove all edges in assignment.
   */
  void clear_edges () {
    vi_t vi, v_end;
    for (tie (vi, v_end)= vertices (*this); vi != v_end; ++vi)
      clear_vertex (*vi, *this); }

  /// Remove all vertices that are not connected to dependent and inpedendent vertices
  void clear_graph ();

  friend int read_graph_eliad (const std::string& file_name, c_graph_t& cg, bool retry);
  friend void stats2block (int inputs, int outputs, const std::vector<c_graph_t>& stats, 
			   c_graph_t& block);
  friend void block2loop (const c_graph_t& block, int loops,
			  c_graph_t& loop);
  friend void unpack_graph (const graph_package_t& gp, c_graph_t& cg);

#ifdef USEXAIFBOOSTER
  friend void read_graph_xaif_booster (const xaifBoosterCrossCountryInterface::LinearizedComputationalGraph& xg, 
				       c_graph_t& cg,
				       std::vector<const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphVertex*>& av,
				       std::vector<edge_address_t>& ev);

#endif // USEXAIFBOOSTER

};

/// Compares two c-graphs
bool operator== (const c_graph_t& g1, const c_graph_t& g2);

/// Compares two c-graphs
inline bool operator!= (const c_graph_t& g1, const c_graph_t& g2) {
  return !(g1 == g2); }

/// Markowitz degree of all vertices in \p cg
int overall_markowitz_degree (const c_graph_t& cg);

  /// Whether \p cg can be transformed into bipartite graph by vertex eliminations
  inline bool vertex_eliminatable (const c_graph_t& cg) {
    for (size_t c= 0; c < cg.dependents.size(); c++)
      if (out_degree (cg.dependents[c], cg) > 0) return false;
    return true;
  }

/// Pair of c-graph edge and boolean to specify an edge elimination 
typedef std::pair<c_graph_t::edge_t,bool> edge_bool_t;

// =====================================================
// line graph
// =====================================================


typedef std::pair<int, int>                                  ad_edge_t;
typedef boost::property<boost::vertex_degree_t, int>                              vertex_degree_property;
typedef boost::property<boost::vertex_name_t, ad_edge_t, vertex_degree_property>  vertex_name_degree_property;

/// Pure BGL type definition of line graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, 
                       vertex_name_degree_property, // edges from c-graph and their label
                       boost::no_property> pure_line_graph_t; 

/** \brief Line graph type

    Type names are related to the c-graph. For instance, a vertex of the line graph is 
    defined as edge_t (since it represent an edge). The types c_graph_t::edge_t and
    line_graph_t::edge_t are not compatible (see edge_vertex_name and find_edge).
*/
class line_graph_t: public pure_line_graph_t {
private:
  int X;        // number of edges (X-, X)
  bool cons_ok; // is consistent enumeration up to date
public:
  /// Type of basic type from BGL
  typedef pure_line_graph_t                                            pure_graph_t;
  /// Edge type 
  typedef pure_line_graph_t::vertex_descriptor                         edge_t;
  /// Face type 
  typedef pure_line_graph_t::edge_descriptor                           face_t;
  /// Edge iterator type 
  typedef boost::graph_traits<pure_line_graph_t>::vertex_iterator             ei_t;
  /// Face iterator type 
  typedef boost::graph_traits<pure_line_graph_t>::edge_iterator               fi_t;
  /// Iterator type of a edge's incoming faces
  typedef boost::graph_traits<pure_line_graph_t>::in_edge_iterator            ifi_t;
  /// Iterator type of a edge's outgoing faces
  typedef boost::graph_traits<pure_line_graph_t>::out_edge_iterator           ofi_t;
  /// Type of property edge label for const line_graph_t
  typedef boost::property_map<pure_line_graph_t, boost::vertex_degree_t>::const_type const_ed_t;
  /// Type of property edge label for non-const line_graph_t
  typedef boost::property_map<pure_line_graph_t, boost::vertex_degree_t>::type       ed_t;
  /** Type of edge-vertex-name property for const line_graph_t

      This means an edge from line graph corresponds/belongs to an edge in c-graph, which
      is represented as two integers for the source and the target vertex
  */
  typedef boost::property_map<pure_line_graph_t, boost::vertex_name_t>::const_type   const_evn_t;
  /** Type of edge-vertex-name property for non-const line_graph_t

      This means an edge from line graph corresponds/belongs to an edge in c-graph, which
      is represented as two integers for the source and the target vertex
  */
  typedef boost::property_map<pure_line_graph_t, boost::vertex_name_t>::type         evn_t;

  std::vector<edge_t>   dependents;   ///< set of dependent vertices

  int x () const {return X;}                   ///< Number of independent vertices
  int y () const {return dependents.size();}   ///< Number of dependent vertices
  int v () const {return (int) num_vertices(*this);}  ///< Number of all vertices, including empty 
  int z () const {return v()-x()-y();}         ///< Number of intermediate vertices

  const c_graph_t*  cgp;   ///< Pointer to corresponding c-graph

  /// Default constructor
  line_graph_t () : X (0), cons_ok (false), cgp (0) {}

  /** \brief Constructor with vertex specification
      \param V_ number of all vertices
      \param X_ number of independent vertices
      \param deps set of dependent vertices
  */
  line_graph_t (int V_, int X_, const std::vector<edge_t>& deps) :
      pure_line_graph_t (V_), X (X_), cons_ok (false), dependents (deps), cgp (0) {
    // rem. in basic blocks vertex may be both dependent and independent
    #ifndef NDEBUG
      // assert (X >= 0 && X < V_); // X==0 is usefull in graph construction
      THROW_EXCEPT_MACRO (X < 0 && X > V_, consistency_exception, "X inconsistent");
      for (size_t c= 0; c < dependents.size(); c++)
	// assert (dependents[c] < (edge_t) V_);
	THROW_EXCEPT_MACRO (dependents[c] >= (edge_t) V_, consistency_exception, "dependents inconsistent");
    #endif
  }

  /// Constructor that builds line graph from c-graph
  line_graph_t (const c_graph_t& cg); 

  /// Copy constructor
  line_graph_t (const line_graph_t& _g) : 
    pure_line_graph_t (_g), X (_g.X), cons_ok (_g.cons_ok),
    dependents (_g.dependents), cgp (_g.cgp) {}

  /// Destructor
  ~line_graph_t () {clear_edges ();}

  /// Assign operator 
  line_graph_t& operator= (const line_graph_t& _g) { 
    clear_edges ();
    pure_line_graph_t::operator= (_g); X= _g.X; cons_ok= _g.cons_ok; cgp= _g.cgp; 
    dependents= _g.dependents; // copy_properties (_g); 
    return *this; }
   
  /*! \brief Swap
   \note Properties not swapped properly!
  */
  void swap (line_graph_t& _g) {
    pure_line_graph_t::swap (_g);
    std::swap (X, _g.X); std::swap (cons_ok, _g.cons_ok); std::swap (cgp, _g.cgp); 
    dependents.swap (_g.dependents); }

  // assumes that graph is okay, use check to verify
  /// Type of edge \p e
  enum vertex_type_t vertex_type (edge_t e) const {
    if (int (e) >= v()) return undefined_vertex;
    return in_degree (e, *this) == 0 ? (out_degree (e, *this) == 0 ? dead_vertex : independent)
      : (out_degree (e, *this) == 0 ? dependent : intermediate); }
 
  /// Copies properties explicitly
  void copy_properties (const line_graph_t& _g);
    
  /** \brief Removes edges explicitly
   *  \note It seems that BGL does not remove all edges in assignment.
   */
  void clear_edges () {
    ei_t ei, e_end;
    for (tie (ei, e_end)= vertices (*this); ei != e_end; ++ei)
      clear_vertex (*ei, *this); }

  /// Remove all vertices that are not connected to dependent and inpedendent vertices
  void clear_graph ();

  /// Check consistency of graph
  bool check () const;

  /// Whether graph is tripartite
  bool is_tripartite () const;

  friend int face_elimination (face_t face, int kr, line_graph_t& lg, accu_graph_t& ac);
  friend void unpack_graph (const graph_package_t& gp, line_graph_t& lg);
};

/** \brief Replaces edge function of BGL
    \param u Source
    \param v Target
    \param g Graph
    \return Face (u, v)
    \note BGL function crashes if u or v is too large for g
*/
template <typename Ad_graph_t>
std::pair<typename Ad_graph_t::edge_descriptor, bool> inline
edge (typename Ad_graph_t::vertex_descriptor u, typename Ad_graph_t::vertex_descriptor v, 
      const Ad_graph_t& g) {
  if (u < num_vertices (g) && v < num_vertices (g))
    return boost::edge (u, v, (const Ad_graph_t&) g);
  else {
    typename Ad_graph_t::edge_descriptor e; return std::make_pair (e, false); }
}

/** \brief Vertex pair representation of an edge in line graph
    \param e Edge from line graph
    \param lg Line graph
    \param i Vertex number of edge source in c-graph (output)
    \param j Vertex number of edge target in c-graph (output)
 */
inline void edge_vertex_name (line_graph_t::edge_t e, const line_graph_t& lg,
			      int& i, int& j) {
  line_graph_t::const_evn_t evn= get(boost::vertex_name, lg); 
  i= evn[e].first; j= evn[e].second;
}
// edge name was already declared

/** \brief Vertex triplet representation of a face
    \param f face
    \param lg Line graph
    \param i Vertex number of face source in c-graph (output)
    \param j Vertex number of face center in c-graph (output)
    \param k Vertex number of face target in c-graph (output)
 */
inline void face_vertex_name (line_graph_t::face_t f, const line_graph_t& lg,
			      int& i, int& j, int& k) {
  line_graph_t::const_evn_t evn= get(boost::vertex_name, lg); 
  line_graph_t::edge_t   ij= source (f, lg), jk= target (f, lg);
  i= evn[ij].first, j= evn[ij].second, k= evn[jk].second;
  THROW_DEBUG_EXCEPT_MACRO (j != evn[jk].first, consistency_exception,
			 "Adjacency corrupted in line graph"); 
}

/// Compares two line graphs
bool operator== (const line_graph_t& g1, const line_graph_t& g2);

/// Compares two line graphs
inline bool operator!= (const line_graph_t& g1, const line_graph_t& g2) {
  return !(g1 == g2); }

/// Markowitz degree of all vertices
int overall_markowitz_degree (const line_graph_t& lg);

/** \brief Markowitz 
    \param j Vertex number in c-graph
    \param lg Line graph
    \return j's Markowitz degree, i.e. number of faces like (*, j, *)
*/
int markowitz_degree (int j, const line_graph_t& lg);


// =====================================================
// triplet type
// =====================================================

/// Triplet of faces, used in face_elimination_history_t
struct triplet_t {
  int  i, j, k;
  triplet_t (int _i, int _j, int _k): i (_i), j (_j), k (_k) {}
  triplet_t (): i (-1), j (-1), k (-1) {}
};

/// Output operator of triplet_t
inline std::ostream& operator<< (std::ostream& stream, const triplet_t& t) {
  return stream << "(" << t.i << ", " << t.j << ", " << t.k << ")"; }


// =====================================================
// predecessor and successor type
// =====================================================

template <typename Ad_graph_t>
class predecessor_t {
public:
  typedef typename Ad_graph_t::vertex_descriptor                        vd_t;
  typedef typename Ad_graph_t::edge_descriptor                          ed_t;
  typedef typename boost::graph_traits<Ad_graph_t>::vertex_iterator     vi_t;
  typedef typename boost::graph_traits<Ad_graph_t>::edge_iterator       gei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::in_edge_iterator    ei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::out_edge_iterator   rei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::degree_size_type    ds_t;
private:
  std::vector<vd_t> independents;
public:
  const Ad_graph_t& adg;

  predecessor_t (const Ad_graph_t& _adg) : adg (_adg) {
    vi_t      vi, v_end;
    tie (vi, v_end)= vertices (adg);
    for (int c= 0; c < adg.x(); c++, vi++)
      independents.push_back(*vi);
  }

  ds_t degree (vd_t v) const {return in_degree (v, adg); }

  std::pair<ei_t, ei_t> edges (vd_t v) const {return in_edges (v, adg); }

  vd_t neighbor (ed_t e) const {return source (e, adg); }

  vd_t neighbor (ei_t ei) const {return source (*ei, adg); }

  ds_t rdegree (vd_t v) const {return out_degree (v, adg); }

  std::pair<rei_t, rei_t> redges (vd_t v) const {return out_edges (v, adg); }

  vd_t rneighbor (ed_t e) const {return target (e, adg); }

  vd_t rneighbor (rei_t rei) const {return target (*rei, adg); }

  const std::vector<vd_t>& first () const {return adg.dependents; }

  const std::vector<vd_t>& last () const {return independents; }

  void clear_vertices (const std::vector<vd_t>& vv) {
    for (size_t c= 0; c < vv.size(); c++) 
      clear_vertex (vv[c], (Ad_graph_t&) adg); }
};


template <typename Ad_graph_t>
class successor_t {
public:
  typedef typename Ad_graph_t::vertex_descriptor                        vd_t;
  typedef typename Ad_graph_t::edge_descriptor                          ed_t;
  typedef typename boost::graph_traits<Ad_graph_t>::vertex_iterator     vi_t;
  typedef typename boost::graph_traits<Ad_graph_t>::edge_iterator       gei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::out_edge_iterator   ei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::in_edge_iterator    rei_t;
  typedef typename boost::graph_traits<Ad_graph_t>::degree_size_type    ds_t;
private:
  std::vector<vd_t> independents;
public:
  const Ad_graph_t& adg;

  successor_t (const Ad_graph_t& _adg) : adg (_adg) {
    vi_t      vi, v_end;
    tie (vi, v_end)= vertices (adg);
    for (int c= 0; c < adg.x(); c++, vi++)
      independents.push_back(*vi);
  }

  ds_t degree (vd_t v) const {return out_degree (v, adg); }

  std::pair<ei_t, ei_t> edges (vd_t v) const {return out_edges (v, adg); }

  vd_t neighbor (ed_t e) const {return target (e, adg); }

  vd_t neighbor (ei_t ei) const {return target (*ei, adg); }

  ds_t rdegree (vd_t v) const {return in_degree (v, adg); }

  std::pair<rei_t, rei_t> redges (vd_t v) const {return in_edges (v, adg); }

  vd_t rneighbor (ed_t e) const {return source (e, adg); }

  vd_t rneighbor (rei_t rei) const {return source (*rei, adg); }

  const std::vector<vd_t>& first () const {return independents; }

  const std::vector<vd_t>& last () const {return adg.dependents; }

  void clear_vertices (const std::vector<vd_t>& vv) {
    for (size_t c= 0; c < vv.size(); c++) 
      clear_vertex (vv[c], (Ad_graph_t&) adg); }
};


// -------------------------------------------------------------------------
// elimination sequences of edges in compute graph
// -------------------------------------------------------------------------

// additional data structures
// --------------------------

/// Edge \p edge to eliminate from c-graph and whether it should be front or back eliminated
struct edge_elim_t {
  c_graph_t::edge_t    edge;
  bool                 front;
};

/// Edge from vertex \p i to vertex \p j to eliminate from c-graph and whether 
/// it should be front or back eliminated
struct edge_pair_elim_t {
  c_graph_t::vertex_t  i, j;
  bool                 front;
};

/// Output operator of edge_pair_elim_t
inline std::ostream& operator<< (std::ostream& stream, const edge_pair_elim_t& ee) {
  return stream << "((" << ee.i << ", " << ee.j << "), " << ee.front << ")"; }

/// Edge from vertex with number \p i to vertex with number \p j to eliminate 
/// from c-graph and whether it should be front or back eliminated
struct edge_ij_elim_t {
  int                           i, j;
  bool                          front;
  edge_ij_elim_t (int _i, int _j, bool _front) : i(_i), j(_j), front(_front) {}
  edge_ij_elim_t () : i(0), j(0), front(false) {} // only for STL
};

/// Output operator of edge_ij_elim_t
inline std::ostream& operator<< (std::ostream& stream, const edge_ij_elim_t& ee) {
  return stream << "((" << ee.i << ", " << ee.j << "), " << ee.front << ")"; }

/// Sequences of edges from the compute graph with information which edges shall be
/// front eliminate
typedef std::vector<edge_elim_t>       edge_elim_seq_t;

/// Sequences of pairs of vertices from the compute graph with information which edges shall be
/// front eliminate
typedef std::vector<edge_pair_elim_t>  edge_pair_elim_seq_t;

/// Sequences of pairs of vertex numbers with information which edges shall be
/// front eliminate
typedef std::vector<edge_ij_elim_t>    edge_ij_elim_seq_t;

// enum accu_op_t {accu_noop, accu_add, accu_mult};

struct accu_exp_t {
  enum ref_kind_t {nothing, exp, lgn, isop};
  enum op_t {add, mult};
  union ref_t {
    line_graph_t::edge_t   node;
    int                    exp_nr;
    op_t                   op; };

  ref_t ref;
  ref_kind_t ref_kind;

  void set_exp (int _exp_nr) {ref_kind= exp; ref.exp_nr= _exp_nr; }
  void set_node (line_graph_t::edge_t _node) {ref_kind= lgn; ref.node= _node; }
  void set_op (op_t _op) {ref_kind= isop; ref.op= _op; }
  // accu_exp_t::ref_kind_t get_ref_kind () const {return ref_kind; }
  // accu_exp_t () : line_graph_node (0), exp_nr (0), op (accu_noop) {} // to sedate STL
  // accu_exp_t (line_graph_t::edge_t l, int e, accu_op_t o) : 
  //                      line_graph_node (l), exp_nr (e), op (o) {}
};

inline std::ostream& operator<< (std::ostream& stream, const accu_exp_t& exp) {
  switch (exp.ref_kind) {
  case accu_exp_t::nothing: stream << "nothing"; break;
  case accu_exp_t::exp:     stream << "expression #" << exp.ref.exp_nr; break;
  case accu_exp_t::lgn:     stream << "line_graph_node #" << exp.ref.node; break;
  case accu_exp_t::isop:    stream << "op " << (exp.ref.op == accu_exp_t::add ? "add" : "mult"); }
  return stream; }

typedef boost::property<boost::vertex_name_t, accu_exp_t>     accu_exp_property;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, 
                       accu_exp_property, // edges from c-graph and operation
                       boost::no_property> pure_accu_exp_graph_t; 

class accu_exp_graph_t : public pure_accu_exp_graph_t {
  int X;
public:
  void set_graph (line_graph_t::edge_t e_out, line_graph_t::edge_t e1,
		    line_graph_t::edge_t e2, std::vector<int> exp_nr); 
  void set_graph (accu_exp_t::op_t op, line_graph_t::edge_t e1, line_graph_t::edge_t e2,
		    std::vector<int> exp_nr);
  void set_graph (line_graph_t::edge_t edge);
  std::vector<pure_accu_exp_graph_t::vertex_descriptor>  dependents;
  int x () const {return X; }
  int y () const {return int (dependents.size()); }
  int v () const {return (int) num_vertices(*this);}
  int z () const {return v()-x()-y();}         
};

struct accu_graph_t {
  const c_graph_t&                                            cg;
  const line_graph_t&                                         lg;
  std::vector<accu_exp_graph_t>                               accu_exp;
  std::vector<ad_edge_t>                                      jacobi_entries;
  std::vector<int>                                            exp_nr;

  accu_graph_t (const c_graph_t& _cg, const line_graph_t& _lg) : cg (_cg), lg (_lg),
    exp_nr (lg.v(), -1) {}
  // accu_graph_t (const c_graph_t& _cg) : cg (_cg), lg (_cg), exp_nr (lg.v(), -1) {}
  void set_jacobi_entries ();
};

#ifdef USEXAIFBOOSTER
enum EdgeRefType_E {LCG_EDGE,
                    JAE_VERT,
                    UNDEFINED};

struct EdgeRef_t {
  c_graph_t::edge_t my_angelLCGedge;
  EdgeRefType_E my_type;
  const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge* my_LCG_edge_p;
  xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex* my_JAE_vertex_p;

  EdgeRef_t (c_graph_t::edge_t _e,
             const xaifBoosterCrossCountryInterface::LinearizedComputationalGraphEdge* _LCGedge_p) :
    my_angelLCGedge(_e),
    my_type(LCG_EDGE),
    my_LCG_edge_p(_LCGedge_p),
    my_JAE_vertex_p(NULL) {}

  EdgeRef_t (c_graph_t::edge_t _e,
             xaifBoosterCrossCountryInterface::JacobianAccumulationExpressionVertex* _JAEvert_p) :
    my_angelLCGedge(_e),
    my_type(JAE_VERT),
    my_LCG_edge_p(NULL),
    my_JAE_vertex_p(_JAEvert_p) {}
};

#endif // USEXAIFBOOSTER

struct edge_reroute_t {
  c_graph_t::edge_t e;
  c_graph_t::edge_t pivot_e;
  bool isPre;

  mutable bool pivot_eliminatable;
  mutable bool increment_eliminatable;

  mutable std::vector<c_graph_t::vertex_t> type3EdgeElimVector;

  edge_reroute_t () {};

  edge_reroute_t (const c_graph_t::edge_t _e,
                  const c_graph_t::edge_t _pivot_e,
                  bool _isPre) :
    e (_e),
    pivot_e (_pivot_e),
    isPre (_isPre),
    pivot_eliminatable (0),
    increment_eliminatable (0) { type3EdgeElimVector.clear(); }
}; // end struct edge_reroute_t

  /// Graph-independent edge elimination.
  /** Class for representing an edge elimination.
   *  The edge is represented by the unsigned ints that correspond to its source and target.
   *  This allows us to represent a rerouting independently of a particular graph instance.
  */
  class EdgeElim {
  public:

    EdgeElim();

    EdgeElim(const c_graph_t::edge_t& e,
	     bool isFront,
	     const c_graph_t& angelLCG);

    EdgeElim(const edge_bool_t& be,
	     const c_graph_t& angelLCG);

    EdgeElim(const edge_ij_elim_t& eij);

    std::string debug() const;

    bool isFront() const;

    unsigned int getSource() const;

    unsigned int getTarget() const;

    c_graph_t::edge_t getE(const c_graph_t& angelLCG) const;

    edge_bool_t getBool(const c_graph_t& angelLCG) const;

    /// returns cost (nontrivial mults) of performing this edge elim on \p angelLCG
    unsigned int getCost(const c_graph_t& angelLCG) const;

  private:

    bool myIsFrontFlag;
    unsigned int mySource;
    unsigned int myTarget;

  }; // end class EdgeElim

  /// Graph-independent rerouting.
  /** Class for representing a rerouting.
   *  The relevant edges are represented by the unsigned ints that correspond to their source and target.
   *  This allows us to represent a rerouting independently of a particular graph instance.
   */
  class Rerouting {
  public:

    Rerouting();

    Rerouting(const c_graph_t::edge_t e,
	      const c_graph_t::edge_t pivot_e,
	      bool isPre,
	      const c_graph_t& angelLCG);

    Rerouting(const edge_reroute_t& er,
	      const c_graph_t& angelLCG);

    std::string debug() const;

    bool isPre() const;

    c_graph_t::edge_t getE(const c_graph_t& angelLCG) const;

    c_graph_t::edge_t getPivotE(const c_graph_t& angelLCG) const;

    edge_reroute_t getER(const c_graph_t& angelLCG) const;

    unsigned int getI() const;
    unsigned int getJ() const;
    unsigned int getK() const;

    bool operator==(const Rerouting& anotherRerouting) const;

  private:

    void init(const c_graph_t::edge_t& e,
	      const c_graph_t::edge_t& pivot_e,
	      bool isPre,
	      const c_graph_t& angelLCG);
       
    unsigned int i, j, k;
    bool pre;

  }; // end class Rerouting

  /// Graph-independent transformation.
  /** Class for representing either a rerouting or an edge elimination.
   *  The relevant edges are represented by the unsigned ints that correspond to their source and target.
   *  This allows us to represent a transformation independently of a particular graph instance.
   */
  class Transformation {
  public:

    Transformation(const EdgeElim& anEdgeElim);

    Transformation(const edge_bool_t& be,
		   const c_graph_t& angelLCG);

    Transformation(const edge_ij_elim_t& an_ij_elim);

    Transformation(const Rerouting& aRerouting);

    Transformation(const edge_reroute_t& aRerouteElim,
		   const c_graph_t& angelLCG);

    std::string debug() const;

    /// returns true iff this Transformation is a rerouting
    bool isRerouting() const;

    const EdgeElim& getEdgeElim() const;
    const Rerouting& getRerouting() const;

  private:

    Transformation();

    bool myIsReroutingFlag;

    Rerouting myRerouting;
    EdgeElim myEdgeElim;

  }; // end class Transformation

struct elimSeq_cost_t {
  std::vector<EdgeElim> edgeElimVector;
  unsigned int bestNumNontrivialEdges;
  unsigned int cost;
  unsigned int costAtBestEdgecount;
  unsigned int numIntermediatesAtBestEdgecount;
  unsigned int numIntermediatesWithoutUnitEdgeAtBestEdgecount;
  size_t lastDesiredElim;		// unused for now
  mutable bool revealedNewDependence;

  elimSeq_cost_t (unsigned int _bestNumNontrivialEdges,
		  unsigned int _cost,
		  unsigned int _costAtBestEdgecount,
		  unsigned int _numIntermediatesAtBestEdgecount,
		  unsigned int _numIntermediatesWithoutUnitEdgeAtBestEdgecount,
		  size_t _lastDesiredElim) :
    bestNumNontrivialEdges (_bestNumNontrivialEdges),
    cost (_cost),
    costAtBestEdgecount (_costAtBestEdgecount),
    numIntermediatesAtBestEdgecount (_numIntermediatesAtBestEdgecount),
    numIntermediatesWithoutUnitEdgeAtBestEdgecount (_numIntermediatesWithoutUnitEdgeAtBestEdgecount),
    lastDesiredElim (_lastDesiredElim),
    revealedNewDependence (false) {}
};

struct transformationSeq_cost_t {
  std::vector<Transformation> transformationVector;
  unsigned int bestNumNontrivialEdges;
  unsigned int cost;
  unsigned int costAtBestEdgecount;
  unsigned int numIntermediatesAtBestEdgecount;
  unsigned int numIntermediatesWithoutUnitEdgeAtBestEdgecount;
  size_t lastDesiredTransformation;	// unused for now
  mutable bool revealedNewDependence;

  transformationSeq_cost_t (unsigned int _bestNumNontrivialEdges,
		  	    unsigned int _cost,
			    unsigned int _costAtBestEdgecount,
			    unsigned int _numIntermediatesAtBestEdgecount,
			    unsigned int _numIntermediatesWithoutUnitEdgeAtBestEdgecount,
			    size_t _lastDesiredTransformation) :
			      bestNumNontrivialEdges (_bestNumNontrivialEdges),
			      cost (_cost),
			      costAtBestEdgecount (_costAtBestEdgecount),
			      numIntermediatesAtBestEdgecount (_numIntermediatesAtBestEdgecount),
			      numIntermediatesWithoutUnitEdgeAtBestEdgecount (_numIntermediatesWithoutUnitEdgeAtBestEdgecount),
			      lastDesiredTransformation (_lastDesiredTransformation),
			      revealedNewDependence (false) {}
};


typedef std::pair<unsigned int,unsigned int> 	uint_pair_t;
typedef std::set<c_graph_t::vertex_t>		vertex_set_t;
typedef std::map< uint_pair_t, vertex_set_t >	refillDependenceMap_t;


} // namespace angel


/** \namespace angel
    \brief Namespace for the complete library
*/



#endif  // _angel_types_include_

