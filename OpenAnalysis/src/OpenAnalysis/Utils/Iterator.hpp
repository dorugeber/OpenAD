/*! \file
  
  \brief Declaration for Iterator class

  \authors Arun Chauhan (2001 was part of Mint)
  \version $Id: Iterator.hpp,v 1.5 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef Iterator_H
#define Iterator_H

namespace OA {

//--------------------------------------------------------------------
/** The Iterator abstract base class defines the basic properties of an iterator. */
class Iterator {
public:
  Iterator () {}
  virtual ~Iterator () {}
//  virtual operator bool () = 0;
  virtual bool isValid() const = 0;
  virtual void operator++ () = 0;
  void operator++ (int) { operator++(); }  // postfix, only forward iterator is defined
};
//--------------------------------------------------------------------

} // end OA namespace

#endif
