// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "xaifBooster/utils/inc/StringConversions.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  double StringConversions::convertToDouble(const std::string& aValue) { 
    double value;
    char *remainder=NULL;
    errno=0;
    value=strtod(aValue.c_str(),&remainder);
    if (errno) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToDouble: "  
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to an double. "
				 << "The error returned by strtod() is: " 
				 << strerror(errno)); 
    } 
    if (strlen(remainder)) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToDouble: "
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to a double. "
				 << "The remainder is :"
				 << remainder);
    } // end if
    return value;
  } 

  float StringConversions::convertToFloat(const std::string& aValue) { 
    float value;
    char *remainder=NULL;
    errno=0;
    value=strtof(aValue.c_str(),&remainder);
    if (errno) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToFloat: "  
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to a float. "
				 << "The error returned by strtof() is: " 
				 << strerror(errno)); 
    } 
    if (strlen(remainder)) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToFloat: "
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to a float. "
				 << "The remainder is :"
				 << remainder);
    } // end if
    return value;
  } 

  int StringConversions::convertToInt(const std::string& aValue) { 
    int value;
    char *remainder=NULL;
    errno=0;
    value=strtol(aValue.c_str(),&remainder,10);
    if (errno) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToInt: "  
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to an int. "
				 << "The error returned by strtol() is: " 
				 << strerror(errno)); 
    } 
    if (strlen(remainder)) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToInt: "
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to an int. "
				 << "The remainder is :"
				 << remainder);
    } // end if
    return value;
  } 

  unsigned int StringConversions::convertToUInt(const std::string& aValue) { 
    int value;
    char *remainder=NULL;
    errno=0;
    value=strtoul(aValue.c_str(),&remainder,10);
    if (errno) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToUInt: "  
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to an unsigned int. "
				 << "The error returned by strtoul() is: " 
				 << strerror(errno)); 
    } 
    if (strlen(remainder)) {
      THROW_LOGICEXCEPTION_MACRO("StringConversions::convertToUInt: "
				 << "the string "
				 << "\"" << aValue.c_str() << "\" " 
				 << "cannot be converted to an unsigned int. "
				 << "The remainder is :"
				 << remainder);
    } // end if
    return value;
  } 

} // end of namespace xaifBooster
