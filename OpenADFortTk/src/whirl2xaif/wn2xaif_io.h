// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################
#ifndef wn2xaif_io_INCLUDED
#define wn2xaif_io_INCLUDED

#include "Open64IRInterface/Open64BasicTypes.h"


#include "whirl2xaif.h"
#include "PUXlationContext.h"

namespace whirl2xaif {

  extern void 
  xlate_IO(xml::ostream& xos, WN *wn, PUXlationContext& context);
  
  extern BOOL
  xlate_IO_ITEM(xml::ostream& xos, WN *wn, PUXlationContext& context);

}; /* namespace whirl2xaif */

#endif /* wn2xaif_io_INCLUDED */

