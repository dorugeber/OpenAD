// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef FileUtil_h
#define FileUtil_h 

#include <string>

namespace FileUtil {

  // FileName: Given a path, returns the filename portion.  If path is
  // NULL or ends in '/', returns the empty string. 
  // Examples:
  //   fname.ext       --> fname.ext
  //   /path/fname.ext --> fname.ext
  //   /usr/lib        --> lib
  //   /usr/           --> 
  //   /               --> 
  extern std::string
  FileName(const char* path); 
  
  // BaseName: Given a path, returns the 'basename' portion, the final
  // component of the pathname, first deleting any trailing '/'.  If path is
  // NULL or empty, returns ".".  (Note: libgen.h is not available on
  // Cygwin!)
  // Examples:
  //   fname.ext       --> fname.ext
  //   /path/fname.ext --> fname.ext
  //   /usr/lib        --> lib
  //   /usr/           --> usr
  //   /               --> /
  extern std::string
  BaseName(const char* path); 
  
  // DirName: Given a path, returns the 'dirname' portion, the pathanem
  // of the parent directory, first deleting any trailing '/'.  If path
  // is NULL or empty, returns "."; if path does not conain a '/'
  // returns ".".  ".".  (Note: libgen.h is not available on Cygwin!)
  // Examples:
  //   /path/fname.ext --> /path
  //   /usr/lib        --> /usr
  //   /usr/           --> /
  //   usr             --> .
  //   /               --> /
  //   .               --> .
  //   ..              --> .
  extern std::string
  DirName(const char* path); 
  
  
}; 

#endif 
