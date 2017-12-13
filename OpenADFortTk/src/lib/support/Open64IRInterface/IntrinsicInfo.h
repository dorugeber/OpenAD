// -*-Mode: C++;-*-
#ifndef IntrinsicInfo_H 
#define IntrinsicInfo_H

#include <map>
#include "Open64BasicTypes.h"

class IntrinsicInfo {

public:
  
  /** 
   * is this an intrinsic?
   */
  static bool isIntrinsic(const WN* aWN_p); 

  /** 
   * strips machine type information from the name
   */
  static const char* intrinsicBaseName(INTRINSIC opcode);

  /** 
   * the key
   */
  struct Key {
    Key(const OPERATOR& opr, const char* name): myWNopr(opr), myName(name) {};
    OPERATOR myWNopr;   // the WN operator
    const char* myName; // string qualifier for calls
  };

  /** 
   * the less than operator for the key needed for map
   */
  struct KeyLT {
    bool operator()(const Key& k1, const Key& k2) const; 
  };


  /**
   * type of intrinsic
   */
  enum IntrinsicType_E{ 
    FLOAT_INTR,             // defined for float (incl. complex), may also be defined for integers
    INTEGER_INTR,           // defined only for integers  
    STRING_INTR,            // defined only for strings
    BOOL_INTR,              // defined only for booleans
    IO_INTR,                // I/O operations
    ARRAY_INTR,             // array operations
    COMPILER_INTERNAL_INTR, // compiler internal methods
    UNDEFINED_INTR_TYPE     // value for default initializer
  }; 


  /** 
   * additional information
   */
  struct Info {
    Info(): myVarOps(false), myNumOp(0), myType(UNDEFINED_INTR_TYPE) {};
    Info(bool varOps, unsigned int aNumOp,IntrinsicType_E aType): myVarOps(varOps), myNumOp(aNumOp), myType(aType){};
    bool myVarOps; // do we have variable number of arguments?
    unsigned int myNumOp; // number of operands to intrinsic
    IntrinsicType_E myType; // the type of the intrinsic
  };

  /** 
   * if this is an intrinsic returns the info
   */
  static const Info&  getIntrinsicInfo(WN* aWN_p); 

  typedef std::map<Key,Info,KeyLT> IntrinsicMap;

  static const IntrinsicMap& getMap();

private:
  
  /** 
   * no def
   */
  IntrinsicInfo();
  
  /** 
   * no def
   */
  ~IntrinsicInfo();

  static IntrinsicMap ourIntrinsicInfoMap;
  static bool ourInitFlag;

  /** 
   * return  true if 'str' is a valid prefix for an intrinsic name; 
   * false otherwise. These prefixes are the machine types the 
   * front end attaches to the intrinsic names.
   */
  static bool lookupIntrinsicPrefix(const char* str);

  /** 
   * internal lookup 
   */
  static bool lookupIntrinsicInfo(const WN* aWN_p, const Info* anInfo); 
  
}; 

#endif

