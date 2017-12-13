// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/XAIFBaseParserHelper.hpp"
#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster { 

  XAIFBaseParserHelper::XAIFBaseParserHelper() :
#include "xaifBooster/tools/codegen/code/helper_initialization.inc"
  {} // end of   XAIFBaseParserHelper::XAIFBaseParserHelper

#include "xaifBooster/tools/codegen/code/helper_definition.inc"

} // end of namespace

