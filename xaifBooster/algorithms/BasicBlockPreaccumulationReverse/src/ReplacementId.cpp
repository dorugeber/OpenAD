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
#include "xaifBooster/algorithms/BasicBlockPreaccumulationReverse/inc/ReplacementId.hpp"

namespace xaifBoosterBasicBlockPreaccumulationReverse { 

  ReplacementId::ReplacementId() : myCurrent(ORIGINAL) { 
  }

  std::string 
  ReplacementId::toString(const ReplacementId_E& aKind)
    throw (PrintingIntException) { 
    std::string returnString;
    switch(aKind) {
    case ORIGINAL:
      returnString="ORIGINAL";
      break;
    case TAPING: 
      returnString="TAPING";
      break;
    case ADJOINT: 
      returnString="ADJOINT";
      break;
    case STOREARGUMENT: 
      returnString="STOREARGUMENT";
      break;
    case STORERESULT: 
      returnString="STORERESULT";
      break;
    case RESTOREARGUMENT: 
      returnString="RESTOREARGUMENT";
      break;
    case RESTORERESULT: 
      returnString="RESTORERESULT";
      break;
    case STORETIMESTEPARGUMENT: 
      returnString="STORETIMESTEPARGUMENT";
      break;
    case RESTORETIMESTEPARGUMENT: 
      returnString="RESTORETIMESTEPARGUMENT";
      break;
    case STRICTANONYMOUSTAPING: 
      returnString="STRICTANONYMOUSTAPING";
      break;
    case STRICTANONYMOUSADJOINT: 
      returnString="STRICTANONYMOUSADJOINT";
      break;
    case STORETIMESTEPRESULT:
      returnString="STORETIMESTEPRESULT";
      break;
    case RESTORETIMESTEPRESULT:
      returnString="RESTORETIMESTEPRESULT";
      break;
    default: 
      throw PrintingIntException("ReplacementId::toString: unknown value",aKind);
      break;
    } // end switch
    return returnString;
  } // end of std::string ReplacementId::toString

  const ReplacementId::ReplacementId_E
  ReplacementId::fromString(const std::string& aName) { 
    ReplacementId_E returnValue;
    if (aName=="ORIGINAL")
      returnValue=ORIGINAL;
    else if (aName=="TAPING")
      returnValue=TAPING;
    else if (aName=="ADJOINT")
      returnValue=ADJOINT;
    else if (aName=="STOREARGUMENT")
      returnValue=STOREARGUMENT;
    else if (aName=="STORERESULT")
      returnValue=STORERESULT;
    else if (aName=="RESTOREARGUMENT")
      returnValue=RESTOREARGUMENT;
    else if (aName=="RESTORERESULT")
      returnValue=RESTORERESULT;
    else if (aName=="STORETIMESTEPARGUMENT")
      returnValue=STORETIMESTEPARGUMENT;
    else if (aName=="RESTORETIMESTEPARGUMENT")
      returnValue=RESTORETIMESTEPARGUMENT;
    else if (aName=="STRICTANONYMOUSTAPING")
      returnValue=STRICTANONYMOUSTAPING;
    else if (aName=="STRICTANONYMOUSADJOINT")
      returnValue=STRICTANONYMOUSADJOINT;
    else if (aName=="STORETIMESTEPRESULT")
      returnValue=STORETIMESTEPARGUMENT;
    else if (aName=="RESTORETIMESTEPRESULT")
      returnValue=RESTORETIMESTEPARGUMENT;
    else  
      THROW_LOGICEXCEPTION_MACRO("ReplacementId::fromString: unknown value >"
			   << aName.c_str() << "<");
    return returnValue;
  } // end of std::string ReplacementId::fromString
  
  void ReplacementId::reset() { 
    myCurrent=ORIGINAL;
  } 

  bool ReplacementId::atEnd() {
    return (myCurrent>RESTORETIMESTEPRESULT);
  } 

  ReplacementId::ReplacementId_E ReplacementId::operator*() const { 
    return myCurrent;
  } 

  ReplacementId& ReplacementId::operator++() { 
    myCurrent=(ReplacementId::ReplacementId_E)(((int)myCurrent)+1);
    return *this;
  } 

} // end of namespace
