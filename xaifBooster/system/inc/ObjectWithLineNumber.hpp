#ifndef _OBJECTWITHLINENUMBER_INCLUDE_
#define _OBJECTWITHLINENUMBER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp"
#include <string>

namespace xaifBooster { 

  /**
   * a base class for classes with an (optional) Line number
   */
  class ObjectWithLineNumber : virtual public Debuggable {
  public:

    ObjectWithLineNumber();

    virtual ~ObjectWithLineNumber() {};
    
    /**
     * Debug routine
     */    
    virtual std::string debug() const;

    /**
     * check if it is set, 
     * this is generally not a required attribute
     */
    bool lineNumberIsSet() const;

    void setLineNumber(unsigned int aLineNumber); 

    unsigned int getLineNumber() const; 

    /**
     * name for member myLineNumber as represented in XAIF schema
     */
    static const std::string our_myLineNumber_XAIFName;

  private:

    /** 
     * optional line number in the input file
     */ 
    unsigned int myLineNumber;

  };  // end  of class ObjectWithLineNumber 

} // end of namespace xaifBooster
                                                                     
#endif

