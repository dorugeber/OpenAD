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
#include "xaifBooster/system/inc/InlinableIntrinsicsConstant.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

namespace xaifBooster { 

  const std::string InlinableIntrinsicsConstant::ourXAIFName("xaif:Constant");
  const std::string InlinableIntrinsicsConstant::our_myType_XAIFName("type");
  const std::string InlinableIntrinsicsConstant::our_myValue_XAIFName("value");
  const std::string InlinableIntrinsicsConstant::our_myId_XAIFName("vertex_id");

  ExpressionVertex& 
  InlinableIntrinsicsConstant::createCopyOfMyself() const {
    Constant* aNewConstant_p=new Constant(myType);
    switch(myType) { 
    case SymbolType::INTEGER_STYPE : 
	aNewConstant_p->setint(getint());
	break;
    case SymbolType::REAL_STYPE : 
	aNewConstant_p->setdouble(getdouble());
	break;
    default: 
      THROW_LOGICEXCEPTION_MACRO("InlinableIntrinsicsConstant::createCopyOfMyself: cannot handle " <<
				 SymbolType::toString(myType).c_str());
      break;
    } // end switch
    return *aNewConstant_p;
  } 

} 
