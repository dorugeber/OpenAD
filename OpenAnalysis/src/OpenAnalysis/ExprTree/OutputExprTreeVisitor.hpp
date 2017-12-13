/*! \file
  
  \brief Concrete ExprTreeVisitor that will use Linearity information
         if available and an IRInterface to attempt to include specific
         information to deps.
  
  \authors Luis Ramos
  \version $Id: OutputExprTreeVisitor.hpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef OutputExprTreeVisitor_H
#define OutputExprTreeVisitor_H

#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/IRInterface/LinearityIRInterface.hpp>
#include <OpenAnalysis/ExprTree/ExprTree.hpp>
#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/ExprTree/MemRefsVisitor.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class OutputExprTreeVisitor : public ExprTreeVisitor, public Annotation {
public:
  OutputExprTreeVisitor(IRHandlesIRInterface& ir);
  ~OutputExprTreeVisitor() {}

  void visitExprTreeBefore(ExprTree&) { }
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

  void output(IRHandlesIRInterface&);

  bool getIsConst() { return mIsConst; }


private:
  bool mIsConst;
  int mId;
  IRHandlesIRInterface& mIR;
};


} // end of OA namespace

#endif
