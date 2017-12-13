// $Id: graph_generator.hpp,v 1.4 2003/03/25 17:42:00 gottschling Exp $
/*
#############################################################
# This file is part of angel released under the BSD license #
# The full COPYRIGHT notice can be found in the top         #
# level directory of the angel distribution                 #
#############################################################
*/


#ifndef 	_graph_generator_include_
#define 	_graph_generator_include_


//
// random graph generator for DAGs similar to real program's c-graph
//

#include <vector>
#include <time.h>

#include "angel_types.hpp"

namespace angel {

/** \brief Generates a random statement
    \param inputs The number of inputs 
    \param expr The number of expressions
    \param p Probability vector (see description)
    \param statement The resulting statement

    The number of an expression's input is characterized by p
    - p[0] is the probability that an expression is unary
    - p[1] the probability that it is binary at most
    - p.size() is the maximal arity 
    - p[p.size()-1] shall be 1.0
*/
void random_statement (int inputs, int expr, const std::vector<double>& p,
		       c_graph_t& statement);

/** \brief Generates a vector of random statements
    \param max_expr The maximal number of expressions in each statement
    \param unary The portion of unary expressions, the remainder are binary.
    \param statement_vector The resulting statement list

    The number of statement is determined by the vector size.
    In contrast to random_statement() it is restricted to
    unary and binary expressions. 
*/
void random_statement_vector (int max_expr, double unary, 
			      std::vector<c_graph_t>& statement_vector);

/** \brief Build a block from a list of statements
    \param inputs The number of the block's inputs
    \param outputs The number of the block's outputs
    \param stats List of statements
    \param block The resulting block
*/
void stats2block (int inputs, int outputs, const std::vector<c_graph_t>& stats, 
		  c_graph_t& block);

/** \brief Generates a random basic block
    \param inputs The number of the block's inputs
    \param outputs The number of the block's outputs
    \param stats The number of statements
    \param max_exp The maximal number of expressions in each statement
    \param unary The portion of unary expressions, the remainder are binary.
    \param block The resulting block
*/
void random_block (int inputs, int outputs, int stats, int max_exp, double unary,
		   c_graph_t& block);

/** \brief Generates a DAG that represents a loop over the block
    \param block The used block
    \param loops The number of loops
    \param loop The resulting loop DAG
*/
void block2loop (const c_graph_t& block, int loops,
		 c_graph_t& loop);


// Initializes the random generator unless SAME_RANDOM_VALUES is defined
struct random_init_t {
  int dummy;
  random_init_t () : dummy (7) { 
#ifndef SAME_RANDOM_VALUES
    srand (time (0));
#endif
  }
};
  
extern random_init_t random_init_object;

} // namespace angel

#endif  // _graph_generator_include_

