#include <iostream>
#include <sstream>

#include "ScalarizedRef.h"
#include "Diagnostics.h"

#include "Open64IRInterface/stab_attr.h"
#include "Open64IRInterface/wn_attr.h"

namespace fortTkSupport { 

  unsigned int ScalarizedRef::ourNextId = 0; 
  const std::string ScalarizedRef::ourPrefix("oaScalRef");

  ScalarizedRef::ScalarizedRef(WN* const aWNp) : 
    myId(ourNextId++), 
    myWNp(aWNp) { 
    std::ostringstream theName;
    theName << ourPrefix.c_str() << myId << std::ends; 
    myName=theName.str();
  } 

  ScalarizedRef::ScalarizedRef(WN* const aWNp, 
			       const std::string& aPostFix) : 
    myId(ourNextId++), 
    myWNp(aWNp) { 
    std::ostringstream theName;
    theName << ourPrefix.c_str() << myId << aPostFix.c_str() << std::ends; 
    myName=theName.str();
  } 

  ScalarizedRef::~ScalarizedRef() {
  }

  const std::string& ScalarizedRef::getName()const { 
    return myName; 
  }

  WN* ScalarizedRef::getWN() const { 
    return myWNp; 
  }

  unsigned int ScalarizedRef::getId() const { 
    return myId; 
  }

  void ScalarizedRef::dump(std::ostream& o) const { 
    o << myName; 
  } 
  
  void ScalarizedRef::ddump() const { 
    dump(std::cerr);
  } 

  bool ScalarizedRef::isRefTranslatableToXAIF(const WN* aWNp) {
    return (isRefSimple(aWNp) || isRefScalarizable(aWNp));
  }

  bool ScalarizedRef::isRefSimple(const WN* aWNp) {
    return (isRefSimpleScalar(aWNp) 
	    || 
	    isRefSimpleArrayElem(aWNp) 
	    || 
	    isRefSimpleArray(aWNp));
  }

  bool ScalarizedRef::isRefSimpleScalar(const WN* aWNp) {
    OPERATOR opr = WN_operator(aWNp);
    switch (opr) {
    case OPR_LDA:
    case OPR_LDMA:
    case OPR_LDID:
    case OPR_LDBITS: 
    case OPR_STID:
    case OPR_STBITS: 
    case OPR_ILOAD: 
    case OPR_ILDBITS: {
      // (for stores, only check LHS (kid1))
      TY_IDX baseobj_ty = WN_GetBaseObjType(aWNp);
      TY_IDX refobj_ty = WN_GetRefObjType(aWNp);
      if (opr == OPR_LDA || opr == OPR_LDMA) {
	// dereference the pointer (assume Fortran)
	refobj_ty = TY_pointed(refobj_ty);
      }
      return (isRefScalar(baseobj_ty, refobj_ty));
    }
    default: 
      break; // fall through
    } // switch
    return false;
  }

  bool ScalarizedRef::isRefSimpleArrayElem(const WN* aWNp) {
    OPERATOR opr = WN_operator(aWNp);
    switch (opr) {
    case OPR_LDA:  
    case OPR_LDMA: 
    case OPR_LDID: // probably never used for this
    case OPR_STID: // probably never used for this
    case OPR_ILOAD:
    case OPR_ISTORE: {
      // yes if baseobj_ty is of type array and reference is non-scalar
      // (for stores, only check LHS (kid1))
      TY_IDX baseobj_ty = WN_GetBaseObjType(aWNp);
      TY_IDX refobj_ty = WN_GetRefObjType(aWNp);
      return (TY_Is_Array(baseobj_ty) && !isRefScalar(baseobj_ty, refobj_ty));
    }
    case OPR_ARRAY:
      return true;
    default: 
      break; // fall through
    } // switch
    return false;
  }

  bool ScalarizedRef::isRefSimpleArray(const WN* aWNp) {
    OPERATOR opr = WN_operator(aWNp);
    switch (opr) {
       case OPR_LDA:  
    case OPR_LDMA: 
    case OPR_LDID: {
      // yes if refobj_ty is of type array
      TY_IDX baseobj_ty = WN_GetBaseObjType(aWNp);
      TY_IDX refobj_ty = WN_GetRefObjType(aWNp);
      return (TY_Is_Array(refobj_ty));
    }
    case OPR_ARRSECTION: // FIXME: can we do arrsection?
    case OPR_ARRAYEXP:
      return true;
    default: 
      break; // fall through
    } // switch
    return false;
  }

  bool ScalarizedRef::isRefScalarizable(const WN* aWNp) {
    OPERATOR opr = WN_operator(aWNp);
    switch (opr) {
    case OPR_LDA: 
    case OPR_LDMA: 
    case OPR_LDID: 
    case OPR_LDBITS: 
    case OPR_STID: 
    case OPR_STBITS: 
    case OPR_ILOAD: 
    case OPR_ISTORE: {
      FORTTK_MSG(1, "ScalarizedRef::isRefScalarizable(" << OPERATOR_name(opr) << ":" << aWNp  << ")");
      // yes if refobj_ty is scalar and reference is non-scalar
      // (for stores, only check LHS (kid1))
      TY_IDX baseobj_ty = WN_GetBaseObjType(aWNp);
      TY_IDX refobj_ty = WN_GetRefObjType(aWNp);
      // Special cases:
      // 1) passing a whole array looks like a scalarizable reference:
      //  call foo(x) ! where x is an array
      //    (ILOAD F8 F8 (0 0 (ty ".predef_F8" 11 8) (ty pointer-to-F8))
      //      (LDID U8 U8 ((st "X" 2 1) 0 (ty pointer-to-array) 0))))
      //  call foo(px) ! where px is an array
      //    (ILOAD F8 F8 (0 0 (ty ".predef_F8" 11 8) (ty pointer-to-F8))
      //      (LDA U8 V ((st "PX" 2 4) 0 (ty pointer-to-array) 0))))
      if (opr == OPR_ILOAD && 
	  (WN_operator(WN_kid0(aWNp)) == OPR_LDID || 
	   WN_operator(WN_kid0(aWNp)) == OPR_LDA)) {
	TY_IDX ty = WN_ty(WN_kid0(aWNp));
	if (TY_Is_Pointer(ty) && TY_Is_Array(TY_pointed(ty))) {
	  FORTTK_MSG(1, "ScalarizedRef::isRefScalarizable returns false");
	  return false;
	}
      } 
      FORTTK_MSG(1, "ScalarizedRef::isRefScalarizable returns " << (TY_Is_Scalar(refobj_ty) && !isRefScalar(baseobj_ty, refobj_ty)));
      return (TY_Is_Scalar(refobj_ty) && !isRefScalar(baseobj_ty, refobj_ty));
    }
    case OPR_STRCTFLD: {
      FORTTK_MSG(1, "ScalarizedRef::isRefScalarizable(" << OPERATOR_name(opr) << ":" << aWNp  << ")");
      // yes if refobj_ty is scalar and reference is non-scalar
      // (for stores, only check LHS (kid1))
      TY_IDX baseobj_ty = WN_GetBaseObjType(aWNp);
      TY_IDX refobj_ty = WN_GetRefObjType(aWNp);
      FORTTK_MSG(1, "ScalarizedRef::isRefScalarizable returns TY_Is_Scalar(" << TY_IDX_index(refobj_ty) << ")=" <<  TY_Is_Scalar(refobj_ty) << " && " << (!isRefScalar(baseobj_ty, refobj_ty)) );
      return true;
    }
    // FIXME: ILOADX, ISTOREX  /  ILDBITS, ISTBITS
    default: 
      break; // fall through
    } // switch
    return false;
  }

  bool ScalarizedRef::isRefScalar(TY_IDX baseobj_ty, 
				  TY_IDX refobj_ty) {
    if (TY_IsNonScalar(refobj_ty)) {
      // This is a reference to a non-scalar or a non-scalar within a
      // non-scalar (e.g. a record or a record within a record)
      return false; 
    } 
    else if (TY_Is_Scalar(refobj_ty)) {
      // Test whether 'baseobj_ty' is assignable to 'refobj_ty'.  If
      // not, we have a non-scalar reference (e.g. a field within a
      // structure; an element within an array).
      return (WN2F_Can_Assign_Types(baseobj_ty, refobj_ty));
    } 
    else {
      return false;
    }
  }

}
