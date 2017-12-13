/*! \file
  
  \brief Abstract IR interface for XAIF-specific analyses.
  
  \authors Michelle Strout
  \version $Id: XAIFIRInterface.hpp,v 1.6 2007/10/08 19:19:13 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef XAIFIRInterface_h
#define XAIFIRInterface_h

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>

namespace OA {
  namespace XAIF {

//! The XAIFIRInterface abstract base class gives a set of methods
//! for querying the source IR for information relevant to XAIF-specific
//! analyses.
class XAIFIRInterface : public virtual IRHandlesIRInterface {
 public:
  XAIFIRInterface() { }
  virtual ~XAIFIRInterface() { }
   
  //! Given a statement return an IRTopMemRefIterator* 
  //! this is an iterator over all the top memory references in a statement
  //! for example, in *p = q[i][j], *p and q[i][j] are the top memory 
  //! references which contain the sub memory references p and q,i,j 
  //! respectively
  //FIXME: this is currently broken, probably want to get rid of it
  //virtual OA_ptr<IRTopMemRefIterator> getTopMemRefIterator(StmtHandle h) = 0; 
   
  //! Given a subprogram return an IRStmtIterator* for the entire
  //! subprogram
  //! The user must free the iterator's memory via delete.
  virtual OA_ptr<IRStmtIterator> getStmtIterator(ProcHandle h) = 0; 

  //! Return an iterator over all the memory reference handles that appear
  //! in the given statement.  Order that memory references are iterated
  //! over can be arbitrary.
  virtual OA_ptr<MemRefHandleIterator> getAllMemRefs(StmtHandle stmt) = 0;

 
};  

  } // end of namespace XAIF
} // end of namespace OA

#endif 
