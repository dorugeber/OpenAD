/*! \file
  
  \brief Abstract basic constant value interface.
  
  \authors Michelle Strout
  \version $Id: ConstValBasicInterface.hpp,v 1.4 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ConstValBasicInterface_h
#define ConstValBasicInterface_h

#include <string>

namespace OA {

class ConstValBasicInterface {
  public:
    ConstValBasicInterface() {}
    virtual ~ConstValBasicInterface() {}

    virtual bool operator<  (ConstValBasicInterface& other) = 0;
    virtual bool operator== (ConstValBasicInterface& other) = 0;
    virtual bool operator!= (ConstValBasicInterface& other) = 0;
    virtual std::string toString() = 0;
};

} // end of namespace OA

#endif 
