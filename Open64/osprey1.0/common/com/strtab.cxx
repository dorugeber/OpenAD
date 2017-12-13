/*

Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it would be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

Further, this software is distributed without any warranty that it is
free of the rightful claim of any third person regarding infringement 
or the like.  Any license provided herein, whether implied or 
otherwise, applies only to this software file.  Patent licenses, if 
any, provided herein do not apply to combinations of this program with 
other software, or any other product whatsoever.  

You should have received a copy of the GNU General Public License along
with this program; if not, write the Free Software Foundation, Inc., 59
Temple Place - Suite 330, Boston MA 02111-1307, USA.

Contact information:  Silicon Graphics, Inc., 1600 Amphitheatre Pky,
Mountain View, CA 94043, or:

http://www.sgi.com

For further information regarding this notice, see:

http://oss.sgi.com/projects/GenInfo/NoticeExplan

*/



#ifdef USE_PCH
#include "common_com_pch.h"
#endif /* USE_PCH */
#pragma hdrstop

#include <alloca.h>
#include <string.h> /* for memmove() */

#include <map>
#include "HashTable.h"
using namespace stlCompatibility;


#ifdef _SGI_SGI
#include <utility>    // for pair< > needed by sgi CC 
#endif

using namespace std;

#include "defs.h"
#include "errors.h"
#include "cxx_memory.h"			// for CXX_NEW

#include "strtab.h"

// The string table is implmeneted as a single character buffer, re-allocated
// if necessary.  Byte 0 of this buffer is alwasy NULL.  Each string is
// copied to the buffer once.  Duplicates are not entered.  A simple hash
// table is used to for quick lookup.  The hash table is a fixed size
// array of STR_BUCKET, each of which a dynamic array of <idx,size> pair.
// The size field is truncated to 8 bits.  It is used for 
// quick comparision to minimize the need to call strcmp.

// we support two types of strings: C-style null-terminated string and
// character array with a specified size (may contain null character within
// the array.

typedef UINT32 STR_INDEX;

#ifdef linux

#define STR_INDEX_size(xxx) (xxx & 0xff)
#define STR_INDEX_index(xxx) (xxx >> 8)

#else

// given a STR_INDEX (which is a 32-bit UINT), the following two functions
// extract the offset of the string in string table (first 24 bits), and the 
// size of the string (not real size, but a truncated_to_8_bits size). 

static inline mUINT8
STR_INDEX_size (STR_INDEX idx)		{ return idx & 0xff; }
static inline mUINT32
STR_INDEX_index (STR_INDEX idx)		{ return idx >> 8; }

#endif /* linux */

// given the offset of a string in string table, and the string size, 
// this function makes a 32-bit hybrid index (basically the reverse
// of the above two functions).

static inline STR_INDEX
make_STR_INDEX (UINT32 size, UINT32 idx)
{
  // if string length larger than 0xff, just use 0xff
  if (size > 0xff)
    size = 0xff;
  return (STR_INDEX) ((idx << 8) | size);
}

struct NULL_TERMINATED_STRING
{
  static char *get_str (char *buffer) {
    return buffer;
  }

  static const char *get_str (const char *buffer) {
    return buffer;
  }

  static UINT32 get_length (const char *buffer) {
    return strlen (buffer);
  }

  static UINT32 get_buffer_length (UINT32 length) {
    return length + 1;		// length + null character
  }

  static void copy (const char *str, UINT32 length, char *buffer) {
    memcpy (buffer, str, length+1);
  }
};


union UNALIGN_INT32
{
  char ch[sizeof(UINT32)];
  UINT32 n;

  UNALIGN_INT32 (UINT32 x) : n (x) {}

  UNALIGN_INT32 (const char *s) {
    for (INT32 i = 0; i < sizeof(UINT32); ++i)
      ch[i] = *s++;
  }
};


struct CHARACTER_ARRAY
{
  //----------------------------------------------------------------------
  // IMPORTANT NOTE:
  //
  // the character array data structure keeps a length field as its first
  // byte. this length should be interpreted as an unsigned number.
  //
  // some compilers (e.g. Sun's CC) sign extend "char" types so an "unsigned
  // char" type is needed to ensure that the length is not interpreted 
  // as a negative number. We defined the UC macro to cast the signed "char"
  // type to an unsigned char so that it is properly interpreted as a 
  // 8-bit unsigned number.
  //
  // Fengmei Zhao and John Mellor-Crummey, Rice University
  //----------------------------------------------------------------------
#define UC(x) ((unsigned char)(x))

  // If the character array is less then 0xff bytes, we store the size in
  // The First Byte Followed By The Array.  Otherwise, The First Byte Is Set
  // To 0xff And The Next 4 Bytes Hold The Size Of The Array As An Unsigned
  // Integer, And The Array Follows.

  static const char *get_str (const char *buffer) {
    if (UC(*buffer) != 0xff)
      return buffer + 1;
    else 
      return buffer + sizeof(UINT32) + 1;
  }

  static char *get_str (char *buffer) {
    if (UC(*buffer) != 0xff)
      return buffer + 1;
    else 
      return buffer + sizeof(UINT32) + 1;
  }

  static UINT32 get_length (const char *buffer) {
    if (UC(*buffer) != 0xff)


      return UC(*buffer);
    else {
      UNALIGN_INT32 unalign (buffer + 1);
      return unalign.n;
    }
  }

  static UINT32 get_buffer_length (UINT32 length) {
    return length < 0xff ? length + 1 : length + 1 + sizeof(UINT32);
  }

  static void copy (const char *str, UINT32 length, char *buffer_) {
    unsigned char *buffer = (unsigned char *) buffer_;
    if (length < 0xff) {
      *buffer++ = length;
    } else {
      *buffer++ = 0xff;
      UNALIGN_INT32 unalign (length);
      for (INT32 i = 0; i < sizeof(UINT32); ++i)
	*buffer++ = unalign.ch[i];
    }
    memcpy (buffer, str, length);
  }

}; // CHARACTER_ARRAY


template <class STR>
struct STR_TAB
{
  char *buffer;       // main buffer storing all strings in string table
  STR_IDX last_idx;   // end of current used storage
  UINT32 buffer_size;  

  // the following 4 function objects are required for setting up the
  // hash table declared after them.

  // hash_key is the "key" of hashtable. STR_INDEX is the "data" of hashtable. 
  // they two compose pair<key, STR_INDEX>, which can serve as the "value type"
  // in map (created by me).

  // hash key, which is a pair of string pointer and string length
  /*
  struct hash_key {
    const char *str;
    mUINT32 size;

    hash_key () {}
	
    hash_key (const char *s, UINT32 l) : str (s), size (l) {}
	
    hash_key (const hash_key& p) : str (p.str), size (p.size) {}
  };
  */
  struct StringHashKey {
    mUINT32 idx;
    mUINT32 size;

    StringHashKey() {}

    StringHashKey(mUINT32 _idx, mUINT32 _size) : 
      idx(_idx), size(_size) {}

    StringHashKey(const StringHashKey& shk) : 
      idx(shk.idx), size(shk.size) {}

    StringHashKey& operator=(const StringHashKey& shk) { 
      if (this != &shk) {
	idx = shk.idx;
	size = shk.size;
      }
      return (*this);
    }
  };


  // class-helper to determine the actual pointer to the string idx memory
  class ExtractStringBufferFromStringTable {
    const STR_TAB<STR>& stringTable;

  public:
    ExtractStringBufferFromStringTable (const STR_TAB<STR>& _stringTable) : 
      stringTable(_stringTable) {}
    
    char * operator() (mUINT32 idx) const {
      char *str = stringTable.buffer + idx;
      return STR::get_str(str);
    }
  };

  ExtractStringBufferFromStringTable extractStringBufferFromStringTable;

  class HashStringHashKey {
  public:
    STR_TAB<STR> *thisEnclosing;

    HashStringHashKey() : thisEnclosing(NULL) {}

    HashStringHashKey(STR_TAB<STR> & _thisEnclosing) :
      thisEnclosing(&_thisEnclosing) {}

    UINT32 operator() (const StringHashKey& key) const {
      char * str = thisEnclosing->extractStringBufferFromStringTable(key.idx);

      UINT32 h = 0;
      const UINT32 length = key.size;
      for (UINT32 i = 0; i < length; ++i)
	h = 5 * h + str[i];
      return h;
    }
  };

  class EqStringHashKey {
  public:
    STR_TAB<STR> *thisEnclosing;

    EqStringHashKey() :
      thisEnclosing(NULL) {}

    EqStringHashKey(STR_TAB<STR> & _thisEnclosing) :
      thisEnclosing(&_thisEnclosing) {}

    bool operator() (const StringHashKey& key1, 
		     const StringHashKey& key2) const {
      if (key1.size != key2.size) return false;
      char * str1 = thisEnclosing->extractStringBufferFromStringTable(key1.idx);
      char * str2 = thisEnclosing->extractStringBufferFromStringTable(key2.idx);
      bool result = (str1[0] == str2[0]) &&
	            (memcmp(str1, str2, key1.size) == 0);
      return result;
    }
  };

  typedef HashTable<StringHashKey, STR_INDEX, HashStringHashKey, EqStringHashKey> HASHTABLE;
  HASHTABLE hash_table;

  STR_TAB(UINT bucket_size) : 
    hash_table(),
    extractStringBufferFromStringTable(*this), 
    buffer (NULL), last_idx (0), buffer_size (0) { 
    hash_table.hashClass().thisEnclosing = this;
    hash_table.eqClass().thisEnclosing = this;
  };
  // operations
    
  void reserve (UINT32 size) {
    if (size > buffer_size - last_idx) {
      // realloc
      while (size > buffer_size - last_idx) {
	if (buffer_size < 0x100000)
	  buffer_size *= 2;
	else
	  buffer_size += 0x80000;
      }
      buffer = (char *) MEM_POOL_Realloc (Malloc_Mem_Pool, buffer, 0,
					  buffer_size); 
    }
  }
    
  void init_hash ();
  
  UINT32 insert (const char *str, UINT32 size);
  
  UINT32 insert (const char *str) {
    return insert (str, strlen (str));
  }
    
  void copy_str (const char *str, UINT32 size);

  void dump(FILE *fout) {
    
  }
}; // STR_TAB




// when inserting a new string/size into the string table
//   first  - copy the string into stringtable's buffer
//   second - calculate its STR_INDEX according to its offset and size
//   third  - try to insert its STR_INDEX/hash_key into hash_table
//   fourth - if a same hash_key already exists, un-insert
template <class STR>
UINT32 STR_TAB<STR>::insert (const char *str, UINT32 size)
{
  // insert the new string into the String Buffer,
  // not into auxiliary String Hash table
  UINT32 oldLastIndex = last_idx;
  copy_str (str, size);  //copy str into buffer at offset 'index'
  STR_INDEX new_index = make_STR_INDEX (size, oldLastIndex);

  // try to insert the new string into the String Hash Table
  typename HASHTABLE::ValueBoolPair insertStatus = 
    hash_table.insert(typename HASHTABLE::KeyValuePair(StringHashKey(oldLastIndex, size), new_index));

  if (insertStatus.second == false) {
    // the new string was already in the StringTable => 
    // uninsert from the String Buffer
    last_idx = oldLastIndex;
    return STR_INDEX_index(insertStatus.first);
  } else {
    // this is the first occurence of the new string
    assert(STR_INDEX_index(insertStatus.first) == oldLastIndex);
    return oldLastIndex;
  }
} // STR_TAB<STR>::insert


template <class STR>
void STR_TAB<STR>::copy_str (const char *str, UINT32 size) {
  UINT32 buffer_size = STR::get_buffer_length (size);
  reserve (buffer_size);
  STR::copy (str, size, buffer + last_idx);
  last_idx += buffer_size;
} // STR_TAB::copy_str


template <class STR>
void STR_TAB<STR>::init_hash () {
  UINT32 idx = 1;			// first entry always null
  while (idx < last_idx) {
    UINT32 length = STR::get_length(buffer + idx);

    STR_INDEX new_index = make_STR_INDEX(length, idx);
    hash_table.insert(typename HASHTABLE::KeyValuePair(StringHashKey(idx, length), new_index));
    idx += STR::get_buffer_length (length);
  }
} // STR_TAB<STR>::init_hash


template <class STRTAB>
static inline void initialize_strtab (STRTAB& strtab, UINT32 size) {
  strtab.buffer_size = size;
  strtab.buffer = (char *) MEM_POOL_Alloc (Malloc_Mem_Pool, size);
  strtab.buffer[0] = 0;
  strtab.last_idx = 1;
}


template <class STRTAB>
static inline void initialize_strtab (STRTAB& strtab, const char *buf, UINT32 size) {
  strtab.hash_table.clear();
  strtab.buffer_size = size;
  strtab.buffer = (char *) MEM_POOL_Alloc (Malloc_Mem_Pool, size);
  memmove (strtab.buffer, buf, size);
  strtab.last_idx = size;
  strtab.init_hash ();
} // Initialize_Strtab
    

  // Global String table
static STR_TAB<NULL_TERMINATED_STRING> Strtab (1000);

STRING_TABLE Str_Table;

UINT32
STR_Table_Size () {
  return Strtab.last_idx;
}

// init an empty table; use by producer (e.g., front end)
void
Initialize_Strtab (UINT32 size) {
  initialize_strtab (Strtab, size);
} 

// initialized the string table with the strtab from an input file
void Initialize_Strtab (const char *buf, UINT32 size) {
  initialize_strtab (Strtab, buf, size);
} // Initialize_Strtab


STR_IDX Save_Str (const char *str) {
  if (str == NULL) return 0;
  return Strtab.insert (str);
} // Save_Str


STR_IDX Save_Str2 (const char *s1, const char *s2) {
  UINT len = strlen (s1) + strlen(s2) + 1;
  char *new_str = (char *) alloca (len);
  strcpy (new_str, s1);
  strcat (new_str, s2);
  return Save_Str (new_str);
} // Save_Str2

STR_IDX Save_Str2i (const char *s1, const char *s2, UINT i) {
  UINT len = strlen (s1) + strlen(s2) + 17;
  char *new_str = (char *) alloca (len);
  sprintf(new_str, "%s%s%d", s1, s2, i);
  return Save_Str (new_str);
} // Save_Str2

char * Index_To_Str (STR_IDX idx) {
  Is_True (idx < Strtab.last_idx, ("Invalid STR_IDX"));
  return NULL_TERMINATED_STRING::get_str (Strtab.buffer + idx);
}

// character array table
const UINT32 TCON_STRTAB_HASH_SIZE = 512;
static STR_TAB<CHARACTER_ARRAY> TCON_strtab (TCON_STRTAB_HASH_SIZE);

UINT32
TCON_strtab_size () {
  return TCON_strtab.last_idx;
}

char * TCON_strtab_buffer () {
  return TCON_strtab.buffer;
}

// init an empty table; use by producer
void Initialize_TCON_strtab (UINT32 size) {
  initialize_strtab (TCON_strtab, size);
}

// init the TCON strtab from an input file
void Initialize_TCON_strtab (const char *buf, UINT32 size) {
  initialize_strtab (TCON_strtab, buf, size);
}

// add string of length "len"; string might not be null-terminated
UINT32 Save_StrN (const char *s1, UINT32 len) {
  if (len == 0) return 0;
  return TCON_strtab.insert(s1, len);
  //  return TCON_strtab.insert (s1, len);
} // Save_StrN

char * Index_to_char_array (UINT32 idx) {
  Is_True (idx < TCON_strtab.last_idx, ("Invalid TCON str index"));
  return CHARACTER_ARRAY::get_str (TCON_strtab.buffer + idx);
}

#ifdef MONGOOSE_BE

template <class STR, class MAP>
void merge_strtab (STR_TAB<STR>& strtab, const char *buf, UINT32 size, MAP& map) {
  map[0] = 0;
  UINT32 idx = 1;
  while (idx < size) {
    const char *str = STR::get_str (buf + idx);
    UINT32 size = STR::get_length (buf + idx);
    UINT32 new_idx = strtab.insert (str, size);
    map[idx] = new_idx;
    idx += STR::get_buffer_length (size);
  }
} // merge_strtab


// merge in a string table buffer from an input file, return a map from
// the old STR_IDX to the new (merged) STR_IDX

void
Merge_Strtab (const char *buf, UINT32 size, STR_IDX_MAP& map) {
  merge_strtab (Strtab, buf, size, map);
}

// merge in a string table buffer from an input file, return a map from
// the old STR_IDX to the new (merged) STR_IDX
void Merge_TCON_Strtab (const char *buf, UINT32 size, STR_IDX_MAP& map) {
  merge_strtab (TCON_strtab, buf, size, map);
}

#endif // MONGOOSE_BE

  
