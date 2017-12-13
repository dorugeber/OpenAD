#ifndef _XAIFBOOSTERTYPECHANGE_SIGNATUREPATTERN_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_SIGNATUREPATTERN_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <bitset>

using namespace xaifBooster; 

namespace xaifBoosterTypeChange { 

  class SignaturePattern { 

  public: 

    static const unsigned short ourMaxParameterCount=128; 

    SignaturePattern();

    /** 
     * track an argument at aPosition
     * if size is not set or aPosition is 
     * not in [1,mySize]
     * an exception is thrown. 
     */
    void trackAt(unsigned short aPosition);

    /** 
     * query if an argument at aPosition
     * is tracked;
     * if size is not set or aPosition is 
     * not in [1,mySize]
     * an exception is thrown. 
     */
    bool isTracked(unsigned short aPosition) const;

    void setSize(unsigned short aSize);

    bool initialized() const; 

    unsigned short getSize()const;

    bool operator== (const SignaturePattern& anotherPattern) const;

    bool operator!= (const SignaturePattern& anotherPattern) const;

    std::string discrepancyPositions(const SignaturePattern& anotherPattern) const;

  private:
    
    short mySize;

    std::bitset<ourMaxParameterCount> myPattern;
    
  }; 

}

#endif
