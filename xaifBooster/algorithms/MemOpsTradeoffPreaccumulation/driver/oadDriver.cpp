// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <iostream>
#include <utility>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/XAIFBaseParser.hpp"
#include "xaifBooster/system/inc/InlinableIntrinsicsParser.hpp"
#include "xaifBooster/system/inc/NonInlinableIntrinsicsParser.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/AlgConfig.hpp"

using namespace xaifBooster;

#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/driver/buildStamp.hpp"


int main(int argc,char** argv) { 
  DbgLoggerManager::instance()->setBinaryBuildInfo(buildStamp);
  DbgLoggerManager::instance()->setSelection(DbgGroup::ERROR 
					     //| DbgGroup::CALLSTACK | DbgGroup::TEMPORARY
					     );
  xaifBoosterMemOpsTradeoffPreaccumulation::AlgConfig algConfig(argc,argv,buildStamp);
  try { 
    algConfig.config();
  } catch (BaseException& e) { 
    DBG_MACRO(DbgGroup::ERROR,
	      "caught exception: " << e.getReason());
    algConfig.usage();
    return -1;
  } // end catch 
  try {   
    xaifBoosterMemOpsTradeoffPreaccumulation::AlgFactoryManager::instance()->init();
    InlinableIntrinsicsParser ip(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue());
    ip.initialize();
    ip.setExternalSchemaLocation(algConfig.getSchemaPath());
    ip.parse(algConfig.getIntrinsicsFileName());
    if (algConfig_p->isSet('N')) {
      NonInlinableIntrinsicsParser nip(ConceptuallyStaticInstances::instance()->getNonInlinableIntrinsicsCatalogue());
      nip.initialize();
      nip.setExternalSchemaLocation(algConfig_p->getSchemaPath());
      nip.parse(algConfig_p->getNIIntrinsicsFileName());
    } 
    XAIFBaseParser p;
    p.initialize(algConfig.getInputValidationFlag());
    p.setExternalSchemaLocation(algConfig.getSchemaPath());
    p.parse(algConfig.getInputFileName());
    CallGraph& Cg(ConceptuallyStaticInstances::instance()->getCallGraph());
    Cg.getScopeTree().forcedPassivation();
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_1); // linearize
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_2); // flatten
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_3); // accumulate Jacobian
    const std::string& oldSchemaLocation(Cg.getSchemaLocation());
    std::string newLocation(oldSchemaLocation,0,oldSchemaLocation.find(' '));
    if (algConfig.getSchemaPath().size())
      newLocation.append(" "+algConfig.getSchemaPath()+"/xaif_output.xsd");
    else 
      newLocation.append(" xaif_output.xsd");
    Cg.resetSchemaLocation(newLocation);
    if (algConfig.isSet('o')) { 
      std::ofstream theOutFile(algConfig.getOutFileName().c_str(),
			       std::ios::out);
      Cg.printXMLHierarchy(theOutFile);
      theOutFile.close();
    } 
    else 
      Cg.printXMLHierarchy(std::cout);
  } catch (BaseException& e) { 
    DBG_MACRO(DbgGroup::ERROR,
	      "caught exception: " << e.getReason());
    return -1;
  } // end catch 
  return 0;
}
