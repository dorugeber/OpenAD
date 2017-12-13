// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/HeuristicParse.hpp"

namespace xaifBoosterMemOpsTradeoffPreaccumulation { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp),
    xaifBoosterBasicBlockPreaccumulation::AlgConfig(argc,argv,buildStamp) {
  } 

  std::string AlgConfig::getSwitches() { 
    return std::string(xaifBoosterBasicBlockPreaccumulation::AlgConfig::getSwitches()+"h");
  } 

  void AlgConfig::config() { 
    xaifBoosterBasicBlockPreaccumulation::AlgConfig::config();
    if (isSet('h'))
      MemOpsTradeoffPreaccumulation::HeuristicParse::fromString(argAsString('h'));
  } 

  void AlgConfig::usage() { 
    xaifBoosterBasicBlockPreaccumulation::AlgConfig::usage();
    std::cout << " MemOpsTradeoffPreaccumulation options: " << std::endl
	      << "             [-h <Heuristic List> ]" << std::endl
	      << "                 List must be in all caps, the first word must be either VERTEX, FACE, or EDGE," << std::endl
	      << "                 followed by applicable heuristics as noted in the readme file." << std::endl
	      << "                 all words are to be separated by a single space" << std::endl;
  } 

} // end of namespace xaifBooster
                                                                     


