#ifndef _XMLPARSER_INCLUDE_
#define _XMLPARSER_INCLUDE_
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
#include <stack>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/utils/inc/XMLParserHelper.hpp"
#include "xaifBooster/utils/inc/HashTable.hpp"

namespace xaifBooster {

  class XMLParser {
  public:
    XMLParser(){};
    virtual ~XMLParser(){};

    void initialize(bool validateAgainstSchema);
    void parse(std::string theXMLFileName);
    virtual void setExternalSchemaLocation(const std::string& theSchemaLocation);
    
    /**
     * converts an XML boolean string
     */
    static bool convertToBoolean(const std::string& aValueString);

    /**
      * convenience method which wraps 
      * xercesc calls to get to the attributes 
      * of a node
      */
     static
     std::string getAttributeValueByName(const std::string& theName);

  protected:

    /** 
     * stack for XMLParserHelpers
     */

    std::stack<XMLParserHelper*> myXMLParserHelperStack; 

    /** 
     * local xercesc parser handle
     */
    XERCES_CPP_NAMESPACE::SAX2XMLReader* myParser_p;

    /**
     * UN: Pointer to attributes of current element
     */

     static const XERCES_CPP_NAMESPACE::Attributes* ourAttributes_p;

  }; // end of class XMLParser

} // end of namespace 

#endif
