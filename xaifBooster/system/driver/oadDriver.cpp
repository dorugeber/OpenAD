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
#include "xaifBooster/system/inc/AlgFactoryManager.hpp"
#include "xaifBooster/system/inc/AlgConfig.hpp"

using namespace xaifBooster;

#include "xaifBooster/system/driver/buildStamp.hpp"

int main(int argc,char** argv) { 
  DbgLoggerManager::instance()->setBinaryBuildInfo(buildStamp);
  DbgLoggerManager::instance()->setSelection(DbgGroup::ERROR 
					     //| DbgGroup::CALLSTACK | DbgGroup::TEMPORARY
					     );
  AlgFactoryManager::instance()->init();
  AlgConfig* algConfig_p=AlgFactoryManager::instance()->makeAlgConfig(argc,argv,buildStamp);
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
    const std::string& oldSchemaLocation(Cg.getSchemaLocation());
    std::string newLocation(oldSchemaLocation,0,oldSchemaLocation.find(' '));
    if (algConfig_p->getSchemaPath().size())
      newLocation.append(" "+algConfig_p->getSchemaPath()+"/xaif.xsd");
    else 
      newLocation.append(" xaif.xsd");
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
  return 0;
}
  
