/*! \file
  
  \brief Definition of ICFGDep.

  \authors Michelle Strout
  \version $Id: ICFGDep.hpp,v 1.5 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ICFGDep_hpp
#define ICFGDep_hpp

#include <set>
#include <map>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Location/Locations.hpp>

#include "DepDFSet.hpp"


namespace OA {
  namespace Activity {

/* 
   Maps each statement to a set of Dependences use,def where use is a 
   differentiable location used at some point when defining the def location.
   Also keeps track of must def locations for a statement.
   Doesn't have use,def pairs for procedure calls.  In statements with 
   procedure calls, just has other uses and defs.

   For example,
        a = foo(x, &y)
   has the Dep <x,a> but not <y,a> or <x,y>.  
*/
    class ICFGDep : public virtual Annotation {
  public:
    ICFGDep() {}
    ~ICFGDep() {}

    //*****************************************************************
    // Interface Implementation
    //*****************************************************************

    //! Return an iterator over all locations whose definition may 
    //! depend on the given use location in the given stmt.
    OA_ptr<LocIterator> 
        getMayDefIterator(StmtHandle stmt, const OA_ptr<Location> use);

    //! Return an iterator over all locations that are differentiable
    //! locations used in the possible definition of the given location,
    //! in the given stmt
    OA_ptr<LocIterator> 
        getDiffUseIterator(StmtHandle stmt, OA_ptr<Location> def);
    
    //! Return an iterator over all locations that are definitely
    //! defined in the given stmt
    OA_ptr<LocIterator> getMustDefIterator(StmtHandle stmt);

    //*****************************************************************
    // Construction methods
    //*****************************************************************
    
    //! map stmt to a DepDFSet
    void mapStmtToDeps(StmtHandle stmt, OA_ptr<DepDFSet> depDFSet)
      { mDepDFSet[stmt] = depDFSet; }

    //! Insert use,def dependence pair
    void insertDepForStmt(StmtHandle stmt, OA_ptr<Location> use,
            OA_ptr<Location> def);

    //! Insert must def location 
    void insertMustDefForStmt(StmtHandle stmt, OA_ptr<Location> def);
   
    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(OA::IRHandlesIRInterface& ir);

    //*****************************************************************
    // Output
    //*****************************************************************
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

  private:
    std::map<StmtHandle,OA_ptr<DepDFSet> > mDepDFSet;

    std::map<StmtHandle,OA_ptr<LocSet> > mMustDefMap;

};


  } // end of Activity namespace
} // end of OA namespace

#endif

