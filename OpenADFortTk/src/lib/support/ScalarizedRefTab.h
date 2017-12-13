// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/lib/support/ScalarizedRefTab.h,v 1.17 2007-10-08 18:28:32 utke Exp $

#ifndef ScalarizedRefTab_H 
#define ScalarizedRefTab_H

#include <set>
#include <map>
#include <string>
#include <iostream>

#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/Open64IRInterface.hpp"
#include "BaseMap.h"
#include "Diagnostics.h"
#include "ScalarizedRef.h"

namespace fortTkSupport { 

class ScalarizedRefTab_Base
{
public:
  enum TableType { 
    W2X, // A table optimized for WHIRL->XAIF lookups
    X2W  // A table optimized for XAIF->WHIRL lookups
  };

public:
  
  typedef std::set<ScalarizedRef*> ScalarizedRefPoolTy;

  ScalarizedRefPoolTy::iterator RefPoolBegin()
  { return scalarizedRefPool.begin(); }

  ScalarizedRefPoolTy::iterator RefPoolEnd()
  { return scalarizedRefPool.end(); }
  
protected:
  ScalarizedRefTab_Base();
  virtual ~ScalarizedRefTab_Base();
  
  // Insert 'x' into the ref pool if not already there
  void
  InsertIntoPool(ScalarizedRef* x)
  {
    if (scalarizedRefPool.find(x) == scalarizedRefPool.end()) {
      scalarizedRefPool.insert(x);
    }
  }

protected:
  
  // Because one scalarized ref can be associated with several actual
  // references, we need a pool in which these references are entered
  // only once.
  ScalarizedRefPoolTy scalarizedRefPool;
};




//***************************************************************************
// ScalarizedRefTab
//***************************************************************************

// ScalarizedRefTab: A dummy template that will be specialized below.
// For mapping certain non-scalar WHIRL references to dummy scalar
// variables
template <ScalarizedRefTab_Base::TableType TabTy>
class ScalarizedRefTab 
  : public ScalarizedRefTab_Base,
    public BaseMap<void*, void*>
{
};


// ---------------------------------------------------------
// ScalarizedRefTab: Specialization for W2X
// ---------------------------------------------------------
typedef ScalarizedRefTab<ScalarizedRefTab_Base::W2X> 
  ScalarizedRefTab_W2X;

template <>
class ScalarizedRefTab<ScalarizedRefTab_Base::W2X>
  : public ScalarizedRefTab_Base,
    public BaseMap<WN*, ScalarizedRef*> {

public:
  // Constructor allocates an empty data structure
  ScalarizedRefTab();
  ScalarizedRefTab(PU_Info* pu) { Create(pu); }
  virtual ~ScalarizedRefTab();
  
  // Create: Fills in map
  void Create(PU_Info* pu);

  // Find: 
  virtual ScalarizedRef* 
  Find(WN* x_, bool mustFind = false) const
  { return BaseMap<WN*, ScalarizedRef*>::Find(x_, mustFind); }
  
  // Find: a version with const params for convenience
  ScalarizedRef* 
  Find(const WN* x_, bool mustFind = false) const
  {
    WN* x = const_cast<WN*>(x_);
    return BaseMap<WN*, ScalarizedRef*>::Find(x, mustFind);
  }
  
  
  // Insert: override to manage scalar ref pool
  virtual bool 
  Insert(WN* x, ScalarizedRef* y) {
    InsertIntoPool(y);
    return BaseMap<WN*, ScalarizedRef*>::Insert(x, y);
  }
  
  // Insert: a version with const params for convenience
  bool 
  Insert(const WN* x_, const ScalarizedRef* y_) {
    WN* x = const_cast<WN*>(x_);
    ScalarizedRef* y = const_cast<ScalarizedRef*>(y_);
    return Insert(x, y);
  }
  
  
  // Dump contents for inspection
  virtual void Dump(std::ostream& o = std::cerr) const;
  virtual void DDump() const;

  void DumpFmt(std::ostream& o = std::cerr, const char* pre = "") const;
  
protected:
  // Should not be used
  ScalarizedRefTab(const ScalarizedRefTab& x) { }
  ScalarizedRefTab& operator=(const ScalarizedRefTab& x) { return *this; }

private: 
};


// ---------------------------------------------------------
// ScalarizedRefTabMap: Specialization for W2X
// ---------------------------------------------------------

class ScalarizedRefTabMap_W2X 
  : public BaseMap<PU_Info*, ScalarizedRefTab_W2X*> {

public:
  ScalarizedRefTabMap_W2X();
  ScalarizedRefTabMap_W2X(PU_Info* pu_forest) { Create(pu_forest); }
  virtual ~ScalarizedRefTabMap_W2X();
  
  void Create(PU_Info* pu_forest);
};


//***************************************************************************
// Variable References and WHIRL/XAIF
//***************************************************************************

// See tech report for explanation of three categories of references.


//***************************************************************************
// 
//***************************************************************************

// ForAllScalarizableRefsOp: Abstract base class for the operator passed
// to the function 'ForAllScalarizableRefs(...)'.  Any caller of this
// function must define its own operator object, using this class
// as a base class and providing a definition for 'operator()'.
class ForAllScalarizableRefsOp {
public:
  ForAllScalarizableRefsOp() { }
  virtual ~ForAllScalarizableRefsOp() { }

  // Given a non-scalar reference 'wn', does something interesting.
  // Returns 0 on success; non-zero on error.
  virtual int operator()(const WN* wn) = 0;
private: 
};

void 
ForAllScalarizableRefs(const WN* wn, ForAllScalarizableRefsOp& op);



// AddToScalarizedRefTabOp: Given a ScalarizedRefTab, add references to it
class AddToScalarizedRefTabOp : public ForAllScalarizableRefsOp {
public:
  AddToScalarizedRefTabOp(ScalarizedRefTab_W2X* tab_, PU_Info* curpu_);
  virtual ~AddToScalarizedRefTabOp();
  
  ScalarizedRefTab_W2X* GetTab() { return tab; }
  
  // Given a non-scalar reference 'wn', create a dummy variable and
  // add to the map.  
  int operator()(const WN* wn);

private:
  typedef std::map<std::string, ScalarizedRef*> WorkMapTy;

private:
  ScalarizedRefTab_W2X* tab;
  PU_Info* curpu;
  
  OA::OA_ptr<Open64IRInterface> ir;
  WorkMapTy workmap;
};

}

#endif 
