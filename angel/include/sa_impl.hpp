// $Id: sa_impl.hpp,v 1.4 2004/04/23 12:59:11 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifdef USE_MPI
#include "angel_comm.hpp"
#endif // USE_MPI

namespace angel {
  using std::vector;

template <class Object_t, class Neighbor_t, class Cost_t, class Temp_t>
int SA (Object_t& object, Neighbor_t neighbor, Cost_t costs, Temp_t temp, int max_iter) {
  int min_costs= costs (object), last_costs= min_costs; // initial costs
  Object_t min_object (object), last_object (object); // corresponding objects

  for (int i= 0; i < max_iter; i++) {
    Object_t new_object (last_object);
    if (!neighbor (new_object)) {
      cout << "No neighbor found!"; return -1;}
    int new_costs= costs (new_object);
    // cout << "LSA costs: " << new_costs;
#ifdef GNUPLOT
    cout << i << "\t" << new_costs << '\n'; // for gnuplot
#endif
    if (new_costs < last_costs) {
      if (new_costs < min_costs) {
	min_costs= new_costs; min_object= new_object; }
      last_costs= new_costs; last_object= new_object; 
      // cout << " accepted due to improvement";
    } else {
      double acc= SA_acceptance (last_costs-new_costs, i, temp);
      double ra= angel::random (1.0);
      if (ra < acc) {
	last_costs= new_costs; last_object= new_object; 
	// cout << " accepted due to Metropolis";
      } } 
    // cout << '\n'; 
  }
  object= min_object;
  return min_costs;
}

template <class Object_t, class Neighbor_t, class Cost_t, class Adaption_t>
int ALSA (Object_t& object, Neighbor_t neighbor, Cost_t costs, Adaption_t adaption,
	  double& gamma, int max_iter) {

  int min_costs= costs (object), last_costs= min_costs; // initial costs
  Object_t min_object (object), last_object (object); // corresponding objects

  for (int i= 0; i < max_iter; i++) {
    Object_t new_object (last_object);
    if (!neighbor (new_object)) {
      cout << "No neighbor found!"; return -1;}
    int new_costs= costs (new_object);
    //cout << "LSA costs: " << new_costs << '\n';
#ifdef GNUPLOT
    cout << i << "\t" << new_costs << '\n'; // for gnuplot
#endif
    adaption (new_costs, gamma);
    if (new_costs < last_costs) {
      if (new_costs < min_costs) {
	min_costs= new_costs; min_object= new_object; }
      last_costs= new_costs; last_object= new_object; 
      // cout << "new configuration accepted due to improvement\n";
    } else {
      double acc= exp ((last_costs-new_costs) / gamma * log (double (i+2)));
      double ra= angel::random (1.0);
      if (ra < acc) {
	last_costs= new_costs; last_object= new_object; 
	// cout << "new configuration accepted due to Metropolis\n";
      } } }

  object= min_object;
  return min_costs;
}

// =====================================================
// neighbourhoods
// =====================================================

  template <class Ad_graph_t, class El_spec_t>
  bool neighbor_last_removable_t::operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh) {
    bool eliminate= eh.seq.size() == 0 || angel::random (1.0) < .5;
    if (eliminate) {
      vector<El_spec_t>   el;
      eliminatable_objects (eh.cg, el);
      if (el.empty() && eh.seq.empty()) return false; // graph was always bipartite
      if (el.size() > 0) {  // if nothing to eliminate -> re-insert
	int nextn= angel::random (int (el.size()));
	El_spec_t next= el[nextn];
	return eh.elimination (next) > 0; } } // elimination successful ?
    eh.seq.resize (eh.seq.size()-1);
    return eh.rebuild_graph (); 
  }

// -------------------------------------------------------------------------

  template <class Ad_graph_t, class El_spec_t>
  bool neighbor_multi_step_t::operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh) {
    bool eliminate= eh.seq.size() == 0 || angel::random (1.0) < .5;
    int steps= angel::random (1, max_steps);
    if (eliminate) {
      for (int i= 0; i < steps; i++) {
	vector<El_spec_t>   el;
	eliminatable_objects (eh.cg, el);
	if (el.empty() && eh.seq.empty()) return false; // graph was always bipartite
	if (el.size() == 0) return true;
	int nextn= angel::random (int (el.size()));
	El_spec_t next= el[nextn];
	bool okay= eh.elimination (next) > 0; // elimination successful ?
	if (!okay) return false; }
      return true; // enough eliminations
    } else {
      eh.seq.resize (std::max (int (eh.seq.size())-steps, 0));
      return eh.rebuild_graph (); } 
  }

// -------------------------------------------------------------------------

  template <class Ad_graph_t, class El_spec_t>
  bool neighbor_sequence_check_t::operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh) {
    bool eliminate= eh.seq.size() == 0 || angel::random (1.0) < .5;
    if (eliminate) {
      vector<El_spec_t>   el;
      eliminatable_objects (eh.cg, el);
      if (el.empty() && eh.seq.empty()) return false; // graph was always bipartite
      if (el.size() > 0) {  // if nothing to eliminate -> re-insert
	int nextn= angel::random (int (el.size()));
	El_spec_t next= el[nextn];
	return eh.elimination (next) > 0; } } // elimination successful ?

    int next_re_ins;
    bool reinsertable= false;
    for (int c= 1; !reinsertable; c++) {
      next_re_ins= angel::random_high (int (eh.seq.size()), c);
      vector<El_spec_t> seq_copy (eh.seq);
      typename vector<El_spec_t>::iterator it= eh.seq.begin() + next_re_ins;
      eh.seq.erase (it);
      reinsertable= eh.rebuild_graph ();
      // if reinsertion failed then restore old seq, cg is unchanged then
      if (!reinsertable) eh.seq.swap (seq_copy); }
    return true; }

// -------------------------------------------------------------------------

  template <class Ad_graph_t, class El_spec_t>
  bool neighbor_check_meta_t::operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh) {
    bool eliminate= eh.seq.size() == 0 || angel::random (1.0) < .5;
    if (eliminate) {
      vector<El_spec_t>   el;
      eliminatable_objects (eh.cg, el);
      if (el.empty() && eh.seq.empty()) return false; // graph was always bipartite
      if (el.size() > 0) {  // if nothing to eliminate -> re-insert
	int nextn= angel::random (int (el.size()));
	El_spec_t next= el[nextn];
	return eh.elimination (next) > 0; } } // elimination successful ?

    int next_re_ins;
    bool reinsertable= false;
    for (int c= 1; !reinsertable; c++) {
      next_re_ins= angel::random_high (int (eh.seq.size()), c);
      elimination_history_t<Ad_graph_t, El_spec_t> eh_copy (eh);
      typename vector<El_spec_t>::iterator it= eh_copy.seq.begin() + next_re_ins;
      El_spec_t re= *it;  // removed elimination
      eh_copy.seq.erase (it);
      reinsertable= eh_copy.rebuild_graph ();
      // if successful check if new graph (eh_copy.cg) is predecessor of eh.cg
      // eh_copy.cg --re--> eh.cg
      if (reinsertable) {
	Ad_graph_t copy_copy (eh_copy.cg);
	angel::eliminate (re, copy_copy);
	reinsertable= eh.cg == copy_copy; }
      if (reinsertable) eh= eh_copy; }
    return true; }


// =====================================================
// Gamma adaption operators
// =====================================================

  void gamma_adaption_max_t::operator() (int costs, double& gamma) {
    if (imp >= D) return;
    if (last_min == 0) { // very first iteration must be distinguished
      last_max= last_min= costs; return; }
    if (costs < last_min) {
      diff= last_max - costs; last_max= last_min= costs;
      if (diff > max_diff) max_diff= diff;
      if (++imp >= D) gamma= scaling * double (max_diff);
    } else if (costs > last_max) last_max= costs;
  }

  void gamma_adaption_average_t::operator() (int costs, double& gamma) {
    if (imp >= D) return;
    if (last_min == 0) { // very first iteration must be distinguished
      last_max= last_min= costs; return; }
    if (costs < last_min) {
      sum_diff+= last_max - costs; last_max= last_min= costs;
      if (++imp >= D) {gamma= scaling * double (sum_diff) / double (imp); }
    } else if (costs > last_max) last_max= costs;
  }

// ============ Parallel computations only if USE_MPI is defined ====================

#ifdef USE_MPI

template <class Temp_t>
int pick_processor_sa (int my_costs, int it, Temp_t temp, const GMPI::Intracomm& comm) {
  int          me= comm.Get_rank(), size= comm.Get_size(),  picked;
  vector<int>  all_costs (size);
  comm.Gather (&my_costs, 1, MPI::INT, &all_costs[0], 1, MPI::INT, 0);
  if (me == 0) {
    int min_costs= *min_element (all_costs.begin(), all_costs.end());
    // compute acceptance probs and their partial sums
    vector<double>  acc_probs (size), psum_probs (size);
    for (size_t c= 0; c < size; c++)
      acc_probs[c]= SA_acceptance (min_costs - all_costs[c], it, temp);
    partial_sum (acc_probs.begin(), acc_probs.end(), psum_probs.begin());
    // pick one according to probabilities
    double sum_probs= psum_probs[size-1], ra= angel::random (sum_probs);
    for (picked= 0; picked < size; picked++)
      if (ra < psum_probs[picked]) break;
    THROW_DEBUG_EXCEPT_MACRO (picked == size, consistency_exception, "No processor picked");
#ifdef WRITE_LOG_FILE
    THROW_DEBUG_EXCEPT_MACRO (!log_file.is_open(), io_exception, "Log file not opened");    
    log_file << "pick_processor_sa at iteration " << it << endl;
    write_vector (log_file, "Processor's costs: ", all_costs);
    write_vector (log_file, "Accumulated probabilities: ", psum_probs);
    log_file << "Random value: " << ra << " --> picked processor " << picked << endl;
#endif
  }
  comm.Bcast (&picked, 1, MPI::INT, 0);
  return picked;
}

template <class Object_t, class Neighbor_t, class Cost_t, class Inner_temp_t, class Outer_temp_t,
          class Pre_comm_t, class Post_comm_t>
int parallel_SA (Object_t& object, Neighbor_t neighbor, Cost_t costs, 
		 Inner_temp_t inner_temp, Outer_temp_t outer_temp, int inner_iter, int max_iter,
		 const GMPI::Intracomm& comm, Pre_comm_t pre_comm, Post_comm_t post_comm) {

  int min_costs= costs (object), last_costs= min_costs; // initial costs
  Object_t min_object (object), last_object (object); // corresponding objects
  GMPI::comm_ref_t<int, Object_t> comm_ref (last_object); // reference used in communication

  for (int i= 0; i < max_iter; i++) {

    int ii; bool inner_completion;
    for (ii= 0, inner_completion= false; !inner_completion; ) {

      Object_t new_object (last_object);
      if (!neighbor (new_object)) {
        cout << "No neighbor found!"; return -1;}
      int new_costs= costs (new_object);
      // log_file << "LSA costs: " << new_costs;
#ifdef GNUPLOT
      log_file << i+ii << "\t" << new_costs << "   # gnuplot \n"; // different files per proc with MPI
#endif
      if (new_costs < last_costs) {
	if (new_costs < min_costs) {
	  min_costs= new_costs; min_object= new_object; }
	last_costs= new_costs; last_object= new_object; 
	// log_file << " accepted due to improvement";
      } else {
	double acc= SA_acceptance (last_costs-new_costs, i, inner_temp);
	double ra= angel::random (1.0);
	if (ra < acc) {
	  last_costs= new_costs; last_object= new_object; 
	  // log_file << " accepted due to Metropolis";
	} } 
      // log_file << '\n'; 
      if (++ii == inner_iter) { 
	inner_completion= true;
	mark_completion (comm.mpi_comm_ref());
      } else inner_completion= test_completion (comm.mpi_comm_ref());
    } // inner for loop

    i+= inner_iter; // all proc are handled as having finished inner loop here
    if (i < max_iter) {
      clean_completion_messages (comm.mpi_comm_ref());
      pre_comm (last_object);
      int sa_root= pick_processor_sa (last_costs, i, outer_temp, comm);
      comm.Bcast (comm_ref, sa_root); 
      post_comm (last_object); }
  } // outer for loop

  // now look for global minimum
  int me= comm.Get_rank(); 
  std::pair<int,int> my_min_costs_rank (min_costs, me), min_costs_rank;
  comm.Allreduce (&my_min_costs_rank, &min_costs_rank, 1, MPI::TWOINT, MPI::MINLOC); 
  int min_root= min_costs_rank.second;
  GMPI::comm_ref_t<int, Object_t> min_comm_ref (min_object); // reference to min
  comm.Bcast (min_comm_ref, min_root);

  object= min_object;
  return min_root;
}

#endif // USE_MPI

} // namespace angel
