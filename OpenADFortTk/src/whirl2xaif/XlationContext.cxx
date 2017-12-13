// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2xaif/XlationContext.cxx,v 1.19 2007-10-08 18:28:34 utke Exp $


#include <stdio.h>
#include "Open64IRInterface/SymTab.h"
#include "Diagnostics.h"
#include "XlationContext.h"

namespace whirl2xaif{

  XlationContext::XlationContext(unsigned int aStackPosition) : 
    myWNp(0), 
    myNextVertexId(0), 
    myNextEdgeId(0),
    myFlags(0),
    myStackPosition(aStackPosition) {
  }

  XlationContext::~XlationContext() {
  }

  WN* XlationContext::getWN() const { 
    if (!myWNp)
      FORTTK_DIE("XlationContext::getWN: myWNp not set");
    return myWNp; 
  }

  bool XlationContext::hasWN() const { 
    return (myWNp)?true:false; 
  }

  void XlationContext::setWN(WN* aWNp) { 
    if (!aWNp)
      FORTTK_DIE("XlationContext::setWN: null pointer passed");
    if (myWNp)
      FORTTK_MSG(4,"XlationContext::setWN: already set to " << myWNp << " new " << aWNp);
    myWNp = aWNp; 
  }

  unsigned int XlationContext::getNewVertexId() { 
    return ++myNextVertexId; 
  }

  unsigned int XlationContext::getVertexId() const { 
    if (!myNextVertexId)
      FORTTK_DIE("XlationContext::getVertexId: invalid use");
    return myNextVertexId; 
  } 

  unsigned int XlationContext::peekVertexId() const { 
    return myNextVertexId + 1; 
  } 

  unsigned int XlationContext::getNewEdgeId() { 
    return ++myNextEdgeId; 
  }

  unsigned int XlationContext::getEdgeId() const { 
    if (!myNextEdgeId)
      FORTTK_DIE("XlationContext::getEdgeId: invalid use");
    return myNextEdgeId; 
  }

  unsigned int XlationContext::peekEdgeId() const { 
    return myNextEdgeId + 1; 
  }

  bool XlationContext::isFlag(XlationContext::Flags_E f) const { 
    return (myFlags & f); 
  }
  void XlationContext::setFlag(XlationContext::Flags_E f) { 
    FORTTK_MSG(2,"XlationContext::setFlag: " 
	       << flagToString(f).c_str() 
	       << " for " 
	       << toString().c_str());
    myFlags = myFlags | f; 
  }
  void XlationContext::unsetFlag(XlationContext::Flags_E f) { 
    FORTTK_MSG(2,"XlationContext::unsetFlag: " 
	       << flagToString(f).c_str() 
	       << " for " 
	       << toString().c_str());
    myFlags = myFlags & ~f; 
  }

  void XlationContext::inheritFlags(const XlationContext& parentContext) { 
    // only ASSIGN and VARREF are inherited
    myFlags = myFlags | (parentContext.myFlags & (ASSIGN|VARREF|HAS_NO_ARR_ELMT)); 
  }

  std::string XlationContext::flagToString(XlationContext::Flags_E aFlag) { 
    std::string returnString;
    switch (aFlag) { 
    case NOFLAG:
      returnString="";
      break;
    case ASSIGN:
      returnString="assign";
      break;
    case VARREF:
      returnString="varref";
      break;
    case DEREF_ADDR:
      returnString="deref_addr";
      break;
    case HAS_LOGICAL_ARG:
      returnString="has_logical_arg";
      break;
    case IS_LOGICAL_ARG:
      returnString="is_logical_arg";
      break;
    case IS_LOGICAL_OPERATION:
      returnString="is_logical_operation";
      break;
    case IO_STMT:
      returnString="io_stmt";
      break;
    case DEREF_IO_ITEM:
      returnString="deref_io_item";
      break;
    case ORIGFMT_IOCTRL:
      returnString="origfmt_ioctrl";
      break;
    case FMT_IO:
      returnString="fmt_io";
      break;
    case CRAY_IO:
      returnString="cray_io";
      break;
    case HAS_NO_ARR_ELMT:
      returnString="has_no_arr_elmt";
      break;
    default: 
      FORTTK_DIE("XlationContext::flagToString: unexpected input " << aFlag );
      break; 
    }
    return returnString;
  }

  std::string XlationContext::toString()const { 
    ostringstream o; 
    o << "(" << myStackPosition << ",wn=" << myWNp;
    if (isFlag(ASSIGN))              o << flagToString(ASSIGN).c_str();
    if (isFlag(VARREF))              o << flagToString(VARREF).c_str();
    if (isFlag(DEREF_ADDR))          o << flagToString(DEREF_ADDR).c_str();
    if (isFlag(HAS_LOGICAL_ARG))     o << flagToString(HAS_LOGICAL_ARG).c_str();
    if (isFlag(IS_LOGICAL_ARG))      o << flagToString(IS_LOGICAL_ARG).c_str();
    if (isFlag(IS_LOGICAL_OPERATION))o << flagToString(IS_LOGICAL_OPERATION).c_str();
    if (isFlag(IO_STMT))             o << flagToString(IO_STMT).c_str();
    if (isFlag(DEREF_IO_ITEM))       o << flagToString(DEREF_IO_ITEM).c_str();
    if (isFlag(ORIGFMT_IOCTRL))      o << flagToString(ORIGFMT_IOCTRL).c_str();
    if (isFlag(FMT_IO))              o << flagToString(FMT_IO).c_str();
    if (isFlag(CRAY_IO))             o << flagToString(CRAY_IO).c_str();
    if (isFlag(HAS_NO_ARR_ELMT))     o << flagToString(HAS_NO_ARR_ELMT).c_str();
    o << ")" << std::ends;
    return o.str();
  } 

  void XlationContext::dump(std::ostream& o, const std::string& indent) const {
    o << indent.c_str() << toString().c_str() << std::endl;
  }

  void 
  XlationContext::ddump() const {
    dump(std::cerr,"");
  }

}
