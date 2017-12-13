/*! \file
  
  \brief Abstract IR interface for determining parameter bindings.
  
  \authors Michelle Strout
  \version $Id: ParamBindingsIRInterface.hpp,v 1.4 2007/10/08 19:19:13 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ParamBindingsIRInterface_h
#define ParamBindingsIRInterface_h

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/ExprTree/ExprTree.hpp>

namespace OA {

typedef IRCallsiteParamIterator ExprHandleIterator;

  namespace DataFlow {


//! This is the primary interface for determining parameter bindings to
//! underlying intermediate representation.
class ParamBindingsIRInterface : public virtual IRHandlesIRInterface {
 public:
  ParamBindingsIRInterface() { }
  virtual ~ParamBindingsIRInterface() { }
  
  //! Given a subprogram return an IRStmtIterator for the entire
  //! subprogram
  virtual OA_ptr<IRStmtIterator> getStmtIterator(ProcHandle h) = 0; 
  
  //! Return an iterator over all the memory reference handles that appear
  //! in the given statement.  Order that memory references are iterated
  //! over can be arbitrary.
  virtual OA_ptr<MemRefHandleIterator> getAllMemRefs(StmtHandle stmt) = 0;
  
  //! Given a MemRefHandle return an iterator over
  //! MemRefExprs that describe this memory reference
  virtual OA_ptr<MemRefExprIterator> 
      getMemRefExprIterator(MemRefHandle h) = 0;


  //! Get IRCallsiteParamIterator for a callsite.
  //! Iterator visits actual parameters in called order.
  virtual OA_ptr<IRCallsiteParamIterator> getCallsiteParams(CallHandle h) = 0;

  //! Given a subprogram return an IRSymIterator for all
  //! symbols that are referenced within the subprogram
  //virtual OA_ptr<IRSymIterator> getRefSymIterator(ProcHandle h) = 0; 

  //! returns true if given symbol is a parameter 
  virtual bool isParam(SymHandle) = 0;

  //! returns true if given symbol is a pass by reference parameter 
  //virtual bool isRefParam(SymHandle) = 0;
               
  //! return the formal parameter that an actual parameter is associated with 
  virtual SymHandle getFormalForActual(ProcHandle caller, CallHandle call, 
                                       ProcHandle callee, ExprHandle param) = 0; 
  //! Given an ExprHandle, return an ExprTree 
  virtual OA_ptr<ExprTree> getExprTree(ExprHandle h) = 0;
  
};  

  } // end of namespace DataFlow
} // end of namespace OA

#endif 
