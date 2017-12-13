#ifndef _XAIFBOOSTERBASICBLOCKPREACCUMULATION_PREACCUMULATIONCOUNTER_INCLUDE_
#define _XAIFBOOSTERBASICBLOCKPREACCUMULATION_PREACCUMULATIONCOUNTER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp"
#include <string>

#include "xaifBooster/algorithms/CrossCountryInterface/inc/AwarenessLevel.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationMetric.hpp"

namespace xaifBooster { 

  class PreaccumulationCounter  {
  private:
    
    unsigned int myNumMultiplications;
    unsigned int myNumAdditions;
    unsigned int myNumDivisions;
    unsigned int myNumSubtractions;
    unsigned int myNumEdges;
    unsigned int myNumNonunitEdges;
    unsigned int myNumNonconstantEdges;

    /**
     * no def
     */
    PreaccumulationCounter(const PreaccumulationCounter&);
	
    /**
     * no def
     */
    PreaccumulationCounter& operator=(const PreaccumulationCounter&); 

    /// the metric by which we measure the quality of a preaccumulation
    static xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::PreaccumulationMetric_E ourPreaccumulationMetric;

    /// the way we measure the scarcity properties of a remainder graph
    static xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E ourAwarenessLevel;

    bool isMoreScarceThan(const PreaccumulationCounter& anotherCounter) const;
    bool hasFewerOpsThan(const PreaccumulationCounter& anotherCounter) const;

  public:

    /**
     * Initializes the values of all counters to 0.
     */
    PreaccumulationCounter();

    /// defines the metric by which we measure the quality of a preaccumulation
    /** The goal of the preaccumulation is set by a command-line flag.
     *  The current default is to minimize operations (complete preaccumulation).
     */
    static void
    setPreaccumulationMetric(xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::PreaccumulationMetric_E aMetric);

    /// defines the way in which we measure the scarcity properties of a remainder graph
    /** The awareness (none/unit/constant) of edge properties is set by a command-line flag.
     *  The current default is to have no awareness (all edges are counted).
     */
    static void
    setAwarenessLevel(xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E anAwarenessLevel);

    unsigned int getNumMultiplications() const;
    unsigned int getNumAdditions() const;
    unsigned int getNumDivisions() const;
    unsigned int getNumSubtractions() const;
    unsigned int getNumEdges() const;
    unsigned int getNumNonunitEdges() const;
    unsigned int getNumNonconstantEdges() const;
    
    void numMultiplicationsInc(unsigned int by=1);
    void numAdditionsInc(unsigned int by=1);
    void numDivisionsInc(unsigned int by = 1);
    void numSubtractionsInc(unsigned int by = 1);
    void numEdgesInc(unsigned int by = 1);
    void numNonunitEdgesInc(unsigned int by = 1);
    void numNonconstantEdgesInc(unsigned int by = 1);

    /**
     * Calls each counter specific reset function.
     */
    void reset();

    /**
     * Resets the value of the specified counter to 0.
     */
    void numMultiplicationsReset();
    void numAdditionsReset();
    void numDivisionsReset();
    void numSubtractionsReset();
    void numEdgesReset();
    void numNonunitEdgesReset();
    void numNonconstantEdgesReset();

    std::string debug() const ;
    
    /**
     * see definition
     */
    bool operator<(const PreaccumulationCounter& anotherCounter) const;

    bool operator==(const PreaccumulationCounter& anotherCounter) const;

    /**
     * increments this counter by anotherCounter
     */
    void incrementBy(const PreaccumulationCounter &anotherCounter);

  }; 

} // end of namespace xaifBooster
                                                                     
#endif

