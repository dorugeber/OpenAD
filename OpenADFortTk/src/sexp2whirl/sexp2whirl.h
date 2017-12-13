// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef sexp2whirl_h
#define sexp2whirl_h

#include <sexp.h>

#include "Open64IRInterface/Open64BasicTypes.h"

namespace sexp2whirl {
  
  namespace XlateFlags {
    enum Flags {
      NONE       = 0x00000000, // 
      FOO1       = 0x00000001, // 
      FOO2       = 0x00000002, // 
      FOO3       = 0x00000004, // 
      FOO4       = 0x00000008, // 
    };
  };
  
  
  // Translate the whole WHIRL_IR, return a PU_FOREST and set GBL_SYMTAB state.
  extern PU_Info*
  TranslateIR(sexp_t* ir, int flags = 0);

  
  // Dump to std::cout
  extern void DumpIR(sexp_t* ir, int flags);

  // Convert to a diagnostic message string 
  extern const char* ErrIR(sexp_t* ir, int flags = 0);
  
}; 
#endif 
