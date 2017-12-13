/*! \file
  
  \brief Abstract iterator for pairs of MemRefHandle and ExprHandles
         where MemRefHandle = ExprHandle.
  
  \authors Michelle Strout
  \version $Id: AssignPairIterator.hpp,v 1.1 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef AssignPairIterator_h
#define AssignPairIterator_h

#include <OpenAnalysis/IRInterface/IRHandles.hpp>

namespace OA {

class AssignPairIterator {
  public:
    AssignPairIterator() {}
    virtual ~AssignPairIterator() {}

    //! right hand side
    virtual ExprHandle currentSource() const = 0;
    //! left hand side
    virtual MemRefHandle currentTarget() const = 0;

    virtual bool isValid() const = 0;
                    
    virtual void operator++() = 0;
    void operator++(int) { ++*this; }
};


} // end of namespace OA

#endif 
