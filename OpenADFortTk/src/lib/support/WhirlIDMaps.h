// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef WhirlIDMaps_INCLUDED_h
#define WhirlIDMaps_INCLUDED_h

#include <iostream>
#include <map>  // STL
#include <set>  // STL
#include <list> // STL

#include "Open64IRInterface/Open64BasicTypes.h"

#include "BaseMap.h"
#include "Diagnostics.h"


namespace fortTkSupport { 

typedef UINT SymTabId;
typedef UINT SymId;
typedef UINT PUId;
typedef UINT WNId;

// A list of ids: Should only be used with scalar id types above
template <class T>
class IdList : public std::list<T> {
public:
  IdList() { }
  ~IdList() { }

  // Returns 0 if not found
  T Find(T id) const
  {
    typename std::list<T>::iterator it;
    for (it = this->begin(); it != this->end(); ++it) {
      T val = *it;
      if (id == val) { return val; }
    }
    return 0;
  }
  
};


//***************************************************************************
// ST_TAB <-> SymTabId maps (global/interprocedural)
//***************************************************************************

class SymTabToSymTabIdMap 
  : public BaseMap<ST_TAB*, SymTabId>
{
public:
  SymTabToSymTabIdMap() { }
  SymTabToSymTabIdMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~SymTabToSymTabIdMap() { }
  
  void Create(PU_Info* pu_forest);
};


// SymTabIdToSymTabMap: In WHIRL, all ST_TAB* are associated with a
// specific PU_Info*, except the global ST_TAB*.  Because of the way
// the symbol table is implemented, it is usually not easy to access the
// symbol table with the ST_TAB*: one needs the corresponding
// PU_Info*.  Consequently, we map a SymTabId to a pair.  When
// entering the global the global ST_TAB* in the map, PU_Info* should
// be NULL.
class SymTabIdToSymTabMap 
  : public std::map<SymTabId, pair<ST_TAB*, PU_Info*> > {

protected:
  typedef std::map<SymTabId, pair<ST_TAB*, PU_Info*> > BaseMap;
  
public:
  SymTabIdToSymTabMap() { }
  SymTabIdToSymTabMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~SymTabIdToSymTabMap() { }
  
  pair<ST_TAB*, PU_Info*>
  Find(SymTabId id, bool mustFind = false) const
  {
    pair<ST_TAB*, PU_Info*> result(NULL, NULL);
    
    const_iterator it = this->find(id);
    if (it != this->end()) {
      result = (*it).second;
    } 
    else if (mustFind) {
      FORTTK_DIE("SymTabIdToSymTabMap: Could not find entry for key '" 
		 << id << "'");
    }
    
    return result;
  }

  void
  Insert(SymTabId id, ST_TAB* stab, PU_Info* pu)
  {
    this->insert(make_pair(id, make_pair(stab, pu))); // do not add duplicates!
  }

  void Create(PU_Info* pu_forest);
};


//***************************************************************************
// ST <-> SymId maps (intra-procedural)
//***************************************************************************

// Note: Instead of creating ST* <-> to SymId maps, we currently use a
// ST's index -- ST_index(ST*) -- in the symbol table as a persistent
// id.  While this is correct, it would be nice to add an interface
// map, even if the map is really only the identity function without
// any state.


//***************************************************************************
// PU <-> PUId maps (global/interprocedural)
//***************************************************************************

class PUToPUIdMap 
  : public BaseMap<PU_Info*, PUId>
{
public:  
  PUToPUIdMap() { }
  PUToPUIdMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~PUToPUIdMap() { }

  void Create(PU_Info* pu_forest);
};


class PUIdToPUMap
  : public BaseMap<PUId, PU_Info*>
{
public:
  PUIdToPUMap() { }
  PUIdToPUMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~PUIdToPUMap() { }
  
  void Create(PU_Info* pu_forest);
};


//***************************************************************************
// WNId <-> WN map
//***************************************************************************

class WNToWNIdMap 
  : public BaseMap<WN*, WNId>
{
public:  
  WNToWNIdMap() { }
  WNToWNIdMap(WN* wn) { Create(wn); }
  virtual ~WNToWNIdMap() { }

  void Create(WN* wn);
};

class WNIdToWNMap 
  : public BaseMap<WNId, WN*>
{
public:
  WNIdToWNMap() { }
  WNIdToWNMap(WN* wn) { Create(wn); }
  virtual ~WNIdToWNMap() { }
  
  void Create(WN* wn);
};


// ---------------------------------------------------------
// 
// ---------------------------------------------------------

// Note: Assumes ownership of the tables it creates
class WNToWNIdTabMap 
  : public BaseMap<PU_Info*, WNToWNIdMap*> {
  
public:
  WNToWNIdTabMap() { }
  WNToWNIdTabMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~WNToWNIdTabMap();
  
  void Create(PU_Info* pu_forest);
  void Destroy();
};

class WNIdToWNTabMap 
  : public BaseMap<PU_Info*, WNIdToWNMap*> {
  
public:
  WNIdToWNTabMap() { }
  WNIdToWNTabMap(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~WNIdToWNTabMap();
  
  void Create(PU_Info* pu_forest);
  void Destroy();
};


//***************************************************************************
// Optional routines for map creation
//***************************************************************************

// CreateSymTabIdMaps: Given a PU forest, initialize the non-NULL
// persistent ID <-> ST_TAB* maps.  STTABIds are guaranteed to be
// unique within the PU forest 'pu_forest'. (The global symbol table
// is included.)
void
CreateSymTabIdMaps(PU_Info* pu_forest, 
		   SymTabToSymTabIdMap* x, SymTabIdToSymTabMap* y);

// CreatePUIdMaps: Given a PU forest, initialize the non-NULL
// persistent ID <-> PU_Info* maps.  PUIds are guaranteed to be unique
// within the PU forest 'pu_forest'.
void
CreatePUIdMaps(PU_Info* pu_forest, PUToPUIdMap* x, PUIdToPUMap* y);

// CreateWhirlIDMaps: Given a WN*, initialize the non-NULL persistent
// ID <-> WN* maps.  WNIds are guaranteed to be unique within the
// WHIRL tree rooted at 'wn'. (Note that 'wn' is usually the result of
// PU_Info_tree_ptr(PU_Info*).)
void
CreateWhirlIdMaps(WN* wn, WNToWNIdMap* x, WNIdToWNMap* y);


}

#endif 
