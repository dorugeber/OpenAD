// $Id: angel_types.cpp,v 1.14 2008/02/28 14:57:33 gottschling Exp $ 
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#include "angel/include/angel_types.hpp"

#include <iostream>
#include <string>
#include <string>

#include "angel/include/eliminations.hpp"
#include "angel/include/angel_tools.hpp"
#include "angel/include/angel_io.hpp"

// =====================================================
// c-graph
// =====================================================

namespace angel {

using namespace std;
using namespace boost;

bool c_graph_t::check () const {

  // inconsistent edges, test not complete, tests only the number
  vi_t vi, v_end;
  size_t sum_edges= 0;
  for (tie (vi, v_end)= vertices (*this); vi != v_end; ++vi)
    sum_edges+= out_degree (*vi, *this);
  if (sum_edges != num_edges (*this)) {
    write_graph ("graph with inconsistent edge number", *this);
    cout << "individually counted: " << sum_edges << ", num_edges: "
	 << num_edges (*this) << std::endl;
    return false;}

  // test vertex type
  for (tie (vi, v_end)= vertices (*this); vi != v_end; ++vi)
    switch (vertex_type (*vi)) {
	case independent: 
	  if (in_degree (*vi, *this) > 0) {
	    cout << *vi << " is independent with indegree > 0\n"; return false;}
	  if (out_degree (*vi, *this) == 0) {
	    cout << *vi << " is independent with outdegree == 0\n"; return false;}
	  break;
	case dependent: 
	  if (in_degree (*vi, *this) == 0) {
	    cout << *vi << " is dependent with indegree == 0\n"; return false;}
	  // (out_degree (*vi, *this) > 0) {
	  //cout << *vi << " is dependent with outdegree > 0\n"; return false;}
	default:    ;    // not yet used in this graph type
    }

  return true;
}

bool c_graph_t::check_initial () const {
  bool ok= check ();
  if (!ok) return false;

  // test intermediate vertices
  vi_t vi, v_end;
  for (tie (vi, v_end)= vertices (*this); vi != v_end; ++vi)
    if (vertex_type (*vi) != independent && vertex_type (*vi) != dependent) {
      if (in_degree (*vi, *this) == 0) {
        cout << *vi << " is intermediate with indegree == 0\n"; return false;}
      if (out_degree (*vi, *this) == 0) {
	cout << *vi << " is intermediate with outdegree == 0\n"; return false;} }

  return true;
}

void c_graph_t::remove_dependents_with_successors () {

  std::vector<vertex_t> dep;
  for (size_t i= 0; i < dependents.size(); i++)
    if (out_degree (dependents[i], *this) == 0)
      dep.push_back (dependents[i]);
  dependents.swap (dep);
}

void c_graph_t::clear_graph () {
  
  typedef c_graph_t::vertex_t v_t;
  typedef vector<v_t>::iterator    it_t;

  vector<bool> reachV;
  reachable_vertices (*this, reachV);
  vector<bool> relV;
  relevant_vertices (*this, relV);

  // better reverse loop since removing vertices invalidates higher indices 
  for (int i= reachV.size()-1; i >= 0; i--)
    if (!reachV[i] || !relV[i]) {
      v_t v= vertex (i, *this);
      clear_vertex (v, *this);
      remove_vertex (v, *this);

      if (i < this->X) 
	this->X--; 
      else {
	it_t it= find (this->dependents.begin(), this->dependents.end(), v);
	if (it != this->dependents.end()) {
	  remove (this->dependents.begin(), this->dependents.end(), v);
	  this->dependents.resize(this->dependents.size()-1);}
      }
      for_each (this->dependents.begin(), this->dependents.end(), dec_greater<v_t> (v));
    }
}




bool operator== (const c_graph_t& g1, const c_graph_t& g2) {

  typedef c_graph_t::vertex_t    vertex_t;
  typedef c_graph_t::vi_t        vi_t;
  typedef c_graph_t::edge_t      edge_t;
  typedef c_graph_t::oei_t       oei_t;

  if (g1.v() != g2.v() || g1.x() != g2.x() || g1.y() != g2.y() || g1.z() != g2.z())
    return false;

  // compare dependents (as sorted copies)
  vector<vertex_t>   d1 (g1.dependents), d2 (g2.dependents); 
  sort (d1.begin(), d1.end());
  sort (d2.begin(), d2.end());
  for (size_t c= 0; c < d1.size(); c++)
    if (d1[c] != d2[c]) return false;

  // compare the out_edges for each pair of vertices
  vi_t vi1, v_end1, vi2, v_end2;
  tie (vi1, v_end1)= vertices (g1);
  tie (vi2, v_end2)= vertices (g2);
  edge_equal_t<c_graph_t> edge_equal (g1, g2);
  edge_less_t<c_graph_t> edge_less (g1, g2);
  for (; vi1 != v_end1; ++vi1, ++vi2) {
    oei_t   oei1, oe_end1, oei2, oe_end2;
    tie (oei1, oe_end1)= out_edges (*vi1, g1);
    vector<edge_t>  oe1 (oei1, oe_end1);
    sort (oe1.begin(), oe1.end(), edge_less);

    tie (oei2, oe_end2)= out_edges (*vi2, g2);
    vector<edge_t>  oe2 (oei2, oe_end2);
    sort (oe2.begin(), oe2.end(), edge_less);

    // now we can compare the sorted out_edges
    for (size_t c= 0; c < oe1.size(); c++)
      if (!edge_equal (oe1[c], oe2[c]))
	return false;
  }

  return true;
}

int overall_markowitz_degree (const c_graph_t& cg) {

  int degree= 0;
  c_graph_t::vi_t   vi, v_end;
  for (boost::tie (vi, v_end)= vertices (cg); vi != v_end; ++vi)
    degree+= in_degree (*vi, cg) * out_degree (*vi, cg);

  return degree;
}


// =====================================================
// line graph
// =====================================================


line_graph_t::line_graph_t (const c_graph_t& cg) {

  const int X1= cg.x(), X2= X1, Y1= cg.y(), Y2= Y1, Z2= num_edges (cg);
  // V2= X2+Y2+Z2;

  pure_line_graph_t gtmp (X2+Y2+Z2);
  pure_line_graph_t::swap (gtmp);
  X= X2; cons_ok= false;
  
  ed_t ew= get(vertex_degree, *this);    // edge weights in line graph
  evn_t evn= get(vertex_name, *this);
  for (int i= 0; i < X2; i++)
    evn[i]= make_pair (i, i), ew[i]= 0;

  // edge weights in c-graph
  property_map<c_graph_t, edge_weight_t>::const_type  cg_ew= get(edge_weight, cg);
  c_graph_t::ei_t     ei1, eend1;

  tie(ei1, eend1)= edges(cg);
  for (int i= X2; i < X2+Z2; ei1++, i++) {
    evn[i]= make_pair (source (*ei1, cg), target (*ei1, cg));
    ew[i]= cg_ew[*ei1]; }

  for (int i= X2+Z2, di= 0; i < X2+Z2+Y2; di++, i++) {
    evn[i]= make_pair (cg.dependents[di], cg.dependents[di]); ew[i]= 0;
    dependents.push_back (i); }

  // edges must be numbered correctly to avoid quadratic complexity
  c_graph_t cg_copy (cg);
  renumber_edges (cg_copy);
  property_map<c_graph_t, edge_index_t>::type eid1= get(edge_index, cg_copy);

  // insert E~2
  c_graph_t::vi_t vi1, vend1;
  tie(vi1, vend1) = vertices(cg_copy);
  for (int i= 0; i < X1; i++, ++vi1) {
    c_graph_t::oei_t oei1, oeend1;
    for (tie (oei1, oeend1)= out_edges (*vi1, cg_copy); oei1 != oeend1; ++oei1) 
      add_edge (i, eid1[*oei1]+X1, *this);
  }
  
  for (tie(ei1, eend1) = edges(cg_copy); ei1 != eend1; ++ei1) {
    int ei1_id= eid1[*ei1];
    c_graph_t::vertex_t vt= target(*ei1, cg_copy);

    // look for dependent nodes (may not be on the end)
    for (size_t i= 0; i < cg_copy.dependents.size(); i++)
      if (vt == cg_copy.dependents[i]) {
	add_edge (ei1_id+X1, X2 + Z2 + i, *this); // mapping of Y from cg to lg numbering
	break; }

    // look for successors (even if vt is dependent)
    c_graph_t::oei_t ei1i, eend1i; // incident edges
    for (tie (ei1i, eend1i)= out_edges (vt, cg_copy); ei1i != eend1i; ++ei1i) 
      add_edge (ei1_id+X1, eid1[*ei1i]+X1, *this);
  }

  THROW_DEBUG_EXCEPT_MACRO (overall_markowitz_degree (cg) != overall_markowitz_degree (*this), 
			       consistency_exception, "Different Markowitz degree in line graph"); 
}


bool line_graph_t::check () const {

  // inconsistent edges, test not complete, tests only the number
  ei_t ei, e_end;
  /*size_t sum_faces= 0;
  for (tie (ei, e_end)= vertices (*this); ei != e_end; ++ei)
    sum_faces+= out_degree (*ei, *this);
  if (sum_faces != num_edges (*this)) {
    write_graph ("graph with inconsistent edge number", *this);
    cout << "individually counted: " << sum_faces << ", num_edges: "
	 << num_edges (*this) << std::endl;
	 return false;} */

  vector<size_t> od (v ());
  fi_t fi, f_end;
  for (tie (fi, f_end)= edges (*this); fi != f_end; ++fi)
    od [source(*fi,*this)]++;

  for (tie (ei, e_end)= vertices (*this); ei != e_end; ++ei)
    if (out_degree (*ei, *this) != od [*ei]) {
      cout << "vertex " << *ei << " has inconsistent edge number ("
	   << out_degree (*ei, *this) << " != " << od [*ei] << ")\n";
      return false;}

  line_graph_t::const_evn_t evn= get(vertex_name, *this);
  for (tie (fi, f_end)= edges (*this); fi != f_end; ++fi) {
    if (evn[source(*fi,*this)].second != evn[target(*fi,*this)].first) {
      cout << "edge label inconsistent\n"; return false; }
    if (int (source(*fi,*this)) >= v()) {
      cout << "edge with inconsistent source ("
	   << source(*fi,*this) << " >= " << v() << ")\n"; return false; }
    if (int (target (*fi,*this)) >= v()) {
      cout << "edge with inconsistent target ("
	   << target (*fi,*this) << " >= " << v() << ")\n"; return false; } }

  for (edge_t e= 0; (int) e < x(); e++)
    if (evn[e].first != evn[e].second) {
      cout << "edge label of independent edge is inconsistent\n"; return false; }

  for (int c= 0; c < y(); c++) {
    edge_t e= dependents[c];
    if (evn[e].first != evn[e].second) {
      cout << "edge label of dependent edge is inconsistent\n"; return false; } }

  return true;
}

bool line_graph_t::is_tripartite () const {
  ei_t ei, e_end;
  for (tie (ei, e_end)= vertices (*this); ei != e_end; ei++)
    if (vertex_type (*ei) == intermediate) {
      if (in_degree (*ei, *this) != 1 || out_degree (*ei, *this) != 1) return false;
      pair<ifi_t, ifi_t> ifp= in_edges (*ei, *this); 
      if (vertex_type (source (*ifp.first, *this)) != independent) return false;
      pair<ofi_t, ofi_t> ofp= out_edges (*ei, *this); 
      if (vertex_type (target (*ofp.first, *this)) != dependent) return false; }
  return true;
}

void line_graph_t::clear_graph () {
  
  typedef line_graph_t::edge_t v_t;
  typedef vector<v_t>::iterator    it_t;

  vector<bool> reachV;
  reachable_vertices (*this, reachV);
  vector<bool> relV;
  relevant_vertices (*this, relV);

  // better reverse loop since removing vertices invalidates higher indices 
  for (int i= reachV.size()-1; i >= 0; i--)
    if (!reachV[i] || !relV[i]) {
      v_t v= vertex (i, *this);
      clear_vertex (v, *this);
      remove_vertex (v, *this);

      if (i < this->X) 
	this->X--; 
      else {
	it_t it= find (this->dependents.begin(), this->dependents.end(), v);
	if (it != this->dependents.end()) {
	  remove (this->dependents.begin(), this->dependents.end(), v);
	  this->dependents.resize(this->dependents.size()-1);}
      }
      for_each (this->dependents.begin(), this->dependents.end(), dec_greater<v_t> (v));
    }
}

bool operator== (const line_graph_t& g1, const line_graph_t& g2) {

  using namespace std;
  typedef line_graph_t::edge_t      edge_t;
  typedef line_graph_t::ei_t        ei_t;
  typedef line_graph_t::face_t      face_t;
  typedef line_graph_t::ofi_t       ofi_t;

  if (g1.v() != g2.v() || g1.x() != g2.x() || g1.y() != g2.y() || g1.z() != g2.z())
    return false;

  // compare dependents (as sorted copies)
  vector<edge_t>   d1 (g1.dependents), d2 (g2.dependents); 
  sort (d1.begin(), d1.end());
  sort (d2.begin(), d2.end());
  for (size_t c= 0; c < d1.size(); c++)
    if (d1[c] != d2[c]) return false;

  // compare the out_edges for each pair of vertices
  ei_t vi1, v_end1, vi2, v_end2;
  tie (vi1, v_end1)= vertices (g1);
  tie (vi2, v_end2)= vertices (g2);
  edge_equal_t<line_graph_t> edge_equal (g1, g2);
  edge_less_t<line_graph_t> edge_less (g1, g2);
  for (; vi1 != v_end1; ++vi1, ++vi2) {
    if (out_degree (*vi1, g1) != out_degree (*vi2, g2))
      return false;

    ofi_t   oei1, oe_end1, oei2, oe_end2;
    tie (oei1, oe_end1)= out_edges (*vi1, g1);
    vector<face_t>  oe1 (oei1, oe_end1);
    sort (oe1.begin(), oe1.end(), edge_less);

    tie (oei2, oe_end2)= out_edges (*vi2, g2);
    vector<face_t>  oe2 (oei2, oe_end2);
    sort (oe2.begin(), oe2.end(), edge_less);

    // now we can compare the sorted out_edges
    for (size_t c= 0; c < oe1.size(); c++)
      if (!edge_equal (oe1[c], oe2[c]))
	return false;
  }

  return true;
}

int overall_markowitz_degree (const line_graph_t& lg) {

  int degree= 0;
  line_graph_t::fi_t fi, f_end;
  for (boost::tie (fi, f_end)= edges (lg); fi != f_end; ++fi)
    degree+= vertex_type (source (*fi, lg), lg) != independent
             && vertex_type (target (*fi, lg), lg) != dependent;

  return degree;
}

int markowitz_degree (int j, const line_graph_t& lg) {

  property_map<pure_line_graph_t, vertex_name_t>::const_type evn= get(vertex_name, lg);
  
  int degree= 0;

  line_graph_t::fi_t fi, f_end;
  for (boost::tie (fi, f_end)= edges (lg); fi != f_end; ++fi) {
    line_graph_t::edge_t   ij= source (*fi, lg), jk= target (*fi, lg);
    THROW_DEBUG_EXCEPT_MACRO (evn[ij].second != evn[jk].first, consistency_exception,
			   "Adjacency corrupted in line graph"); 
    if (vertex_type (ij, lg) == independent
	|| vertex_type (jk, lg) == dependent) continue;
    degree+= evn[ij].second == j; }

  return degree;
}

void line_graph_t::copy_properties (const line_graph_t& _g) {

  line_graph_t::evn_t evn= get(vertex_name, *this);
  line_graph_t::ed_t   el= get(vertex_degree, *this);  // edge label

  line_graph_t::const_evn_t cevn= get(vertex_name, _g);
  line_graph_t::const_ed_t   cel= get(vertex_degree, _g);  // edge label

  for (size_t i= 0; i < num_vertices (*this); i++) {
    evn[i]= cevn[i]; el[i]= cel[i]; }
}

void accu_exp_graph_t::set_graph (line_graph_t::edge_t e_out, line_graph_t::edge_t e1,
				  line_graph_t::edge_t e2, std::vector<int> exp_nr) {
  for (int c= 0; c < 5; c++) add_vertex (*this);
  boost::property_map<pure_accu_exp_graph_t, boost::vertex_name_t>::type vprop= get (vertex_name, *this);
  if (exp_nr[e1] == -1) vprop[0].set_node (e1); else vprop[0].set_exp (exp_nr[e1]);
  if (exp_nr[e2] == -1) vprop[1].set_node (e2); else vprop[1].set_exp (exp_nr[e2]);
  if (exp_nr[e_out] == -1) vprop[2].set_node (e_out); else vprop[2].set_exp (exp_nr[e_out]);
  vprop[3].set_op (accu_exp_t::mult); vprop[4].set_op (accu_exp_t::add);
  add_edge (0, 3, *this); add_edge (1, 3, *this); add_edge (2, 4, *this); add_edge (3, 4, *this);
  X= 3; dependents.push_back (4);
}

void accu_exp_graph_t::set_graph (accu_exp_t::op_t op, line_graph_t::edge_t e1, 
				  line_graph_t::edge_t e2,
				  std::vector<int> exp_nr) { 
  for (int c= 0; c < 3; c++) add_vertex (*this);
  boost::property_map<pure_accu_exp_graph_t, boost::vertex_name_t>::type vprop= get (vertex_name, *this);
  if (exp_nr[e1] == -1) vprop[0].set_node (e1); else vprop[0].set_exp (exp_nr[e1]);
  if (exp_nr[e2] == -1) vprop[1].set_node (e2); else vprop[1].set_exp (exp_nr[e2]);
  vprop[2].set_op (op);
  add_edge (0, 2, *this); add_edge (1, 2, *this); 
  X= 2; dependents.push_back (2);
}  

void accu_exp_graph_t::set_graph (line_graph_t::edge_t edge) {
  add_vertex (*this);
  boost::property_map<pure_accu_exp_graph_t, boost::vertex_name_t>::type vprop= get (vertex_name, *this);
  vprop[0].set_node (edge); X= 1;
}

void accu_graph_t::set_jacobi_entries () {
  jacobi_entries.resize (accu_exp.size(), make_pair (0, 0));
  THROW_DEBUG_EXCEPT_MACRO ((int) exp_nr.size() != lg.v(), consistency_exception,
			 "Array exp_nr has wrong size"); 
  THROW_DEBUG_EXCEPT_MACRO (!lg.check(), consistency_exception, "Line graph inconsistent"); 
  THROW_DEBUG_EXCEPT_MACRO (!lg.is_tripartite(), consistency_exception, "Line graph not tripartite"); 
  line_graph_t::const_evn_t evn= get(vertex_name, lg);
  line_graph_t::ei_t ei, e_end;
  for (tie (ei, e_end)= vertices (lg); ei != e_end; ei++) {
    if (lg.vertex_type (*ei) == intermediate) {
      if (exp_nr[*ei] == -1) {
	accu_exp.resize (accu_exp.size() + 1);
	accu_exp[accu_exp.size()-1].set_graph(*ei);
	jacobi_entries.push_back (evn[*ei]);
      } else
	jacobi_entries[exp_nr[*ei]]= evn[*ei]; 
    }
  }
  THROW_DEBUG_EXCEPT_MACRO (accu_exp.size() != jacobi_entries.size(), consistency_exception, 
			 "Wrong number of Jacobi entries");
}

  EdgeElim::EdgeElim() {
  }

  EdgeElim::EdgeElim(const c_graph_t::edge_t& e,
		     bool isFront,
		     const c_graph_t& angelLCG) :
                       myIsFrontFlag (isFront),
		       mySource (source(e, angelLCG)),
		       myTarget (target(e, angelLCG)) {
  }

  EdgeElim::EdgeElim(const edge_bool_t& be,
		     const c_graph_t& angelLCG) :
                       myIsFrontFlag (be.second),
		       mySource (source(be.first, angelLCG)),
		       myTarget (target(be.first, angelLCG)) {
  }

  EdgeElim::EdgeElim(const edge_ij_elim_t& eij) :
                       myIsFrontFlag (eij.front),
		       mySource (eij.i),
		       myTarget (eij.j) {
  }

  std::string EdgeElim::debug() const {
    std::ostringstream out;
    myIsFrontFlag ? out << "front"
                  : out << "back";
    out << "eliminate edge (" << mySource << "," << myTarget << ")" << std::ends;
    return out.str();
  } // end EdgeElim::debug()

  bool EdgeElim::isFront() const {
    return myIsFrontFlag;
  } // end EdgeElim::isFront()

  unsigned int EdgeElim::getSource() const {
    return mySource;
  } // end EdgeElim::getSource()

  unsigned int EdgeElim::getTarget() const {
    return myTarget;
  } // end EdgeElim::getTarget()

  c_graph_t::edge_t EdgeElim::getE(const c_graph_t& angelLCG) const {
    return getEdge(mySource, myTarget, angelLCG);
  } // end EdgeElim::getE()

  edge_bool_t EdgeElim::getBool(const c_graph_t& angelLCG) const {
    return make_pair(getEdge(mySource, myTarget, angelLCG), myIsFrontFlag);
  } // end EdgeElim::getBool()

  unsigned int EdgeElim::getCost(const c_graph_t& angelLCG) const {
    boost::property_map<c_graph_t, EdgeType>::const_type eType = get(EdgeType(), angelLCG);
    if (eType[getE(angelLCG)] == UNIT_EDGE)
      return 0;
    // this edge is nonunit => cost unless the other edge is unit
    unsigned int cost = 0;
    if (myIsFrontFlag) { // front elimination
      c_graph_t::oei_t oei, oe_end;
      for (tie(oei, oe_end) = out_edges(myTarget, angelLCG); oei != oe_end; ++oei)
        if (eType[*oei] != UNIT_EDGE)
          cost++;
    }
    else { // back elimination
      c_graph_t::iei_t iei, ie_end;
      for (tie(iei, ie_end) = in_edges(mySource, angelLCG); iei != ie_end; ++iei)
        if (eType[*iei] != UNIT_EDGE)
          cost++;
    }
    return cost;
  } // end EdgeElim::getCost()

  Rerouting::Rerouting() {
  }

  Rerouting::Rerouting(const c_graph_t::edge_t e,
		       const c_graph_t::edge_t pivot_e,
		       bool isPre,
		       const c_graph_t& angelLCG) {
    init(e, pivot_e, isPre, angelLCG);
  }

  Rerouting::Rerouting(const edge_reroute_t& er,
		       const c_graph_t& angelLCG) {
    init(er.e, er.pivot_e, er.isPre, angelLCG);
  }

  std::string Rerouting::debug() const {
    std::ostringstream out;
    pre ? out << "preroute edge (" << i << "," << k << ") about pivot edge (" << j << "," << k << ")" << std::ends
	: out << "postroute edge (" << i << "," << k << ") about pivot edge (" << i << "," << j << ")" << std::ends;
    return out.str();
  } // end Rerouting::debug()

  bool Rerouting::isPre() const {
    return pre;
  } // end Rerouting::isPre()
  
  c_graph_t::edge_t Rerouting::getE(const c_graph_t& angelLCG) const {
    // e goes from i to k, regardless of whether it's a prerouting or a postrouting
    return getEdge(i, k, angelLCG);
  } // end Rerouting::getE()

  c_graph_t::edge_t Rerouting::getPivotE(const c_graph_t& angelLCG) const {
    return pre ? getEdge(j, k, angelLCG)
	       : getEdge(i, j, angelLCG);
  } // end Rerouting::getPivotE()

  edge_reroute_t Rerouting::getER(const c_graph_t& angelLCG) const {
    return edge_reroute_t (getE(angelLCG), getPivotE(angelLCG), pre);
  } // end Rerouting::getER()

  unsigned int Rerouting::getI() const {
    return i;
  } // end Rerouting::getI()

  unsigned int Rerouting::getJ() const {
    return j;
  } // end Rerouting::getJ()

  unsigned int Rerouting::getK() const {
    return k;
  } // end Rerouting::getK()

  bool Rerouting::operator==(const Rerouting& anotherRerouting) const {
    if (this->isPre() == anotherRerouting.isPre()
     && this->getI() == anotherRerouting.getI()
     && this->getJ() == anotherRerouting.getJ()
     && this->getK() == anotherRerouting.getK())
      return true;
    else
      return false;
  } // end Rerouting::operator==()

  void Rerouting::init(const c_graph_t::edge_t& e,
		       const c_graph_t::edge_t& pivot_e,
		       bool isPre,
		       const c_graph_t& angelLCG) {
    if (isPre) {
      THROW_EXCEPT_MACRO(target(e, angelLCG) != target(pivot_e, angelLCG),
		      consistency_exception,
		      "edge_ijk_reroute_t: the edge and the pivot edge must have the same target for a prerouting");
      i = source(e, angelLCG);
      j = source(pivot_e, angelLCG);
      k = target(e, angelLCG);
    }
    else {
      THROW_EXCEPT_MACRO(source(e, angelLCG) != source(pivot_e, angelLCG),
		      consistency_exception,
		      "edge_ijk_reroute_t: the edge and the pivot edge must have the same target for a prerouting");
      k = target(e, angelLCG);
      j = target(pivot_e, angelLCG);
      i = source(e, angelLCG);
    }
    pre = isPre;
  } // end Rerouting::init()

  Transformation::Transformation(const EdgeElim& anEdgeElim) :
				   myIsReroutingFlag (false),
				   myEdgeElim (anEdgeElim) {
  }

  Transformation::Transformation(const edge_bool_t& be,
				 const c_graph_t& angelLCG) :
				   myIsReroutingFlag (false),
				   myEdgeElim (be, angelLCG) {
  }

  Transformation::Transformation(const edge_ij_elim_t& an_ij_elim) :
				   myIsReroutingFlag (false),
				   myEdgeElim (an_ij_elim) {
  }

  Transformation::Transformation(const Rerouting& aRerouting) :
				   myIsReroutingFlag (true),
				   myRerouting (aRerouting) {
  }

  Transformation::Transformation(const edge_reroute_t& aRerouteElim,
				 const c_graph_t& angelLCG) :
				   myIsReroutingFlag (true),
				   myRerouting (aRerouteElim, angelLCG) {
  }

  std::string Transformation::debug() const {
    std::ostringstream out;
    myIsReroutingFlag ? out << myRerouting.debug().c_str()
                      : out << myEdgeElim.debug().c_str();
    return out.str();
  } // end Transformation::debug()

  bool Transformation::isRerouting() const {
    return myIsReroutingFlag;
  } // end Transformation::isRerouting()

  const EdgeElim& Transformation::getEdgeElim() const {
    return myEdgeElim;
  } // end Transformation::getEdgeElim()

  const Rerouting& Transformation::getRerouting() const {
    return myRerouting;
  } // end Transformation::getRerouting()

} // namespace angel


// =========================== doxygen input for whole lib

/*! \mainpage ANGEL Reference Manual
  \author Peter Gottschling et.al.
  \version current snapshot

  \section intro Introduction

ANGEL-Automatic differentiation Nested Graph Elimination Library is 
a template library using the Boost Graph Library and the
Standard C++ Library.
It provides a sparse representation of c-graphs
and their dual line graphs,
 vertex, edge and face elimination,
a wide range of heuristics, 
simulated annealing, and
a set of utility functions.
*/

