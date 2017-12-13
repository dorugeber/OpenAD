#ifndef __HashTable_H__
#define __HashTable_H__


#include <stddef.h>
#include <assert.h>
#include <stdio.h>

#include <map>
#include <vector>
#include <iostream>


#define xDEBUG(flag,code) { if (flag) {code; fflush(stdout);} }
#define DEB_HashTable 0

namespace stlCompatibility {

using namespace std;

// Hash functions for standard types (taken from ESTL)
template <class _Key> struct Hash { };

inline size_t hashString(const char* __s)
{
  unsigned long __h = 0; 
  for ( ; *__s; ++__s)
    __h = 5*__h + *__s;
  
  return size_t(__h);
}
template<> struct Hash<char*>
{
  size_t operator()(const char* __s) const { return hashString(__s); }
};
template<> struct Hash<const char*>
{
  size_t operator()(const char* __s) const { return hashString(__s); }
};
template<> struct Hash<char> {
  size_t operator()(char __x) const { return __x; }
};
template<> struct Hash<unsigned char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
template<> struct Hash<signed char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
template<> struct Hash<short> {
  size_t operator()(short __x) const { return __x; }
};
template<> struct Hash<unsigned short> {
  size_t operator()(unsigned short __x) const { return __x; }
};
template<> struct Hash<int> {
  size_t operator()(int __x) const { return __x; }
};
template<> struct Hash<unsigned int> {
  size_t operator()(unsigned int __x) const { return __x; }
};
template<> struct Hash<long> {
  size_t operator()(long __x) const { return __x; }
};
template<> struct Hash<unsigned long> {
  size_t operator()(unsigned long __x) const { return __x; }
};
//--------------------------------------------------------------------


template <class Key, class Value, 
	  class KeyHash = Hash<Key>,
	  class KeyEq = equal_to<Key> >
class HashTable {
public:
  typedef Key KeyType;
  typedef Value ValueType;
  typedef KeyHash hasher;
  typedef KeyEq key_equal;

  typedef pair<Key, Value> value_type;
  typedef pair<Key, Value> KeyValuePair;
  typedef pair<const Value, bool> ValueBoolPair;

private:
  typedef vector<KeyValuePair> KeyValuePairVector;
  typedef typename KeyValuePairVector::iterator KeyValuePairVectorIterator;
  typedef map<size_t, KeyValuePairVector, less<size_t> > Ht;
  typedef typename Ht::iterator HtIterator;
  typedef typename Ht::const_iterator HtConstIterator;

  Ht ht; // size_t->KeyValuePairVector
  size_t _size;
  KeyHash keyHash;
  KeyEq keyEq;

public:
  KeyHash & hashClass() { return keyHash; }
  KeyEq & eqClass() { return keyEq; }

  HashTable() {
    _size = 0;
  }

  virtual ~HashTable() {
    clear();
  }

public:
  void clear() {
    if (size() == 0) return;
    HtIterator htIterator;
    for ( htIterator = ht.begin();
	  htIterator != ht.end();
	  htIterator ++ ) {
      htIterator->second.clear();
    }
    ht.clear();
    _size = 0;
  }

  size_t size() const {
    return _size;
  }

  bool empty() const {
    return (_size == 0);
  }

  size_t count(const Key& k) const {
    size_t h = keyHash(k);
    HtConstIterator htIterator = ht.find(h);
    if (htIterator == ht.end()) return 0;
    return htIterator->second.size();
  } // count

  ValueBoolPair find(const Key& k) const {
    size_t h = keyHash(k);
    HtConstIterator htIterator = ht.find(h);
    if (htIterator == ht.end()) return ValueBoolPair((const Value)NULL, false);

    const KeyValuePairVector& kvpv = htIterator->second;
    typename KeyValuePairVector::const_iterator kvpvIterator;
    for ( kvpvIterator = kvpv.begin(); 
	  kvpvIterator != kvpv.end(); 
	  kvpvIterator ++ ) {
      const Key& kk = kvpvIterator->first;
      if (keyEq(k, kk) == true) return ValueBoolPair(kvpvIterator->second, true);
    }
    return ValueBoolPair((const Value)NULL, false);
  } // find

  ValueBoolPair insert(const KeyValuePair& p) {
    size_t h = keyHash(p.first);
    HtIterator htIterator = ht.find(h);
    if (htIterator != ht.end()) {
      KeyValuePairVector& kvpv = htIterator->second;
      xDEBUG(DEB_HashTable, 
	     printf("insert: h=%u, ht.size()=%u, ht[h].size()=%u\n", h, ht.size(), kvpv.size());
	     );
      KeyValuePairVectorIterator kvpvIterator;
      for ( kvpvIterator = kvpv.begin(); 
	    kvpvIterator != kvpv.end(); 
	    kvpvIterator ++ ) {
	Key& kk = kvpvIterator->first;
	if (keyEq(p.first, kk) == true) {
	  return ValueBoolPair(kvpvIterator->second, false);
	}
      }
      kvpv.push_back(p);
    } else {
      xDEBUG(DEB_HashTable, 
	     printf("insert: h=%u, ht.size()=%u, ht[h].size()=%u\n", h, ht.size(), 0));
      KeyValuePairVector kvpv;
      kvpv.push_back(p);
      pair<HtIterator, bool> tmp1 = ht.insert(typename Ht::value_type(h, kvpv));
      assert(tmp1.second == true);
    }
    _size ++;
    return ValueBoolPair(p.second, true);
  } // insert

  ValueBoolPair erase(const Key& k) {
    size_t h = keyHash(k);
    xDEBUG(DEB_HashTable, printf("erase: h=%u\n", h));

    HtIterator htIterator = ht.find(h);
    if (htIterator == ht.end()) return ValueBoolPair((const Value)NULL, false);

    KeyValuePairVector& kvpv = htIterator->second;
    KeyValuePairVectorIterator kvpvIterator;
    for ( kvpvIterator = kvpv.begin(); 
	  kvpvIterator != kvpv.end(); 
	  kvpvIterator ++ ) {
      Key& kk = kvpvIterator->first;
      if (keyEq(k, kk) == true) {
	Value v = kvpvIterator->second;
	kvpv.erase(kvpvIterator);
	_size --;
	if (kvpv.empty()) ht.erase(htIterator);
	return ValueBoolPair(v, true);
      }
    }
    
    assert(false);
    return ValueBoolPair((const Value)NULL, false); // should never reach!
  } // erase

  ValueBoolPair modify(const KeyValuePair& p) {
    assert(false);
    return ValueKeyPair((const Value)NULL, false);
  } // modify

// The action must not change the membership of the data structure
// Value v can be changed
class ForAllAction {
  public:
  virtual void handle(const Key k, Value &v) = 0;
};

// oreder is NOT defined
  void forAll(ForAllAction & action) {
    HtIterator htIterator;
    KeyValuePairVectorIterator kvpvIterator;

    for ( htIterator = ht.begin();
	  htIterator != ht.end();
	  htIterator ++ ) {
      KeyValuePairVector& kvpv = htIterator->second;
      for ( kvpvIterator = kvpv.begin(); 
	    kvpvIterator != kvpv.end(); 
	    kvpvIterator ++ ) {
	const Key& k = kvpvIterator->first;
	Value& v = kvpvIterator->second;

	action.handle(k, v);
      }
    }
  } //forAll
};

} // namespace

#endif


