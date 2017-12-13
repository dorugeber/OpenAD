/*! \file
  
  \brief The OutputBuilder utility functions.
  
  \authors Michelle Strout
  \version $Id: OutputBuilder.cpp,v 1.3 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/


#include "OutputBuilder.hpp"
#include <sstream>

namespace OA {

std::string int2string(const int& number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}
 
std::string bool2string(const bool& val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}
 

} // end of OA namespace


