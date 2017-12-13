// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/FrontEndDecorations.hpp"

#include "xaifBooster/system/inc/AlgConfig.hpp"
#include "xaifBooster/system/inc/CallGraphVertex.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/AlgFactoryManager.hpp"

namespace xaifBooster { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    CommandLineParser(argc,argv),
    myBuildStamp(buildStamp),
    myConfiguredFlag(false),
    myInputValidationFlag(false) {
    registerIt(&ourConfig, &ourUsage,"iocdgGBNsTvpbVDFh");
  } 

  void AlgConfig::registerIt(RegFP theConfig,
			     RegFP theUsage,
			     const std::string& switches) { 
    bool isNew=true;
    if (std::find(myConfigFPList.begin(),myConfigFPList.end(),theConfig)==myConfigFPList.end())
      myConfigFPList.push_back(theConfig);
    else 
      isNew=false;
    if (std::find(myUsageFPList.begin(),myUsageFPList.end(),theUsage)==myUsageFPList.end())
      myUsageFPList.push_back(theUsage);
    else 
      isNew=false;
    for (std::string::const_iterator sit=switches.begin();
	 sit!=switches.end();
	 ++sit) { 
      if (mySwitches.find(*sit)==std::string::npos)
	mySwitches+=*sit;
      else {
	if (isNew)
	  THROW_LOGICEXCEPTION_MACRO("AlgConfig::register: ambiguous switch '" << *sit << "'" ); 
      }
    }
  } 

  std::string AlgConfig::getSwitches() { 
    return mySwitches;
  } 

  void AlgConfig::config() { 
    for (RegFPList::iterator it=myConfigFPList.begin();
	 it!=myConfigFPList.end();
	 ++it)
      (*it)();
  } 

  void AlgConfig::ourConfig() {
    AlgFactoryManager::instance()->getAlgConfig()->myConfig();
  }

  void AlgConfig::myConfig() { 
    if (!myConfiguredFlag) { 
      // avoid doing this twice
      parse(getSwitches());
      if (isSet('h')){ 
	usage();
      }
      myInputFileName=argAsString('i');
      myIntrinsicsFileName=argAsString('c');
      if (isSet('s')) 
	mySchemaPath=argAsString('s');
      if (isSet('o')) 
	myOutFileName=argAsString('o');
      if (isSet('d')) 
	DbgLoggerManager::instance()->setFile(argAsString('d'));
      if (isSet('g')) 
	DbgLoggerManager::instance()->setSelection(argAsInt('g'));
      if (isSet('G')) 
	DbgLoggerManager::instance()->setGraphicsFormat(argAsString('G'));
      if (isSet('B')) 
	DbgLoggerManager::instance()->setSvgViewer(argAsString('B'));
      if (isSet('N')) 
	myNIIntrinsicsFileName=argAsString('N');
      if (isSet('p'))
	Symbol::addSymbolNamesToPassivate(argAsString('p'));
      if (isSet('b'))
	SubroutineCall::noBlackBoxOptimism();
      if (isSet('V'))
	PrintManager::setVerbose();
      if (isSet('D'))
	CallGraphVertex::setInitializeDerivativeComponentsFlag();
      if (isSet('T'))
	DbgLoggerManager::instance()->addTags(argAsString('T'));
      myInputValidationFlag=isSet('v');
      if (isSet('F'))
	Symbol::setFrontEndDecorations(FrontEndDecorations::fromString(argAsString('F')));
      myConfiguredFlag=true;
    } 
  } 

  void AlgConfig::usage() { 
    for (RegFPList::iterator it=myUsageFPList.begin();
	 it!=myUsageFPList.end();
	 ++it)
      (*it)();
  } 

  void AlgConfig::ourUsage() {
    AlgFactoryManager::instance()->getAlgConfig()->myUsage();
  }

  void AlgConfig::myUsage() { 
    std::cout << "driver (" << myBuildStamp.c_str() << ") usage:" << std::endl
	      << myArgv[0]
	      << " -i <inputFile> -c <intrinsicsCatalogueFile> " << std::endl
	      << " common options: " << std::endl
	      << "             [-s <pathToSchema> ] " << std::endl
	      << "                 XAIF schema path, defaults to directory that contains the input file" << std::endl
	      << "             [-o <outputFile> ] [-d <debugOutputFile> ]" << std::endl
	      << "                 both default to std::cout" << std::endl
	      << "             [-N <nonInlinableIntrinsicsCatalogueFile> ]" << std::endl
	      << "             [-g <debugGroup> ]" << std::endl
	      << "                 with debugGroup >=0 the sum of any of: " << std::endl
	      << "                 "<< DbgGroup::printAll().c_str() << std::endl
	      << "                 defaults to 0(ERROR)" << std::endl
              << "             [-G <format>] debugging graphics format, where <format > is one of:" << std::endl
              << "                 ps - postscript format displayed with ghostview (default)" << std::endl 
              << "                 svg - scalable vector graphics format displayed in firefox unless the -B switch is used to specify another SVG viewer" << std::endl 
              << "             [-B <executable>] svg viewer program, where <executable> is something like:" << std::endl
              << "                 firefox (default)" << std::endl 
              << "                 google-chrome" << std::endl 
              << "             [-T \"<list of tags to narrow debug output>\" ]" << std::endl 
              << "                 space separated list enclosed in double quotes" << std::endl
              << "             [-p \"<list of symbols to forcibly passivate>\" ]" << std::endl 
              << "                 space separated list enclosed in double quotes" << std::endl
              << "             [-b] pessimistic assumptions for black box routines" << std::endl 
	      << "             [-v] validate the input against the schema" << std::endl
	      << "             [-V] verbose xaif output" << std::endl
	      << "             [-D] initialize derivative components of active variables" << std::endl
	      << "             [-F <style> ]" << std::endl
	      << "                 front-end decoration style, where <style> is one of: " << std::endl
	      << "                 "<< FrontEndDecorations::printAll().c_str() << std::endl
	      << "                 defaults to OPEN64_STYLE" << std::endl
	      << "             [-h] print this help message" << std::endl
      ;
  } 

  const std::string& AlgConfig::getInputFileName() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return myInputFileName; 
  } 

  bool AlgConfig::getInputValidationFlag() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return myInputValidationFlag; 
  } 

  const std::string& AlgConfig::getIntrinsicsFileName() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return myIntrinsicsFileName; 
  } 

  const std::string& AlgConfig::getNIIntrinsicsFileName() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return myNIIntrinsicsFileName; 
  } 

  const std::string& AlgConfig::getSchemaPath() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return mySchemaPath; 
  } 

  const std::string& AlgConfig::getOutFileName() const { 
    if (!myConfiguredFlag)
      THROW_LOGICEXCEPTION_MACRO("AlgConfig::config() has not been called"); 
    return myOutFileName; 
  } 

} 
                                                                    
