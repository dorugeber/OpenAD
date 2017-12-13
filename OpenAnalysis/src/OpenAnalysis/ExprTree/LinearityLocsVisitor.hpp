/*! \file
  
  \brief Concrete ExprTreeVisitor that will use Linearity information
         if available and an IRInterface to attempt to include specific
         information to deps.
  
  \authors Luis Ramos
  \version $Id: LinearityLocsVisitor.hpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef LinearityVisitor_H
#define LinearityVisitor_H

#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/IRInterface/LinearityIRInterface.hpp>
#include <OpenAnalysis/ExprTree/ExprTree.hpp>
#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/Linearity/LinearityDepsSet.hpp>
#include <OpenAnalysis/Linearity/LinearityMatrixStandard.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/DataFlow/ParamBindings.hpp>
#include <OpenAnalysis/Linearity/Interface.hpp>
#include <OpenAnalysis/ExprTree/MemRefsVisitor.hpp>

namespace OA {

//--------------------------------------------------------------------
/*! 
*/
class LinearityLocsVisitor : public ExprTreeVisitor {
public:
  LinearityLocsVisitor(OA_ptr<Linearity::LinearityIRInterface> ir,
                       OA_ptr<Linearity::Interface> rc,
                       OA_ptr<Alias::Interface> alias,
                       OA_ptr<DataFlow::ParamBindings> paramBind);
  ~LinearityLocsVisitor() {}

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

  bool getIsConst() { return mIsConst; }

  //! interface for results of visiting the expression tree
  OA_ptr<Linearity::LinearityDepsSet> getDepsSet() { return mDeps; }

private:
  OA_ptr<Linearity::LinearityDepsSet> mDeps;
  OA_ptr<Linearity::LinearityIRInterface> mIR;
  OA_ptr<Linearity::Interface> mLM;
  OA_ptr<Alias::Interface> mAlias;
  OA_ptr<DataFlow::ParamBindings> mParamBind;
  bool mIsConst;
};


} // end of OA namespace

#endif
