#ifndef _XMLPRINTABLE_INCLUDE_
#define _XMLPRINTABLE_INCLUDE_
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

namespace xaifBooster { 

  /**
   * an abstract base class for printing
   * of the XML Hierarchy
   * everything that is XMLprintable should also 
   * be debuggable...
   */
  class XMLPrintable : public virtual Debuggable {
  public:

    XMLPrintable() {};

    virtual ~XMLPrintable() {};
    
    /** 
     * XML printing of the object and its 
     * direct children according to the XAIF hierarchy
     * this is supposed to be a recursive invocation 
     * allowing to print an subhierarchy by invocation on the 
     * top level node of the subhierarchy
     */
    virtual void printXMLHierarchy(std::ostream& os) const =0 ; 

    /**
     * when in a debugging session dump the whole 
     * subhierarchy to std::cout 
     */
    void dumpAsXML() const; 

  };  // end  of class XMLPrintable 

} // end of namespace xaifBooster
                                                                     
#endif

