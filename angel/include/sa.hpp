// $Id: sa.hpp,v 1.13 2005/04/12 04:22:57 jean_utke Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifndef 	_sa_include_
#define 	_sa_include_


//
//  functions and operators for simulated annealing
//

#include <cmath> 
#include <numeric>
#include <vector>

#include "angel_exceptions.hpp"
#include "angel_types.hpp"
#include "angel_tools.hpp"
#include "angel_io.hpp"
#include "eliminations.hpp"
#include "heuristics.hpp"

#ifdef USE_MPI
#include "angel_comm.hpp"
#endif // USE_MPI

namespace angel {

// =====================================================
// Generic SA algorithms
// =====================================================

/// Functor that returns logarithmic temperature for LSA
class LOG_temperature_t {
  double   gamma;
public:
  /// Set Gamma in constructor
  LOG_temperature_t (double p_gamma) : gamma (p_gamma) {}
  /// LOG temperature in iteration \p it
  double operator() (int it) const {
    return gamma / log (double (it+2)); }
};

/// Functor that returns fixed temperature
class fixed_temperature_t {
  double   t;
public:
  /// Set t in constructor
  fixed_temperature_t (double p_t) : t (p_t) {}
  /// Fixed temperature in iteration \p it
  double operator() (int it) const {
    return t; }
};

/** \brief Probability to accept new object in SA
    \param diff The difference between old and new costs, must be < 0.
    \param it Current number of iterations
    \param temp Functor that computes temperature for iteration number \p it
 */
template <class Temp_t>
inline double SA_acceptance (int diff, int it, Temp_t temp) {
  return exp (diff / temp (it)); }

/** \brief Simulated Annealing in a general form
    \param object Some state in the configuration space.
    \param neighbor Neighborhood relation applicable to object
    \param costs Cost operator applicable to object
    \param temp Temperature operator depending on iteration number, e.g. LOG_temperature_t
    \param max_iter Maximal number of iterations

    Object_t, Neighbor_t and Cost_t can be arbitrary
    as long as their objects can allow to execute neighbor (object)
    with change of object and to execute cost (object) where object
    can be const and an int-compatible value is returned.
    The temperature operator is expected to take an int parameter and
    to return a double result.
    \note At the moment there are only applications with 
    instantiations of elimination_history_t
    as Object_t.
 */
template <class Object_t, class Neighbor_t, class Cost_t, class Temp_t>
int SA (Object_t& object, Neighbor_t neighbor, Cost_t costs, Temp_t temp, int max_iter);

/** \brief Logarithmic Simulated Annealing in a general form
    \param object Some state in the configuration space.
    \param neighbor Neighborhood relation applicable to object
    \param costs Cost operator applicable to object
    \param gamma \f$\Gamma\f$ in LSA
    \param max_iter Maximal number of iterations

    Object_t, Neighbor_t and Cost_t can be arbitrary
    as long as their objects can allow to execute neighbor (object)
    with change of object and to execute cost (object) where object
    can be const and an int-compatible value is returned.
    \note At the moment there are only applications with face_elimination_history_t
    as Object_t.
 */
template <class Object_t, class Neighbor_t, class Cost_t>
inline int LSA (Object_t& object, Neighbor_t neighbor, Cost_t costs, double gamma, int max_iter) {
  LOG_temperature_t temp (gamma);
  return SA (object, neighbor, costs, temp, max_iter); }

/** \brief Metropolis with fixed temperature in a general form
    \param object Some state in the configuration space.
    \param neighbor Neighborhood relation applicable to object
    \param costs Cost operator applicable to object
    \param t temperature
    \param max_iter Maximal number of iterations

    Object_t, Neighbor_t and Cost_t can be arbitrary
    as long as their objects can allow to execute neighbor (object)
    with change of object and to execute cost (object) where object
    can be const and an int-compatible value is returned.
    \note At the moment there are only applications with face_elimination_history_t
    as Object_t.
 */
template <class Object_t, class Neighbor_t, class Cost_t>
inline int FTSA (Object_t& object, Neighbor_t neighbor, Cost_t costs, double t, int max_iter) {
  fixed_temperature_t temp (t);
  return SA (object, neighbor, costs, temp, max_iter); }

/** \brief Adaptive Logarithmic Simulated Annealing in generic form
    \param object Some state in the configuration space.
    \param neighbor Neighborhood relation applicable to object
    \param costs Cost operator applicable to object
    \param adaption Operator that can change \f$\Gamma\f$
    \param gamma Initial \f$\Gamma\f$ in LSA
    \param max_iter Maximal number of iterations
    \sa LSA

    ALSA is LSA with the difference that \f$\Gamma\f$ can be changed adaptively. 
    The adaption operator records the
    costs and may change \f$\Gamma\f$ on this base.
    Another difference to LSA is that \f$\Gamma\f$ is passed by reference (instead of by value)
    in order to give feedback about the adaption.
    \note At the moment there are only applications with face_elimination_history_t
    as Object_t.
 */
template <class Object_t, class Neighbor_t, class Cost_t, class Adaption_t>
int ALSA (Object_t& object, Neighbor_t neighbor, Cost_t costs, Adaption_t adaption,
	  double& gamma, int max_iter);

// =====================================================
// cost operators
// =====================================================



/** \brief Computes the elimination costs for arbitrary elimination history type

    Costs for an object eh are 
    -# the costs for reaching \c eh.cg from \c eh.og (stored in \c eh.ccosts) and
    -# the costs for transforming \c eh.cg into final graph (bi-/tri-partite) using heuristic \c h
*/
template <class Heuristic_t>
class SA_elimination_cost_t {
  Heuristic_t  h;
public:
  /// Constructor defining the heuristic \c h
  SA_elimination_cost_t (Heuristic_t p_h) : h (p_h) {}

  /// Operator() computes the costs of \c eh
  template <class Ad_graph_t, class El_spec_t>
  int operator() (const elimination_history_t<Ad_graph_t, El_spec_t>& eh) {
    std::vector<El_spec_t>  el_seq;
    int rcosts= use_heuristic (eh.cg, el_seq, h);
    return eh.ccosts + rcosts; }  // costs: (og -> cg) + (cg -> J)
};


// =====================================================
// neighbourhoods
// =====================================================

/// Swap two vertices in sequence (historical, only vertex elimination)
void neighbor_swap (const std::vector<int>& old_seq, std::vector<int>& seq);	

/** \brief SA neighborhood either eliminate sth from eh.cg or undo last elimination
    \param eh (for operator()) 
    \return If it was successful
*/
struct neighbor_last_removable_t {
  template <class Ad_graph_t, class El_spec_t>
  bool operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh);
};

/** \brief SA neighborhood for multiple eliminations or re-insertions

    It either eliminate between 1 and \c max_steps faces from \c eh.cg 
    or undo last one to \c max_steps eliminations.
*/
class neighbor_multi_step_t {
  int max_steps;
public:
  /// Constructor defining the maximal number of steps
  neighbor_multi_step_t (int m) : max_steps (m) {}
  template <class Ad_graph_t, class El_spec_t>
  bool operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh);
};

/** \brief SA neighborhood either eliminate face from \c eh.cg or undo some previous elimination
    \param eh 
    \return If it was successful

    Some element is removed from the elimination sequence and then it is checked if the 
    remaining sequence can be applied to the original graph \c eh.og. 
    In case of failure the elimination sequence is restored and another element is tried
    until some shortened sequence is applicable.
*/
struct neighbor_sequence_check_t {
  template <class Ad_graph_t, class El_spec_t>
  bool operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh);
};



// -------------------------------------------------------------------------

//
// SA neighborhood either eliminate face from feh.lg 
//   or undo some previous elimination f_k from feh.seq = (f_1, .., f_n)
//   such that feh.olg --(f_1, ..., f_(k-1), f_(k+1), ..., f_n, f_k)--> feh.lg
// new graph feh.lg', i.e. feh.olg --(f_1, ..., f_(k-1), f_(k+1), ..., f_n)-->feh.lg'
//   is a predecessor of feh.lg in the meta-graph, see tec report for details
// returns if it was successful
//

/** \brief SA neighborhood either eliminate face from eh.cg or undo some previous elimination
    \param eh 
    \return If it was successful

    Some element is removed from the elimination sequence and then it is check if the 
    remaining sequence can be applied to the original graph \c eh.og. 
    In addition it is checked if the resulting graph is a predecessor of \c eh.cg in
    the meta-graph.
    In case of failure the elimination sequence is restored and another element is tried
    until some shortened sequence is applicable.
*/
struct neighbor_check_meta_t {
  template <class Ad_graph_t, class El_spec_t>
  bool operator() (elimination_history_t<Ad_graph_t, El_spec_t>& eh);
};

// =====================================================
// Gamma adaption operators
// =====================================================

/** \brief \f$\Gamma\f$ adaption on maximal min-max-difference

    Between two improvements of the objective functions the maximal (worst) value
    is subtracted from the minimal (best) value. This difference is computed for D 
    improvements. Then the maximal difference is used as \f$\Gamma\f$.
    \warning In contrast to other operator classes (e.g. neighbor_sequence_check_t), 
             objects of this class can only be used in one run! For another run
	     a new object must be created to set private variables to their initial state.
    \sa ALSA
*/
class gamma_adaption_max_t {
  int D, diff, max_diff, last_min, last_max, imp;
  double scaling;
public:
  /** \brief  Constructor 
      \param p_D The number of improvement before \f$\Gamma\f$ is changed
      \param p_scaling Scaling factor for \f$\Gamma\f$, if omitted 1.0 is taken (no scaling)
  */
  gamma_adaption_max_t (int p_D, double p_scaling= 1.0) : 
    D (p_D), diff (0), max_diff (0), last_min (0), imp (0), scaling (p_scaling) {
    THROW_DEBUG_EXCEPT_MACRO (D <= 0 && scaling <= 0.0, consistency_exception, 
			   "D and scaling must be greater 0"); }

  /** \brief Operator, which finally changes \f$\Gamma\f$
      \param costs The costs (objective function) of current iteration,
                   must be larger than 0
      \param gamma \f$\Gamma\f$ in current iteration, may be changed
  */
  void operator() (int costs, double& gamma);
};

/** \brief \f$\Gamma\f$ adaption on average min-max-difference

    Same as gamma_adaption_max_t with average instead of maximal difference.
    \warning In contrast to other operator classes (e.g. neighbor_sequence_check_t), 
             objects of this class can only be used in one run! For another run
	     a new object must be created to set private variables to their initial state.
    \sa gamma_adaption_max_t
    \sa ALSA
*/
class gamma_adaption_average_t {
  int D, sum_diff, last_min, last_max, imp;
  double scaling;
public:
  /** \brief  Constructor 
      \param p_D The number of improvement before \f$\Gamma\f$ is changed
      \param p_scaling Scaling factor for \f$\Gamma\f$, if omitted 1.0 is taken (no scaling)
  */
  gamma_adaption_average_t (int p_D, double p_scaling= 1.0) : 
    D (p_D), sum_diff (0), last_min (0), imp (0), scaling (p_scaling) {
    THROW_DEBUG_EXCEPT_MACRO (D <= 0 && scaling <= 0.0, consistency_exception, 
			   "D and scaling must be greater 0"); }

  /** \brief Operator, which finally changes \f$\Gamma\f$
      \param costs The costs (objective function) of current iteration,
                   must be larger than 0
      \param gamma \f$\Gamma\f$ in current iteration, may be changed
  */
  void operator() (int costs, double& gamma);
};



#ifdef USE_MPI

/// Pick randomly some processor w.r.t. the probalities computed by the costs with temp
template <class Temp_t>
int pick_processor_sa (int my_costs, int it, Temp_t temp, const MPI::Intracomm& comm);

/** \brief Parallel simulated annealing in a general form
    \param object Some state in the configuration space.
    \param neighbor Neighborhood relation applicable to object
    \param costs Cost operator applicable to object
    \param inner_temp Local temperature operator depending on iteration number
    \param outer_temp Global temperature operator used to select object between processors
    \param inner_iter Maximal number of iterations between two communications
    \param max_iter Maximal number of iterations
    \param pre_comm Operator which is applied before communication (except for global minimum)
    \param post_comm Operator which is applied after communication (except for global minimum)
      Returns the processor that stores the object with the minimal costs.
    Object_t, Neighbor_t and Cost_t can be arbitrary
    as long as their objects can allow to execute neighbor (object)
    with change of object and to execute cost (object) where object
    can be const and an int-compatible value is returned.
    The temperature operators are expected to take an int parameter and
    to return a double result.
    const \note At the moment there are only applications with 
    instantiations of elimination_history_t
    as Object_t.
 */
template <class Object_t, class Neighbor_t, class Cost_t, class Inner_temp_t, class Outer_temp_t,
          class Pre_comm_t, class Post_comm_t>
int parallel_SA (Object_t& object, Neighbor_t neighbor, Cost_t costs, 
		 Inner_temp_t inner_temp, Outer_temp_t outer_temp, int inner_iter, int max_iter,
		 const GMPI::Intracomm& comm, Pre_comm_t pre_comm, Post_comm_t post_comm);

/// Parallel SA without pre and post comm operators
template <class Object_t, class Neighbor_t, class Cost_t, class Inner_temp_t, class Outer_temp_t>
inline int parallel_SA (Object_t& object, Neighbor_t neighbor, Cost_t costs, 
			Inner_temp_t inner_temp, Outer_temp_t outer_temp, int inner_iter, int max_iter,
			const GMPI::Intracomm& comm) {
  empty_operator_t<Object_t> empty_pre_post_comm;
  return parallel_SA (object, neighbor, costs, inner_temp, outer_temp, inner_iter, max_iter, comm,
		      empty_pre_post_comm, empty_pre_post_comm);
}

#endif // USE_MPI

} // namespace angel

#include "sa_impl.hpp"

#endif  // _sa_include_

