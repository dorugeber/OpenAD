#ifndef _GENERICTRAVERSEINVOKE_INCLUDE_
#define _GENERICTRAVERSEINVOKE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/GenericAction.hpp"

namespace xaifBooster { 

  /** 
   * class for generic traversal and invocation 
   * through the internal representation
   * following the 'visitor' design pattern
   */
  class GenericTraverseInvoke {

  public: 

    virtual ~GenericTraverseInvoke(){};

    /**
     * to be called in traverseToChildren 
     */
    void genericTraversal(const GenericAction::GenericAction_E anAction_c); 

    /** 
     * to be implemented in every leaf class of the hierarchy that 
     * is instantiated as part of the internal representation. 
     * the traversal is intended to go into 
     * 1. the associated algorithm class if available
     * 2. any member that itself is part of the internal representation 
     * 
     * The traversal is NOT to descend into any member that is part 
     * of system algorithms. 
     * For example in Assignment we traverse to the left and the right hand  
     * sides but not into myReplacementAssignmentList as the latter is 
     * only part of the linearization/ssa code
     */ 
    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c)=0; 

    /** 
     * some generic initialization for algorithm instances
     */
    virtual void algorithm_init(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_1(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_2(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_3(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_4(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_5(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_6(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_7(){};

    /** 
     * a generic algorithm action
     */
    virtual void algorithm_action_8(){};

  private: 

    /** 
     * perform the invocation
     */
    void genericInvocation(const GenericAction::GenericAction_E anAction_c); 

  }; // end of class GenericTraverseInvoke

}
#endif
