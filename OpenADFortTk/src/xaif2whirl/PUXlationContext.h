// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/PUXlationContext.h,v 1.4 2008-02-21 14:32:05 utke Exp $

#ifndef PUXlationContext_INCLUDED
#define PUXlationContext_INCLUDED

#include <list> 
#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "WhirlParentize.h"
#include "WhirlIDMaps.h"
#include "Symbol.h"
#include "XAIFSymToSymbolMap.h"
#include "XlationContext.h"

namespace xaif2whirl {

  /**
   * PUXlationContext for xaif2whirl: Represents information about a
   * XAIF->WHIRL translation context.  Designed to convey information
   * about a parent context to children contexts.
   * 
   * PUXlationContext maintains an internal stack of XlationContexts,
   * allowing users to 1) create new child contexts (push), 2) destroy
   * and return to parent contexts (pop) and 3) query the current
   * context.  The current translation context provides information
   * about the containing WHIRL context.  There should always be at
   * least one context on the stack.  (Upon creation, one context exists
   * on the stack.)  Because this class is designed to represent
   * information about one procedure (WHIRL PU) at a time, information
   * that applies for a whole PU is located outside instead of inside
   * the stack.
   *
   * For xaif2whirl, a translation context for an XAIF construct
   * corresponds to its parent construct.  Thus there will be one
   * translation context on the stack for every 'indendation' level.
   * Context flags indicate significant facts about the parent construct
   * and will typically correspond to XAIF concepts (as opposed to
   * WHIRL).
   */

  class PUXlationContext {
  public: 

    /**
     * the originator is some name that we can use 
     * for debugging purposes
     */
    PUXlationContext(const std::string& anOriginator, PU_Info* pu_forest);

    ~PUXlationContext();

    // -------------------------------------------------------
    // stacked XlationContext manipulation (Create, Delete...)
    // -------------------------------------------------------
  
    /**
     * Create a new child context and make it the current context.
     */
    void createXlationContext();

    /**
     * Create a new child context and make it the current context.
     * We pass flags that should apply to the new context.  Note that
     * that these flags *do not* override any inherited flags.
     */
    void createXlationContext(XlationContext::Flags_E f);

    /** 
     * Delete the current context and make its parent the current
     * context.  Everything (including memory) associated with the old
     * context will be deleted.
     */
    void deleteXlationContext();
  
    /** 
     * Returns the current context
     */
    XlationContext& currentXlationContext();

    // -------------------------------------------------------
    // PU level maps and data
    // -------------------------------------------------------

    WN* findParentWN(WN*);
    WN* findParentBlockWN(WN*);
    fortTkSupport::WhirlParentMap* getWNParentMap() const;
    void setWNParentMap(fortTkSupport::WhirlParentMap* aWhirlParentMapP);
  
    std::pair<ST_TAB*, PU_Info*> findSymTab(fortTkSupport::SymTabId stabId);
  
    PU_Info* findPU(fortTkSupport::PUId aPUid);

    fortTkSupport::WNId findWNId(WN* wn);
    fortTkSupport::WNToWNIdMap* getWNToWNIdMap() const;
    void setWNToWNIdMap(fortTkSupport::WNToWNIdMap* aWNToWNIdMapP);

    WN* findWN(fortTkSupport::WNId aWNId, bool mustFind = false);
    fortTkSupport::WNIdToWNMap* getWNIdToWNMap() const;
    void setWNIdToWNMap(fortTkSupport::WNIdToWNMap* aWNIdToWNMapP);

    fortTkSupport::Symbol* findSym(const std::string& scopeid, const std::string& symid);
    fortTkSupport::XAIFSymToSymbolMap& getXAIFSymToSymbolMap();
    
    // -------------------------------------------------------
    // Misc
    // -------------------------------------------------------

    void dump(std::ostream& o, const std::string& indent) const;
    void ddump() const;

    static void setPrefix(const std::string& aPrefix);
    static const std::string& getPrefix();

  private: 
    /**
     * no def
     */
    PUXlationContext(const XlationContext& x);

    /**
     * no def
     */
    PUXlationContext();

    /**
     * no def
     */
    PUXlationContext& operator=(const PUXlationContext& x);
  
    fortTkSupport::SymTabIdToSymTabMap mySymTabIdToSymTabMap;

    fortTkSupport::PUIdToPUMap myPUIdToPUMap;

    fortTkSupport::XAIFSymToSymbolMap myXAIFSymToSymbolMap;

    /**
     * we don't own this
     */
    fortTkSupport::WhirlParentMap* myWNParentMapP;

    /**
     * we don't own this
     */
    fortTkSupport::WNToWNIdMap* myWNToWNIdMapP;
  
    /**
     * we don't own this
     */
    fortTkSupport::WNIdToWNMap* myWNIdToWNMapP;

    // a globally defined prefix for all newly created symbols
    static std::string ourPrefix;

    /** Use a list instead a stack so that we can easily examine
     *  contents.  The top of the stack will be the *front* of the
     * list. (push_front, pop_front).
     */
    typedef std::list<XlationContext> XlationContextStack;

    /**
     *  the context stack, we own all the entries in it
     */
    XlationContextStack myXlationContextStack;

    /**
     * this makes a new XlationContext
     * and pushes it on top
     */
    void PushNewXlationContext(XlationContext::Flags_E f);

    /**
     * some string passed to the ctor to indicate who created this
     */
    std::string myOriginator;

  };

}

#endif /* XlationContext_INCLUDED */
