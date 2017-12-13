// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/XercesStrX.h,v 1.4 2006-05-12 16:12:23 utke Exp $

#ifndef XercesStrX_INCLUDED_h
#define XercesStrX_INCLUDED_h

#include <iostream>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLString.hpp>

namespace xaif2whirl { 

  /** 
   * A class for transcoding of XMLCh data to local code page.
   * Based on samples/SAX2Print/Sax2Print.hpp within Xerces-C++
   */
  class XercesStrX {

  public :
    /**
     *  Note: toTranscode can be NULL.
     */
    XercesStrX(const XMLCh* const toTranscode) {
      local = xercesc::XMLString::transcode(toTranscode);
    }
  
    ~XercesStrX() { xercesc::XMLString::release(&local); }

    const char* c_str() const { return local; }

    // For debugging
    static void DumpXMLStr(std::ostream& os, const XMLCh* const xmlstr);
    static void DDumpXMLStr(const XMLCh* const xmlstr);

  private :
    char* local;
  };


  inline std::ostream& 
  operator<<(std::ostream& os, const XMLCh* const toDump)
  {
    XercesStrX::DumpXMLStr(os, toDump);
    return os;
  }

  inline std::ostream& 
  operator<<(std::ostream& os, const XercesStrX& toDump)
  {
    os << toDump.c_str();
    return os;
  }

}

#endif // XercesStrX_INCLUDED_h
