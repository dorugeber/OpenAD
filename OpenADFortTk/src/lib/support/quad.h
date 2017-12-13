// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef FortTk_quad_INCLUDED
#define FortTk_quad_INCLUDED

#include <../../common/com/targ_const.h> // QUAD_TYPE
namespace FortTk {

  // Used to translate the bits within TCONs. We can safely assume 64
  // bit values are available on modern platforms.  This is used
  // simply to convey bits, so we are not concerned with little/big
  // endian issues.
  struct uint128_t {
    
    uint128_t() : hi(0), lo(0) { }
    ~uint128_t() { }
    
    uint128_t(const uint128_t& v) : hi(v.hi), lo(v.lo) { }
    uint128_t(const QUAD_TYPE& v) { *this = v; }
    
    uint64_t hi;
    uint64_t lo;
  };
  
  inline uint128_t& 
  assign(uint128_t& qd1, QUAD_TYPE& qd2) {
    qd1.hi = *( ((uint64_t*)&qd2) + 1 ); // high 64 bits
    qd1.lo = *( (uint64_t*)&qd2 );       // low 64 bits
    return qd1;
  }
  
  inline QUAD_TYPE& 
  assign(QUAD_TYPE& qd1, uint128_t& qd2) {
    *( ((uint64_t*)&qd1) + 1 ) = qd2.hi; // high 64 bits
    *( (uint64_t*)&qd1 )       = qd2.lo; // low 64 bits
    return qd1;
  }

}; 

#endif
