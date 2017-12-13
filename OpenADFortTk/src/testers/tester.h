// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef tester_INCLUDED_h
#define tester_INCLUDED_h

#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"


namespace whirltester {
  
  int
  TestIR(std::ostream& os, PU_Info* pu_forest);
  
  int
  TestIR_OA(std::ostream& os, PU_Info* pu_forest);
  
  int
  TestIR_whirl2f(std::ostream& os, PU_Info* pu_forest);
  
}; /* namespace tester */


#endif // tester_INCLUDED_h
