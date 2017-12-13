#include <stdio.h>

#include "PUXlationContext.h"
#include "Open64IRInterface/SymTab.h"
#include "Diagnostics.h"
#include "whirl2xaif.h"

namespace whirl2xaif { 
  
  PUXlationContext::PUXlationContext(const std::string& anOriginator, Open64IRInterface& anIrInterface) : 
    myWNParentMapP(NULL), 
    myStab2idMapP(NULL), 
    myPU2idMapP(NULL), 
    myWN2idMapP(NULL),
    myScalarizedRefTab_W2Xp(NULL),
    myOriginator(anOriginator),
    myF90Flag(false),
    myIrInterface(anIrInterface) {
    myXlationContextStack.push_front(XlationContext(0));
  }

  PUXlationContext::~PUXlationContext() {
    // clear the stack
    myXlationContextStack.clear(); 
  }

  void PUXlationContext::createXlationContext() {
    PushNewXlationContext(XlationContext::NOFLAG, NULL);
  }

  void PUXlationContext::createXlationContext(XlationContext::Flags_E f) {
    PushNewXlationContext(f, NULL);
  }

  void PUXlationContext::createXlationContext(XlationContext::Flags_E f, WN* aWNp) {
    PushNewXlationContext(f, aWNp);
  }

  void PUXlationContext::PushNewXlationContext(XlationContext::Flags_E f, WN* aWNp) {
    if (myXlationContextStack.empty()) 
      // this should never happen
      FORTTK_DIE("PUXlationContext::PushNewXlationContext: empty context stack");
    const XlationContext& parentXlationContext = myXlationContextStack.front();
    myXlationContextStack.push_front(XlationContext(myXlationContextStack.size()));
    currentXlationContext().inheritFlags(parentXlationContext);
    if (aWNp)
      currentXlationContext().setWN(aWNp);
    currentXlationContext().setFlag(f);
  }

  void PUXlationContext::deleteXlationContext() {
    if (myXlationContextStack.size() > 1) {
      // maintain invariant that there is at least one context
      myXlationContextStack.pop_front();
    }
  }

  XlationContext& PUXlationContext::currentXlationContext() { 
    return myXlationContextStack.front(); 
  }

  WN* PUXlationContext::getMostRecentWN() {
    WN* theWNp=0;
    for (XlationContextStack::iterator it=myXlationContextStack.begin();
	 it!=myXlationContextStack.end();
	 ++it) {
      if (it->hasWN()) { 
	theWNp=it->getWN();
	break;
      }
    }
    if (!theWNp)
      FORTTK_DIE("PUXlationContext::getMostRecentWN: none found");
    return theWNp;
  }

  WN* PUXlationContext::findParentWN(WN* wn) {
    if (!myWNParentMapP)
      FORTTK_DIE("PUXlationContext::findParentWN: myWNParentMapP not set");
    if(!wn)
      FORTTK_DIE("PUXlationContext::findParentWN: null pointer passed");
    return (myWNParentMapP->Find(wn));
  }

  WN* PUXlationContext::findParentBlockWN(WN* wn) {
    if (!myWNParentMapP)
      FORTTK_DIE("PUXlationContext::FindParentBlockWN: myWNParentMapP not set");
    if(!wn)
      FORTTK_DIE("PUXlationContext::FindParentBlockWN: null pointer passed");
    return (myWNParentMapP->FindBlock(wn)); 
  }

  fortTkSupport::WhirlParentMap* PUXlationContext::getWNParentMap() const { 
    if (!myWNParentMapP)
      FORTTK_DIE("PUXlationContext::getWNParentMap: myWNParentMapP not set");
    return myWNParentMapP; 
  }

  void PUXlationContext::setWNParentMap(fortTkSupport::WhirlParentMap* aWhirlParentMapP) { 
    if (!aWhirlParentMapP)
      FORTTK_DIE("PUXlationContext::setWNParentMap: null pointer passed");
    if (myWNParentMapP) {
      if (myWNParentMapP==aWhirlParentMapP) { 
	FORTTK_MSG(2,"PUXlationContext::setWNParentMap: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setWNParentMap: was set to " 
		    << myWNParentMapP 
		    << " new settting is " 
		    << aWhirlParentMapP);
      }
    }
    myWNParentMapP = aWhirlParentMapP; 
  }

 fortTkSupport::SymTabId PUXlationContext::findSymTabId(ST_TAB* stab) {
    if (!myStab2idMapP)
      FORTTK_DIE("PUXlationContext::findSymTabId: myStab2idMapP not set");
    if (!stab)
      FORTTK_DIE("PUXlationContext::findSymTabId: null pointer passed");
    return (myStab2idMapP->Find(stab, true /*mustfind*/)); 
  }

  fortTkSupport::SymTabToSymTabIdMap* PUXlationContext::getSymTabToIdMap() const { 
    if (!myStab2idMapP)
      FORTTK_DIE("PUXlationContext::getSymTabToIdMap: myStab2idMapP not set");
    return myStab2idMapP; 
  }

  void PUXlationContext::setSymTabToIdMap(fortTkSupport::SymTabToSymTabIdMap* aSymTabToSymTabIdMapP) { 
    if (!aSymTabToSymTabIdMapP)
      FORTTK_DIE("PUXlationContext::setSymTabToIdMap: null pointer passed");
    if (myStab2idMapP)
      FORTTK_DIE("PUXlationContext::setSymTabToIdMap: already set");
    myStab2idMapP = aSymTabToSymTabIdMapP; 
  }
  
  fortTkSupport::PUId PUXlationContext::findPUId(PU_Info* pu) {
    if (!myPU2idMapP)
      FORTTK_DIE("PUXlationContext::findPUId: myPU2idMapP not set");
    if (!pu)
      FORTTK_DIE("PUXlationContext::findPUId: null pointer passed");
    return (myPU2idMapP->Find(pu));
  }

  fortTkSupport::PUToPUIdMap* PUXlationContext::getPUToIdMap() const { 
    if (!myPU2idMapP)
      FORTTK_DIE("PUXlationContext::getPUToIdMap: myPU2idMapP not set");
    return myPU2idMapP; 
  }

  void PUXlationContext::setPUToIdMap(fortTkSupport::PUToPUIdMap* aPUToPUIdMapP) {
    if (!aPUToPUIdMapP)
      FORTTK_DIE("PUXlationContext::setPUToIdMap: null pointer passed");
    if (myPU2idMapP)
      FORTTK_DIE("PUXlationContext::setPUToIdMap: already set");
    myPU2idMapP = aPUToPUIdMapP; 
  }

  fortTkSupport::WNId PUXlationContext::findWNId(WN* wn) {
    if (!myWN2idMapP)
      FORTTK_DIE("PUXlationContext::findWNId: myWN2idMapP not set");
    if (!wn)
      FORTTK_DIE("PUXlationContext::findWNId: null pointer passed");
    return myWN2idMapP->Find(wn);
  }

  fortTkSupport::WNToWNIdMap* PUXlationContext::getWNToIdMap() const { 
    if (!myWN2idMapP)
      FORTTK_DIE("PUXlationContext::getWNToIdMap: myWN2idMapP not set");
    return myWN2idMapP; 
  }

  void PUXlationContext::setWNToIdMap(fortTkSupport::WNToWNIdMap* aWNToWNIdMapP) { 
    if (!aWNToWNIdMapP)
      FORTTK_DIE("PUXlationContext::setWNToIdMap: null pointer passed");
    // JU: this is being reset hmm
    if (myWN2idMapP) { 
      if (myWN2idMapP==aWNToWNIdMapP) { 
	FORTTK_MSG(2,"PUXlationContext::setWNToIdMap: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setWNToIdMap: already set to "
		    << myWN2idMapP  
		    << " new " 
		    << aWNToWNIdMapP);
      }
    }
    myWN2idMapP = aWNToWNIdMapP; 
  }

  int PUXlationContext::findUDDUChainId(WN* wnexpr) {
    if (myUdduchains.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::findUDDUChainId: myUdduchains not set");
    if (!wnexpr) 
      FORTTK_DIE("PUXlationContext::findUDDUChainId: null pointer passed");
    OA::MemRefHandle h((OA::irhandle_t)wnexpr);
    int duudKey=myUdduchains->getUDDUChainId(h);
    WN* parentWN_p;
    if (duudKey==0) {
      USRCPOS srcpos;
      USRCPOS_srcpos(srcpos) = WN_Get_Linenum(wnexpr);
      int aLineNumber=USRCPOS_linenum(srcpos);
      std::ostringstream ostr;
      ostr << "findUDDUChainId: no key for >"; 
      Open64IRInterface::DumpWN(wnexpr, ostr);
      ostr << "<"; 
      parentWN_p=findParentWN(wnexpr);
      while (duudKey==0 && parentWN_p) { 
	// for instance for character arrays there is considerable confusion 
	// in the OA interface which node should have the proper information, 
	// e.g. if we should refer to the 
	// ARRAY node or the child LDA node. 
	OA::MemRefHandle parenth((OA::irhandle_t)parentWN_p);
	duudKey=myUdduchains->getUDDUChainId(parenth);
	if (duudKey==0) { 
	  if (fortTkSupport::Diagnostics::getDiagnosticFilterLevel() > 1) {
	    ostr << " or parent >"; 
	    Open64IRInterface::DumpWN(parentWN_p, ostr);
	    ostr << "<"; 
	  }
	  if (!aLineNumber) { 
	    USRCPOS_srcpos(srcpos) = WN_Get_Linenum(parentWN_p);
	    aLineNumber=USRCPOS_linenum(srcpos);
	  }
	  parentWN_p=findParentWN(parentWN_p);
	}
      }
      if (duudKey==0) { 
	if (aLineNumber)
	  ostr << " line " << aLineNumber; 
        FORTTK_MSG(1,ostr.str().c_str()); 
      }
      else { 
	ostr << " eventually found " << duudKey << " in >"; 
	Open64IRInterface::DumpWN(parentWN_p, ostr);
	ostr << "<"; 
	FORTTK_MSG(2,ostr.str().c_str()); 
      }
    }
    else { 
      std::ostringstream ostr;
      ostr << "findUDDUChainId: for: "; 
      Open64IRInterface::DumpWN(wnexpr, ostr);
      ostr << " found " << duudKey; 
      FORTTK_MSG(2,ostr.str().c_str()); 
    }
    return duudKey;
  }

  int PUXlationContext::getAliasMapKey(WN* wnexpr) {
    if (myAliasMapXAIF_p.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::getAliasMapKey: myAliasMapXAIF_p not set");
    if (!wnexpr)
      FORTTK_DIE("PUXlationContext::getAliasMapKey: null pointer passed");

    OA::MemRefHandle theHandle ((OA::irhandle_t)wnexpr);
    int theAliasMapSetKey = myAliasMapXAIF_p->getMapSetId(theHandle);
    WN* parentWN_p;
    if (theAliasMapSetKey == 0) {
      std::ostringstream ostr;
      ostr << "PUXlationContext::getAliasMapKey: no key for >";
      Open64IRInterface::DumpWN(wnexpr, ostr);
      ostr << "<"; 
      parentWN_p = findParentWN(wnexpr);
      // Recursively check parents until a key is found, or there are no parents left
      while (theAliasMapSetKey == 0 && parentWN_p) {
	// for instance for character arrays there is considerable confusion in the OA interface
	// which node should have the proper information,
	// e.g. if we should refer to the ARRAY node or the child LDA node.
	OA::MemRefHandle theParentHandle ((OA::irhandle_t)parentWN_p);
	theAliasMapSetKey = myAliasMapXAIF_p->getMapSetId(theParentHandle);
	if (theAliasMapSetKey == 0) {
	  if (fortTkSupport::Diagnostics::getDiagnosticFilterLevel() > 1) {
	    ostr << " or parent >"; 
	    Open64IRInterface::DumpWN(parentWN_p, ostr);
	    ostr << "<"; 
	  }
	  parentWN_p=findParentWN(parentWN_p);
	}
      } // end while
      if (theAliasMapSetKey == 0) { 
        FORTTK_MSG(1,ostr.str().c_str()); 
      }
      else { 
	ostr << " eventually found " << theAliasMapSetKey << " in >"; 
	Open64IRInterface::DumpWN(parentWN_p, ostr);
	ostr << "<"; 
	FORTTK_MSG(2,ostr.str().c_str()); 
      }
    } // end if theAliasMapSetKey is zero
    else { // theAliasMapSetKey is nonzero 
      std::ostringstream ostr;
      ostr << "PUXlationContext::getAliasMapKey: for: "; 
      Open64IRInterface::DumpWN(wnexpr, ostr);
      ostr << " found " << theAliasMapSetKey; 
      FORTTK_MSG(2,ostr.str().c_str()); 
    }

    std::ostringstream ostr;
    Open64IRInterface::DumpWN(wnexpr, ostr);
    FORTTK_MSG(2,"PUXlationContext::getAliasMapKey(): returns " <<  theAliasMapSetKey << " for " << ostr.str().c_str());

    return theAliasMapSetKey;
  } // end PUXlationContext::getAliasMapKey()

  OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> PUXlationContext::getUDDUChains() const { 
    if (myUdduchains.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::getUDDUChains: myUdduchains not set");
    return myUdduchains; 
  }

  void PUXlationContext::setUDDUChains(OA::OA_ptr<OA::XAIF::UDDUChainsXAIF> aUdduchainsMap) { 
    if (aUdduchainsMap.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::setUDDUChains: uninitialized OA pointer passed");
    // JU: this is being reset hmm
    if (!myUdduchains.ptrEqual(NULL)) {
      if (myUdduchains.ptrEqual(aUdduchainsMap)) { 
	FORTTK_MSG(2,"PUXlationContext::setUDDUChains: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setUDDUChains: already set to "
		    << myUdduchains 
		    << " new " 
		    << aUdduchainsMap);
      }
    }
    myUdduchains = aUdduchainsMap; 
  }

  int PUXlationContext::findDoChainId(WN* wn) {
    if (myDoChains.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::findDOChainId: myDoChains not set");
    if (!wn) 
      FORTTK_DIE("PUXlationContext::findDOChainId: null pointer passed");
    OA::StmtHandle h((OA::irhandle_t)wn);
    int doKey=myDoChains->getChainId(h);
    if (doKey==0) {
      std::ostringstream ostr;
      ostr << "findDOChainId: no key for >"; 
      Open64IRInterface::DumpWN(wn, ostr);
      ostr << "<"; 
      FORTTK_MSG(1,ostr.str().c_str()); 
    }
    else { 
      std::ostringstream ostr;
      ostr << "findDOChainId: for: "; 
      Open64IRInterface::DumpWN(wn, ostr);
      ostr << " found " << doKey; 
      FORTTK_MSG(2,ostr.str().c_str()); 
    }
    return doKey;
  }

  OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> PUXlationContext::getDoChains() const { 
    if (myDoChains.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::getDoChains: myDoChains not set");
    return myDoChains; 
  }

  void PUXlationContext::setDoChains(OA::OA_ptr<OA::XAIF::ReachDefsOverwriteXAIF> aDoChainsMap) { 
    if (aDoChainsMap.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::setDoChains: uninitialized OA pointer passed");
    // JU: this is being reset hmm
    if (!myDoChains.ptrEqual(NULL)) {
      if (myDoChains.ptrEqual(aDoChainsMap)) { 
	FORTTK_MSG(2,"PUXlationContext::setDoChains: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setDoChains: already set to "
		    << myDoChains 
		    << " new " 
		    << aDoChainsMap);
      }
    }
    myDoChains = aDoChainsMap; 
  }

  fortTkSupport::ScalarizedRef* PUXlationContext::findScalarizedRef(WN* wn) {
    if (!myScalarizedRefTab_W2Xp)
      FORTTK_DIE("PUXlationContext::findScalarizedRef: myScalarizedRefTab_W2Xp not set");
    if (!wn)
      FORTTK_DIE("PUXlationContext::findScalarizedRef: null pointer passed");
    return myScalarizedRefTab_W2Xp->Find(wn);
  }

  void PUXlationContext::setScalarizedRefTab(fortTkSupport::ScalarizedRefTab_W2X* aScalarizedRefTab_W2Xp) { 
    if (!aScalarizedRefTab_W2Xp)
      FORTTK_DIE("PUXlationContext::setScalarizedRefTab: null pointer passed");
    // JU: this is being reset hmm
    if (myScalarizedRefTab_W2Xp) { 
      if (myScalarizedRefTab_W2Xp==aScalarizedRefTab_W2Xp) { 
	FORTTK_MSG(2,"PUXlationContext::setScalarizedRefTab: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setScalarizedRefTab: already set to "
		    << myScalarizedRefTab_W2Xp
		    << " new " 
		    << aScalarizedRefTab_W2Xp);
      }
    }
    myScalarizedRefTab_W2Xp = aScalarizedRefTab_W2Xp; 
  }

  fortTkSupport::ScalarizedRefTab_W2X* PUXlationContext::getScalarizedRefTab() const { 
    if (!myScalarizedRefTab_W2Xp)
      FORTTK_DIE("PUXlationContext::getScalarizedRefTab: not set");
    return myScalarizedRefTab_W2Xp; 
  }

  int PUXlationContext::isActiveSym(ST* st) { 
    if(myActivity.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::IsActiveSym: myActivity not set");
    if (!st)
      FORTTK_DIE("PUXlationContext::IsActiveSym: null pointer passed");
    // this works with Jaewook's analysis: 
    return myActivity->isActive(OA::SymHandle((OA::irhandle_t)st)); 

#if 0
    // this is for context sensitive activity analysis
    // see if this is a module variable
    if (ST_is_in_module(st) && !ST_is_external(st)) { 
      // try to find it in the global set
      if (fortTkSupport::IntraOAInfo::isGlobalSymbolActive(st))
	return true; 
      // else look in the local information
    }
    OA::SymHandle sym = OA::SymHandle((OA::irhandle_t)st) ; 
    OA::OA_ptr<Open64IRInterface> theIR=Whirl2Xaif::getOAAnalMap().GetIRInterface();
    OA::OA_ptr<OA::MemRefExpr> symMRE = theIR->convertSymToMemRefExpr(sym);
    OA::ProcHandle proc((OA::irhandle_t)Current_PU_Info);
    std::cout << "MemRefExpr" << std::endl;
    symMRE->output(*theIR);
    std::cout << "**********" << std::endl;
    OA::OA_ptr<OA::LocIterator> symMRElocs_I = myAlias->getAliasResults(proc)->getMayLocs(*symMRE,proc);
    // we now have the locations that may alias the symbol and  need to compare these 
    // against the locations determined to be active by the activity analysis. 
    std::cout << "ActiveSym before for loop" << std::endl;
    for ( ; symMRElocs_I->isValid(); (*symMRElocs_I)++ ) {
      std::cout << "ActiveSym inside for loop" << std::endl;  
      // std::cout << "Procedure Name" << theIR->toString(proc) << std::endl;
      OA::OA_ptr<OA::LocIterator> activeLoc_I = 
          myActivity->getActiveLocsIterator(proc);
      for ( ; activeLoc_I->isValid(); (*activeLoc_I)++ ) {
	std::cout << "Found Active Location" << std::endl;
	if (activeLoc_I->current()->mayOverlap(*(symMRElocs_I->current()))) {
          std::cout << "Found Overlap, returning true" << std::endl;
	  return true;
	}
      }
    }
    std::cout << "ActiveSym outside for loop" << std::endl;
    // didn't find it in all the active locations
    return false;
#endif

  }

  int PUXlationContext::isActiveStmt(PU_Info* pu, WN* wn) { 
    if(myActivity.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::IsActiveStmt: myActivity not set");
    if (!pu || !wn)
      FORTTK_DIE("PUXlationContext::IsActiveStmt: null pointer passed");
    return myActivity->isActive(OA::ProcHandle((OA::irhandle_t)pu),
				OA::StmtHandle((OA::irhandle_t)wn)); 
  }

  int PUXlationContext::isActiveVarRef(PU_Info* pu, WN* wn) { 
    if(myActivity.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::IsActiveVarRef: myActivity not set");
    if (!pu || !wn)
      FORTTK_DIE("PUXlationContext::IsActiveVarRef: null pointer passed");
    return myActivity->isActive(OA::ProcHandle((OA::irhandle_t)pu),
				OA::MemRefHandle((OA::irhandle_t)wn)); 
  }

  void PUXlationContext::setActivity(OA::OA_ptr<OA::Activity::InterActiveFortran> anActivityMap) { 
    if(anActivityMap.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::SetActivity: null OA_ptr passed");
    if(!myActivity.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::SetActivity: already set");
    myActivity = anActivityMap; 
  }

  void PUXlationContext::setAlias(OA::OA_ptr<OA::Alias::InterAliasMap> anAliasMap) { 
    if(anAliasMap.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::SetAlias: null OA_ptr passed");
    if(!myAlias.ptrEqual(NULL))
      FORTTK_DIE("PUXlationContext::SetAlias: already set");
    myAlias = anAliasMap; 
  }

  void PUXlationContext::setAliasMapXAIF(OA::OA_ptr<OA::XAIF::AliasMapXAIF> anAliasMapXAIF_p) { 
    if (anAliasMapXAIF_p.ptrEqual(NULL)) 
      FORTTK_DIE("PUXlationContext::setAliasMapXAIF: uninitialized OA pointer passed");
    // JU: this is being reset hmm
    if (!myAliasMapXAIF_p.ptrEqual(NULL)) {
      if (myAliasMapXAIF_p.ptrEqual(anAliasMapXAIF_p)) { 
	FORTTK_MSG(2,"PUXlationContext::setAliasMapXAIF: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setAliasMapXAIF: already set to "
		    << myAliasMapXAIF_p
		    << " new " 
		    << anAliasMapXAIF_p);
      }
    }
    myAliasMapXAIF_p = anAliasMapXAIF_p; 
  }

  void PUXlationContext::dump(std::ostream& o, const std::string& indent) const {
    o << "(myOriginator=" << myOriginator.c_str() << " ";
    o << ")\n";
    for (XlationContextStack::const_iterator it=myXlationContextStack.begin();
	 it!=myXlationContextStack.end();
	 ++it) 
      it->dump(o,indent+"  ");
  }

  void PUXlationContext::ddump() const {
    dump(std::cerr,"");
  }
  
  bool PUXlationContext::isF90() const { 
    return myF90Flag;
  }

  void PUXlationContext::setF90(bool aFlag) { 
    myF90Flag=aFlag;
  }

  Open64IRInterface& PUXlationContext::getIrInterface() { 
    return myIrInterface;
  } 

} // end namespace whirl2xaif

