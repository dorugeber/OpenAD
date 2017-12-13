// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/xaif2whirl/PUXlationContext.cxx,v 1.4 2008-02-21 14:32:05 utke Exp $

#include <stdio.h>

#include "PUXlationContext.h"
#include "Open64IRInterface/SymTab.h"
#include "Diagnostics.h"

namespace xaif2whirl { 

  std::string PUXlationContext::ourPrefix;

  PUXlationContext::PUXlationContext(const std::string& anOriginator, PU_Info* pu_forest) : 
    mySymTabIdToSymTabMap(pu_forest), 
    myPUIdToPUMap(pu_forest), 
    myWNParentMapP(NULL), 
    myWNToWNIdMapP(NULL),
    myWNIdToWNMapP(NULL),
    //myXAIFSymToSymbolMapP(NULL),
    myOriginator(anOriginator) {
    myXlationContextStack.push_front(XlationContext());
  }

  PUXlationContext::~PUXlationContext() {
    // clear the stack
    myXlationContextStack.clear(); 
  }

  void PUXlationContext::createXlationContext() {
    PushNewXlationContext(XlationContext::NOFLAG);
  }

  void PUXlationContext::createXlationContext(XlationContext::Flags_E f) {
    PushNewXlationContext(f);
  }

  void PUXlationContext::PushNewXlationContext(XlationContext::Flags_E f) {
    if (myXlationContextStack.empty()) 
      // this should never happen
      FORTTK_DIE("PUXlationContext::PushNewXlationContext: empty context stack");
    const XlationContext& parentXlationContext = myXlationContextStack.front();
    myXlationContextStack.push_front(XlationContext());
    currentXlationContext().inheritFlagsDown(parentXlationContext);
    currentXlationContext().setFlag(f);
  }

  void PUXlationContext::deleteXlationContext() {
    if (myXlationContextStack.size() > 1) {
      // maintain invariant that there is at least one context
      (++(myXlationContextStack.begin()))->inheritFlagsUp(*(myXlationContextStack.begin()));
      myXlationContextStack.pop_front();
    }
  }

  XlationContext& PUXlationContext::currentXlationContext() { 
    return myXlationContextStack.front(); 
  }

  WN* PUXlationContext::findParentWN(WN* wn) {
    if (!myWNParentMapP)
      FORTTK_DIE("PUXlationContext::FindParentWN: myWNParentMapP not set");
    if(!wn)
      FORTTK_DIE("PUXlationContext::FindParentWN: null pointer passed");
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
    // JU: this appears to be reset, HMMM
    if (myWNParentMapP) {
      if (myWNParentMapP==aWhirlParentMapP) { 
	FORTTK_MSG(2,"PUXlationContext::setWNParentMap: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setWNParentMap: already set to " 
		   << myWNParentMapP 
		   << " new " 
		   << aWhirlParentMapP);
      }
    }
    myWNParentMapP = aWhirlParentMapP; 
  }

  std::pair<ST_TAB*, PU_Info*>  PUXlationContext::findSymTab(fortTkSupport::SymTabId stabId) {
    return (mySymTabIdToSymTabMap.Find(stabId, true /*mustfind*/)); 
  }

  PU_Info* PUXlationContext::findPU(fortTkSupport::PUId aPUId) {
    return (myPUIdToPUMap.Find(aPUId));
  }

  fortTkSupport::WNId PUXlationContext::findWNId(WN* aWNp) {
    if (!myWNToWNIdMapP)
      FORTTK_DIE("PUXlationContext::findWNId: myWNToWNIdMapP not set");
    if (!aWNp)
      FORTTK_DIE("PUXlationContext::findWNId: null pointer passed");
    return myWNToWNIdMapP->Find(aWNp);
  }

  fortTkSupport::WNToWNIdMap* PUXlationContext::getWNToWNIdMap() const { 
    if (!myWNToWNIdMapP)
      FORTTK_DIE("PUXlationContext::getWNToWNIdMap: myWNToWNIdMapP not set");
    return myWNToWNIdMapP; 
  }

  void PUXlationContext::setWNToWNIdMap(fortTkSupport::WNToWNIdMap* aWNToWNIdMapP) { 
    if (!aWNToWNIdMapP)
      FORTTK_DIE("PUXlationContext::setWNToWNIdMap: null pointer passed");
    // JU: this is being reset hmm
    if (myWNToWNIdMapP) {
      if (myWNToWNIdMapP==aWNToWNIdMapP) { 
	FORTTK_MSG(2,"PUXlationContext::setWNToWNIdMap: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setWNToWNIdMap: already set to " 
		   << myWNToWNIdMapP  
		   << " new " 
		   << aWNToWNIdMapP);
      }
    }
    myWNToWNIdMapP = aWNToWNIdMapP; 
  }

  WN* PUXlationContext::findWN(fortTkSupport::WNId aWNId, bool mustFind) {
    if (!myWNIdToWNMapP)
      FORTTK_DIE("PUXlationContext::findWN: myWNIdToWNMapP not set");
    return myWNIdToWNMapP->Find(aWNId,mustFind);
  }

  fortTkSupport::WNIdToWNMap* PUXlationContext::getWNIdToWNMap() const { 
    if (!myWNIdToWNMapP)
      FORTTK_DIE("PUXlationContext::getWNIdToWNMap: myWNIdToWNMapP not set");
    return myWNIdToWNMapP; 
  }

  void PUXlationContext::setWNIdToWNMap(fortTkSupport::WNIdToWNMap* aWNIdToWNMapP) { 
    if (!aWNIdToWNMapP)
      FORTTK_DIE("PUXlationContext::setWNIdToWNMap: null pointer passed");
    // JU: this is being reset hmm
    if (myWNIdToWNMapP) {
      if (myWNIdToWNMapP==aWNIdToWNMapP) { 
	FORTTK_MSG(2,"PUXlationContext::setWNIdToWNMap: already set to the same");
      }
      else { 
	FORTTK_MSG(2,"PUXlationContext::setWNIdToWNMap: already set to " 
		   << myWNIdToWNMapP  
		   << " new " 
		   << aWNIdToWNMapP);
      }
    }
    myWNIdToWNMapP = aWNIdToWNMapP; 
  }

  fortTkSupport::Symbol* PUXlationContext::findSym(const std::string& scopeid, 
				    const std::string& symid) { 
    return (myXAIFSymToSymbolMap.Find(scopeid.c_str(), symid.c_str())); 
  }

  fortTkSupport::XAIFSymToSymbolMap& PUXlationContext::getXAIFSymToSymbolMap() {
    return myXAIFSymToSymbolMap;
  } // end PUXlationContext::getXAIFSymToSymbolMap()

  void PUXlationContext::setPrefix(const std::string& aPrefix) { 
    ourPrefix=aPrefix;
  }

  const std::string& PUXlationContext::getPrefix() {
    return ourPrefix;
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

}
