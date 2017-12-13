// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2xaif/XlationContext.h,v 1.23 2007-10-08 18:28:34 utke Exp $

#ifndef XlationContext_INCLUDED
#define XlationContext_INCLUDED

//************************** System Include Files ***************************

#include <list>
#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "WhirlParentize.h"
#include "WhirlIDMaps.h"
#include "OAMaps.h"
#include "Open64IRInterface/SymTab.h"
#include "ScalarizedRefTab.h"

namespace whirl2xaif{

  /**
   * XlationContext for whirl2xaif: Represents information about a
   * WHIRL->XAIF translation context.  Designed to contain information
   * about a particular level. Contained in a stack in PUXlationContext.
   */
  class XlationContext {
  public: 

    XlationContext(unsigned int aStackPosition=0);
  
    ~XlationContext();
  
    WN* getWN() const;
    bool hasWN() const;
    void setWN(WN* aWNp);

    /** 
     * regular use
     */
    unsigned int getNewVertexId();

    /** 
     * return current id, 0 is considered invalid
     */
    unsigned int getVertexId() const;

    /** 
     * return a peek of the next id
     */
    unsigned int peekVertexId() const;

    /** 
     * regular use
     */
    unsigned int getNewEdgeId();

    /** 
     * return current id, 0 is considered invalid
     */
    unsigned int getEdgeId() const;

    /** 
     * return a peek of the next id
     */
    unsigned int peekEdgeId() const;

    std::string toString() const;
  
    void dump(std::ostream& o, const std::string& indent) const;

    void ddump() const;

    enum Flags_E {
      NOFLAG               = 0x00000000, // for initialization
      ASSIGN               = 0x00000001, // within xaif:Assignment;        inherited
      VARREF               = 0x00000002, // within xaif:VariableReference; inherited
      DEREF_ADDR           = 0x00000004, 
      HAS_LOGICAL_ARG      = 0x00000200, 
      IS_LOGICAL_ARG       = 0x00000400,
      IS_LOGICAL_OPERATION = 0x00000800,
      IO_STMT              = 0x00004000, // candidate for removal
      DEREF_IO_ITEM        = 0x00008000, // candidate for removal
      ORIGFMT_IOCTRL       = 0x00010000, // candidate for removal
      FMT_IO               = 0x00020000, // candidate for removal
      CRAY_IO              = 0x00040000, // candidate for removal
      HAS_NO_ARR_ELMT      = 0x00200000  // inherited
    };

    static std::string flagToString(Flags_E aFlag);

    bool isFlag(Flags_E f) const ;
    void setFlag(Flags_E f);
    void unsetFlag(Flags_E f);
    void inheritFlags(const XlationContext& parentContext);
  
  private:

    /**
     * we don't own this one
     */
    WN* myWNp;

    /**
     * next unique vertex id for this context
     */
    unsigned int myNextVertexId; 

    /**
     * next unique edge id for this context
     */
    unsigned int myNextEdgeId;

    /** 
     * the flags
     */ 
    unsigned int myFlags;

    /**
     * for debugging purposes - the stack position
     */
    unsigned int myStackPosition;

  };

}// end namespace

#endif 
