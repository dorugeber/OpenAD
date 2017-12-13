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
#include "xaifBooster/system/inc/InfoMapKey.hpp"

namespace xaifBooster { 

  InfoMapKey::InfoMapKey() : 
    myKey(0),
    myKind(NOT_SET) { 
  }

  void InfoMapKey::setReference(unsigned int key) {
    if (myKind!=NOT_SET) 
      THROW_LOGICEXCEPTION_MACRO("InfoMapKey::setReference: already set");
    if (key==0) 
      myKind=NO_INFO;
    else
      myKind=SET;
    myKey=key;
  }

  void InfoMapKey::setTemporary() {
    if (myKind!=NOT_SET) 
      THROW_LOGICEXCEPTION_MACRO("InfoMapKey::setTemporary: already set");
    myKind=TEMP_VAR;
  }

  unsigned int InfoMapKey::getKey() const { 
    if (myKind!=SET) 
      THROW_LOGICEXCEPTION_MACRO("InfoMapKey::getKey: not a regular key");
    return myKey;
  }

  InfoMapKey::InfoMapKey_E 
  InfoMapKey::getKind() const { 
    return myKind;
  }

  std::string 
  InfoMapKey::debug() const { 
    std::ostringstream out;
    out << "InfoMapKey[" << this 
	<< ",myKey=" << myKey
	<< ",myKind=" << myKind
	<< "]" << std::ends;  
    return out.str();
  } 

} // end of namespace xaifBooster
