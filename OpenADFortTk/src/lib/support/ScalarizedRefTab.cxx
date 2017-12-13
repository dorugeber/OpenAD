// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/lib/support/ScalarizedRefTab.cxx,v 1.21 2006/05/12 16:12:22 utke Exp $

#include <sstream>

#include "OpenAnalysis/ExprTree/ExprTree.hpp"

#include "Open64IRInterface/Open64BasicTypes.h"

#include "ir_reader.h"   


#include "ScalarizedRefTab.h"

#include "Diagnostics.h"


namespace fortTkSupport { 

  ScalarizedRefTab_Base::ScalarizedRefTab_Base()
  {
  }

  ScalarizedRefTab_Base::~ScalarizedRefTab_Base()
  {
  }

  ScalarizedRefTabMap_W2X::ScalarizedRefTabMap_W2X()
  {
  }

  ScalarizedRefTabMap_W2X::~ScalarizedRefTabMap_W2X()
  {
    for (iterator it = begin(); it != end(); ++it) {
      delete (*it).second; // ScalarizedRefTab_W2X*
    }
    clear();
  }

  void
  ScalarizedRefTabMap_W2X::Create(PU_Info* pu_forest)
  { 
    Open64IRProcIterator procIt(pu_forest);
    for ( ; procIt.isValid(); ++procIt) { 
      PU_Info* pu = (PU_Info*)procIt.current().hval();
    
      ScalarizedRefTab_W2X* tab = new ScalarizedRefTab_W2X(pu);
      Insert(pu, tab);
    }
  }


  // ScalarizedRefTab
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>::ScalarizedRefTab() {}
  
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>:: ~ScalarizedRefTab() {
    // Clear table ref pool
    for (ScalarizedRefPoolTy::iterator it = scalarizedRefPool.begin();
	 it != scalarizedRefPool.end(); ++it) {
      delete (*it); // ScalarizedRef*
    }
    scalarizedRefPool.clear();
  
    // Clear Table
    clear();
  }


  void
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>::
  Create(PU_Info* pu)
  { 
    WN* wn_pu = PU_Info_tree_ptr(pu);
    WN* fbody = WN_func_body(wn_pu);
    AddToScalarizedRefTabOp op(this, pu);
    ForAllScalarizableRefs(fbody, op);
  }

  void
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>::
  Dump(std::ostream& o) const
  {
    DumpFmt(std::cerr);
  }

  void
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>::
  DDump() const
  {
    Dump(std::cerr);
  }

  void
  ScalarizedRefTab<ScalarizedRefTab_Base::W2X>::
  DumpFmt(std::ostream& o, const char* pre) const
  {
    std::string p = pre;
    std::string p1 = p + "  ";

    o << p << "{ ================== Begin ScalarizedRefTab Dump ("
      << size() << " Entries):\n";
  
    for (const_iterator it = begin(); it != end(); ++it) {
      WN* wn = (*it).first;
      ScalarizedRef* sym = (*it).second;
      o << p1 << wn << " --> ";
      sym->dump(o);
      o << std::endl;
    }
    o << p << "End ScalarizedRefTab Dump ================== }\n";
  }


  void 
  ForAllScalarizableRefs(const WN* wn, ForAllScalarizableRefsOp& op)
  {
    // Special base case
    if (wn == NULL) { return; }

    OPERATOR opr = WN_operator(wn);
    if (ScalarizedRef::isRefScalarizable(wn)) {
    
      // Base case
      int ret = op(wn); // FIXME: what to do on error?
      //bool foo = IsRefScalarizable(wn); // for debugging

      // Special recursive case: Since WHIRL stores are statements (not
      // expressions) we need to check the RHS (kid0) of the implied
      // assignment for non-scalar references.
      if (OPERATOR_is_store(opr)) {
	ForAllScalarizableRefs(WN_kid0(wn), op);
      }

    } 
    else if (!OPERATOR_is_leaf(opr)) {
    
      // General recursive case
      if (WN_operator(wn) == OPR_BLOCK) {
	WN *kid = WN_first(wn);
	while (kid) {
	  ForAllScalarizableRefs(kid, op);
	  kid = WN_next(kid);
	}
      } 
      else {
	for (INT kidno = 0; kidno < WN_kid_count(wn); kidno++) {
	  WN* kid = WN_kid(wn, kidno);
	  ForAllScalarizableRefs(kid, op);
	}
      }
    
    }
  }


  AddToScalarizedRefTabOp::
  AddToScalarizedRefTabOp(ScalarizedRefTab_W2X* tab_, PU_Info* curpu_)
    : tab(tab_), curpu(curpu_)
  { 
    FORTTK_ASSERT(tab_, fortTkSupport::Diagnostics::UnexpectedInput);
    ir = new Open64IRInterface();
  }

  AddToScalarizedRefTabOp::~AddToScalarizedRefTabOp()
  {
    workmap.clear();
  }


  // Given a non-scalar reference 'wn', create a dummy variable and
  // add to the map.  
  int 
  AddToScalarizedRefTabOp::operator()(const WN* wn) 
  {
    // create a hash of this reference
    ostringstream o;
    ir->DumpWN((WN*)wn,o);
    string hashVal = o.str();
  
    // if <hash, sym> not already in workmap, insert <hash, new sym>
    ScalarizedRef* sym = NULL;
    WorkMapTy::iterator it = workmap.find(hashVal);
    if (it == workmap.end()) {
      sym = new ScalarizedRef(const_cast<WN*>(wn));
      workmap.insert(make_pair(hashVal, sym));
    } 
    else {
      sym = (*it).second;
    }
  
    FORTTK_MSG(1, "AddToScalarizedRefTabOp::operator() : hashVal : '" << hashVal.c_str() << "' --> " <<  sym->getName().c_str() << " (" << sym << ',' << wn << ")");

    tab->Insert(wn, sym);
    return 0;
  }

}
