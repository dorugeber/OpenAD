#include "IntrinsicInfo.h"
#include "IFDiagnostics.h"
#include "wn_attr.h"

IntrinsicInfo::IntrinsicMap IntrinsicInfo::ourIntrinsicInfoMap;
bool IntrinsicInfo::ourInitFlag=false;

const IntrinsicInfo::IntrinsicMap& IntrinsicInfo::getMap() { 
  if (!ourInitFlag) { 
    // Common mathematical functions
    ourIntrinsicInfoMap[Key( OPR_NEG,           NULL)         ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_ADD,           NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_SUB,           NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_MPY,           NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_DIV,           NULL)         ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SQRT")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DSQRT")      ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_SQRT,          NULL)         ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SUM")        ]=Info(false,1,ARRAY_INTR); 
    // modulo/remainder
    ourIntrinsicInfoMap[Key( OPR_MOD,           NULL)         ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MODULO")     ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_REM,           NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "AMOD")       ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "DMOD")       ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MOD")        ]=Info(false,2,FLOAT_INTR); 
    // trigonometric
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SIN")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DSIN")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "COS")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DCOS")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "TAN")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DTAN")       ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ASIN")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ACOS")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ATAN")       ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SINH")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DSINH")      ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "COSH")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DCOSH")      ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "TANH")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DTANH")      ]=Info(false,1,FLOAT_INTR);
    // exp/log
    ourIntrinsicInfoMap[Key( OPR_CALL,          "EXP")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DEXP")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "LOG")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DLOG")       ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ALOG")       ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "LOG10")      ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "EXPEXPR")    ]=Info(false,2,FLOAT_INTR);
    // string ops
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CEQEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CNEEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CGEEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CGTEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CLEEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "CLTEXPR")    ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "LEN")        ]=Info(false,1,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "INDEX")      ]=Info(true, 3,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "LEN")        ]=Info(false,1,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "LEN_TRIM")   ]=Info(false,1,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "TRIM")       ]=Info(false,1,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SCAN")       ]=Info(false,2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_CALL,"CONCATEXPR") ]=Info(true, 2,STRING_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ICHAR")      ]=Info(false,1,STRING_INTR);
    
    // string assignment
    // String Assigment is Treated as a special case of Intrinsic functions.
    // We assume that Intrinsic Functions has no side Effects whereas 
    // CASSIGNSTMT has side effects. Therefore, we expect its behaviuor to be similar
    // to the AssignPair sattement. Therefore, Removed from list of Intrinsics.
    //ourIntrinsicInfoMap[Key( OPR_INTRINSIC_CALL,"CASSIGNSTMT")]=Info(false,1,STRING_INTR);
   
    // rounding and conversion
    ourIntrinsicInfoMap[Key( OPR_ABS,           NULL)         ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ABS")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DABS")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "IABS")       ]=Info(false,1,INTEGER_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SIGN")       ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DSIGN")      ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_RND,           NULL)         ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_TRUNC,         NULL)         ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "INT")        ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "NINT")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "TRANSFER")   ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CEIL,          NULL)         ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_FLOOR,         NULL)         ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "REAL")       ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "FLOAT")      ]=Info(false,1,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "DBLE")       ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "AIMAG")      ]=Info(false,1,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "TRANSPOSE")  ]=Info(false,1,ARRAY_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "RESHAPE")    ]=Info(false,2,ARRAY_INTR); 
    ourIntrinsicInfoMap[Key( OPR_COMPLEX,       NULL)         ]=Info(false,2,FLOAT_INTR); 
    // logical (and bitwise logical) operations 
    ourIntrinsicInfoMap[Key( OPR_BNOT,          NULL)         ]=Info(false,1,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_BAND,          NULL)         ]=Info(false,2,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_BIOR,          NULL)         ]=Info(false,2,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_BXOR,          NULL)         ]=Info(false,2,BOOL_INTR); 
    ourIntrinsicInfoMap[Key( OPR_LNOT,          NULL)         ]=Info(false,1,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_LAND,          NULL)         ]=Info(false,2,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_LIOR,          NULL)         ]=Info(false,2,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CAND,          NULL)         ]=Info(false,2,BOOL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CIOR,          NULL)         ]=Info(false,2,BOOL_INTR); 
    // comparison operations
    ourIntrinsicInfoMap[Key( OPR_EQ,            NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_NE,            NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_GT,            NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_GE,            NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_LT,            NULL)         ]=Info(false,2,FLOAT_INTR);
    ourIntrinsicInfoMap[Key( OPR_LE,            NULL)         ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ANY")        ]=Info(false,1,ARRAY_INTR);
    // max/min
    ourIntrinsicInfoMap[Key( OPR_MAX,            NULL)        ]=Info(false,2,FLOAT_INTR); 
    ourIntrinsicInfoMap[Key( OPR_MIN,            NULL)        ]=Info(false,2,FLOAT_INTR); 
    // array operations
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MAXVAL")     ]=Info(false,1,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MAXLOC")     ]=Info(false,1,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MINVAL")     ]=Info(false,1,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MINLOC")     ]=Info(false,1,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "LBOUND")     ]=Info(false,2,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "UBOUND")     ]=Info(false,2,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SIZE")       ]=Info(false,2,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "SHAPE")      ]=Info(false,1,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_INTRINSIC_OP,  "F90INDEX")   ]=Info(false,2,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "PRESENT")    ]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ASSOCIATED") ]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "ALLOCATED")  ]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_NULLIFY,       NULL)         ]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "_ALLOCATE")  ]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "_DEALLOCATE")]=Info(false,1,COMPILER_INTERNAL_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MAXVAL")     ]=Info(false,2,ARRAY_INTR);
    ourIntrinsicInfoMap[Key( OPR_CALL,          "MINVAL")     ]=Info(false,2,ARRAY_INTR);
    // shifting operations
    ourIntrinsicInfoMap[Key( OPR_SHL,            NULL)        ]=Info(false,2,INTEGER_INTR);
    ourIntrinsicInfoMap[Key( OPR_ASHR,           NULL)        ]=Info(false,2,INTEGER_INTR); 
    // compiler internal routine for cleanup at the end of PROGRAM
    ourIntrinsicInfoMap[Key( OPR_CALL,          "_END")       ]=Info(false,0,COMPILER_INTERNAL_INTR);
    ourInitFlag = true;
  } 
  return ourIntrinsicInfoMap;
} 

bool IntrinsicInfo::lookupIntrinsicInfo(const WN* aWN_p, const IntrinsicInfo::Info* anInfo) { 
  OPERATOR opr = WN_operator(aWN_p);
  IntrinsicMap::const_iterator finder;

  /*! PLM : I see Intrinsic Call apperas only for CASSIGNSTMT
            at this point. As CASSIGNSTMT is removed from the
            list of Intrinsics. I am commenting out 
            OPR_INTRINSIC_CALL test case, otherwise it gives
            and error message on encountering OPR_INTRINSIC_CALL
            for CASSIGNSTMT. 
 
  if (opr==OPR_INTRINSIC_CALL) { 
    // get the name and strip machine type information 
    const char* inm = intrinsicBaseName(WN_intrinsic(aWN_p));
    finder=getMap().find(Key(opr,inm));
    if (finder==getMap().end()) { 
      DIE("IntrinsicInfo::isIntrinsic: no entry for OPR_INTRINSIC_CALL %s ",inm);
    }
  }

  */

  if (opr==OPR_INTRINSIC_OP) { 
    // get the name and strip machine type information
    const char* inm = intrinsicBaseName(WN_intrinsic(aWN_p));
    finder=getMap().find(Key(opr,inm));
    if (finder==getMap().end()) {
      DIE("IntrinsicInfo::isIntrinsic: no entry for OPR_INTRINSIC_OP %s ",inm);
    }
  }
  else if (opr==OPR_CALL) { 
    // get the name
    ST* st = WN_st(aWN_p);
    const char* funcNm = ST_name(st);
    // if we don't find it in the table we either forgot to add an entry
    // or the call is not to an intrinsic
    finder=getMap().find(Key(opr,funcNm));
  }
  else { 
    // the rest is for the OPERATORs without discriminating name which we 
    // hopefully have complete
    finder=getMap().find(Key(opr,NULL));
  }
  if (finder!=getMap().end())
    anInfo=&((*finder).second);
  return finder!=getMap().end();
} 

bool IntrinsicInfo::isIntrinsic(const WN* aWN_p) { 
  const Info* anInfo_p;
  return lookupIntrinsicInfo(aWN_p,anInfo_p);
}

const IntrinsicInfo::Info& IntrinsicInfo::getIntrinsicInfo(WN* aWN_p) { 
  const Info* anInfo_p;
  if(!lookupIntrinsicInfo(aWN_p,anInfo_p))
    DIE("IntrinsicInfo::getIntrinsicInfo: not a known intrinsic");
  return *anInfo_p;
}

bool IntrinsicInfo::KeyLT::operator()(const IntrinsicInfo::Key& k1, const IntrinsicInfo::Key& k2) const {
  if (k1.myWNopr<k2.myWNopr)
    return true;
  else if (k1.myWNopr>k2.myWNopr)
    return false;
  else if (k1.myWNopr==OPR_CALL)
    return strcmp(k1.myName, k2.myName) < 0;
  else
    return false;
}

const char* IntrinsicInfo::intrinsicBaseName(INTRINSIC intrn) {
  // almost none of the Open64 code uses std::string
  const char* opc_str = WN_intrinsic_name(intrn); // INTRINSIC_name(intrn);
  const char* opc_str_base = opc_str;
  const int prefixLength(2);
  // Test for two-character prefixes (up to two)
  char buf[prefixLength+1];
  for (int i = 0; i < 2; ++i) {
    // If a prefix begins 'opc_str_base', shift pointer
    strncpy(buf, opc_str_base, prefixLength); 
    buf[prefixLength] = '\0';
    bool pfix = lookupIntrinsicPrefix(buf);
    if (pfix) { 
      opc_str_base += prefixLength; 
    } else {
      break; // no need to continue
    }
  }
  // Special case: test for one-character prefix
  strncpy(buf, opc_str_base, 1); 
  buf[1] = '\0';
  bool pfix = lookupIntrinsicPrefix(buf);
  if (pfix) {
    // an exception
    if ( !(strcmp(opc_str_base, "VALTMP") == 0) ) {
      opc_str_base++;
    }
  }
  return opc_str_base;
}

extern "C" int prefixTableCmp(const char** e1, const char** e2) {
  return strcmp(*e1, *e2);
}

extern "C" typedef int (*compare_fn_t)(const void *, const void *);

bool IntrinsicInfo::lookupIntrinsicPrefix(const char* str){ 
  static const char* prefixTable[] = {
    "V",                    // void
    "I1", "I2", "I4", "I8", // integer
    "U1", "U2", "U4", "U8", // unsigned integer
    "F4", "F8", "FQ",       // floating point
    "C4", "C8", "CQ"        // complex
    // "C_" - C intrinsics
    // "S_" - UPC intrinsics
  }; 
  static unsigned int prefixTableElemSz = sizeof(const char*);
  static unsigned int prefixTableSz = (sizeof(prefixTable) / sizeof(const char*));
  static bool prefixTableSorted = false;
  // first sort it 
  if (!prefixTableSorted) {
    qsort(prefixTable, 
	  prefixTableSz, 
	  prefixTableElemSz, 
	  (compare_fn_t)prefixTableCmp);
    prefixTableSorted = true;
  }
  // Search for entry str
  void* e = bsearch(&str, 
		    prefixTable, 
		    prefixTableSz, 
		    prefixTableElemSz,
		    (compare_fn_t)prefixTableCmp);
  return (e != NULL);
}
