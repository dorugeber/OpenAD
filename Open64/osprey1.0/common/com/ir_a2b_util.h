/* -*-Mode: C++;-*- */
/* $Id: ir_a2b_util.h,v 1.3 2005-01-24 20:59:43 eraxxon Exp $ */
/* * BeginRiceCopyright *****************************************************
 * 
 * ******************************************************* EndRiceCopyright */

/* ====================================================================
 * ====================================================================
 *
 *
 * Nathan Tallent.
 *
 * Description:
 *
 * Utilities for converting between ascii and whirl.
 *
 * ====================================================================
 * ====================================================================
 */

#ifndef ir_a2b_util_INCLUDED
#define ir_a2b_util_INCLUDED

#include "defs.h"
#include "cxx_memory.h"			// for CXX_NEW
#include "errors.h"			// for Fail_FmtAssertion
#include "x_string.h"                   // for strcasecmp()

/* ====================================================================
 *
 * Templated schemes for translating between integral values and
 * symbolic names and vice-versa.  Should be quite adequate for
 * Open64.
 *
 * Note 1: For each mapping routine designated by a (*), a O(n) table
 * check is performed, where n is the size of the table, the first
 * time the routine is called.
 *
 * Note 2: n will be a relatively small compile-time constant.
 * 
 * 1. Translate between an enumerated value (INT values between 0 and
 *    n) and a unique string (case insensitive).
 *
 *    enum to string: O(1); no additional memory allocation. (*)
 *
 *    string to enum: O(log n). The first time the routine is called,
 *      an auxilliary table of size n pointers is allocated and sorted
 *      (O(n*log n)) to be later used with binary-search. (*)
 *
 * 2. Translate between a set of flags (a set is a UINT64) to a
 *    comma-separated list of strings (case insensitive) and vice-versa.
 *
 *    flag to string: O(log n); no additional table memory allocation. (*)
 *
 *    string to flag: O(log n). The first time the routine is called,
 *      an auxilliary table of size n pointers is allocated and sorted
 *      (O(n*log n)) to be later used with binary-search. (*)
 *
 * ====================================================================
 */

// Example for an enumerated type (flags are very similar)
//
// Given an enumerated type, 
//   enum TY_KIND {
//     KIND_APPLES,
//     KIND_ORANGES,
//     KIND_PEARS,
//     KIND_COUNT 
//   };     
//
// the user defines a table mapping the enumerated type to unique
// strings.  The table should have an entry for every value between 0
// and n and should use ir_a2b::enum2str_tbl_entry_t as the base class of
// the entry type:
//
//   struct myentry_t : public ir_a2b::enum2str_tbl_entry_t
//    ... 
//   };
//
//   myentry_t TYKindToNameTbl[KIND_COUNT] = 
//     myentry_t(KIND_APPLES,  "APPLES"),
//     myentry_t(KIND_ORANGES, "ORANGES"),
//     myentry_t(KIND_PEARS,   "PEARS")
//   };
//
// Given this table, the mappings can be performed using the routines
//   ir_a2b::MapEnumToStr(...)
//   ir_a2b::MapStrToEnum(...)
//
// See symtab.cxx for one set of examples.

namespace ir_a2b {

//----------------------------------------------------------------------
// enum2str_tbl_entry_t, flag2str_tbl_entry_t
//----------------------------------------------------------------------

// enum2str_tbl_entry_t: The virtual base class for an enumeration->string
// table entry.
struct enum2str_tbl_entry_t {
  enum2str_tbl_entry_t() { }
  enum2str_tbl_entry_t(const enum2str_tbl_entry_t& x) { }
  virtual ~enum2str_tbl_entry_t() { }

  // get the integer representation for the entry
  virtual INT getEnumVal() const = 0;
  
  // get the string for the entry
  virtual const char* getStr() const = 0;
  
  // cmp: for use with qsort(), bsearch()
  static int 
  cmp(const void * entry1, const void * entry2) {
    enum2str_tbl_entry_t* e1 = (enum2str_tbl_entry_t*)entry1;
    enum2str_tbl_entry_t* e2 = (enum2str_tbl_entry_t*)entry2;
    return ux_strcasecmp(e1->getStr(), e2->getStr());
  }
  
};


// flag2str_tbl_entry_t: The virtual base class for an enumeration->string
// table entry.
struct flag2str_tbl_entry_t {
  flag2str_tbl_entry_t() { }
  flag2str_tbl_entry_t(const flag2str_tbl_entry_t& x) { }
  virtual ~flag2str_tbl_entry_t() { }

  // get the integer representation for the entry
  virtual UINT64 getFlagVal() const = 0;
  
  // get the string for the entry
  virtual const char* getStr() const = 0;
  
  // flagcmp: for use with qsort(), bsearch()
  static int 
  flagcmp(const void * entry1, const void * entry2) {
    flag2str_tbl_entry_t* e1 = (flag2str_tbl_entry_t*)entry1;
    flag2str_tbl_entry_t* e2 = (flag2str_tbl_entry_t*)entry2;
    UINT64 v1 = e1->getFlagVal();
    UINT64 v2 = e2->getFlagVal();
    // to prevent type overflow we do not do: (INT64)(e1 - e2)
    if (v1 == v2) { 
      return 0; 
    }
    else if (v1 < v2) {
      return -1;
    }
    else {
      return 1;
    }
  }
  
  // strcmp: for use with qsort(), bsearch()
  static int 
  strcmp(const void * entry1, const void * entry2) {
    flag2str_tbl_entry_t* e1 = (flag2str_tbl_entry_t*)entry1;
    flag2str_tbl_entry_t* e2 = (flag2str_tbl_entry_t*)entry2;
    return ux_strcasecmp(e1->getStr(), e2->getStr());
  }
  
};


// CheckEnumTable: Ensures that the enum-to-string table meets the
// basic constraints for the mapping to work correctly.
//
// N.B.: 'table' is not part of the template-parameter-list since we
// only need a separate instantiation for each ENUM_TBL_ENTRY_T.
template <typename ENUM_TBL_ENTRY_T>
void
CheckEnumTable(const char* tablenm, const ENUM_TBL_ENTRY_T* table, INT tablesz)
{
  // Ensure that the table is sorted and contains all entries between
  // 0 and n
  for (INT val = 0; val < tablesz; ++val) {
    if (table[val].getEnumVal() != val) {
      const char* str = table[val].getStr() ? table[val].getStr() : "";
      Fail_FmtAssertion("ir_a2b::CheckEnumTable: Error in table '%s': invalid entry: %d -> { %d %s }", tablenm, val, table[val].getEnumVal(), str);
    }
  }
}


// CheckFlagTable: Ensures that the flag-to-string table meets the
// basic constraints for the mapping to work correctly.
//
// N.B.: 'table' is not part of the template-parameter-list since we
// only need a separate instantiation for each FLG_TBL_ENTRY_T.
template <typename FLG_TBL_ENTRY_T>
void
CheckFlagTable(const char* tablenm, const FLG_TBL_ENTRY_T* table, INT tablesz)
{
  // Ensure table is sorted
  UINT64 val, prevval = 0;
  for (INT i = 0; i < tablesz; ++i) {
    val = table[i].getFlagVal();
    if (i != 0 && !(prevval < val)) {
      Fail_FmtAssertion("ir_a2b::CheckFlagTable: Error in table '%s': %llx preceeds %llx (out of order)", tablenm, prevval, val);
    }
    prevval = table[i].getFlagVal();
  }
}


//----------------------------------------------------------------------
// MapEnumToStr, MapStrToEnum
//----------------------------------------------------------------------

// MapEnumToStr: Given 1) a enum2str_tbl_entry_t table (sorted by
// enumeration value), its name and the number of entries therein and
// 2) an enumeration value, return a string representation.
//
// N.B.: 'table' is part of the template-parameter-list to ensure
// there is a separate instantiation for each distinct source table.
template <typename ENUM_TBL_ENTRY_T, 
	  const ENUM_TBL_ENTRY_T* table, INT tablesz>
const char*
MapEnumToStr(const char* tablenm, INT val)
{
  // Check Table
  static bool isTableChecked = false;
  if (!isTableChecked) {
    CheckEnumTable(tablenm, table, tablesz);
    isTableChecked = true;
  }
  
  // Perform mapping
  if (0 <= val && val < tablesz) {
    return table[val].getStr();
  }
  else {
    Fail_FmtAssertion("ir_a2b::MapEnumToStr: Error accessing table '%s': no entry for '%d'.", tablenm, val);
  }
}


// MapStrToEnum: Given 1) a enum2str_tbl_entry_t table (sorted by
// enumeration value), its name and the number of entries therein and
// 2) a string, return an enumeration value.
//
// N.B.: 'table' is part of the template-parameter-list to ensure
// there is a separate instantiation for each distinct source table.
template <typename ENUM_TBL_ENTRY_T, 
	  const ENUM_TBL_ENTRY_T* table, INT tablesz>
INT
MapStrToEnum(const char* tablenm, const char* str)
{
  struct key_t : public enum2str_tbl_entry_t {
    key_t(const char* s_) : s(s_) { }
    virtual ~key_t() { }
    
    virtual INT getEnumVal() const { return 0; }
    virtual const char* getStr() const { return s; }
    
    const char* s;
  };

  // Check table
  static bool isTableChecked = false;
  if (!isTableChecked) {
    CheckEnumTable(tablenm, table, tablesz);
    isTableChecked = true;
  }

  // Compute translation table on demand (shallow copy of source table
  // is fine).  This table is sorted by string (instead of
  // enumeration) value.
  static ENUM_TBL_ENTRY_T* Str2EnumTbl = NULL; 
  if (!Str2EnumTbl) {
    Str2EnumTbl = CXX_NEW_ARRAY(ENUM_TBL_ENTRY_T, tablesz, Malloc_Mem_Pool);
    memcpy(Str2EnumTbl, table, sizeof(ENUM_TBL_ENTRY_T) * tablesz);
    qsort(Str2EnumTbl, tablesz, sizeof(ENUM_TBL_ENTRY_T), 
	  enum2str_tbl_entry_t::cmp);
  }
  
  // Use binary search to find the enumeration value
  key_t key(str);
  ENUM_TBL_ENTRY_T* found = 
    (ENUM_TBL_ENTRY_T*)bsearch(&key, Str2EnumTbl, tablesz, 
			       sizeof(ENUM_TBL_ENTRY_T), 
			       enum2str_tbl_entry_t::cmp);
  FmtAssert(found, ("ir_a2b::MapStrToEnum: Error accessing table '%s': no entry for '%s'.", tablenm, str));
  return found->getEnumVal();
}


//----------------------------------------------------------------------
// MapFlagsToStr, MapStrToFlags
//----------------------------------------------------------------------

struct flag_key_t : public flag2str_tbl_entry_t {
  flag_key_t(UINT64 flg_, const char* str_) : flg(flg_), str(str_) { }
  virtual ~flag_key_t() { }
  
  virtual UINT64 getFlagVal() const { return flg; }
  virtual const char* getStr() const { return str; }

  UINT64      flg;
  const char* str;
};


// MapFlagsToStr: Given 1) a flag2str_tbl_entry_t table (sorted by flag
// value), its name and the number of entries therein and 2) a set of
// flags, return a string representation (comma-separated flag list)
//
// N.B.: 'table' is part of the template-parameter-list to ensure
// there is a separate instantiation for each distinct source table.
template <typename FLAG_TBL_ENTRY_T, 
	  const FLAG_TBL_ENTRY_T* table, INT tablesz>
const char*
MapFlagsToStr(const char* tablenm, UINT64 flags)
{
  // Check table
  static bool isTableChecked = false;
  if (!isTableChecked) {
    CheckFlagTable(tablenm, table, tablesz);
    isTableChecked = true;
  }

  // Maintain this buffer to simplify usage; it should never be very large.
  static char* buf = NULL;
  static UINT bufSz = 256;
  if (!buf) {
    buf = TYPE_MEM_POOL_ALLOC_N(char, Malloc_Mem_Pool, bufSz);
  }

  // For each flag, find the string representation
  buf[0] = '\0';
  UINT curBufLen = 0;
  bool isBufEmpty = true;
  for (UINT64 curflg = 0x1000000000000000LL; curflg != 0; 
       curflg = curflg >> 1) {
    if ( !(flags & curflg) ) {
      continue;
    }

    // Use binary search to find the flag value
    flag_key_t key(curflg, NULL);
    FLAG_TBL_ENTRY_T* found = 
      (FLAG_TBL_ENTRY_T*)bsearch(&key, table, tablesz, 
				 sizeof(FLAG_TBL_ENTRY_T), 
				 flag2str_tbl_entry_t::flagcmp);
    FmtAssert(found, ("ir_a2b::MapFlagsToStr: Error accessing table '%s': no entry for '%llx'.", tablenm, curflg));

    // Add string to list
    const char* flgstr = found->getStr();
    curBufLen += strlen(flgstr);
    if (curBufLen+2 > bufSz) { // comma + terminator
      UINT oldSz = bufSz;
      bufSz *= 2;
      buf = TYPE_MEM_POOL_REALLOC_N(char, Malloc_Mem_Pool, buf, 
				    oldSz, bufSz);
    }
    if (!isBufEmpty) {
      strcat(buf, ",");
    }
    strcat(buf, flgstr);
    isBufEmpty = false;
  }
  
  return buf;
}


// MapStrToFlags: Given 1) a flag2str_tbl_entry_t table (sorted by
// flag value), its name and the number of entries therein and
// 2) a comma-separated flag string, return a set of flags.
//
// N.B.: 'table' is part of the template-parameter-list to ensure
// there is a separate instantiation for each distinct source table.
template <typename FLAG_TBL_ENTRY_T, 
	  const FLAG_TBL_ENTRY_T* table, INT tablesz>
UINT64
MapStrToFlags(const char* tablenm, const char* str)
{
  // Check table
  static bool isTableChecked = false;
  if (!isTableChecked) {
    CheckFlagTable(tablenm, table, tablesz);
    isTableChecked = true;
  }
  
  // Compute translation table on demand (shallow copy of source table
  // is fine).  This table is sorted by string (instead of
  // enumeration) value.
  static FLAG_TBL_ENTRY_T* Str2FlagTbl = NULL; 
  if (!Str2FlagTbl) {
    Str2FlagTbl = CXX_NEW_ARRAY(FLAG_TBL_ENTRY_T, tablesz, Malloc_Mem_Pool);
    memcpy(Str2FlagTbl, table, sizeof(FLAG_TBL_ENTRY_T) * tablesz);
    qsort(Str2FlagTbl, tablesz, sizeof(FLAG_TBL_ENTRY_T), 
	  flag2str_tbl_entry_t::strcmp);
  }
  
  UINT64 flags = 0;
  
  // For each flag-string in the list, find the numerical flag
  // note: strtok() will destroy the string so we use strdup
  char* flgstr = ux_strdup(str);
  for (char* tok = strtok(flgstr, ","); (tok != NULL);
       tok = strtok((char*)NULL, ",")) {
    // Use binary search to find the flag value
    flag_key_t key(0, tok);
    FLAG_TBL_ENTRY_T* found = 
      (FLAG_TBL_ENTRY_T*)bsearch(&key, Str2FlagTbl, tablesz, 
				 sizeof(FLAG_TBL_ENTRY_T), 
				 flag2str_tbl_entry_t::strcmp);
    if (tok[0] != '\0') {
      FmtAssert(found, ("ir_a2b::MapStrToFlags: Error accessing table '%s': no entry for '%s'.", tablenm, tok));
    }
    flags |= found->getFlagVal();
  }
  free(flgstr);
  
  return flags;
}


}; // namespace ir_a2b

#endif /* ir_a2b_util_INCLUDED */
