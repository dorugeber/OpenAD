// $Id: angel_tools.cpp,v 1.4 2008/02/28 14:57:33 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#include "angel/include/angel_tools.hpp"

#include <queue>

#include "angel/include/angel_exceptions.hpp"
#include "angel/include/angel_io.hpp"
#include "angel/include/eliminations.hpp"

namespace angel {

using namespace std;
using namespace boost;

bool reachable (const c_graph_t::vertex_t src,
                const c_graph_t::vertex_t tgt,
                c_graph_t& angelLCG) {
  property_map<pure_c_graph_t, VertexVisited>::type visited = get(VertexVisited(), angelLCG);

  c_graph_t::oei_t oei, oe_end;
  for (tie (oei, oe_end) = out_edges (src, angelLCG); oei != oe_end; ++oei) {
    if (target(*oei, angelLCG) == tgt) return true;
    if (!visited[target(*oei, angelLCG)]) { // call recursively on unvisited successors
      visited[target(*oei, angelLCG)] = true;
      if (reachable (target(*oei, angelLCG), tgt, angelLCG)) return true;
    }
  } // end all outedges

  return false;
} // end

void vertex_upset (const c_graph_t::vertex_t v,
                   const c_graph_t& angelLCG,
                   vertex_set_t& upset) {
  upset.clear();
  upset.insert(v);

  c_graph_t::oei_t oei, oe_end;
  for (tie (oei, oe_end) = out_edges (v, angelLCG); oei != oe_end; ++oei) {
    vertex_set_t successor_upset;
    vertex_upset (target(*oei, angelLCG), angelLCG, successor_upset);
    upset.insert(successor_upset.begin(), successor_upset.end());
  }
}

void vertex_downset (const c_graph_t::vertex_t v,
		     const c_graph_t& angelLCG,
		     vertex_set_t& downset) {
  downset.clear();
  downset.insert(v);

  c_graph_t::iei_t iei, ie_end;
  for (tie (iei, ie_end) = in_edges (v, angelLCG); iei != ie_end; ++iei) {
    vertex_set_t predecessor_downset;
    vertex_downset (source (*iei, angelLCG), angelLCG, predecessor_downset);
    downset.insert(predecessor_downset.begin(), predecessor_downset.end());
  }
}

c_graph_t::edge_t getEdge(unsigned int i,
			  unsigned int j,
			  const c_graph_t& angelLCG) {
  c_graph_t::edge_t e;
  bool found_e;
  tie (e, found_e) = edge(i, j, angelLCG);
  THROW_EXCEPT_MACRO(!found_e, consistency_exception, "getEdge: edge could not be found from src,tgt");
  return e;
} // end getEdge()

bool lex_less_face (line_graph_t::face_t e1, line_graph_t::face_t e2,
		    const line_graph_t& lg) {

  line_graph_t::edge_t s1= source (e1, lg), s2= source (e2, lg),
                       t1= target (e1, lg), t2= target (e2, lg);

  // vertices i, j and k of e1 and e2
  property_map<pure_line_graph_t, vertex_name_t>::const_type evn= get(vertex_name, lg);
  c_graph_t::vertex_t vi1= evn[s1].first, vj1= evn[s1].second, vk1= evn[t1].second,
                       vi2= evn[s2].first, vj2= evn[s2].second, vk2= evn[t2].second;

#ifndef NDEBUG
  if (evn[s1].second != evn[t1].first || evn[s2].second != evn[t2].first) {
    cout << "face " << s1 << "," << t1 << " or " << s2 << "," << t2 << " mismatch\n";
    write_graph ("graph", lg);
    write_vertex_property (std::cout, "edge names ", evn, lg); }
#endif

  return vj1 < vj2 || (vj1 == vj2 && vi1 < vi2) || (vj1 == vj2 && vi1 == vi2 && vk1 <= vk2);
}

// =====================================================
// vertex properties
// =====================================================

void in_out_path_lengths (const c_graph_t& cg,
			  std::vector<int>& vni, std::vector<int>& vli, 
			  std::vector<int>& vno, std::vector<int>& vlo) {

  vni.resize (num_vertices (cg));
  vli.resize (num_vertices (cg));
  vno.resize (num_vertices (cg));
  vlo.resize (num_vertices (cg));

  graph_traits<c_graph_t>::vertex_iterator     vi, v_end;
  int c;

  // initialize independent vertices and set vi to first intermediate vertex
  tie(vi, v_end)= vertices(cg);
  for (c= 0; c < cg.x(); c++, ++vi) {
    vni[c]= 1; vli[c]= 0; }

  // set other vertices from predecessors
  for (; vi != v_end; c++, ++vi) {
    vni[c]= 0; vli[c]= 0;
    graph_traits<c_graph_t>::in_edge_iterator iei, ie_end;
    for (tie(iei, ie_end)= in_edges(*vi, cg); iei != ie_end; ++iei) {
      c_graph_t::vertex_descriptor p= source (*iei, cg);
      vni[c]+= vni[p]; vli[c]+= vni[p] + vli[p]; }
  }

  // initialize vertices without successors
  tie(vi, v_end)= vertices(cg); --v_end; // to the last not behind
  c= num_vertices (cg) - 1;
  for (; out_degree (*v_end, cg) > 0; c--, --v_end) {
    vno[c]= 1; vlo[c]= 0; }

  // set other vertices from successors
  for (; c >= 0; c--, --v_end) {
    vno[c]= 0; vlo[c]= 0;
    graph_traits<c_graph_t>::out_edge_iterator oei, oe_end;
    for (tie(oei, oe_end)= out_edges(*v_end, cg); oei != oe_end; ++oei) {
      c_graph_t::vertex_descriptor s= target (*oei, cg);
      vno[c]+= vno[s]; vlo[c]+= vno[s] + vlo[s]; }
  }
}

void number_dependent_vertices (const c_graph_t& cg, std::vector<int>& v) {
  typedef c_graph_t::vertex_t                  vertex_t;

  v.resize (num_vertices (cg));
  fill (v.begin(), v.end(), 0);

  for (size_t c= 0; c < cg.dependents.size(); c++) {
    vertex_t dep= cg.dependents[c];
    // which vertices are relevant for dep ?
    queue<vertex_t> q; q.push (dep);
    vector<int>     dv (v.size()); dv[dep]= 1;

    while (!q.empty()) {
      vertex_t            v= q.front();
      c_graph_t::iei_t    iei, ie_end;
      for (tie(iei, ie_end)= in_edges(v, cg); iei != ie_end; ++iei) {
	vertex_t vin= source (*iei, cg);
	if (!dv[vin]) {
	  dv[vin]= 1; q.push (vin); } }
      q.pop(); }

    for (size_t i= 0; i < v.size(); i++)
      v[i]+= dv[i];
  }
}

void number_independent_vertices (const c_graph_t& cg, std::vector<int>& v) {

  typedef c_graph_t::vertex_descriptor                  vertex_t;
  typedef graph_traits<c_graph_t>::vertex_iterator      vi_t;
  //  typedef graph_traits<c_graph_t>::in_edge_iterator     ie_t;
  typedef graph_traits<c_graph_t>::adjacency_iterator   ai_t;

  v.resize (num_vertices (cg));
  std::fill (v.begin(), v.end(), 0);

  vi_t                          ivi, iv_end;
  int                           c= 0;
  for (tie(ivi, iv_end)= vertices(cg), c= 0; c < cg.x() && ivi != iv_end; ++c, ++ivi) {
    // which vertices are reachable from *ivi ?
    std::queue<vertex_t> q; q.push (*ivi);
    std::vector<int>     dv (v.size()); dv[*ivi]= 1;

    while (!q.empty()) {
      vertex_t v= q.front();
      ai_t     ai, a_end;
      for (tie(ai, a_end)= adjacent_vertices(v, cg); ai != a_end; ++ai)
	if (!dv[*ai]) {
	  dv[*ai]= 1; q.push (*ai); }
      q.pop(); 
    }

    for (size_t i= 0; i < v.size(); i++)
      v[i]+= dv[i];
  }
}

// =====================================================
// graph transformations
// =====================================================


// independent vertices shall be first ones after permutation
void permutate_vertices (const c_graph_t& gin, const vector<c_graph_t::vertex_t>& p,
			 c_graph_t& gout) {
  
  typedef c_graph_t::vertex_t vertex_t;
  // permutate vector of dependent vertices
  vector<vertex_t> deps;
  for (size_t c= 0; c < gin.dependents.size(); c++)
    deps.push_back (p[gin.dependents[c]]);

  c_graph_t gtmp (gin.v(), gin.x(), deps);
  int en= 0; // counter for edge_number
  
  c_graph_t::const_ew_t ewc= get(edge_weight, gin);  
  c_graph_t::ew_t       ew= get(edge_weight, gout);  
  c_graph_t::ei_t       ei, e_end;
  for (tie (ei, e_end)= edges (gin); ei != e_end; ++ei) {
    bool ins; c_graph_t::edge_t edge;
    tie (edge, ins)= add_edge (p[source(*ei,gin)], p[target(*ei,gin)], en++, gtmp);
    ew[edge]= ewc[*ei]; }

  // it may be usefull to sort the out_edges of each vertex 

  gtmp.next_edge_number= renumber_edges (gtmp);
  gout.swap (gtmp);
}

void independent_vertices_to_front (const c_graph_t& gin, 
				    const vector<c_graph_t::vertex_t>& indeps,
				    c_graph_t& gout) {
  typedef c_graph_t::vertex_t vertex_t;
  THROW_EXCEPT_MACRO (gin.x() != int (indeps.size()), consistency_exception, 
		   "Wrong number of independents");
  vector<vertex_t> counter (gin.v());

  int cindep= 0, cnotin= 0;
  for (int c= 0; c < gin.v(); c++)
    if (std::find (indeps.begin(), indeps.end(), vertex (c, gin)) != indeps.end())
      counter[c]= cindep++;
    else
      counter[c]= cnotin++;

  for (int c= 0; c < gin.v(); c++)
    if (std::find (indeps.begin(), indeps.end(), vertex (c, gin)) == indeps.end())
      counter[c]+= cindep;

  permutate_vertices (gin, counter, gout);
}

int renumber_edges (c_graph_t& cg) {

  graph_traits<c_graph_t>::edge_iterator      ei, e_end;
  property_map<c_graph_t, edge_index_t>::type  eid= get (edge_index, cg);

  int       c;
  for (tie (ei, e_end)= edges (cg), c= 0; ei != e_end; ++ei, c++)
    eid[*ei]= c;
  return c;
}

void take_over_successors (c_graph_t::vertex_t v1, c_graph_t::vertex_t v2, 
			   int offset, const c_graph_t& g1, 
			   int& edge_number, c_graph_t& g2) {

  c_graph_t::oei_t oei, oe_end;
  for (tie (oei, oe_end)= out_edges (v1, g1); oei != oe_end; ++oei) {
    int t= int (target (*oei, g1)), it= offset + t;
    THROW_DEBUG_EXCEPT_MACRO (it <= 0 || it >= g2.v(), consistency_exception, 
			   "Target out of range");
    c_graph_t::vertex_t vt= vertex (it, g2); // equivalent vertex in g2
    add_edge (v2, vt, edge_number++, g2); 
  }
}



// -----------------------------------------------------
// some preprocessing removals
// -----------------------------------------------------

int remove_hoisting_vertices (c_graph_t& cg) {

  int hv= 0;
  c_graph_t::vi_t   vi, v_end;
  for (tie (vi, v_end)= vertices (cg); vi != v_end; ++vi)
    if (cg.vertex_type (*vi) == intermediate 
	&& in_degree (*vi, cg) == 1 
	&& out_degree (*vi, cg) == 1)
	hv+= vertex_elimination (*vi, cg);
  return hv;
}

void remove_parallel_edges (c_graph_t& cg) {

  typedef std::pair<c_graph_t::edge_t,bool> eb_t;

  c_graph_t gtmp (cg.v(), cg.x(), cg.dependents);
  int edge_number= 0;

  property_map<c_graph_t, edge_weight_t>::type   ew1= get(edge_weight, cg),
                                                  ew2= get(edge_weight, gtmp);

  c_graph_t::vi_t      vi, v_end;
  for (boost::tie (vi, v_end)= vertices (cg); vi != v_end; vi++) {
    std::vector<c_graph_t::vertex_t> targets;
    c_graph_t::oei_t      oei, oe_end;
    for (boost::tie (oei, oe_end)= out_edges (*vi, cg); oei != oe_end; oei++) {
      c_graph_t::vertex_t t= target (*oei, cg);
      eb_t edge_double= edge (*vi, t, gtmp);         // already inserted ?
      if (edge_double.second)                       // if so
	ew2[edge_double.first]+= ew1[*oei];
      else {
	eb_t new_edge= add_edge (*vi, t, edge_number++, gtmp);
	ew2[new_edge.first]=  ew1[*oei];
	targets.push_back (t); } 
    } }
  
  cg.swap (gtmp);
}

void remove_trivial_edges (c_graph_t& cg) {

  c_graph_t::ew_t ew= get(edge_weight, cg);
  for (bool found= true; found; ) {
    // write_edge_property (std::cout, "edge weights ", ew, cg);
    found= false;
    c_graph_t::ei_t ei, e_end;
    for (tie (ei, e_end)= edges (cg); ei != e_end; ++ei) {
      c_graph_t::edge_t   e= *ei;
      if (ew[e] == 1) {
	int ee; // number of eliminated edges
	if (cg.vertex_type (source (e, cg)) == independent) 
	  ee= front_edge_elimination (e, cg);
	else ee= back_edge_elimination (e, cg); 
	if (ee > 0) {found= true; break;} // after elimination iterators may be invalid
      } } }
}

// =====================================================
// Functions for partial accumulation simulated annealing (scarcity exploitation)
// =====================================================

double gen_prob() {
  double a = rand();
  double b = rand();
  return (a > b) ? b/a
		 : a/b;
} // end gen_prob()

unsigned int chooseTarget_sa(std::vector<double>& deltaE) {
  #define ECONST 2.71828

  //write_vector("deltaE (before adjustment): ", deltaE);

  // prefer improvements, if there are any
  double best_improvement = 100;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    if(best_improvement > deltaE[i])
      best_improvement = deltaE[i];
  if(best_improvement < 0) {
    //cout << "best_improvement of " << best_improvement << " was recognized" << endl;
    for (unsigned int i = 0; i < deltaE.size(); i++)
      if (deltaE[i] > -1)
	deltaE[i] = 0;
      else
	deltaE[i] = pow(ECONST, (-deltaE[i]));
  }
  else {
    for (unsigned int i = 0; i < deltaE.size(); i++)
      deltaE[i] = 1.0/pow(ECONST,deltaE[i] + 1);
  }

  //write_vector("deltaE (after adjustment): ", deltaE);

  // normalize the probabilities
  double deltasum = 0;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltasum += deltaE[i];
  //cout << "deltasum = " << deltasum << endl;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltaE[i] = deltaE[i]/deltasum;
  //write_vector("deltaE (normalized): ", deltaE);

  // choose a vector index randomly
  double Pr = gen_prob();
  double current_ptr = deltaE[0];
  unsigned int choice_index = 0;
  while(current_ptr < Pr) {
    choice_index++;
    current_ptr += deltaE[choice_index];
  }
  //cout << "got index " << choice_index << " with prob " << Pr <<  endl;
  return choice_index;
} // end chooseTarget_sa()

int chooseEdgeElimRandomly(std::vector<double>& deltaE) {
  if (gen_prob() > 0.95)
    return -1;

  #define ECONST 2.71828
  //write_vector("deltaE (before adjustment): ", deltaE);
  for (unsigned int i = 0; i < deltaE.size(); i++)
    deltaE[i] = 1.0/pow(ECONST,deltaE[i] + 1);
  //write_vector("deltaE (after adjustment): ", deltaE);

  // normalize the probabilities
  double deltasum = 0;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltasum += deltaE[i];
  //cout << "deltasum = " << deltasum << endl;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltaE[i] = deltaE[i]/deltasum;
  //write_vector("deltaE (normalized): ", deltaE);

  // choose a vector index randomly
  double Pr = gen_prob();
  double current_ptr = deltaE[0];
  unsigned int choice_index = 0;
  while(current_ptr < Pr) {
    choice_index++;
    current_ptr += deltaE[choice_index];
  }
  return choice_index;
} // end chooseEdgeElimRandomly()

int chooseEdgeElimRandomlyGreedy(std::vector<double>& deltaE) {
  #define ECONST 2.71828
  //write_vector("deltaE (before adjustment): ", deltaE);

  // select against those that arent best
  double best = deltaE[0];
  size_t best_index = 0;
  for (size_t i = 0; i < deltaE.size(); i++) {
    if (deltaE[i] < best) {
      best = deltaE[i];
      best_index = i;
    }
  }
  for (size_t i = 0; i < deltaE.size(); i++)
    if (deltaE[i] > best)
      deltaE[i] += 30000;

  for (unsigned int i = 0; i < deltaE.size(); i++)
    deltaE[i] = 1.0/pow(ECONST,deltaE[i] + 1);
  //write_vector("deltaE (after adjustment): ", deltaE);

  // normalize the probabilities
  double deltasum = 0;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltasum += deltaE[i];
  //cout << "deltasum = " << deltasum << endl;
  for(unsigned int i = 0; i < deltaE.size(); i++)
    deltaE[i] = deltaE[i]/deltasum;
  //write_vector("deltaE (normalized): ", deltaE);

  // choose a vector index randomly
  double Pr = gen_prob();
  double current_ptr = deltaE[0];
  unsigned int choice_index = 0;
  while(current_ptr < Pr) {
    choice_index++;
    current_ptr += deltaE[choice_index];
  }

  return choice_index;
} // end chooseEdgeElimRandomly()

} // namespace angel
