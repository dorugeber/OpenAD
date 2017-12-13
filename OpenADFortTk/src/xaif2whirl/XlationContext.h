// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/xaif2whirl/XlationContext.h,v 1.9 2006/05/12 16:12:24 utke Exp $

#ifndef XlationContext_INCLUDED
#define XlationContext_INCLUDED

//************************** System Include Files ***************************

#include <list>
#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"

namespace xaif2whirl{

  /**
   * XlationContext for xaif2whirl: Represents information about a
   * XAIF->WHIRL translation context.  Designed to contain information
   * about a particular level. Contained in a stack in PUXlationContext.
   */
  class XlationContext {
  public: 

    XlationContext();
  
    ~XlationContext();
    
    void dump(std::ostream& o, const std::string& indent) const;

    void ddump() const;

    enum Flags_E {
      NOFLAG           = 0x00000000,
      ACTIVETYPE       = 0x00000001, // active symbol ref, determined from symbol type (inheritUp)
      VALUESELECTOR    = 0x00000002, // value selector, is also set for passive vars (inheritDown) 
      DERIVSELECTOR    = 0x00000004, // deriv selector (inheritDown)
      SUPPRESSSELECTOR = 0x00000008, // suppress any selectors for certain intrinsics (inheritDown)
      VARREF           = 0x00000010, // within variable reference (inheritDown)
      LVALUE           = 0x00000020, // var ref should be an lvalue (inheritDown) 
      ARRAY            = 0x00000040, // an array reference (inheritDown)
      ARRAYIDX         = 0x00000080, // an array index expr (inheritDown)
      EXPRSIMPLE       = 0x00000100  // within a 'simple' expr, used exclusively for CFG exprs. (inheritDown)
    };

    bool isFlag(Flags_E f) const;
    void setFlag(Flags_E f);
    void unsetFlag(Flags_E f);
    void inheritFlagsUp(const XlationContext& childContext);
    void inheritFlagsDown(const XlationContext& parentContext);
  
  private:

    /** 
     * the flags
     */ 
    unsigned int myFlags;

  };

}// end namespace

#endif 
