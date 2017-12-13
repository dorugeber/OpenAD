// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgConfig.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/AlgFactoryManager.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/Sequence.hpp"

namespace xaifBoosterBasicBlockPreaccumulation { 

  AlgConfig::AlgConfig(int argc, 
		       char** argv,
		       const std::string& buildStamp) :
    xaifBooster::AlgConfig(argc,argv,buildStamp),
    xaifBoosterLinearization::AlgConfig(argc,argv,buildStamp) {
    registerIt(&ourConfig,&ourUsage,"nCAmQaHMRS");
  } 

  STATIC_ALG_CONFIG_FUNC_DEF_MACRO

  void AlgConfig::myConfig() { 
    if (isSet('n')) 
      xaifBoosterBasicBlockPreaccumulation::Sequence::permitNarySax();
    if (isSet('C'))
      xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::setRuntimeCounters();
    if (isSet('A')) {
      xaifBoosterCrossCountryInterface::AwarenessLevel::checkValid(xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E(argAsInt('A')));
      xaifBoosterCrossCountryInterface::Elimination::setAwarenessLevel(xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E(argAsInt('A')));
      PreaccumulationCounter::setAwarenessLevel(xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E(argAsInt('A')));
    } // end A
    if (isSet('m'))
      xaifBoosterCrossCountryInterface::Elimination::setAllowMaintainingFlag();
    if (isSet('Q')) 
      xaifBoosterBasicBlockPreaccumulation::Sequence::permitAliasedLHSs();
    if (isSet('a'))
      xaifBoosterBasicBlockPreaccumulation::Sequence::useRandomizedHeuristics();
    if (isSet('M')) {
      PreaccumulationMetric::checkValid(PreaccumulationMetric::PreaccumulationMetric_E(argAsInt('M')));
      xaifBoosterBasicBlockPreaccumulation::Sequence::setPreaccumulationMetric(PreaccumulationMetric::PreaccumulationMetric_E(argAsInt('M')));
      PreaccumulationCounter::setPreaccumulationMetric(PreaccumulationMetric::PreaccumulationMetric_E(argAsInt('M')));
    } // end M
    if (isSet('R')) 
      xaifBoosterBasicBlockPreaccumulation::Sequence::useReroutings();
    if (isSet('S')) 
      xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::oneGraphPerStatement();
    if (isSet('H'))
      xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::hideDPsAsICs();
  }

  void AlgConfig::myUsage() { 
    std::cout << " BasicBlockPreaccumulation options: " << std::endl
	      << "             [-n] allow n-ary sax operations" << std::endl
	      << "             [-H] hide derivative propagators as inlinable calls" << std::endl
              << "             [-C] turn on runtime counters"  << std::endl
	      << "             [-A <level>] set heuristic awareness of unit/constant edges (0: no awareness, 1: unit awareness, 2: constant awareness), defaults to unit awareness" << std::endl
              << "             [-m] allow scarcity-preserving operations that maintain the nontrivial edge count (default is strict reduction)"  << std::endl
              << "             [-Q] turn off alias checking among LHSs in the same sequence (meant to be temporary, waiting on better alias analysis)"  << std::endl
              << "             [-a] run randomized heuristics in addition to deterministic ones" << std::endl
	      << "             [-M <0|1>] set the preaccumulation metric (0: minimize operations, 1: scarcity exploitation), defaults to scarcity" << std::endl
	      << "             [-R] activate preaccumulation heuristics that use reroutings (for scarcity exploitation only)" << std::endl
	      << "             [-S] limit computational graphs to single statements" << std::endl;
  } 

} // end of namespace xaifBooster
                                                                     


