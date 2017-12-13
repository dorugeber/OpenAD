#ifndef _ASSIGNMENTALGBASE_INCLUDE_
#define _ASSIGNMENTALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/BasicBlockElementAlgBase.hpp"

namespace xaifBooster {  
  
  class Assignment;

  class AssignmentAlgBase: public BasicBlockElementAlgBase { 

  public: 

    AssignmentAlgBase(const Assignment& theContaining);

    virtual ~AssignmentAlgBase();

    virtual std::string debug() const;

    const Assignment& getContainingAssignment() const;

  private: 

    /** 
     * not defined
     */
    AssignmentAlgBase();

    /** 
     * not defined
     */
    AssignmentAlgBase(const AssignmentAlgBase&);

    /** 
     * not defined
     */
    AssignmentAlgBase& operator=(const AssignmentAlgBase&);

  }; 

} // end of namespace 

#endif
