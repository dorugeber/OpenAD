/*! \file
  
  \brief Maps StmtHandles to set of locations that OutVary based on
         certain independent locations.

  \authors Michelle Strout
  \version $Id: VaryStandard.hpp,v 1.9 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef VaryStandard_hpp
#define VaryStandard_hpp

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/ReachDefs/Interface.hpp>
#include <OpenAnalysis/DataFlow/LocDFSet.hpp>

namespace OA {
  namespace Activity {



class VaryStandard {
  public:
    VaryStandard(ProcHandle p);
    ~VaryStandard() {}

    //! Return an iterator for set of independent locations
    //! the analysis calculated OutVary from
    OA_ptr<LocIterator> getIndepSetIterator();

    //! Return an iterator that will iterate over the set of locations
    //! that are in OutVary set for stmt
    OA_ptr<LocIterator> getOutVaryIterator(StmtHandle s);

    //! returns a location data flow set of locations that 
    //! vary coming into the statement for the given call
    //FIXME: an iterator or make getOutVaryIterator return a set?
    OA_ptr<DataFlow::LocDFSet> getInVarySet(ExprHandle call);

    //*****************************************************************
    // Construction methods 
    //*****************************************************************

    //! insert an independent location
    void insertIndepLoc(OA_ptr<Location> loc);

    //! set the OutVary set for a statement
    void copyIntoOutVary(StmtHandle s, 
                         OA_ptr<DataFlow::LocDFSet> dfset);

    //! set the InVary set for a call
    void copyIntoInVary(ExprHandle call, 
                        OA_ptr<DataFlow::LocDFSet> dfset);

    //*****************************************************************
    // Output
    //*****************************************************************

    //! incomplete output of info for debugging, just lists stmts
    //! and associated set of OutVary locations
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

  private:
    // data members
    std::map<StmtHandle,OA_ptr<DataFlow::LocDFSet> > mOutVary;
    std::map<ExprHandle,OA_ptr<DataFlow::LocDFSet> > mInVary;
    OA_ptr<DataFlow::LocDFSet>  mIndepLocSet;

};

  } // end of Activity namespace
} // end of OA namespace

#endif

