// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef whirl2xaif_i
#define whirl2xaif_i

#include <IntrinsicXlationTable.h>
#include <OAMaps.h>
#include <ScalarizedRefTab.h>
#include <WhirlIDMaps.h>

#include <stab_attr.h>
#include <wn_attr.h>
#include <IFDiagnostics.h>


#define whirl2xaif_divider_comment \
  "********************************************************************"


namespace whirl2xaif {
  
  enum status {
    good,
    bad
  };
  
  extern IntrinsicXlationTable   IntrinsicTable;
  
//  extern PUToOAAnalInfoMap       OAAnalMap;
//  extern ScalarizedRefTabMap_W2X ScalarizedRefTableMap;
//  extern WNToWNIdTabMap          WNToWNIdTableMap;
  
}; 

#endif 
