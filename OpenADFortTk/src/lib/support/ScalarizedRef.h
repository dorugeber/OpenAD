#ifndef ScalarizedRef_H 
#define ScalarizedRef_H

#include "Open64IRInterface/Open64BasicTypes.h"

namespace fortTkSupport { 

  /** 
   * this represents a scalarized reference
   * i.e. a dummy variable 
   * that represents a (chain of) dereference
   */
  class ScalarizedRef {
  
  public:
    /** 
     * there may be no postfix string 
     */
    ScalarizedRef(WN* const aWNp);

    ScalarizedRef(WN* const aWNp, 
		  const std::string& aPostFix);

    virtual ~ScalarizedRef();
  
    const std::string& getName() const;

    WN* getWN() const;
  
    unsigned int getId() const;

    void dump(std::ostream& o) const; 

    void ddump() const; 

    /** 
     * Returns whether 'wn' is a variable
     * reference that can be translated into XAIF.  'wn' must be an
     * expression; IOW it cannot be a store (=statement).
     */
    static bool isRefTranslatableToXAIF(const WN* wn);

    /**
     * WHIRL stores represent the lhs var-ref (e.g. OPR_ISTORE,OPR_STID)
     */
    static bool isRefSimple(const WN* wn);
    static bool isRefSimpleScalar(const WN* wn);
    static bool isRefSimpleArrayElem(const WN* wn);
    static bool isRefSimpleArray(const WN* wn);
    static bool isRefScalarizable(const WN* wn);
    static bool isRefScalar(TY_IDX baseobj_ty, 
			    TY_IDX refobj_ty);

  private:

    /** 
     * no def
     */
    ScalarizedRef(const ScalarizedRef& x);

    /** 
     * no def
     */
    ScalarizedRef& operator=(const ScalarizedRef& x);
  
    void Ctor(WN* wn, const char* x);
  
    unsigned int myId; 

    std::string myName;

    /** 
     * the first occurance of the reference in PU
     */
    WN* myWNp; 
  
    /** 
     * to ensure global uniqueness
     */
    static unsigned int ourNextId; 

    static const std::string ourPrefix;

  };

}

#endif
