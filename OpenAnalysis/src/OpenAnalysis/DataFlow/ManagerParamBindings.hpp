/*! \file
  
  \brief Declarations of the AnnotationManager that determines
         parameter bindings and generates a ParamBindings object.

  \authors Michelle Strout
  \version $Id: ManagerParamBindings.hpp,v 1.2 2007/10/08 19:19:11 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ManagerParamBindings_h
#define ManagerParamBindings_h

//--------------------------------------------------------------------
// OpenAnalysis headers

#include "ParamBindings.hpp"
#include <OpenAnalysis/CallGraph/CallGraphInterface.hpp>
#include <OpenAnalysis/ExprTree/EvalToMemRefVisitor.hpp>
#include <OpenAnalysis/IRInterface/ParamBindingsIRInterface.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExprVisitor.hpp>

namespace OA {
  namespace DataFlow {


/*! 
   Generates ParamBindings
*/
class ManagerParamBindings {
public:
  ManagerParamBindings(OA_ptr<ParamBindingsIRInterface> _ir);
  ~ManagerParamBindings () {}

  OA_ptr<ParamBindings> 
      performAnalysis( OA_ptr<CallGraph::CallGraphInterface> callGraph );

private: // member variables
  OA_ptr<ParamBindingsIRInterface> mIR;

};

/*!
   Visitor over memory reference expressions that finds all symbols
   that are formal parameters and indicates that information to
   the provided ParamBindings class.
 */
class FormalFinderVisitor : public virtual MemRefExprVisitor {
  public:
    FormalFinderVisitor(OA_ptr<ParamBindingsIRInterface> ir, ProcHandle proc,
                        OA_ptr<ParamBindings> paramBind)
      : mIR(ir), mProc(proc), mParamBind(paramBind)
    { }
    ~FormalFinderVisitor() {}

    void visitNamedRef(NamedRef& ref); 
    void visitUnnamedRef(UnnamedRef& ref); 
    void visitUnknownRef(UnknownRef& ref);
    void visitAddressOf(AddressOf& ref);
    void visitDeref(Deref& ref);
    // default handling of more specific SubSet specificiations
    void visitSubSetRef(SubSetRef& ref); 

  private:
    OA_ptr<ParamBindingsIRInterface> mIR;
    ProcHandle mProc;
    OA_ptr<ParamBindings> mParamBind;
};


  } // end of DataFlow namespace
} // end of OA namespace

#endif
