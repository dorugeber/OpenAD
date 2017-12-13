// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XlationContext.cxx,v 1.10 2006/05/12 16:12:24 utke Exp $


#include <iostream>
#include <sstream>
#include "Open64IRInterface/SymTab.h"
#include "Diagnostics.h"
#include "XlationContext.h"

namespace xaif2whirl{

  XlationContext::XlationContext() : 
    myFlags(0) {
  }

  XlationContext::~XlationContext() {
  }

  bool XlationContext::isFlag(XlationContext::Flags_E f) const { 
    return (myFlags & f); 
  }

  void XlationContext::setFlag(XlationContext::Flags_E f) { 
    if (VALUESELECTOR & f) { 
      if (DERIVSELECTOR & myFlags) {
	std::ostringstream ctxt_contents;
	dump(ctxt_contents,"");
	FORTTK_MSG(2,"XlationContext::setFlag: cannot set VALUESELECTOR while DERIVSELECTOR "
		    << ctxt_contents.str().c_str());
      }
    }
    if (DERIVSELECTOR & f) { 
      if (VALUESELECTOR & myFlags) { 
	std::ostringstream ctxt_contents;
	dump(ctxt_contents,"");
	FORTTK_MSG(2,"XlationContext::setFlag: cannot set DERIVSELECTOR while VALUESELECTOR " 
		    << ctxt_contents.str().c_str());
      }
    }
    myFlags = myFlags | f; 
  }

  void XlationContext::unsetFlag(XlationContext::Flags_E f) { 
    myFlags = myFlags & ~f; 
  }

  void XlationContext::inheritFlagsDown(const XlationContext& parentContext) { 
    myFlags = myFlags | (parentContext.myFlags & (VALUESELECTOR|DERIVSELECTOR|SUPPRESSSELECTOR|VARREF|LVALUE|ARRAY|ARRAYIDX|EXPRSIMPLE)); 
  } 
  void XlationContext::inheritFlagsUp(const XlationContext& childContext) { 
    myFlags = myFlags | (childContext.myFlags & ACTIVETYPE); 
  }

  void XlationContext::dump(std::ostream& o, const std::string& indent) const {
    o << indent.c_str() << "(";
    if (isFlag(ACTIVETYPE))          o << " active";
    if (isFlag(VALUESELECTOR))       o << " value_sel";
    if (isFlag(DERIVSELECTOR))       o << " deriv_sel";
    if (isFlag(SUPPRESSSELECTOR))    o << " suppr_sel";
    if (isFlag(VARREF))              o << " varref";
    if (isFlag(LVALUE))              o << " lvalue";
    if (isFlag(ARRAY))               o << " array";
    if (isFlag(ARRAYIDX))            o << " arrayidx";
    if (isFlag(EXPRSIMPLE))          o << " exprsimple";
    o << ")" << std::endl;
  }

  void 
  XlationContext::ddump() const {
    dump(std::cerr,"");
  }

}
