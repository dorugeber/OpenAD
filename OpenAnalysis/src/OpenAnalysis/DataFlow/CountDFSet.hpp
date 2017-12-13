/*! \file
  
  \brief DataflowSet that consists of a count

  \authors Michelle Strout 
  \version $Id: CountDFSet.hpp,v 1.3 2007/10/08 19:19:11 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef CountDFSet_H
#define CountDFSet_H

#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

// abstract interface that this class implements
#include <OpenAnalysis/DataFlow/DataFlowSet.hpp>

namespace OA {
  namespace DataFlow {


class CountDFSet : public virtual DataFlowSet {
public:
  CountDFSet();
  CountDFSet(int count);
  CountDFSet(const CountDFSet &other);
  ~CountDFSet() {}


  OA_ptr<DataFlowSet> clone();
  
  // param for these can't be const because will have to 
  // dynamic cast to specific subclass
  bool operator ==(DataFlowSet &other) const;

  bool operator !=(DataFlowSet &other) const;

  void dump(std::ostream &os);

  void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> ir);
  
  //========================================================
  // methods specific to CountDFSet
  //========================================================
  
protected:
  int mCount;

};



  } // end of DataFlow namespace
} // end of OA namespace

#endif
