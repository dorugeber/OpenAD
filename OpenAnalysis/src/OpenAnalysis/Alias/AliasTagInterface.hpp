/*! \file
  
  \brief Declarations for the abstract Alias interface.

  \authors Michelle Strout

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef AliasTagInterface_H
#define AliasTagInterface_H

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExpr.hpp>
#include <OpenAnalysis/Alias/AliasTag.hpp>

namespace OA {
  namespace AliasTag {

     
typedef enum {
    NOALIAS,     
    MAYALIAS,  
    MUSTALIAS   
} AliasResultType;

//! Iterator over Alias Tags
class AliasTagIterator {
public:
  AliasTagIterator() { }
  virtual ~AliasTagIterator() { }

  virtual AliasTag current() = 0;  // Returns the current item.
  virtual bool isValid() = 0;        // False when all items are exhausted.
        
  virtual void operator++() = 0;
  void operator++(int) { ++*this; } ;

  virtual void reset() = 0;
};


class AliasTagInterface {
  public:
    AliasTagInterface() {}
    virtual ~AliasTagInterface() {};

    //! indicate the aliasing relationship between two memory references
    virtual AliasResultType alias(MemRefHandle ref1, 
                                  MemRefHandle ref2) = 0;
       
    //! Return an iterator over alias tags that represent locations the 
    //! given memory reference or memory reference expression may reference
    
    //virtual OA_ptr<TagIterator> getAliasTags( OA_ptr<MemRefExpr> mre, 
    //                    CallContext context, SymHandle stmt ) = 0;
    
    virtual OA_ptr<AliasTagIterator> getAliasTags( OA_ptr<MemRefExpr> mre )=0;

    //virtual OA_ptr<TagIterator> getAliasTags( MemRefHandle memref,
    //                    CallContext context, SymHandle stmt ) = 0;
                
    virtual OA_ptr<AliasTagIterator> getAliasTags( MemRefHandle memref )=0;

    //! Given an AliasTag, returns an iterator over MemRefExprs that 
    //! may access the given tag.
    //FIXME: will need to uncomment at some point, MMS 12/16/07
    // virtual OA_ptr<MemRefExprIterator> getMemRefExprIterator(AliasTag tag)=0;
   
    //! get iterator over all memory references that information is
    //! available for
    // FIXME: MMS 12/16/07, is this used anymore?
    //virtual OA_ptr<MemRefIterator> getMemRefIter() = 0;

};

//typedef MemRefHandleIterator MemRefIterator;

  } // end of Alias namespace
} // end of OA namespace

#endif

