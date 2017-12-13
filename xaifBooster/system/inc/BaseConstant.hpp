#ifndef _BASECONSTANT_INCLUDE_
#define _BASECONSTANT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/FrontEndType.hpp"
#include "xaifBooster/system/inc/SymbolType.hpp"
#include "xaifBooster/utils/inc/Debuggable.hpp"

namespace xaifBooster { 

  /**
   * \todo cannot handle COMPLEX
   */
  class BaseConstant: public Debuggable {
  public:

    BaseConstant(const SymbolType::SymbolType_E aType) : 
      myType(aType) {};

    ~BaseConstant(){};

    virtual std::string debug() const;

    double getdouble()const;
    int getint()const;
    bool getbool()const;
    char getchar()const;
    void setdouble(double);
    void setint(int);
    void setbool(bool);
    void setchar(char);
    void setFromString(const std::string& aValue);

    SymbolType::SymbolType_E getType() const;

    std::string toString() const; 

    const FrontEndType& getFrontEndType() const;

    void setFrontEndType(const FrontEndType&);

  protected: 

    const SymbolType::SymbolType_E myType;

    FrontEndType myFrontEndType;

  private : 

    BaseConstant ();

    /**
     * obviously we don't need 
     * anything special for 
     * string constants
     */
    union myValueUnion { 
      double f;
      int i;
      bool b;
      char c;
    };

    myValueUnion myValue;

    /** 
     * the original string if set from string
     */
    std::string myOriginalStringRep;

    static const std::string ourBooleanTrueString;
    static const std::string ourBooleanFalseString;

  }; // end of class BaseConstant
 
} // end of namespace xaifBooster
                                                                     
#endif
