/*! \file
  
  \brief Declarations for the abstract interface interprocedural Alias results.

  \authors Michelle Strout
  \version $Id: InterAliasInterface.hpp,v 1.3 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef InterAliasInterface_H
#define InterAliasInterface_H

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/Location/Locations.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>

namespace OA {
  namespace Alias {

class InterAliasInterface {
  public:
    InterAliasInterface() {}
    virtual ~InterAliasInterface() {};

    //! Returns alias analysis results for the given procedure
    virtual OA_ptr<Alias::Interface> getAliasResults(ProcHandle) = 0;

};

  } // end of Alias namespace
} // end of OA namespace

#endif

