// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "sstream"
#include "xaifBooster/system/inc/PositionSet.hpp"

namespace xaifBooster { 

  bool PositionSet::has(unsigned int aPosition) const {
    for (IntList::const_iterator i=myIntList.begin();
	 i!=myIntList.end() && *i<=aPosition;
	 ++i) 
      if (*i==aPosition)
	return true;
    return false;
  } 
  
  void PositionSet::add(const PositionSet& aSet) {
    IntList::iterator thisListI=myIntList.begin();
    IntList::const_iterator otherListI=aSet.myIntList.begin();
    while (otherListI!=aSet.myIntList.end()) { 
      if (thisListI!=myIntList.end()) { 
	if (*thisListI<*otherListI) { 
	  ++thisListI;
	} 
	else if (*thisListI==*otherListI) { 
	  ++otherListI;
	  ++thisListI;
	}
	else {  // is greater  
	  myIntList.insert(thisListI,*otherListI);
	  ++otherListI;
	} // end else 
      } // end if 
      else{  // we point to the end of this list
	myIntList.insert(thisListI,*otherListI);
	++otherListI;
      } // end else 
    } // end while
  } // end PositionSet::add

  void PositionSet::add(unsigned int aPosition) {
    IntList::iterator thisListI=myIntList.begin();
    while (thisListI!=myIntList.end()) { 
      if (*thisListI<aPosition) { 
	  ++thisListI;
      } 
      else if (*thisListI==aPosition) { 
	return;
      }
      else {  // is greater  
	myIntList.insert(thisListI,aPosition);
	return;
      } // end else 
    } // end while
    myIntList.insert(thisListI,aPosition);
  } // end PositionSet::add

  std::string PositionSet::debug () const { 
    std::ostringstream out;
    out << "PositionSet[" << this 
	<< ",{"; 
    for (IntList::const_iterator i=myIntList.begin();
	 i!=myIntList.end();
	 ++i) 
      out << *i << " ";
    out << "}]" << std::ends;  
    return out.str();
  } // end of PositionSet::debug

} 
