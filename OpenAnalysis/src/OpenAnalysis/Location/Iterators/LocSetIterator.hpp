/*! \file
  
  \brief Declarations for the Location-set iterator class.

  \authors Michelle Strout, Andy Stone
  \version $Id: LocSetIterator.hpp,v 1.1 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef LocSetIterator_H
#define LocSetIterator_H

#include <OpenAnalysis/Location/LocSet.hpp>
#include <OpenAnalysis/Location/Location.hpp>
#include <OpenAnalysis/Location/Iterators/LocIterator.hpp>

namespace OA {

//! general iterator for Locations
class LocSetIterator : public LocIterator {
  public:
    LocSetIterator(OA_ptr<LocSet> pSet);
    ~LocSetIterator();

    OA_ptr<Location> current();
    bool isValid();
    void operator++();
    void reset();

  private:
    OA_ptr<LocSet> mSet;
    LocSet::const_iterator mIter;
};

} // end namespace
#endif
