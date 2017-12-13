#ifndef _FRONTENDTYPE_INCLUDE_
#define _FRONTENDTYPE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

namespace xaifBooster { 

  /**
   * an opaque attribute to pass along 
   * front end specific type information 
   * For  now this is a string which can 
   * be empty
   */
  class FrontEndType : public std::string { 
  public: 
    FrontEndType(); 
    FrontEndType(const std::string& s); 
    static const std::string our_attribute_XAIFName;
  };

} 
                                                                     
#endif
