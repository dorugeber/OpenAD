// $Id: graph_generator.cpp,v 1.6 2008/02/28 14:57:33 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#include "angel/include/graph_generator.hpp"

#include <vector>
#include <cstdlib>
#include <numeric>

#include "angel/include/angel_exceptions.hpp"
#include "angel/include/angel_types.hpp"
#include "angel/include/angel_tools.hpp"

#ifndef NDEBUG
#include "angel/include/angel_io.hpp"
#endif

namespace angel {

using namespace std;
using namespace boost;

// -------------------------------------------------------------------------

void random_statement (int inputs, int expr, const vector<double>& p,
		       c_graph_t& statement) {

  typedef c_graph_t::vertex_t    vertex_t;

  int maxar= int (p.size()); // maximal arity of expressions

  // assert (inputs + expr - 1 <= expr * maxar); // all inputs usable
  THROW_DEBUG_EXCEPT_MACRO (inputs + expr - 1 > expr * maxar, consistency_exception,
			 "Un-usable inputs");
  

  // how many inputs each expression shall have
  // (statement inputs have zero)
  int V= inputs + expr; // number of vertices
  vector<int> expi (V);
  for (int i= inputs; i < V; i++)
    expi[i]= angel::random (p) + 1;

  // if expression inputs aren't enough then p will be ignored, 
  // i.e. expi increased randomly
  for (int m= inputs + expr - 1 - accumulate (expi.begin(), expi.end(), 0);
       m > 0; m--)
    for (;;) {
      int e= angel::random (inputs, V-1);
      if (expi[e] < maxar) {expi[e]++; break; }}

  // build graph with last vertex as dependent
  vector<vertex_t> deps (1);
  deps[0]= V - 1;
  c_graph_t gtmp (V, inputs, deps);
  
  // random successor for each input and intermediate expr.
  for (int i= V - 2; i >= 0; i--) {
    vertex_t v= vertex (i, gtmp);
    for (;;) {
      int s= angel::random (max (i+1, inputs), V-1);
      vertex_t vs= vertex (s, gtmp);
      if (int (in_degree (vs, gtmp)) < expi[s]) {
	add_edge (v, vs, gtmp); break;}
    }}

  // if some expression desires more inputs 
  // then reuse statement inputs
  for (int i= inputs; i < V; i++) {
    vertex_t v= vertex (i, gtmp);
    for (; int (in_degree (v, gtmp)) < expi[i];) {
      int pr= angel::random (0, inputs-1);
      vertex_t vpr= vertex (pr, gtmp);
      // c_graph_t::edge_t e;
      // bool found;
      // boost::tie (e, found)= edge (vpr, v, gtmp);
      // if (!found) add_edge (vpr, v, gtmp);
      add_edge (vpr, v, gtmp); // double arcs allowed e.g. x*x
    }
  }

  statement.swap (gtmp);
}

// -------------------------------------------------------------------------

void random_statement_vector (int max_expr, double unary, 
			      std::vector<c_graph_t>& statement_vector) {
  // assert (unary >= 0.0 && unary <= 1.0);
  THROW_DEBUG_EXCEPT_MACRO (unary < 0.0 || unary > 1.0, consistency_exception,
			 "unary must be between 0 and 1"); 
  std::vector<double> p (2); // probabilities of arities
  p[0]= unary; p[1]= 1.0;

  // assert (statement_vector.size() > 0);
  THROW_DEBUG_EXCEPT_MACRO (statement_vector.size() == 0, consistency_exception,
			 "statement vector must be non-empty"); 
  for (size_t i= 0; i < statement_vector.size(); i++) {
    int expr= angel::random (1, max_expr); // number of expr.
    // #inputs in [1, expr+1], average most probable, good for unary=0.5
    int inputs= (angel::random (expr) + angel::random (expr)) / 2 + 1;
    random_statement (inputs, expr, p, statement_vector[i]);
#ifndef NDEBUG
    write_graph ("random_statement", statement_vector[i]);
#endif
  }
}

// -------------------------------------------------------------------------

void stats2block (int inputs, int outputs, const std::vector<c_graph_t>& stats, 
		  c_graph_t& block) {
  typedef c_graph_t::vertex_t          vertex_t;
  typedef c_graph_t::oei_t             oei_t;

  int nstats= int (stats.size());
  // assert (nstats >= outputs); // not more outputs than statements
  THROW_DEBUG_EXCEPT_MACRO (nstats < outputs, consistency_exception,
			 "More outputs than statements"); 

#ifndef NDEBUG
  for (int i= 0; i < nstats; i++) { // really statements ?
    const c_graph_t& stat= stats[i];
    // assert (stat.dependents.size() == 1);
    THROW_DEBUG_EXCEPT_MACRO (stat.dependents.size() != 1, consistency_exception,
			   "Statements must have 1 output"); 
    // assert (stat.dependents[0] == vertex (stat.v()-1, stat)); 
    THROW_DEBUG_EXCEPT_MACRO (stat.dependents[0] != vertex (stat.v()-1, stat), consistency_exception,
			   "Statements output must be last vertex"); }
#endif

  int V= inputs; // number of vertices (in block)
  // difference of vertex number between block and statement
  vector<int> soffset (nstats); 
  for (int i= 0; i < nstats; i++) {
    soffset[i]= V - stats[i].x();
    V+= stats[i].v() - stats[i].x(); }

  // build graph without dependents
  vector<vertex_t> deps;
  c_graph_t gtmp (V, inputs, deps);

  // copy statements into block (without edges from statement inputs)
  int en= 0;
  for (int i= 0; i < nstats; i++) {
    const c_graph_t& stat= stats[i];
    int so= soffset[i];
    for (int iv= stat.x(); iv < stat.v(); iv++)
      take_over_successors (vertex (iv, stat), vertex (iv+so, gtmp), 
			    so, stat, en, gtmp); }

  // each output can be set to some statement output
  // since vertex 0 is always input it can be used to mark output as unset
  deps.resize (outputs); 

  // list of potential successors (each used once)
  // initialized with block outputs
  typedef pair<int,int> succ_t;
  vector<succ_t> sl;
  for (int i= 0; i < outputs; i++)
    sl.push_back (make_pair (-1, i));

  // find successor for each statement output 
  for (int i= nstats-1; i >= 0; i--) {
    vertex_t vb= vertex (soffset[i] + stats[i].v() - 1, gtmp); // stat. output
    int si= angel::random (int (sl.size()));
    succ_t succ= sl[si];
    int succs= succ.first, succi= succ.second;
    if (succs == -1)           // block output
      deps[succi]= vb;
    else {                     // statement input
      const c_graph_t& sstat= stats[succs];     // succ. statement
      vertex_t ivs= vertex (succi, sstat);       // input vertex in sstat
      // connect vb with successors of ivs (their equivalents in block)
      take_over_successors (ivs, vb, soffset[succs], sstat, en, gtmp);
    }
    // remove succ from list, maybe more efficient with index
    remove (sl.begin(), sl.end(), succ); sl.resize (sl.size()-1);
    // add current statement's input to successor list
    for (int j= 0; j < stats[i].x(); j++) 
      sl.push_back (make_pair (i, j));
  }

  // remaining outputs are assigned to some statement (not yet used as output)
  while (sl.size() > 0 && sl[0].first == -1) {
    int output= sl[0].second;
    // remove this output from successor list
    remove (sl.begin(), sl.end(), sl[0]); sl.resize (sl.size()-1);
    vertex_t vb;
    do {                                // find non-output statement
      int ss= angel::random (nstats);
      vb= vertex (soffset[ss] + stats[ss].v() - 1, gtmp); // stat. output
    } while (find (deps.begin(), deps.end(), vb) != deps.end());
    deps[output]= vb;
  }

  // each block input is unified with some free statement input
  for (int i= 0; i < inputs; i++) {
    // assert (sl.size() > 0); // enough free statement inputs ?
    THROW_EXCEPT_MACRO (sl.size() == 0, consistency_exception, "Not enough inputs"); 
    vertex_t vi= vertex (i, gtmp);
    succ_t succ= sl[angel::random (int (sl.size()))];
    int succs= succ.first, succi= succ.second;
    const c_graph_t& sstat= stats[succs];     // statement
    vertex_t ivs= vertex (succi, sstat);       // substituted vertex
    // connect block input with successors of ivs (their equivalents in block)
    take_over_successors (ivs, vi, soffset[succs], sstat, en, gtmp);
    // remove succ from list, maybe more efficient with index
    remove (sl.begin(), sl.end(), succ); sl.resize (sl.size()-1);
  }

  // remaining free statement inputs are unified 
  // with some block input or statement output (smaller statement)
  // potential predecessors stored in pl
  vector<vertex_t> pl;
  for (int i= 0; i < inputs; i++) {
    vertex_t v= vertex (i, gtmp); pl.push_back (v); }
  for (int i= 0; i < nstats; i++) {
    vertex_t v= vertex (soffset[i] + stats[i].v() - 1, gtmp); // stat. output
    pl.push_back (v); }
  while (sl.size() > 0) {
    succ_t succ= sl[sl.size()-1];              // last one easier to delete 
    int succs= succ.first, succi= succ.second;
    const c_graph_t& sstat= stats[succs];     // statement
    vertex_t ivs= vertex (succi, sstat);       // substituted vertex
    vertex_t vp= pl[angel::random (inputs+succs)];
    take_over_successors (ivs, vp, soffset[succs], sstat, en, gtmp);
    sl.resize (sl.size()-1);
  }

  gtmp.dependents.swap (deps);

  block.swap (gtmp);
}

// -------------------------------------------------------------------------

void random_block (int inputs, int outputs, int stats, int max_exp, double unary,
		   c_graph_t& block) {

  std::vector<c_graph_t> statement_vector (stats);
  random_statement_vector (max_exp, unary, statement_vector);
  stats2block (inputs, outputs, statement_vector, block);

}

// -------------------------------------------------------------------------

// will be confused if some vertex in block is both dependent and independent
void block2loop (const c_graph_t& block, int loops,
		 c_graph_t& loop) {


  typedef c_graph_t::vertex_t vertex_t;
  typedef c_graph_t::oei_t    oei_t;

  // some vertex sizes
  int binputs= block.x(), boutputs= block.y(), bv= block.v(),
      linputs= binputs <= boutputs ? binputs : binputs + (loops-1)*(binputs-boutputs),
      // loutputs= binputs >= boutputs ? boutputs : boutputs + (loops-1)*(boutputs-binputs),
      isize= min (binputs, boutputs), // size of interface between blocks
      lv= loops * block.v() - (loops-1) * isize;
  vector<vertex_t> dependents;
  c_graph_t gtmp (lv, linputs, dependents);

  // input vertices of loop, initialized with input of block
  vector<vertex_t> input (binputs);
  for (int c= 0; c < binputs; c++) input[c]= c;

  // first block is simply copied
  int en= 0; // counter for edge_number
  c_graph_t::ei_t      ei, e_end;
  for (tie (ei, e_end)= edges (block); ei != e_end; ++ei)
    add_edge (source (*ei, block), target (*ei, block), en++, gtmp);

  for (int l= 1; l < loops; l++) {
    // vertex_t goffset= l * (lv - min (binputs, boutputs)); // first vertex in new block
    vertex_t goffset= l * (bv - isize), // corresponds to first vertex of new block in loop
             lgoffset= goffset - bv + isize;  // first vertex of previous block

    // unify output of last block with input of current block (first isize vertices resp.)
    for (int i= 0; i < isize; i++) {
      vertex_t bv= vertex (i, block); // vertex in block
      // vertex_t lv= vertex (lgoffset + block.dependents[i], gtmp);
      vertex_t lv= lgoffset + block.dependents[i]; // corresponds to vertex in loop
      oei_t      oei, oe_end;
      for (tie (oei, oe_end)= out_edges (bv, block); oei != oe_end; ++oei) {
	add_edge (lv, goffset + target (*oei, block), en++, gtmp); }
    }

    // look for loop inputs and outputs
    for (int i= isize; i < binputs; i++)
      input.push_back (goffset + vertex_t (i));
    for (int i= isize; i < boutputs; i++)
      dependents.push_back (lgoffset + block.dependents[i]);

    // copy the remainder of the block
    for (int i= isize; i < bv; i++) {
      vertex_t bv= vertex (i, block), // vertex in block
               lv= goffset + vertex_t (i);
      oei_t    oei, oe_end;
      for (tie (oei, oe_end)= out_edges (bv, block); oei != oe_end; ++oei) 
	add_edge (lv, vertex_t (goffset + target (*oei, block)), en++, gtmp);     
    }
  } 
  
  // output of last block is always output of loop
  vertex_t goffset= (loops - 1) * (bv - isize);
  for (int i= 0; i < boutputs; i++)
    dependents.push_back (goffset + block.dependents[i]);

  gtmp.X= int (input.size()); // will be used in independent_vertices_to_front
  gtmp.dependents.swap (dependents); 

  independent_vertices_to_front (gtmp, input, loop);

  // remove all vertices that do not lie on a path from independent 
  // to dependent nodes
  loop.clear_graph (); 
  loop.next_edge_number= renumber_edges (loop);
}

// Initializes the random generator unless SAME_RANDOM_VALUES is defined
random_init_t random_init_object;

} // namespace angel
