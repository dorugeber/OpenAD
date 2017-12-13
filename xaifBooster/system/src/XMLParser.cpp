// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include "xaifBooster/system/inc/XMLParser.hpp"
#include "xaifBooster/utils/inc/XMLParserMessage.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include <sstream>

namespace xaifBooster {

  const XERCES_CPP_NAMESPACE::Attributes* XMLParser::ourAttributes_p=0;
  
  /*
   * UN: performs basis initializations related to
   * configuration of the XMLReader
   */

  void XMLParser::initialize(bool validateAgainstSchema) { 
    DBG_MACRO(DbgGroup::CALLSTACK, "in XMLParser::initialize" );
    try { 

      // per process parser init
      XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize();
      
      myParser_p = XERCES_CPP_NAMESPACE::XMLReaderFactory::createXMLReader();
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgSAX2CoreValidation,validateAgainstSchema);
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgXercesDynamic,false);
      // we need the following for schema processing:
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgSAX2CoreNameSpaces,true);
      // we need the following to support default values coming from the schema:
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgXercesSchema,true);
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgXercesSchemaFullChecking,validateAgainstSchema);
      // we need the following for schema processing:
      myParser_p->setFeature(XERCES_CPP_NAMESPACE::XMLUni::fgSAX2CoreNameSpacePrefixes,true); 

    } catch (const XERCES_CPP_NAMESPACE::XMLException& e) {
      THROW_LOGICEXCEPTION_MACRO("XMLParser::initialize: XMLException: "
				 << XMLParserMessage(e.getMessage()));
    }
  } // end of XMLParser::initialize

   void XMLParser::parse(std::string theXMLFileName) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XMLParser::parse" );
    try {
      myParser_p->parse(theXMLFileName.c_str());
    }
    catch (const xercesc::XMLException& e) {
      THROW_LOGICEXCEPTION_MACRO("XMLParser::parse: XMLException:"
                                 << XMLParserMessage(e.getMessage()));
    }
    catch (BaseException& e) {
      throw e;
    }
    catch (...) {
      THROW_LOGICEXCEPTION_MACRO("XMLParser::parse: caught an unspecified exception");
    }
  } // end of parse

  void XMLParser::setExternalSchemaLocation(const std::string& theSchemaLocation) { 
    DBG_MACRO(DbgGroup::CALLSTACK, 
	      "in XMLParser::setExternalSchemaLocation for " 
	      << theSchemaLocation.c_str());
    try {
      myParser_p->setProperty(XERCES_CPP_NAMESPACE::XMLUni::fgXercesSchemaExternalSchemaLocation,
			      (void*)XERCES_CPP_NAMESPACE::XMLString::transcode(theSchemaLocation.c_str()));
    }
    catch (const xercesc::XMLException& e) {
      THROW_LOGICEXCEPTION_MACRO("XMLParser::setExternalSchemaLocation: XMLException:"
                                 << XMLParserMessage(e.getMessage()));
    }
    catch (BaseException& e) {
      throw e;
    }
    catch (...) {
      THROW_LOGICEXCEPTION_MACRO("XMLParser::setExternalSchemaLocation: caught an unspecified exception");
    }
  } // end of setExternalSchemaLocation


  bool
  XMLParser::convertToBoolean(const std::string& aValueString) { 
    bool returnWhat;
    if (aValueString=="true" || aValueString=="1")
      returnWhat=true;
    else if (aValueString=="false" || aValueString=="0")
      returnWhat=false;
    else 
      THROW_LOGICEXCEPTION_MACRO("XMLParser::convertToBoolean: invalid input >"
				 << aValueString.c_str()
				 << "<");
    return returnWhat;
  } // end of XMLParser::convertToBoolean

  std::string XMLParser::getAttributeValueByName(const std::string& theName) {
    unsigned int len = ourAttributes_p->getLength();
    for (unsigned int index = 0; index < len; index++) {
      if (XMLParserMessage(ourAttributes_p->getQName(index)).toString()==theName)
	return XMLParserMessage(ourAttributes_p->getValue(index)).toString();
    }
    THROW_LOGICEXCEPTION_MACRO("XMLParser::getAttributeValueByName didn't get attribute for >" << theName.c_str() << "<");
  } // end of XMLParser::getAttributeValueByName

} // end of namespace 
