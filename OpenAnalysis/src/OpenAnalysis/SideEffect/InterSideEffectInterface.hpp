/*! \file
  
  \brief Contains definition of abstract interface for Interprocedural
         SideEffect results.

  \authors Michelle Strout
  \version $Id: InterSideEffectInterface.hpp,v 1.5 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef InterSideEffectInterface_hpp
#define InterSideEffectInterface_hpp

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/SideEffect/SideEffectInterface.hpp>


namespace OA {
  namespace SideEffect {

typedef ProcHandleIterator ProcIterator;

//! Abstract interface for InterSideEffect results.
class InterSideEffectInterface {
  public:
    InterSideEffectInterface() {}
    virtual ~InterSideEffectInterface() {}

    //! Return an iterator over all procedures for which side
    //! effect information is available 
    virtual OA_ptr<ProcIterator> getKnownProcIterator() = 0;

    //*************************************************************************
    
    //! Return an iterator over all locations that may be
    //! modified locally within the called procedure.  
    //! Locations modified directly in the called procedure and not
    //! by a function call in the called procedure.
    virtual OA_ptr<LocIterator> getLMODIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are may be
    //! modified within the called procedure or within a procedure
    //! called by the called procedure.  
    virtual OA_ptr<LocIterator> getMODIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are definitely
    //! modified locally within the called procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLDEFIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are definitely
    //! modified within the called procedure or within a procedure
    //! called by the called procedure.  
    virtual OA_ptr<LocIterator> getDEFIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the called procedure before being
    //! definitely modified.  
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLUSEIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that may be 
    //! used before being defined within the called procedure or 
    //! within a procedure called by the called procedure.  
    virtual OA_ptr<LocIterator> getUSEIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the called procedure.
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLREFIterator(CallHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used within the called procedure or within a procedure
    //! called by the called procedure.  
    virtual OA_ptr<LocIterator> getREFIterator(CallHandle p) = 0;

    //*************************************************************************
    
    //! Return an iterator over all locations that are may be
    //! modified locally within the given procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLMODIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are may be
    //! modified within the given procedure or within a procedure
    //! called by the given procedure.  
    virtual OA_ptr<LocIterator> getMODIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are definitely
    //! modified locally within the given procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLDEFIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are definitely
    //! modified within the given procedure or within a procedure
    //! called by the given procedure.  
    virtual OA_ptr<LocIterator> getDEFIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the given procedure before being
    //! definitely modified.  
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLUSEIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that may be 
    //! used before being defined within the given procedure or 
    //! within a procedure called by the given procedure.  
    virtual OA_ptr<LocIterator> getUSEIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the given procedure.
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLREFIterator(ProcHandle p) = 0;

    //! Return an iterator over all locations that are 
    //! used within the given procedure or within a procedure
    //! called by the given procedure.  
    virtual OA_ptr<LocIterator> getREFIterator(ProcHandle p) = 0;

    //*****************************************************************
    // Output
    //*****************************************************************

    virtual void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir) = 0;

};


  } // end of SideEffect namespace
} // end of OA namespace

#endif

