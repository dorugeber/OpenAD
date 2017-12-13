/*! \file
  
  \brief Concrete ExprTreeVisitor that will use Linearity information
         if available and an IRInterface to attempt to include specific
         information to deps.
  
  \authors Luis Ramos
  \version $Id: OutputNewExprTreeVisitor.hpp,v 1.1 2008/02/06 19:58:48 utke Exp $

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
#include <OpenAnalysis/NewExprTree/NewExprTree.hpp>
#include <OpenAnalysis/NewExprTree/NewExprTreeVisitor.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class OutputNewExprTreeVisitor : public NewExprTreeVisitor, public Annotation {
public:
  OutputNewExprTreeVisitor(IRHandlesIRInterface& ir);
  ~OutputNewExprTreeVisitor() {}

  void visitExprTreeBefore(NewExprTree&) { }
  void visitExprTreeAfter(NewExprTree&) { }

  //---------------------------------------
  // method for each ExprTree::Node subclass
  //---------------------------------------
  // default base class so that visitors can handle unknown
  // node sub-classes in a generic fashion
  void visitNode(NewExprTree::Node&);

  void visitOpNode(NewExprTree::OpNode& n);
  void visitCallNode(NewExprTree::CallNode& n);
  void visitMemRefNode(NewExprTree::MemRefNode& n);
  void visitConstSymNode(NewExprTree::ConstSymNode& n);
  void visitConstValNode(NewExprTree::ConstValNode& n);

  void output(IRHandlesIRInterface&);

  bool getIsConst() { return mIsConst; }


private:
  bool mIsConst;
  int mId;
  IRHandlesIRInterface& mIR;
};


} // end of OA namespace

#endif
