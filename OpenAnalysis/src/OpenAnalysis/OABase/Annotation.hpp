/*! \file
  
  \brief All analysis results have this as their base class.

  \authors Michelle Strout
  \version $Id: Annotation.hpp,v 1.2 2007/10/08 19:19:14 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef Annotation_H
#define Annotation_H

#include <iostream>
#include <string>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/Utils/OutputBuilderText.hpp>

namespace OA {

class Annotation {
  protected:
    // output builder for all annotations
    static OA_ptr<OutputBuilder> sOutBuild;

  public:
    Annotation() 
      { if (sOutBuild.ptrEqual(0)) sOutBuild = new OutputBuilderText; }
      // FIXME: this could lead to performance problems but it
      // seems the only other option is to have some kind of
      // initialization method called before calling any output
      // methods or hardcode to a specific OutputBuilder subclass

    virtual ~Annotation() {}

    static void configOutput(OA_ptr<OutputBuilder> ob) { sOutBuild = ob; }

    // methods all annotations must provide
    virtual void output(IRHandlesIRInterface& ir) = 0;
};

} // end of OA namespace

#endif

