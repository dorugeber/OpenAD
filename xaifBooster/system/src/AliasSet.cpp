// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/AliasSet.hpp"
#include "xaifBooster/system/inc/AliasRange.hpp"

namespace xaifBooster { 

  AliasSet::AliasSet() { 
  }

  AliasSet::~AliasSet() { 
    for (AliasRangePList::const_iterator it=myAliasRangePList.begin();
	 it==myAliasRangePList.end();
	 ++it)
      delete (*it);       
  }

  std::string AliasSet::debug() const {
    std::ostringstream out;
    out << "AliasSet[" << this 
	<< "]" << std::ends; 
    return out.str();
  } // end of AliasSet::debug
  
  void AliasSet::printXMLHierarchy(std::ostream& os) const {
    for (AliasRangePList::const_iterator it=myAliasRangePList.begin();
	 it!=myAliasRangePList.end();
	 ++it)
      (*it)->printXMLHierarchy(os);       
  } 

  void AliasSet::addAlias(unsigned int lower,
			  unsigned int upper,
			  bool isPartial) { 
    if (myAliasRangePList.empty()) { 
      myAliasRangePList.push_back(new AliasRange(lower,upper,isPartial));
      return;
    }
    AliasRange theNewRange(lower,upper,isPartial);
    AliasRangePList::iterator it=myAliasRangePList.end();
    --it; // start at the end and point to the last
    while (true) { 
      if (theNewRange.sameAs(**it))
	return;
      if (theNewRange.isGreaterThan(**it)) { 
	// the first one for which it is greater
	if (theNewRange.overlapsWith(**it)
	    || 
	    theNewRange.isContainedIn(**it)) { 
	  if (!(*it)->isPartial() 
	      ||
	      !theNewRange.isPartial())
	    THROW_LOGICEXCEPTION_MACRO("AliasSet::addAlias: overlapping full reference ranges, fix the code");
	  (*it)->absorb(theNewRange);
	  return;
	}
	else if (theNewRange.bordersWith(**it) 
		 &&
		 theNewRange.isPartial() 
		 &&
		 (*it)->isPartial()) { 
	  (*it)->absorb(theNewRange);
	  return;
	}
	else { 
	  // increment iterator by one
	  ++it; 
	  // and insert before it
	  myAliasRangePList.insert(it,new AliasRange(lower,upper,isPartial)); 
	  return;
	}
      }
      else {
	// *theNewRange must be less than **it
	if (it==myAliasRangePList.begin()) { 
	  myAliasRangePList.insert(it,new AliasRange(lower,upper,isPartial)); 
	  // insert before it
	  return;
	}
	--it;
      }
    } 
    // if we haven't returned yet then we are in trouble
    THROW_LOGICEXCEPTION_MACRO("AliasSet::addAlias: problem with insertion");
  } 

  bool AliasSet::sharesAliasWith(const AliasSet& anotherSet) const {
    if (myAliasRangePList.empty() || anotherSet.myAliasRangePList.empty()) { 
      return false;
    }
    AliasRangePList::const_iterator myI=myAliasRangePList.begin();
    AliasRangePList::const_iterator theOtherI=anotherSet.myAliasRangePList.begin();
    while (myI!=myAliasRangePList.end()
	   &&
	   theOtherI!=anotherSet.myAliasRangePList.end()) { 
      // both sets are ordered
      if ((*myI)->isGreaterThan(**theOtherI)) { 
	++theOtherI; 
      }
      else if ((*myI)->overlapsWith(**theOtherI)
	       || 
	       (*myI)->isContainedIn(**theOtherI)
	       ||
	       (*theOtherI)->isContainedIn(**myI)) { 
	return true;
      }
      else  // myI must be less than theOtherI
	++myI;
    }
    return false;
  } 

  bool AliasSet::mustAlias(const AliasSet& anotherSet) const {
    if (myAliasRangePList.size()!=1 
	|| 
	anotherSet.myAliasRangePList.size()!=1) { 
      return false;
    }
    AliasRangePList::const_iterator myI=myAliasRangePList.begin();
    AliasRangePList::const_iterator theOtherI=anotherSet.myAliasRangePList.begin();
    if ((*myI)->isContainedIn(**theOtherI)
	&&
	(*theOtherI)->isContainedIn(**myI)) { 
      return true;
    }
    return false;
  } 
  
  bool AliasSet::subSetOf(const AliasSet& anotherSet) const { 
    // some obvious things first:
    if (myAliasRangePList.empty()) { 
      return true;
    }
    if (anotherSet.myAliasRangePList.empty() 
	&& 
	!myAliasRangePList.empty()) { 
      return false;
    }
    // go through both sets:
    AliasRangePList::const_iterator myI=myAliasRangePList.begin();
    AliasRangePList::const_iterator theOtherI=anotherSet.myAliasRangePList.begin();
    while (myI!=myAliasRangePList.end()
	   &&
	   theOtherI!=anotherSet.myAliasRangePList.end()) { 
      // both sets are ordered
      if ((*myI)->isGreaterThan(**theOtherI)) { 
	++theOtherI; 
      }
      else { 
	if ((*myI)->isContainedIn(**theOtherI)){ 
	  ++myI;
	} 
	else { 
	  return false;
	}
      } 
    }
    return (myI==myAliasRangePList.end());
  }

  const AliasSet::AliasRangePList& AliasSet::getAliasRangePList() const { 
    return myAliasRangePList;
  } 


} // end of namespace
