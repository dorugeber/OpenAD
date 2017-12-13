// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef FortTk_Exception_H 
#define FortTk_Exception_H

#include <iostream>
#include <sstream>
#include <string>


namespace fortTkSupport {

  class BaseException {
  public:
    BaseException() { }
    virtual ~BaseException() { }
    
    virtual const std::string& GetMessage() const = 0;
    virtual void Report(std::ostream& os) const = 0;
    virtual void Report() const = 0;
  };

  // A generic FortTk exception with file/line information
  class Exception : public BaseException {
  public:
    Exception(const char* m,
	      const char* filenm = NULL, unsigned int lineno = 0);
    Exception(std::string m,
	      const char* filenm = NULL, unsigned int lineno = 0);
    virtual ~Exception();
    
    virtual const std::string& GetMessage() const { return msg; }
    virtual void Report(std::ostream& os) const { 
      os << "OpenADFortTk::Exception: " << msg << std::endl;
    }
    virtual void Report() const { Report(std::cerr); }

  private:
    void Ctor(std::string& m, 
	      const char* filenm = NULL, unsigned int lineno = 0);
    
    std::string msg;
  };

  // A fatal FortTk exception that generally should be unrecoverable
  class FatalException : public Exception {
  public:
    FatalException(const char* m,
		   const char* filenm = NULL, unsigned int lineno = 0);
    FatalException(std::string m,
		   const char* filenm = NULL, unsigned int lineno = 0);
    virtual ~FatalException();
    
    virtual void Report(std::ostream& os) const { 
      os << "OpenADFortTk::FatalException: " << GetMessage() << std::endl;
    }
    virtual void Report() const { Report(std::cerr); }

  };
  
} 

#endif // FortTk_Exception_H
