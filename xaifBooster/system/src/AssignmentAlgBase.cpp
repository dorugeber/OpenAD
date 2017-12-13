// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <sstream>

#include "xaifBooster/system/inc/AssignmentAlgBase.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

namespace xaifBooster {  
  
  AssignmentAlgBase::AssignmentAlgBase(const Assignment& theContaining) : 
    BasicBlockElementAlgBase(theContaining) { 
  }

  AssignmentAlgBase::~AssignmentAlgBase(){}

  std::string
  AssignmentAlgBase::debug() const {
    std::ostringstream out;
    out << "AssignmentAlgBase[" << this
        << ",myContainingAssignment=" << getContainingAssignment().debug().c_str()
        << "]" << std::ends;
    return out.str();
  }

  const Assignment& AssignmentAlgBase::getContainingAssignment() const { 
    return dynamic_cast<const Assignment&>(getContaining());
  }
} 
