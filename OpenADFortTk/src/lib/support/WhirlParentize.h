// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/WhirlParentize.h,v 1.5 2007-10-08 18:28:33 utke Exp $
#ifndef WhirlParentize_h_INCLUDED
#define WhirlParentize_h_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"

namespace fortTkSupport { 

  // FindParentWNBlock: Given two WHIRL nodes, a subtree 'wn_tree' and an
  // some descendent 'wn', return the BLOCK WN that contains 'wn', or
  // NULL.  
  // 
  // Warning: This routine does not require creation of a
  // 'WhirlParentMap'.  Use sparingly and carefully or order statistics
  // will bite you!
  extern WN* 
  FindParentWNBlock(const WN* wn_tree, const WN* wn);


  /** 
   * A WHIRL abstract syntax tree only contains child pointers -- no
   * parent pointers.  WhirlParentMap provides parent pointers -- in
   * an auxillary object -- for times when they would be useful.
   */
  class WhirlParentMap {
  public:
  
    WhirlParentMap();
    WhirlParentMap(const WN* wn); // automatically creates the map
    ~WhirlParentMap();
  
    // Create: Given a WN*, creates the map.  Clears any previously existing map.
    void Create(const WN* wn);
  
    // Clear: Clears the map.
    void Clear();

    // Find: Given a WN*, return its parent.  N.B. Does not check that
    // the map exists.
    WN* Find(const WN* wn) { return (WN *)WN_MAP_Get(parentMap, wn); }
  
    // Find: Given a WN*, find the first enclosing block or NULL.
    // N.B. Does not check that the map exists.
    WN* FindBlock(const WN* wn);
  
    // Insert: Given a WN* and its parent, insert in the map. Overwrites
    // any existing value.
    void Insert(const WN* wn, const WN* parent) {
      WN_MAP_Set(parentMap, (WN*)wn, (void *)parent);
    }
  
    // GetMap: returns the parent map.  Intended to be only used with
    // Open64 routines that want a parent map.
    WN_MAP GetMap() const { return parentMap; }

  private:
    void Ctor();
    void Parentize(const WN* wn);
  
  private:
    // For now keep the implementation using Open64's WN_MAP
    MEM_POOL memPool;
    WN_MAP parentMap;
  };



}

#endif 
