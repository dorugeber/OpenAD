// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/PlainBasicBlock.hpp"
#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/Scope.hpp"


namespace xaifBooster { 

  const std::string PlainBasicBlock::ourXAIFName("xaif:BasicBlock");
  const std::string PlainBasicBlock::our_myId_XAIFName("vertex_id");
  const std::string PlainBasicBlock::our_myScopeId_XAIFName("scope_id");

  PlainBasicBlock::PlainBasicBlock(Scope& theScope) : 
    myScope_r(theScope) { 
  } 

  PlainBasicBlock::~PlainBasicBlock() {
    for (BasicBlockElementList::const_iterator li=myElementList.begin();
         li!=myElementList.end();
         li++) { 
      if (*li)
	delete *li;
    } 
  }

  void
  PlainBasicBlock::supplyAndAddBasicBlockElementInstance(BasicBlockElement& theBasicBlockElement) { 
    myElementList.push_back(&theBasicBlockElement);
  } // end of PlainBasicBlock::supplyAndAddBasicBlockElementInstance

  void
  PlainBasicBlock::supplyAndAddBasicBlockElementInstanceAtBeginning(BasicBlockElement& theBasicBlockElement) { 
    myElementList.push_front(&theBasicBlockElement);
  } // end of PlainBasicBlock::supplyAndAddBasicBlockElementInstanceAtBeginning

  std::string PlainBasicBlock::debug () const { 
    std::ostringstream out;
    out << "PlainBasicBlock[" << this 
	<< ",myElementList[#items=" << myElementList.size() << "]"
	<< "]" << std::ends;  
    return out.str();
  } // end of PlainBasicBlock::debug

  const PlainBasicBlock::BasicBlockElementList& PlainBasicBlock::getBasicBlockElementList() const { 
    return myElementList;
  } 

  PlainBasicBlock::BasicBlockElementList& PlainBasicBlock::getBasicBlockElementList() { 
    return myElementList;
  } 

  Scope& PlainBasicBlock::getScope() const { 
    return myScope_r;
  } 

  bool
  PlainBasicBlock::hasExpression(const Expression& anExpression) const {
    for (PlainBasicBlock::BasicBlockElementList::const_iterator li = myElementList.begin();
         li != myElementList.end(); ++li)
      if ((*li)->hasExpression(anExpression))
        return true;
    return false;
  } // end PlainBasicBlock::hasExpression()
 
} // end of namespace xaifBooster 
