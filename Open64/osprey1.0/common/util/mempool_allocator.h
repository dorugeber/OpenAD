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


/* -*-Mode: c++;-*- (Tell emacs to use c++ mode) */

#ifndef mempool_allocator_INCLUDED
#define mempool_allocator_INCLUDED

#ifndef mempool_INCLUDED
#include "mempool.h"
#endif

template <class T>
class mempool_allocator {
private:
  MEM_POOL* pool;
  template <class U> friend class mempool_allocator;

public:
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef T         value_type;

  template <class U> struct rebind {
    typedef mempool_allocator<U> other;
  };

  mempool_allocator() : pool(Malloc_Mem_Pool) {} // The default value.
  mempool_allocator(MEM_POOL* p) : pool(p) {}
  mempool_allocator(const mempool_allocator& a) : pool(a.pool) {}
  template <class U> mempool_allocator(const mempool_allocator<U>& a)
    : pool(a.pool) {}
  ~mempool_allocator() {}

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const { return &x; }


// Solaris CC workaround
// There are two important changes, one to allocate(), the other to deallocate().
// First, the first argument of deallocate() was changed to void*;
// second, the return type of allocate() was changed to void*.
// Both change are according to the default Allocator in <memory>

#if !defined(__GNUC__) && defined(_SOLARIS_SOLARIS)
  void*   allocate(size_type n, const void* = 0)
    { return n != 0 ? TYPE_MEM_POOL_ALLOC_N(T, pool, n) : pointer(0); }
#else
  pointer allocate(size_type n, const void* = 0) 
    { return n != 0 ? TYPE_MEM_POOL_ALLOC_N(T, pool, n) : pointer(0); }
#endif


#if !defined(__GNUC__) && defined(_SOLARIS_SOLARIS)
  void deallocate(void*  p, size_type) {
    if (p)
      MEM_POOL_FREE(pool, p);
  }
#else
  void deallocate(pointer p, size_type ) {
    if (p)
      MEM_POOL_FREE(pool, p);
  }
#endif


// Solaris CC workaround
// when compiling fb_whirl.cxx, the compiler complains "Error: Cstd/./memory line 490: Error: 
// Too many arguments in call to "mempool_allocator<FB_Info_Switch>::max_size() const"".
// It seems that alloctor_interface<> call mempool_allocator<>::max_size() with a size_t 
// argument. So overloaded this function.

#if !defined(__GNUC__) && defined(_SOLARIS_SOLARIS)
  size_type max_size(size_t) const { return size_t(-1) / sizeof(T); }
#endif

  size_type max_size() const { return size_t(-1) / sizeof(T); }

  void construct(pointer p, const T& val) { new(p) T(val); }
  void destroy(pointer p) { p->~T(); }
};

template<>
class mempool_allocator<void> {
public:
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef void*       pointer;
  typedef const void* const_pointer;
  typedef void        value_type;

  template <class U> struct rebind {
    typedef mempool_allocator<U> other;
  };
};
#endif //  mempool_allocator_INCLUDED
