// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/XercesStrX.cxx,v 1.2 2006-05-12 16:12:23 utke Exp $

#include <iostream>

#include "XercesStrX.h"

namespace xaif2whirl {

  void 
  XercesStrX::DumpXMLStr(std::ostream& os, const XMLCh* const xmlstr) {
    XercesStrX toDump(xmlstr);
    os << toDump.c_str();
    os.flush();
  }
  
  void
  XercesStrX::DDumpXMLStr(const XMLCh* const xmlstr) {
    DumpXMLStr(std::cout, xmlstr);
  }
  
}


