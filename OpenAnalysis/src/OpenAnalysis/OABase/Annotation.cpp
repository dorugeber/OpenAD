/*! \file
  
  \brief Need to initialize static variable

  \authors Michelle Strout
  \version $Id: Annotation.cpp,v 1.2 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "Annotation.hpp"

namespace OA {

// initializing class static variable
OA_ptr<OutputBuilder> Annotation::sOutBuild;

} // end of OA namespace


