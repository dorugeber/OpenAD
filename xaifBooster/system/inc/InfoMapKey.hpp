#ifndef _INFOMAPKEY_INCLUDE_
#define _INFOMAPKEY_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp"

namespace xaifBooster { 

  /** 
   * class to represent generic key 
   * into some map also covering 
   * special cases which don't have an instance
   * in the map
   */
  class InfoMapKey : public Debuggable { 

  public:
    
    InfoMapKey();

    ~InfoMapKey() {};

    /** 
     * indicate there is a regular key
     * or indicate there is no information, i.e. key  is 0
     */
    void setReference(unsigned int key);

    /** 
     * use for internally created temporaries
     * which require special handling
     */
    void setTemporary();

    enum InfoMapKey_E {NOT_SET,   // not initialized
		       SET,       // has regular key
		       NO_INFO,   // has 0 key
		       TEMP_VAR}; // temporary
    /**
     * get the key, if no regular key throws exception
     */
    unsigned int getKey() const;

    /**
     * return the kind of key
     */
    InfoMapKey_E getKind() const;

    std::string debug() const ;
    
  private:

    /**
     * key into map
     * initialized to invalid value 0
     */
    unsigned int myKey;

    /**
     * key kind initialized to NOT_SET
     */
    InfoMapKey_E myKind;

  }; // end of class InfoMapKey

} // end of namespace xaifBooster
                                                                     
#endif
