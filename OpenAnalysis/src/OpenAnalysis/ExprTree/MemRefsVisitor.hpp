/*! \file
  
  \brief Concrete ExprTreeVisitor that determines which memory references
         are in an expression.
  
  \authors Michelle Strout, Barbara Kreaseck
  \version $Id: MemRefsVisitor.hpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef MemRefsVisitor_H
#define MemRefsVisitor_H

#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
//#include <OpenAnalysis/Alias/Interface.hpp>

namespace OA {

  // Seems like there should already be a MemRefIterator in the OA namespace
  // but I cannot seem to locate it, so here it is:
  //! An iterator over MemRefHandle's
  class MemRefIterator : public virtual OA::MemRefHandleIterator,
                         public OA::IRHandleSetIterator<OA::MemRefHandle>
  {
  public:
    MemRefIterator(OA::OA_ptr<std::set<OA::MemRefHandle> > pList) 
      : OA::IRHandleSetIterator<OA::MemRefHandle>(pList) {}
    ~MemRefIterator() {}

    void operator++() { OA::IRHandleSetIterator<OA::MemRefHandle>::operator++(); }
    bool isValid() const
      { return OA::IRHandleSetIterator<OA::MemRefHandle>::isValid(); }
    OA::MemRefHandle current() const
      { return OA::IRHandleSetIterator<OA::MemRefHandle>::current(); }
    void reset() { OA::IRHandleSetIterator<OA::MemRefHandle>::reset(); }
  };

//--------------------------------------------------------------------
//! concrete ExprTree visitor that determines which memory references are in
//! an expression.

class MemRefsVisitor : public ExprTreeVisitor {
public:
  MemRefsVisitor() : mHasMemRef(false) { }
  ~MemRefsVisitor() {}

  void visitExprTreeBefore(ExprTree&) 
  { mMemRefSet = new std::set<MemRefHandle>; }
  void visitExprTreeAfter(ExprTree&) { }

  //---------------------------------------
  // method for each ExprTree::Node subclass
  //---------------------------------------
  // default base class so that visitors can handle unknown
  // node sub-classes in a generic fashion
  void visitNode(ExprTree::Node&);

  void visitOpNode(ExprTree::OpNode& n);
  void visitCallNode(ExprTree::CallNode& n);
  void visitMemRefNode(ExprTree::MemRefNode& n);
  void visitConstSymNode(ExprTree::ConstSymNode& n);
  void visitConstValNode(ExprTree::ConstValNode& n);

  //! interface for results of visiting the expression tree
  bool hasMemRef() { return mHasMemRef; }
  OA_ptr<MemRefHandleIterator> getMemRefsIterator()
  { OA_ptr<MemRefIterator> retval;
    retval = new MemRefIterator(mMemRefSet);
    return retval;
  }

private:
  bool mHasMemRef;
  OA_ptr<std::set<MemRefHandle> > mMemRefSet;
  //OA_ptr<Alias::Interface> mAlias;
};



} // end of OA namespace

#endif
