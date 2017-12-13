/*! \file
  
  \brief Abstract class for sets of dataflow facts.

  \authors Michelle Strout (April 2004)
           similar to John Mellor-Crummey's DataFlowSet.h
  \version $Id: DataFlowSet.hpp,v 1.10 2007/10/08 19:19:11 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef DataFlowSet_h
#define DataFlowSet_h

#include <iostream>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>

namespace OA {
  namespace DataFlow {

class DataFlowSet {
protected:
  //DataFlowSet() {}, why did I do this before?
public:
  DataFlowSet() {}
  virtual ~DataFlowSet() {}
  
  // FIXME: is this comment true?
  // param for these can't be const because will have to 
  // dynamic cast to specific subclass

  //virtual DataFlowSet& operator =(DataFlowSet &other) = 0;
  
  virtual bool operator ==(DataFlowSet &other) const = 0;
  virtual bool operator !=(DataFlowSet &other) const = 0;

  //virtual DataFlowSet* setUnion(DataFlowSet *other) = 0;
  //virtual DataFlowSet* setIntersect(DataFlowSet *other) = 0;
  
  virtual OA_ptr<DataFlowSet> clone() = 0;

  virtual void dump(std::ostream &os) = 0;
  virtual void dump(std::ostream &os, OA_ptr<IRHandlesIRInterface>) = 0;
};

  } // end of DataFlow namespace
} // end of OA namespace

#endif
