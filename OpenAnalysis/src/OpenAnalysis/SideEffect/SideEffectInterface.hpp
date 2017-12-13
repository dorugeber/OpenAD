/*! \file
  
  \brief Contains definition of abstract interface for SideEffect results.

  \authors Michelle Strout
  \version $Id: SideEffectInterface.hpp,v 1.5 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef SideEffectInterface_hpp
#define SideEffectInterface_hpp

#include <set>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Location/Locations.hpp>


namespace OA {
  namespace SideEffect {

//! Abstract interface for SideEffect results.
class Interface {
  public:
    Interface() {}
    virtual ~Interface() {}

    //! Return an iterator over all locations that are may be
    //! modified locally.
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLMODIterator() = 0;

    //! Return an iterator over all locations that are may be
    //! modified within the procedure or within a procedure
    //! called by the procedure.  
    virtual OA_ptr<LocIterator> getMODIterator() = 0;

    //! Return an iterator over all locations that are definitely
    //! modified locally within the procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLDEFIterator() = 0;

    //! Return an iterator over all locations that are definitely
    //! modified within the procedure or within a procedure
    //! called by the procedure.  
    virtual OA_ptr<LocIterator> getDEFIterator() = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the procedure before being
    //! definitely modified.  
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLUSEIterator() = 0;

    //! Return an iterator over all locations that may be 
    //! used before being defined within the procedure or 
    //! within a procedure called by the procedure.  
    virtual OA_ptr<LocIterator> getUSEIterator() = 0;

    //! Return an iterator over all locations that are 
    //! used locally within the procedure.
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    virtual OA_ptr<LocIterator> getLREFIterator() = 0;

    //! Return an iterator over all locations that are 
    //! used within the procedure or within a procedure
    //! called by the procedure.  
    virtual OA_ptr<LocIterator> getREFIterator() = 0;

    //*****************************************************************
    // Output
    //*****************************************************************

    virtual void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir) = 0;

};


  } // end of SideEffect namespace
} // end of OA namespace

#endif

