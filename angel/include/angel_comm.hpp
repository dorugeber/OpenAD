// $Id: angel_comm.hpp,v 1.10 2004/10/16 04:18:17 jean_utke Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/

#ifndef 	_angel_comm_include_
#define 	_angel_comm_include_

#ifdef USE_MPI

#include "angel_types.hpp"
#include "gmpi.hpp"
#include "eliminations.hpp"

namespace angel {
  /// Buffer type for all communications
  typedef GMPI::buffer_t<int>     comm_buffer_t;

  /// Graph ID, e.g. to identify received int arrays
  enum graph_id_t {c_graph_id, line_graph_id, accu_graph_id}; 

  typedef GMPI::comm_ref_t<int, c_graph_t>      c_graph_comm_ref_t;

  typedef GMPI::comm_ref_t<int, line_graph_t>   line_graph_comm_ref_t;

  // Tags used in angel
  const int completion_tag= 3377;

}

// additional operators for GMPI
namespace GMPI {
  // =============== reading from and writing into buffers ===========
  
  /// Read a vertex from buffer
  inline const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
						 angel::c_graph_t::vertex_t& input) {
    input= buffer.read (); return buffer; }

  /// Write a vertex into buffer
  inline angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, 
					   const angel::c_graph_t::vertex_t& output) {
    buffer.write (output); return buffer; }
  
  /// Read a edge elimination from buffer
  inline const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
						 angel::edge_ij_elim_t& input) {
    int tmp; buffer >> input.i >> input.j >> tmp; 
    input.front= (bool) tmp; return buffer; }

  /// Write a edge elimination into buffer
  inline angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, 
					   const angel::edge_ij_elim_t& output) {
    buffer << output.i << output.j << (int) output.front; return buffer; }

  /// Read a face elimination from buffer
  inline const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
					     angel::triplet_t& input) {
    buffer >> input.i >> input.j >> input.k; return buffer; }

  /// Write a face elimination into buffer
  inline angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, 
					   const angel::triplet_t& output) {
    buffer << output.i << output.j << output.k; return buffer; }

  /// Read a c graph from buffer
  const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
					  angel::c_graph_t& cg);
  
  /// Write a c graph into buffer
  angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, const angel::c_graph_t& cg);

  /// Read a line graph from buffer
  const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
					  angel::line_graph_t& input);
  
  /// Write a line graph into buffer
  angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, 
				    const angel::line_graph_t& output);  

  /// Read elimination history, og is not communicated
  template <typename Ad_graph_t, typename El_spec_t>
  const angel::comm_buffer_t& operator>> (const angel::comm_buffer_t& buffer, 
					  angel::elimination_history_t<Ad_graph_t,El_spec_t>& input) {
    // Clear graph and sequence
    Ad_graph_t empty; input.cg= empty; input.seq.resize (0);
    buffer >> input.ccosts >> input.cg >> input.seq;
    return buffer;
  }

  /// Write elimination history, og is not communicated
  template <typename Ad_graph_t, typename El_spec_t>
  angel::comm_buffer_t& operator<< (angel::comm_buffer_t& buffer, 
				    const angel::elimination_history_t<Ad_graph_t,El_spec_t>& input) {
    buffer << input.ccosts << input.cg << input.seq;
    return buffer; }

} // namespace GMPI


#endif // USE_MPI

#endif // 	_angel_comm_include_
