// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include <stdio.h>

#include "XlationCtxt.h"


CtxtFlags::CtxtFlags()
  : flags(0)
{
}

CtxtFlags::~CtxtFlags()
{
}

void 
CtxtFlags::Dump(std::ostream& o) const
{
  o << "{context}";
}

void 
CtxtFlags::DDump() const
{
  Dump(std::cerr);
}

