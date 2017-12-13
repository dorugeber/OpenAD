// $Id: angel_tools.hpp,v 1.10 2004/02/22 18:44:46 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifndef 	_angel_tools_include_
#define 	_angel_tools_include_


//
//
//

#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <deque>

#include "angel_exceptions.hpp"
#include "angel_types.hpp"

namespace angel {

  using std::vector;
  using boost::tie;
  using boost::get;
  using boost::graph_traits;

//                       some Operators
// ===========================================================

class write_vertex_op_t {
  const c_graph_t& cg;
public:
  write_vertex_op_t (const c_graph_t& _cg) : cg (_cg) {}
  std::ostream& operator() (std::ostream& stream, c_graph_t::vertex_t v) {
    stream << v;
    return stream;
  }
};

class write_edge_op_t {
  const c_graph_t& cg;
public:
  write_edge_op_t (const c_graph_t& _cg) : cg (_cg) {}
  std::ostream& operator() (std::ostream& stream, c_graph_t::edge_t e) {
    stream << "(" << source (e, cg) << ", " << target (e, cg) << ")";
    return stream;
  }
};

class write_edge_bool_op_t {
  const c_graph_t& cg;
public:
  write_edge_bool_op_t (const c_graph_t& _cg) : cg (_cg) {}
  std::ostream& operator() (std::ostream& stream, std::pair<c_graph_t::edge_t,bool> eb) {
    c_graph_t::edge_t e= eb.first;
    stream << "((" << source (e, cg) << ", " << target (e, cg) << "), "
	   << (eb.second ? "forward)" : "reverse)");
    return stream;
  }
};

class write_edge_name_op_t {
  const line_graph_t& lg;
  line_graph_t::const_evn_t  evn;
public:
  write_edge_name_op_t (const line_graph_t& _lg) : lg (_lg) {
    evn= get(boost::vertex_name, lg); }
  std::ostream& operator() (std::ostream& stream, line_graph_t::edge_t e) {
    stream << '(' << evn[e].first << ", " << evn[e].second << ')';
    return stream;
  }
};

class write_face_op_t {
  const line_graph_t& lg;
  line_graph_t::const_evn_t  evn; 
public:
  write_face_op_t (const line_graph_t& _lg) : lg (_lg) {
    evn= get(boost::vertex_name, lg); }
  std::ostream& operator() (std::ostream& stream, line_graph_t::face_t f) {
    line_graph_t::edge_t   ij= source (f, lg), jk= target (f, lg);
    int i= evn[ij].first, j= evn[ij].second, k= evn[jk].second;
    THROW_DEBUG_EXCEPT_MACRO (j != evn[jk].first, consistency_exception, "Adjacency corrupted in line graph"); 
    stream << '(' << i << ", " << j << ", " << k << ')';
    return stream;
  }
};


class write_face_number_op_t {
  const line_graph_t& lg;
public:
  write_face_number_op_t (const line_graph_t& _lg) : lg (_lg) {}
  std::ostream& operator() (std::ostream& stream, line_graph_t::face_t f) {
    line_graph_t::edge_t   ij= source (f, lg), jk= target (f, lg);
    stream << '(' << ij << ", " << jk << ')';
    return stream;
  }
};


//                        Iterations
// ===========================================================

/// Applies \p op to all edges of \p adg, graph can be changed 
template <typename Ad_graph_t, typename Op_t>
inline void for_all_edges (Ad_graph_t& adg, const Op_t& op) {
  typename graph_traits<Ad_graph_t>::edge_iterator  ei, e_end;
  for (tie (ei, e_end)= edges (adg); ei != e_end; ++ei)
    op (*ei, adg);
}

/// Applies \p op to all edges of \p adg, op can be changed, e.g. for outputs
template <typename Ad_graph_t, typename Op_t>
inline void for_all_edges (const Ad_graph_t& adg, Op_t& op) {
  typename graph_traits<Ad_graph_t>::edge_iterator  ei, e_end;
  for (tie (ei, e_end)= edges (adg); ei != e_end; ++ei)
    op (*ei, adg);
}

/// Applies \p op to all in-edges of \p v, graph can be changed 
template <typename Ad_graph_t, typename Op_t>
inline void for_all_in_edges (typename Ad_graph_t::vertex_descriptor v, 
			      Ad_graph_t& adg, const Op_t& op) {
  typename graph_traits<Ad_graph_t>::in_edge_iterator  iei, ie_end;
  for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ++iei)
    op (*iei, adg);
}

/// Applies \p op to all out-edges of \p v, graph can be changed 
template <typename Ad_graph_t, typename Op_t>
inline void for_all_out_edges (typename Ad_graph_t::vertex_descriptor v, 
			       Ad_graph_t& adg, const Op_t& op) {
  typename graph_traits<Ad_graph_t>::out_edge_iterator  oei, oe_end;
  for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ++oei)
    op (*oei, adg);
}

/// Applies \p op to all in-edges of \p v and sum it
template <typename Ad_graph_t, typename Op_t>
inline int sum_over_all_in_edges (typename Ad_graph_t::vertex_descriptor v, 
				  Ad_graph_t& adg, const Op_t& op) {
  int value= 0;
  typename graph_traits<Ad_graph_t>::in_edge_iterator  iei, ie_end;
  for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ++iei)
    value+= op (*iei, adg);
  return value;
}

/// Applies \p op to all out-edges of \p v and sum it
template <typename Ad_graph_t, typename Op_t>
inline int sum_over_all_out_edges (typename Ad_graph_t::vertex_descriptor v, 
				   const Ad_graph_t& adg, const Op_t& op) {
  int value= 0;
  typename graph_traits<Ad_graph_t>::out_edge_iterator  oei, oe_end;
  for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ++oei)
    value+= op (*oei, adg);
  return value;
}

/// Returns successor set of \p v as vector \p vec
template <typename Ad_graph_t>
inline void successor_set (typename Ad_graph_t::vertex_descriptor v, 
			   const Ad_graph_t& adg, 
			   typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  vec.resize (0); vec.reserve (out_degree (v, adg));
  typename graph_traits<Ad_graph_t>::out_edge_iterator  oei, oe_end;
  for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ++oei)
    vec.push_back (target (*oei, adg));
}

/// Returns successor set of \p v as vector \p vec, vertices are sorted
template <typename Ad_graph_t>
inline void sorted_successor_set (typename Ad_graph_t::vertex_descriptor v, 
				  const Ad_graph_t& adg, 
				  typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  successor_set (v, adg, vec);
  std::sort (vec.begin(), vec.end());
}

/// Returns successor set of \p v as vector \p vec
template <typename Ad_graph_t>
inline void predecessor_set (typename Ad_graph_t::vertex_descriptor v, 
			     const Ad_graph_t& adg, 
			     typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  vec.resize (0); vec.reserve (out_degree (v, adg));
  typename graph_traits<Ad_graph_t>::in_edge_iterator  iei, ie_end;
  for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ++iei)
    vec.push_back (source (*iei, adg));
}

/// Returns successor set of \p v as vector \p vec, vertices are sorted
template <typename Ad_graph_t>
inline void sorted_predecessor_set (typename Ad_graph_t::vertex_descriptor v, 
				    const Ad_graph_t& adg, 
				    typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  predecessor_set (v, adg, vec);
  std::sort (vec.begin(), vec.end());
}

/// Answers a reachability query from src to tgt
bool reachable (const c_graph_t::vertex_t src,
		const c_graph_t::vertex_t tgt,
		c_graph_t& angelLCG);

/// Returns a set of vertices in \p adg that depend on \p v
void vertex_upset (const c_graph_t::vertex_t v,
		   const c_graph_t& angelLCG,
		   vertex_set_t& upset);

/// Returns a set of vertices in \p adg that \p v depends on
void vertex_downset (const c_graph_t::vertex_t v,
		     const c_graph_t& angelLCG,
		     vertex_set_t& downset);

/// Sorts arbitrary vector and removes double elements
template <typename El_t>
void unique_vector (std::vector<El_t>& v) {
  std::sort (v.begin(), v.end());
  typename vector<El_t>::iterator it= unique (v.begin(), v.end());
  v.resize (it-v.begin());
}


/// Returns set of vertices (in \p vec) that have common successors with \p v 
template <typename Ad_graph_t>
inline void common_successors (typename Ad_graph_t::vertex_descriptor v, 
			       const Ad_graph_t& adg, 
			       typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  typename graph_traits<Ad_graph_t>::out_edge_iterator  oei, oe_end;
  typename graph_traits<Ad_graph_t>::in_edge_iterator   iei, ie_end;
  for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ++oei)
    for (tie (iei, ie_end)= in_edges (target (*oei, adg), adg); iei != ie_end; ++iei)
      if (source (*iei, adg) != v)
        vec.push_back (source (*iei, adg));
  unique_vector (vec);
}

/// Returns set of vertices (in \p vec) that have same successor set as \p v 
template <typename Ad_graph_t>
inline void same_successors (typename Ad_graph_t::vertex_descriptor v, 
			     const Ad_graph_t& adg, 
			     typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  typename graph_traits<Ad_graph_t>::out_edge_iterator      oei, oe_end;
  typename graph_traits<Ad_graph_t>::in_edge_iterator       iei, ie_end;
  typename std::vector<typename Ad_graph_t::vertex_descriptor>   sv, s;
 
  sorted_successor_set (v, adg, sv);
  for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ++oei)
    for (tie (iei, ie_end)= in_edges (target (*oei, adg), adg); iei != ie_end; ++iei)
      if (source (*iei, adg) != v) {
        sorted_successor_set (source (*iei, adg), adg, s);
	if (s == sv) vec.push_back (source (*iei, adg)); }
  unique_vector (vec);
}

/// Returns set of vertices (in \p vec) that have common predecessors with \p v 
template <typename Ad_graph_t>
inline void common_predecessor (typename Ad_graph_t::vertex_descriptor v, 
				const Ad_graph_t& adg, 
				typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  typename graph_traits<Ad_graph_t>::out_edge_iterator  oei, oe_end;
  typename graph_traits<Ad_graph_t>::in_edge_iterator   iei, ie_end;
  for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ++iei)
    for (tie (oei, oe_end)= out_edges (source (*iei, adg), adg); oei != oe_end; ++oei)
      if (target (*oei, adg) != v)
        vec.push_back (target (*oei, adg));
  unique_vector (vec);
}

/// Returns set of vertices (in \p vec) that have same predecessor set as \p v 
template <typename Ad_graph_t>
inline void same_predecessors (typename Ad_graph_t::vertex_descriptor v, 
			       const Ad_graph_t& adg, 
			       typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  typename graph_traits<Ad_graph_t>::out_edge_iterator      oei, oe_end;
  typename graph_traits<Ad_graph_t>::in_edge_iterator       iei, ie_end;
  typename std::vector<typename Ad_graph_t::vertex_descriptor>   pv, p;
 
  sorted_predecessor_set (v, adg, pv);
  for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ++iei) 
    for (tie (oei, oe_end)= out_edges (source (*iei, adg), adg); oei != oe_end; ++oei) 
      if (target (*oei, adg) != v) {
        sorted_predecessor_set (target (*oei, adg), adg, p);
	if (p == pv) vec.push_back (target (*oei, adg)); }
  unique_vector (vec);
}

/// Returns set of vertices (in \p vec) that have same predecessor and successor set as \p v 
template <typename Ad_graph_t>
inline void same_neighbors (typename Ad_graph_t::vertex_descriptor v, 
			    const Ad_graph_t& adg, 
			    typename std::vector<typename Ad_graph_t::vertex_descriptor>& vec) {
  typename std::vector<typename Ad_graph_t::vertex_descriptor> same_p, same_s;
  same_predecessors (v, adg, same_p); // same pred. as i
  same_successors (v, adg, same_s);
  vec.resize (std::max (same_p.size(), same_s.size()));
  typename std::vector<typename Ad_graph_t::vertex_descriptor>::iterator vend;
  vend= std::set_intersection (same_p.begin(), same_p.end(), same_s.begin(), same_s.end(), vec.begin());
  vec.resize (vend-vec.begin());
}

//                        Iterations
// ===========================================================

template<typename It_t, typename Op_t>
std::ostream& write_iterators (std::ostream& stream, const std::string& s, 
			       It_t begin, It_t end, Op_t op) {
  stream << s << " = {";
  if (begin != end) op (stream, *begin++); 
  for (; begin != end; ++begin)
    stream << ", ", op (stream, *begin);
  stream << '}' << std::endl;
  return stream;
}  

//                      Others
// ===========================================================

// e is an edge of g1, same_egde return a pointer to 
// the same edge (equal source and equal target) in g2 (or e_end)
/** \brief Returns same edge in another graph
    
    \p e is an edge of \p g1, same_egde returns an edge_iterator to 
    the same edge (equal source and equal target) in \p g2 (or e_end if not found)
*/
template <typename Ad_graph_t>
inline typename graph_traits<Ad_graph_t>::edge_iterator
same_edge (typename Ad_graph_t::edge_descriptor e,
	   const Ad_graph_t& g1, const Ad_graph_t& g2) {
  typename graph_traits<Ad_graph_t>::edge_iterator ei, e_end;
  typename Ad_graph_t::vertex_descriptor s= source (e, g1), 
                                         t= target (e, g1);
  for (tie (ei, e_end)= edges (g2); ei != e_end; ++ei)
    if (source (*ei, g2) == s && target (*ei, g2) == t)
      return ei;
  return e_end; // not found
}


/// Functional equivalent for graph class method (more boost-like)
template <typename Ad_graph_t> 
vertex_type_t vertex_type (typename Ad_graph_t::vertex_descriptor v, 
			   const Ad_graph_t& adg) {
  return adg.vertex_type (v); 
}

/// Compares edges of different graphs 
template <typename Ad_graph_t> 
struct edge_equal_t : public std::binary_function<typename Ad_graph_t::edge_descriptor,
                                                  typename Ad_graph_t::edge_descriptor,bool> {
  const Ad_graph_t& g1;
  const Ad_graph_t& g2;
  /// Both graphs are given in the constructor
  edge_equal_t (const Ad_graph_t& _g1, const Ad_graph_t& _g2) : 
    g1 (_g1), g2 (_g2) {}
  /// Tests whether \p e1 and \p e2 have same source and target
  bool operator() (typename Ad_graph_t::edge_descriptor e1,
		   typename Ad_graph_t::edge_descriptor e2) {
    typename Ad_graph_t::vertex_descriptor s1= source (e1, g1), s2= source (e2, g2);
    return s1 == s2 && target (e1, g1) == target (e2, g2);}
};

/// Compares edges of different graphs (lexicographically)
template <typename Ad_graph_t> 
struct edge_less_t : public std::binary_function<typename Ad_graph_t::edge_descriptor,
                                            typename Ad_graph_t::edge_descriptor,bool> {
  const Ad_graph_t& g1;
  const Ad_graph_t& g2;
  /// Both graphs are given in the constructor
  edge_less_t (const Ad_graph_t& _g1, const Ad_graph_t& _g2) : 
    g1 (_g1), g2 (_g2) {}
  /// Returns whether \p e1 is lexicographically less than \p e2 w.r.t. source and target
  bool operator() (typename Ad_graph_t::edge_descriptor e1,
		   typename Ad_graph_t::edge_descriptor e2) {
    typename Ad_graph_t::vertex_descriptor s1= source (e1, g1), s2= source (e2, g2);
    return s1 < s2 || (s1 == s2 && target (e1, g1) < target (e2, g2));}
};

template <typename Ad_graph_t> 
inline int count_parallel_edges (typename Ad_graph_t::edge_descriptor e, 
				 const Ad_graph_t& g) {
  typename Ad_graph_t::vertex_descriptor s= source (e, g), t= target (e, g);
  typename graph_traits<Ad_graph_t>::out_edge_iterator oei, oe_end;
  int pe= 0;
  for (tie (oei, oe_end)= out_edges (s, g); oei != oe_end; oei++)
    if (target (*oei, g) == t) pe++;
  return pe;
}

/// Returns the edge in \p angelLCG that has source \p i and target \p j.
c_graph_t::edge_t getEdge(unsigned int i,
			  unsigned int j,
			  const c_graph_t& angelLCG);

// =====================================================
// comparisons
// =====================================================

/// Returns whether \p e1 is lexicographically greater than \p e2 w.r.t. source and target
inline bool lex_greater (c_graph_t::edge_t e1, c_graph_t::edge_t e2,
			 const c_graph_t& cg) {

  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg);

  return s1 > s2 || (s1 == s2 && target (e1, cg) >= target (e2, cg));

}

/// Returns whether \p e1 is lexicographically less than \p e2 w.r.t. source and target
inline bool lex_less (c_graph_t::edge_t e1, c_graph_t::edge_t e2,
		      const c_graph_t& cg) {

  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg);

  return s1 < s2 || (s1 == s2 && target (e1, cg) <= target (e2, cg));

}

/// Returns whether \p e1 is lexicographically greater than \p e2 w.r.t. target and source 
inline bool inv_lex_greater (c_graph_t::edge_t e1, c_graph_t::edge_t e2,
			     const c_graph_t& cg) {

  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg),
                      t1= target (e1, cg), t2= target (e2, cg); 
  return t1 > t2 || (t1 == t2 && s1 >= s2);
}

/// Returns whether \p e1 is lexicographically less than \p e2 w.r.t. target and source 
inline bool inv_lex_less (c_graph_t::edge_t e1, c_graph_t::edge_t e2,
			  const c_graph_t& cg) {

  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg),
                      t1= target (e1, cg), t2= target (e2, cg);
  return t1 < t2 || (t1 == t2 && s1 <= s2); 
}

inline bool lex_greater (edge_bool_t eb1, edge_bool_t eb2, const c_graph_t& cg) {

  c_graph_t::edge_t   e1= eb1.first, e2= eb2.first;
  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg), 
                       t1= target (e1, cg), t2= target (e2, cg); 
  bool                 f1= eb1.second, f2= eb2.second;
  c_graph_t::vertex_t j1= f1 ? t1 : s1, j2= f2 ? t2 : s2;

  // e1 is eliminated during vertex elimination of j1 (e2 resp.)
  if (j1 > j2) return true; else if (j1 < j2) return false;

  // prefer front elimination, rm 1
  if (f1 && !f2) return true; else if (!f1 && f2) return false;
  
  // f1==f2, start with front elimination
  if (f1) return s1 > s2; else return t1 > t2;
}

inline bool lex_less (edge_bool_t eb1, edge_bool_t eb2, const c_graph_t& cg) {

  c_graph_t::edge_t   e1= eb1.first, e2= eb2.first;
  c_graph_t::vertex_t s1= source (e1, cg), s2= source (e2, cg), 
                       t1= target (e1, cg), t2= target (e2, cg); 
  bool                 f1= eb1.second, f2= eb2.second;
  c_graph_t::vertex_t j1= f1 ? t1 : s1, j2= f2 ? t2 : s2;

  // e1 is eliminated during vertex elimination of j1 (e2 resp.)
  if (j1 < j2) return true; else if (j1 > j2) return false;

  // prefer front elimination, rm 1
  if (f1 && !f2) return true; else if (!f1 && f2) return false;
  
  // f1==f2, start with front elimination
  if (f1) return s1 < s2; else return t1 < t2;
}

bool lex_less_face (line_graph_t::face_t e1, line_graph_t::face_t e2,
		    const line_graph_t& lg);

// to use lex_less_face with std::sort, see face_elimination_heuristic.cpp for an example
class lex_less_face_line_t {
  const line_graph_t& lg;

public:
  lex_less_face_line_t (const line_graph_t& g) : lg (g) {}

  bool operator() (const line_graph_t::face_t& e1,
		   const line_graph_t::face_t& e2) {
    return lex_less_face (e1, e2, lg); }
};

// same as lex_less_face_line_t, but for decreasing order in std::sort
// it is supposed that faces do not occur twice, 
// i.e. e1 != e2 iff (i,j,k)_1 != (i,j,k)_2,
// so e1 > e2 iff not e1 < e2 
class not_lex_less_face_line_t {
  const line_graph_t& lg;

public:
  not_lex_less_face_line_t (const line_graph_t& g) : lg (g) {}

  bool operator() (const line_graph_t::face_t& e1,
		   const line_graph_t::face_t& e2) {
    return !lex_less_face (e1, e2, lg); }
};

// =====================================================
// random number generators
// =====================================================

/// Random value between \p min and \p max, i.e. from [min, max]
inline int random (int min, int max) {	
  return min + int (double (max-min+1)*rand() / (RAND_MAX+1.0)); }

/// Random value from [0, n)
inline double random (double n) {
  return n*rand() / (RAND_MAX+1.0); }

/// Random value between 0 and n-1, i.e. from [0, n)
inline int random (int n) {
  return int (double (n)*rand() / (RAND_MAX+1.0)); }

/// Random value from [0, n) where larger values have higher probability (increases with exp)
inline int random_high (int n, int exp= 2) {
  double tmp= rand() / (RAND_MAX+1.0);
  return int (double (n) * (1 - pow (tmp, exp))); }

/** \brief Random number characterized by \p p, the accumulated probabities

    - p[0] is the probability of returning 0
    - p[1] the probability of returning 0 or 1
    - p.size()-1 is the maximal return value
    - p[p.size()-1] must be 1.0
 */ 
inline int random (const std::vector<double>& p) {
  size_t ps= p.size();
#ifndef NDEBUG
  for (size_t j= 0; j < ps; j++)
    // assert (p[j] > 0.0 && p[j] <= 1.0);
    THROW_EXCEPT_MACRO (p[j] < 0.0 || p[j] > 1.0, consistency_exception, "p[i] not between 0 and 1");
  for (size_t j= 1; j < ps; j++)
    // assert (p[j-1] <= p[j]);
    THROW_EXCEPT_MACRO (p[j-1] > p[j], consistency_exception, "p[i] > p[i-1]");
  // assert (p[ps-1] == 1.0);
  THROW_EXCEPT_MACRO (p[ps-1] != 1.0, consistency_exception, "Last value must be 1");
#endif
  double r= random (1.0);
  for (size_t j= 0; j < ps; j++)
    if (r < p[j]) return int (j);
  return int (ps);
}


// =====================================================
// path lengths
// =====================================================

/** \brief Returns for each vertex the number of paths and their overall length
    \param cg c-graph
    \param vni Number of all incoming paths for each vertex
    \param vli Overall path length of all incoming paths for each vertex
    \param vno Number of all outgoing paths for each vertex
    \param vlo Overall path length of all outgoing paths for each vertex 
    \note Function is not implemented for line graphs because there is no application (now).
 */ 
void in_out_path_lengths (const c_graph_t& cg,
			  std::vector<int>& vni, std::vector<int>& vli, 
			  std::vector<int>& vno, std::vector<int>& vlo);

// =====================================================
// find nodes in line graph that correspond to edge (i,j) in c-graph
// =====================================================

/// Searches an edge in line graph that corresponds to (s,t)
inline bool find_edge (int s, int t, const line_graph_t& lg, 
		       std::vector<line_graph_t::edge_t>& ev) {
  ev.resize (0);
  line_graph_t::const_evn_t evn= get(boost::vertex_name, lg);
  line_graph_t::ei_t        ei, eend;
  for (tie (ei, eend)= vertices (lg); ei != eend; ++ei) 
    if (evn[*ei].first == s && evn[*ei].second == t) ev.push_back (*ei);
  return !ev.empty();
}

// =====================================================
// test whether c-/line graph is bi-/tri-partite
// =====================================================

/// Tests if \p cg is bi-partite
inline bool is_bipartite (const c_graph_t& cg) {
  c_graph_t::ei_t     ei, e_end;
  for (tie(ei, e_end) = edges (cg); ei != e_end; ++ei)
    if (vertex_type (source (*ei, cg), cg) != independent 
	|| vertex_type (target (*ei, cg), cg) != dependent)
      return false;
  return true;
}

/// Tests if \p lg is tri-partite
inline bool is_tripartite (const line_graph_t& lg) {
  line_graph_t::fi_t     fi, f_end;
  for (tie(fi, f_end) = edges (lg); fi != f_end; ++fi)
    if (vertex_type (source (*fi, lg), lg) != independent 
	&& vertex_type (target (*fi, lg), lg) != dependent)
      return false;
  return true;
}

// =====================================================
// vertex properties
// =====================================================

/// Returns for each vertex how many dependent vertices depent on it
void number_dependent_vertices (const c_graph_t& cg, std::vector<int>& v);

/// Returns for each vertex on how many independent vertices it depends
void number_independent_vertices (const c_graph_t& cg, std::vector<int>& v);

/** \brief Computes all reachable vertices for c- and line graphs

    I.e. there are pathes from independent nodes to these nodes.
    Uses breadth first search over all independent vertices.
 */
template <typename Ad_graph_t> 
void reachable_vertices (const Ad_graph_t& adg, std::vector<bool>& rv) {

  typedef typename Ad_graph_t::vertex_descriptor                         vertex_t;
  typedef typename graph_traits<Ad_graph_t>::vertex_iterator      vi_t;
  typedef typename graph_traits<Ad_graph_t>::adjacency_iterator   ai_t;

  rv.resize (num_vertices (adg));
  std::fill (rv.begin(), rv.end(), false);

  typename std::queue<vertex_t> q;
  vi_t                          vi, v_end;
  int                           c;
  // all independent vertices are reachable and inserted into the queue
  for (tie(vi, v_end)= vertices(adg), c= 0; c < adg.x() && vi != v_end; ++c, ++vi) {
    rv[*vi]= true; q.push (*vi); }

  // search for reachable (intermediate and dependent) vertices
  while (!q.empty()) {
    vertex_t v= q.front();
    ai_t ai, a_end;
    for (tie(ai, a_end)= adjacent_vertices(v, adg); ai != a_end; ++ai)
      if (!rv[*ai]) {
	rv[*ai]= true; q.push (*ai); }
    q.pop(); }
}	

/** \brief Computes all relevant vertices for c- and line graphs

    I.e. there are pathes from these nodes to dependent nodes.
    Uses backward breadth first search over all dependent vertices. 
 */
template <typename Ad_graph_t> 
void relevant_vertices (const Ad_graph_t& adg, std::vector<bool>& rv) {

  typedef typename Ad_graph_t::vertex_descriptor               vertex_t;
  typedef typename graph_traits<Ad_graph_t>::vertex_iterator   vi_t;
  typedef typename graph_traits<Ad_graph_t>::in_edge_iterator  ie_t;

  rv.resize (num_vertices (adg));
  std::fill (rv.begin(), rv.end(), false);

  typename std::queue<vertex_t> q;
  // all dependent vertices are relevant and inserted into the queue
  for (size_t i= 0; i < adg.dependents.size(); i++) {
    vertex_t v= adg.dependents[i];
    rv[v]= true; q.push (v); }

  // search for relevant (intermediate and independent) vertices
  while (!q.empty()) {
    vertex_t v= q.front();
    ie_t     iei, ie_end;
    for (tie(iei, ie_end)= in_edges(v, adg); iei != ie_end; ++iei) {
      vertex_t vin= source (*iei, adg);
      if (!rv[vin]) {
	rv[vin]= true; q.push (vin); } }
    q.pop();  }
}

template <typename Neighbor_t>
bool search_path (const std::vector<c_graph_t::vertex_t>& from, 
		  const std::vector<c_graph_t::vertex_t>& to, const Neighbor_t& n,
		  std::vector<c_graph_t::vertex_t>& path,
		  bool breadth_first= false);

template <typename Neighbor_t>
int maximal_paths (c_graph_t::vertex_t v, const Neighbor_t& nin, 
		   std::vector<std::vector<c_graph_t::vertex_t> >& paths);

template <typename Neighbor_t>
inline int minimal_in_out_degree (c_graph_t::vertex_t v, const Neighbor_t& nin) {
  std::vector<std::vector<c_graph_t::vertex_t> >    all_paths;
  return maximal_paths (v, nin, all_paths);
}

/// Minimal Markowitz degree for each vertex
inline void minimal_markowitz_degree (const c_graph_t& cg, std::vector<int>& v) {
  v.resize (cg.v());
  predecessor_t<c_graph_t> pred (cg);
  successor_t<c_graph_t>   succ (cg);
  c_graph_t::vi_t          vi, v_end;
  for (tie (vi, v_end)= vertices (cg); vi != v_end; ++vi) {
    if (vertex_type (*vi, cg) == intermediate)
      v[*vi]= minimal_in_out_degree (*vi, pred) * minimal_in_out_degree (*vi, succ); }
}

/// Sum of minimal Markowitz degrees
inline int overall_minimal_markowitz_degree (const c_graph_t& cg) {
  std::vector<int> v;
  minimal_markowitz_degree (cg, v);
  return std::accumulate (v.begin(), v.end(), 0);
}  


// =====================================================
// graph transformations
// =====================================================

/// Permutates vertices, vertex v in \p gin becomes \p p[v] in \p gout
void permutate_vertices (const c_graph_t& gin, const std::vector<c_graph_t::vertex_t>& p,
			 c_graph_t& gout);

/// Independent vertices, given by \p indeps, becomes first vertices in \p gout
void independent_vertices_to_front (const c_graph_t& gin, 
				    const std::vector<c_graph_t::vertex_t>& indeps,
				    c_graph_t& gout);

/// Sets all vertex labels (in ed) to 1
inline void put_unit_vertex_weight (line_graph_t& lg) {
  line_graph_t::ed_t  ed= get(boost::vertex_degree, lg);
  line_graph_t::ei_t  ei, eend;
  for (tie(ei, eend) = vertices (lg); ei != eend; ++ei) ed[*ei]= 1;
}

/// Sets all edge labels (in ew) to 1
inline void put_unit_edge_weight (c_graph_t& cg) {
  c_graph_t::ew_t     ew= get(boost::edge_weight, cg);
  c_graph_t::ei_t     ei, eend;
  for (tie(ei, eend) = edges (cg); ei != eend; ++ei) ew[*ei]= 1;
}

/// Renumber edges of \p cg continously, i.e. to [0..num_edges-1]
int renumber_edges (c_graph_t& cg);

// v2 take over the successors of v1
// - v1 is a vertex of g1 and v2 is a vertex of g2
// - each successor of v1 has an equivalent in g2
//   such that the indices of both vertices differ by offset
// - edge_number is read and updated
// - useful to compose graphs, e.g. in stats2block
//
void take_over_successors (c_graph_t::vertex_t v1, c_graph_t::vertex_t v2, 
			   int offset, const c_graph_t& g1,  
			   int& edge_number, c_graph_t& g2);


// -----------------------------------------------------
// remove irrelevant and unreachable edges
// -----------------------------------------------------
//
// remove_irrelevant_edges removes all in-edges of vertix 'i' if 'i' has no out-edges
//   and continues, in this case, recursively with the predecessors of 'i'
// remove_unreachable_edges removes all out-edges of vertix 'i' if 'i' has no in-edges
//   and continues, in this case, recursively with the successors of 'i'
// remove_edges does both
// all functions return the number of removed edges
// the set of removed edges is not complete, e.g. 'i' may have out-edges but no path to 
//   dependent nodes
//

/** \brief Removes irrelevant edges from \p adg starting with \p i

    It removes all in-edges of vertix \p i if \p i has no out-edges
    and continues, in this case, recursively with the predecessors of \p i.
 */
template <typename Ad_graph_t>
int remove_irrelevant_edges (typename Ad_graph_t::vertex_descriptor i, 
			     Ad_graph_t& adg, bool fast= false) {
  typedef typename Ad_graph_t::vertex_descriptor               vertex_t;
  typedef typename graph_traits<Ad_graph_t>::in_edge_iterator  iei_t;

  if (fast) {
    int e= in_degree (i, adg);
    if (out_degree (i, adg) == 0) {
      clear_vertex (i, adg);
      return e; }
    else return 0; }

  int nre= 0; // number of removed edges
  typename std::queue<vertex_t> q;
  q.push (i);

  while (!q.empty()) {
    vertex_t v= q.front();
    if (out_degree (v, adg) == 0) {
      iei_t iei, ie_end;
      for (tie (iei, ie_end)= in_edges (v, adg); iei != ie_end; ) {
	q.push (source (*iei, adg));
	remove_edge (*iei, adg); 
	nre++;
	tie (iei, ie_end)= in_edges (v, adg); // now without first in-edge
      }
    }
    q.pop(); }
  return nre;
}

/** \brief Removes unreachable edges from \p adg starting with \p i

    It removes all out-edges of vertix \p i if \p i has no in-edges
    and continues, in this case, recursively with the successors of \p i.
 */
template <typename Ad_graph_t>
int remove_unreachable_edges (typename Ad_graph_t::vertex_descriptor i, 
			      Ad_graph_t& adg, bool fast= false) {
  typedef typename Ad_graph_t::vertex_descriptor                  vertex_t;
  typedef typename graph_traits<Ad_graph_t>::out_edge_iterator    oei_t;

  if (fast) {
    int e= out_degree (i, adg);
    if (in_degree (i, adg) == 0) {
      clear_vertex (i, adg);
      return e; }
    else return 0; }

  int nre= 0; // number of removed edges
  typename std::queue<vertex_t> q;
  q.push (i);

  while (!q.empty()) {
    vertex_t v= q.front();
    if (in_degree (v, adg) == 0) {
      oei_t oei, oe_end;
      for (tie (oei, oe_end)= out_edges (v, adg); oei != oe_end; ) {
	q.push (target (*oei, adg));
	remove_edge (oei, adg); // iterator is faster than edge itself
	nre++;
	tie (oei, oe_end)= out_edges (v, adg); // now without first out-edge
      }
    }
    q.pop(); }
  return nre;
}

/// Removes irrelevant and unreachable edges from \p adg starting with \p i
template <typename Ad_graph_t>
inline int remove_edges (typename Ad_graph_t::vertex_descriptor i, 
			 Ad_graph_t& adg) {
  return remove_irrelevant_edges (i, adg)
         + remove_unreachable_edges (i, adg);
}

// -----------------------------------------------------
// clear graph
// -----------------------------------------------------

template <typename vertex_t>
struct dec_greater : public std::unary_function<vertex_t, void> {
  vertex_t vc;
  dec_greater (vertex_t _vc) : vc (_vc) {}
  void operator() (vertex_t& v) {if (v > vc) v--;}
};

// clear_graph is member function of c_graph_t and line_graph_t either

// -----------------------------------------------------
// some preprocessing removals
// -----------------------------------------------------

/// Removes all vertices with one predecessor and one successor from \p cg
int remove_hoisting_vertices (c_graph_t& cg);

/** \brief Removes parallel edges

    - edges are inserted in the order of their first occurrence
    e.g. (2, 3), (2, 5), (2, 3), (2, 4), (2, 5) -> (2, 3), (2, 5), (2, 4)
    - edge weights are summed for parallel edges
*/
void remove_parallel_edges (c_graph_t& cg);

/// Eliminates all edges with label 1, front elimination is preferred
void remove_trivial_edges (c_graph_t& cg);

/// Empty operator class for dummy arguments
template <class operand_t>
struct empty_operator_t {
  void operator() (const operand_t&) {}
  void operator() (operand_t&) {}
};

/// First index in \p ith of \p p blocks where overall size is \p n (indices 0..n-1)
inline size_t block_begin (size_t i, size_t p, size_t n) {
  return i * (n/p) + std::min (i, n%p); }

// =====================================================
// Functions for simulated annealing for partial accumulation (scarcity exploitation)
// =====================================================

/** \brief Returns a random number between zero and one.
    \sa chooseTarget_sa
*/
double gen_prob();

/** \brief Randomly chooses an index into the vector \p deltaE
    \param deltaE a vector of changes in energy.  Normalized probabilities are calculated for each entry
    \sa gen_prob
*/
unsigned int chooseTarget_sa(std::vector<double>& deltaE);

int chooseEdgeElimRandomly(std::vector<double>& deltaE);
int chooseEdgeElimRandomlyGreedy(std::vector<double>& deltaE);

} // namespace angel


#endif  // _angel_tools_include_

