/*! \file
  
  \brief Concrete ExprTreeVisitor that will use reaching constant information
         if available and an IRInterface to attempt to evaluate 
         an expression tree to a ConstValBasicInterface.
  
  \authors Barbara Kreaseck, Michelle Strout
  \version $Id: EvalToConstVisitor.hpp,v 1.7 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef EvalToConstVisitor_H
#define EvalToConstVisitor_H

#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/IRInterface/ConstValBasicInterface.hpp>
#include <OpenAnalysis/IRInterface/EvalToConstVisitorIRInterface.hpp>
#include <OpenAnalysis/ReachConsts/Interface.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class EvalToConstVisitor : public ExprTreeVisitor {
public:
  EvalToConstVisitor(OA_ptr<EvalToConstVisitorIRInterface> ir,
                     OA_ptr<ReachConsts::Interface> rc);
  ~EvalToConstVisitor() {}

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

  //! interface for results of visiting the expression tree
  OA_ptr<ConstValBasicInterface> getConstVal() { return mEvalResult; }

private:
  OA_ptr<ConstValBasicInterface> mEvalResult;
  OA_ptr<EvalToConstVisitorIRInterface> mIR;
  OA_ptr<ReachConsts::Interface> mReachConsts;
};


} // end of OA namespace

#endif
