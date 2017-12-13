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
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"

using namespace xaifBooster;

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/driver/buildStamp.hpp"


int main(int argc,char** argv) { 
  DbgLoggerManager::instance()->setBinaryBuildInfo(buildStamp);
  DbgLoggerManager::instance()->setSelection(DbgGroup::ERROR 
					     //| DbgGroup::CALLSTACK | DbgGroup::TEMPORARY
					     );
  xaifBoosterBasicBlockPreaccumulation::AlgFactoryManager* theFManager_p=
    dynamic_cast<xaifBoosterBasicBlockPreaccumulation::AlgFactoryManager*>(xaifBoosterBasicBlockPreaccumulation::AlgFactoryManager::instance());
  theFManager_p->init();
  xaifBoosterBasicBlockPreaccumulation::AlgConfig* algConfig_p=theFManager_p->makeAlgConfig(argc,argv,buildStamp);
  try { 
    algConfig_p->config();
  } catch (BaseException& e) { 
    DBG_MACRO(DbgGroup::ERROR,
	      "caught exception: " << e.getReason());
    algConfig_p->usage();
    return -1;
  } // end catch 
  try {   
    InlinableIntrinsicsParser ip(ConceptuallyStaticInstances::instance()->getInlinableIntrinsicsCatalogue());
    ip.initialize();
    ip.setExternalSchemaLocation(algConfig_p->getSchemaPath());
    ip.parse(algConfig_p->getIntrinsicsFileName());
    if (algConfig_p->isSet('N')) {
      NonInlinableIntrinsicsParser nip(ConceptuallyStaticInstances::instance()->getNonInlinableIntrinsicsCatalogue());
      nip.initialize();
      nip.setExternalSchemaLocation(algConfig_p->getSchemaPath());
      nip.parse(algConfig_p->getNIIntrinsicsFileName());
    } 
    XAIFBaseParser p;
    p.initialize(algConfig_p->getInputValidationFlag());
    p.setExternalSchemaLocation(algConfig_p->getSchemaPath());
    p.parse(algConfig_p->getInputFileName());
    CallGraph& Cg(ConceptuallyStaticInstances::instance()->getCallGraph());
    Cg.getScopeTree().forcedPassivation();
    DBG_MACRO(DbgGroup::TIMING,"before linearize");
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_1); // linearize
    DBG_MACRO(DbgGroup::TIMING,"before flatten");
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_2); // flatten
    DBG_MACRO(DbgGroup::TIMING,"before accumulation");
    Cg.genericTraversal(GenericAction::ALGORITHM_ACTION_3); // accumulate Jacobian
    DBG_MACRO(DbgGroup::TIMING,"before unparse");
    const std::string& oldSchemaLocation(Cg.getSchemaLocation());
    std::string newLocation(oldSchemaLocation,0,oldSchemaLocation.find(' '));
    if (algConfig_p->getSchemaPath().size())
      newLocation.append(" "+algConfig_p->getSchemaPath()+"/xaif_output.xsd");
    else 
      newLocation.append(" xaif_output.xsd");
    Cg.resetSchemaLocation(newLocation);
    if (algConfig_p->isSet('o')) { 
      std::ofstream theOutFile(algConfig_p->getOutFileName().c_str(),
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
  DBG_MACRO(DbgGroup::TIMING,"done");
  DBG_MACRO(DbgGroup::METRIC,"total number of assignments: " << xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::getAssignmentCounter() << std::endl << "                           "
	    << "total number of sequences: " << xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::getSequenceCounter() << std::endl << "                           "
	    << "totals for preaccumulation metrics: " << xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::getGlobalPreaccumulationCounter().debug().c_str());
  return 0;
}
