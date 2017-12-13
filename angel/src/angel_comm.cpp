// $Id: angel_comm.cpp,v 1.5 2008/02/28 14:57:32 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#include "angel/include/angel_comm.hpp"

#ifdef USE_MPI

#include <algorithm>

namespace GMPI {

using namespace std;
using namespace boost;
using namespace angel;

comm_buffer_t& operator<< (comm_buffer_t& buffer, const c_graph_t& cg) {
  int x= cg.x(), y= cg.y(), v= cg.v(), e= num_edges (cg);
  buffer.reserve (y + v + 2 * e + 5);
  buffer << c_graph_id << x << y << v << e;
  for (int c= 0; c < y; c++) buffer << cg.dependents[c];

  // for each vertex: #out_edge;, each out_edge: target+weight
  c_graph_t::const_ew_t ew= get(edge_weight, cg);  
  c_graph_t::vi_t       vi, v_end;
  for (tie (vi, v_end)= vertices (cg); vi != v_end; vi++) {
    buffer << out_degree (*vi, cg);
    c_graph_t::oei_t  oei, oe_end;
    for (tie (oei, oe_end)= out_edges (*vi, cg); oei != oe_end; oei++) 
      buffer << target (*oei, cg) << ew[*oei]; }
  return buffer;
}

const comm_buffer_t& operator>> (const comm_buffer_t& buffer, c_graph_t& cg) {
  int x, y, v, e, idtmp;
  buffer >> idtmp >> x >> y >> v >> e;
  // graph_id_t id= (graph_id_t) idtmp;
  // if (id != c_graph_id) throw comm_exception ("Buffer contains no c graph");
  vector<c_graph_t::vertex_t>     depvec (y);
  for (int c= 0; c < y; c++) buffer >> depvec[c];
  c_graph_t gtmp (v, x, depvec); cg= gtmp;

  c_graph_t::ew_t ew= get(edge_weight, cg);  
  for (int cv= 0; cv < v; cv++) {
    int out_degree= buffer.read();
    for (int ce= 0; ce < out_degree; ce++) {
      int target= buffer.read(), weight= buffer.read();
      pair<c_graph_t::edge_t, bool> newedge= add_edge (cv, target, cg);
      ew[newedge.first]= weight; } }
  return buffer;
}

comm_buffer_t& operator<< (comm_buffer_t& buffer, const line_graph_t& lg) {
  int x= lg.x(), y= lg.y(), v= lg.v(), e= num_edges (lg);
  buffer.reserve (y + 4 * v + e + 5);
  buffer << line_graph_id << x << y << v << e;
  for (int c= 0; c < y; c++) buffer << lg.dependents[c];

  // for each vertex: degree+c_edge_name+#out_edge; each out_edge: target
  line_graph_t::const_ed_t   ed= get(vertex_degree, lg);  
  line_graph_t::const_evn_t  evn= get(vertex_name, lg);  
  line_graph_t::ei_t         ei, e_end;
  for (tie (ei, e_end)= vertices (lg); ei != e_end; ei++) {    
    buffer << ed[*ei] << evn[*ei].first << evn[*ei].second << out_degree (*ei, lg);
    line_graph_t::ofi_t  ofi, of_end;
    for (tie (ofi, of_end)= out_edges (*ei, lg); ofi != of_end; ofi++)
      buffer << target (*ofi, lg); }
  return buffer;
}

const comm_buffer_t& operator>> (const comm_buffer_t& buffer, line_graph_t& lg) {
  int x, y, v, e, idtmp;
  buffer >> idtmp >> x >> y >> v >> e;
  // graph_id_t id= (graph_id_t) idtmp;
  // if (id != line_graph_id) throw comm_exception ("Buffer contains no line graph");
  vector<line_graph_t::edge_t>     depvec (y);
  for (int c= 0; c < y; c++) buffer >> depvec[c];
  line_graph_t gtmp (v, x, depvec); lg= gtmp;

  line_graph_t::ed_t   ed= get(vertex_degree, lg);  
  line_graph_t::evn_t  evn= get(vertex_name, lg);  
  for (int cv= 0; cv < v; cv++) {
    int out_degree;
    buffer >> ed[cv] >> evn[cv].first >> evn[cv].second >> out_degree;
    for (int ce= 0; ce < out_degree; ce++) {
      int target= buffer.read(); 
      add_edge (cv, target, lg); } }
  return buffer;
}
  
} // namespace GMPI

#endif // USE_MPI
