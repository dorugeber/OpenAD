#include <map>
#include <set>
#include "cleanUpWhirl.h"
#include "wn_tree_util.h"
#include "wn_util.h"
#include "symtab.h"

#define xDEBUG(flag,code) { if (flag) {code; fflush(stdout);} }
#define DEB_CleanUpWhirl 0

// the front-end introduces temporary variables 
// for all kinds of expressions, in particular 
// control flow related and in indices, already before the 
// conversion to whirl.  These temporaries obfuscate 
// the data dependencies by introducing an extra 
// level of indirection. Replacing the temporary 
// references with the respective right-hand-side 
// expressions of their assignments is possible everywhere with 
// the one exception of dimension computations within 
// variable declarations. In the latter case there is no
// location to represent these computations in whirl other than 
// an explicit  assignment statement to a temporary. 
// Therefore we currently do not remove any temporary assignments or 
// the respective entries in the  symbol table. 
void cleanUpPUInfo(PU_Info* aPUInfo_p) { 
  typedef std::map<ST*,std::pair<WN*,int> > STPtoWNPmap;
  STPtoWNPmap tempMap; 
  bool skipKids=false;
  WN* thePU_WN_p = PU_Info_tree_ptr(aPUInfo_p);
  xDEBUG(DEB_CleanUpWhirl, printf("cleanUpWhirl: doing %s\n",ST_name(WN_st(thePU_WN_p))););
  WN* parentWN_p=0;
  WN_TREE_CONTAINER<PRE_ORDER> aWNPtree(thePU_WN_p);
  WN_TREE_CONTAINER<PRE_ORDER>::iterator aWNPtreeIterator=aWNPtree.begin();
  while (aWNPtreeIterator != aWNPtree.end()) { 
    WN* curWN_p = aWNPtreeIterator.Wn();
    OPERATOR opr = WN_operator(curWN_p);
    if (opr==OPR_STID) {  // definitions
      if (ST_is_temp_var(WN_st(curWN_p))) {
	ST* tempST_p=WN_st(curWN_p);
	// is it not in the set?
	STPtoWNPmap::iterator mapIter=tempMap.find(tempST_p);
	if (mapIter == tempMap.end()) { //not found
	  // add it
	  tempMap.insert(std::pair<ST*,std::pair<WN*,int> >(tempST_p, std::pair<WN*,int>(WN_kid0(curWN_p),1)));
	  const char* tmpName = ST_name(tempST_p); 
	  ST* puST_p = ST_ptr(PU_Info_proc_sym(aPUInfo_p));
	  const char* puName = ST_name(puST_p);
	  xDEBUG(DEB_CleanUpWhirl, printf("cleanUpWhirl: recorded temporary %s defined in %s\n",tmpName, puName););
	}
	else { 
	  // in many cases these are single assignment but in some cases 
	  // there reassignments for instance when used in conjunction 
	  // with PRESENT for optional parameters
	  // where a different const value is assigned. 
	  WN* defWN_p=(*mapIter).second.first;
	  if (WN_operator(defWN_p)==OPR_INTCONST 
	      && 
	      WN_operator(WN_kid0(curWN_p))== OPR_INTCONST
	      && WN_const_val(defWN_p)!=WN_const_val(WN_kid0(curWN_p))) {
	    // assigning different values, i.e. can't replace
	    Set_ST_keep_in_openad(tempST_p);
	    ++((*mapIter).second.second);
	  }
	  else { 
	    const char* tmpName = ST_name(tempST_p); 
	    ST* puST_p = ST_ptr(PU_Info_proc_sym(aPUInfo_p));
	    const char* puName = ST_name(puST_p);
	    DevWarn("cleanUpWhirl: recorded temporary %s is redefined in %s\n",tmpName, puName);
	  }
	}
      }
    }
    if (opr==OPR_LDID){ // uses
      // if we refer to a temp variable
      if (ST_is_temp_var(WN_st(curWN_p))) { 
	// that variable should have been added to the set
	// so find it: 
	ST* tempST_p=WN_st(curWN_p);
	STPtoWNPmap::iterator mapIter=tempMap.find(tempST_p);
	if (mapIter==tempMap.end()) { //not found
	  // This shouldn't happen with local temporaries 
	  // but some temporaries are  not local. For instance 
	  // if we are in contained PU using an array from a containing PU 
	  // then the array size comes down in a temporary defined in the 
	  // containing PU
	  const char* tmpName = ST_name(tempST_p); 
	  ST* puST_p = ST_ptr(PU_Info_proc_sym(aPUInfo_p));
	  const char* puName = ST_name(puST_p);
	  xDEBUG(DEB_CleanUpWhirl, printf("cleanUpWhirl: no (local) definition for temporary %s in %s, probably defined in enclosing PU\n",tmpName,puName););
	}
	else { 
	  // make sure the parent is set by now
	  if (!aWNPtreeIterator.Get_parent_wn())
	  Fatal_Error("cleanUpWhirl: no parent set");
	  skipKids=true;
	  // there was some very hackish logic implemented in whirl2f 
	  // that sets an ST entry in all CASEGOTO nodes (even though they 
	  // are not supposed to have symbols at all) that are  the children 
	  // of a block following the LDID that is the first child of a 
	  // SWITCH. If the switch condition is an expression we need to 
	  // retain the temporary so here we inject the condtion that we will not 
	  // to the replacement for an LDID directly under a SWITCH
	  // and also not replace if we detected more than one definition
	  if (WN_operator(aWNPtreeIterator.Get_parent_wn())!=OPR_SWITCH && (*mapIter).second.second==1) { 
	    // replace the current node within the parent
	    WN_kid(aWNPtreeIterator.Get_parent_wn(),aWNPtreeIterator.Get_kid_index()) = WN_COPY_Tree((*mapIter).second.first);
	    const char* tmpName = ST_name(tempST_p); 
	    ST* puST_p = ST_ptr(PU_Info_proc_sym(aPUInfo_p));
	    const char* puName = ST_name(puST_p);
	    xDEBUG(DEB_CleanUpWhirl, printf("cleanUpWhirl: subsituted temporary %s in %s\n",tmpName, puName););
	  }
	}
      }
    } 
    // advance the iterator
    if (skipKids || opr==OPR_XPRAGMA){
      // XPRAGMAs may refer to temporaries before they are assigned
      aWNPtreeIterator.WN_TREE_next_skip();
      skipKids=false;
    }
    else
      ++aWNPtreeIterator;
  } // end while
}

// the C wrapper 
extern "C" void cleanUpPUInfoTree(PU_Info* aPUInfoTree_p) { 
  if (!aPUInfoTree_p) { return; }
  cleanUpPUInfo(aPUInfoTree_p);
  // we cannot recur on the children because 
  // they are written *before* the parents and their 
  // Whirl rep is deallocated
}
