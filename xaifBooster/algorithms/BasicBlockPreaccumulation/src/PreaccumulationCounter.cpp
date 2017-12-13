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
#include <sstream>

#include "xaifBooster/utils/inc/LogicException.hpp"

#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PreaccumulationCounter.hpp"

namespace xaifBooster { 
 
  xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::PreaccumulationMetric_E
  PreaccumulationCounter::ourPreaccumulationMetric = xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::SCARCITY_METRIC;

  xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E
  PreaccumulationCounter::ourAwarenessLevel = xaifBoosterCrossCountryInterface::AwarenessLevel::UNIT_AWARENESS;

  void
  PreaccumulationCounter::setPreaccumulationMetric(xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::PreaccumulationMetric_E aMetric) {
    ourPreaccumulationMetric = aMetric;
  } // end PreaccumulationCounter::setPreaccumulationMetric()

  void
  PreaccumulationCounter::setAwarenessLevel(xaifBoosterCrossCountryInterface::AwarenessLevel::AwarenessLevel_E anAwarenessLevel) {
    ourAwarenessLevel = anAwarenessLevel;
  } // end PreaccumulationCounter::setAwarenessLevel()

  PreaccumulationCounter::PreaccumulationCounter() : 
    myNumMultiplications(0), 
    myNumAdditions(0),
    myNumDivisions(0),
    myNumSubtractions(0),
    myNumEdges(0),
    myNumNonunitEdges(0),
    myNumNonconstantEdges(0) {
  } // end PreaccumulationCounter::PreaccumulationCounter()
  
  unsigned int PreaccumulationCounter::getNumMultiplications() const {
    return myNumMultiplications;
  } // end PreaccumulationCounter::getNumMultiplications()
  
  unsigned int PreaccumulationCounter::getNumAdditions() const {
    return myNumAdditions;
  } // end PreaccumulationCounter::getNumAdditions()
  
  unsigned int PreaccumulationCounter::getNumDivisions() const {
    return myNumDivisions;
  } // end PreaccumulationCounter::getNumDivisions()
  
  unsigned int PreaccumulationCounter::getNumSubtractions() const {
    return myNumSubtractions;
  } // end PreaccumulationCounter::getNumSubtractions()
  
  unsigned int PreaccumulationCounter::getNumEdges() const {
    return myNumEdges;
  } // end PreaccumulationCounter::getNumEdges()
  
  unsigned int PreaccumulationCounter::getNumNonunitEdges() const {
    return myNumNonunitEdges;
  } // end PreaccumulationCounter::getNumNonunitEdges()
  
  unsigned int PreaccumulationCounter::getNumNonconstantEdges() const {
    return myNumNonconstantEdges;
  } // end PreaccumulationCounter::getNumNonconstantEdges()
  
  void PreaccumulationCounter::numMultiplicationsInc(unsigned int by) {
    myNumMultiplications += by;
  } // end PreaccumulationCounter::numMultiplicationsInc()
  
  void PreaccumulationCounter::numAdditionsInc(unsigned int by) {
    myNumAdditions += by;
  } // end PreaccumulationCounter::numAdditionsInc()
  
  void PreaccumulationCounter::numDivisionsInc(unsigned int by) {
    myNumDivisions += by;
  } // end PreaccumulationCounter::numDivisionsInc()
  
  void PreaccumulationCounter::numSubtractionsInc(unsigned int by) {
    myNumSubtractions += by;
  } // end PreaccumulationCounter::numSubtractionsInc(unsigned int by)
  
  void PreaccumulationCounter::numEdgesInc(unsigned int by) {
    myNumEdges += by;
  } // end PreaccumulationCounter::numEdgesInc()
  
  void PreaccumulationCounter::numNonunitEdgesInc(unsigned int by) {
    myNumNonunitEdges += by;
  } // end PreaccumulationCounter::numNonunitEdgesInc()
  
  void PreaccumulationCounter::numNonconstantEdgesInc(unsigned int by) {
    myNumNonconstantEdges += by;
  } // end PreaccumulationCounter::numNonconstantEdgesInc()
  
  void PreaccumulationCounter::reset() {
    numMultiplicationsReset();
    numAdditionsReset();
    numDivisionsReset();
    numEdgesReset();
    numNonunitEdgesReset();
    numNonconstantEdgesReset();
  }
  
  void PreaccumulationCounter::numMultiplicationsReset() {
    myNumMultiplications = 0;
  } // end PreaccumulationCounter::numMultiplicationsReset()
  
  void PreaccumulationCounter::numAdditionsReset() {
    myNumAdditions = 0;
  } // end PreaccumulationCounter::numAdditionsReset()
  
  void PreaccumulationCounter::numDivisionsReset() {
    myNumDivisions = 0;
  } // end PreaccumulationCounter::numDivisionsReset()
 
  void PreaccumulationCounter::numSubtractionsReset() {
    myNumSubtractions = 0;
  } // end PreaccumulationCounter::numSubtractionsReset()
 
  void PreaccumulationCounter::numEdgesReset() {
    myNumEdges = 0;
  } // end PreaccumulationCounter::numEdgesReset()

  void PreaccumulationCounter::numNonunitEdgesReset() {
    myNumNonunitEdges = 0;
  } // end PreaccumulationCounter::numNonunitEdgesReset()

  void PreaccumulationCounter::numNonconstantEdgesReset() {
    myNumNonconstantEdges = 0;
  } // end PreaccumulationCounter::numNonconstantEdgesReset()
 
  std::string PreaccumulationCounter::debug () const {
    std::ostringstream out;
    out << "PreaccumulationCounter[" << this 
	<< ",Multiplications=" << myNumMultiplications
	<< ",Additions=" << myNumAdditions
	<< ",Divisions=" << myNumDivisions
	<< ",Subtractions=" << myNumSubtractions
	<< ",NumEdges=" << myNumEdges
	<< ",NumNonunitEdges=" << myNumNonunitEdges
	<< ",NumNonconstantEdges=" << myNumNonconstantEdges
	<< "]" << std::ends;
    return out.str();
  } // end of Symbol::debug

  bool PreaccumulationCounter::isMoreScarceThan(const PreaccumulationCounter& anotherCounter) const {
    unsigned int myNumNontrivialEdges, theirNumNontrivialEdges;
    switch (ourAwarenessLevel) {
      case xaifBoosterCrossCountryInterface::AwarenessLevel::NO_AWARENESS: {
        myNumNontrivialEdges = myNumEdges;
        theirNumNontrivialEdges = anotherCounter.getNumEdges();
        break;
      } // end NO_AWARENESS
      case xaifBoosterCrossCountryInterface::AwarenessLevel::UNIT_AWARENESS: {
        myNumNontrivialEdges = myNumNonunitEdges;
        theirNumNontrivialEdges = anotherCounter.getNumNonunitEdges();
        break;
      } // end UNIT_AWARENESS
      case xaifBoosterCrossCountryInterface::AwarenessLevel::CONSTANT_AWARENESS: {
        myNumNontrivialEdges = myNumNonconstantEdges;
        theirNumNontrivialEdges = anotherCounter.getNumNonconstantEdges();
        break;
      } // end CONSTANT_AWARENESS
    } // end switch (ourAwarenessLevel)
    if (myNumNontrivialEdges < theirNumNontrivialEdges)
      return true;
    if (myNumNontrivialEdges > theirNumNontrivialEdges)
      return false;
    // at this point, they are equally scarce
    return hasFewerOpsThan(anotherCounter);
  } // end isMoreScarceThan()

  bool PreaccumulationCounter::hasFewerOpsThan(const PreaccumulationCounter& anotherCounter) const {
    if ((myNumMultiplications + myNumDivisions)
      < (anotherCounter.getNumMultiplications() + anotherCounter.getNumDivisions()))
      return true;
    if ((myNumMultiplications + myNumDivisions)
      > (anotherCounter.getNumMultiplications() + anotherCounter.getNumDivisions()))
      return false;
    // at this point we know that mults+divides are equal
    if ((myNumAdditions + myNumSubtractions)
      < (anotherCounter.getNumAdditions() + anotherCounter.getNumSubtractions()))
      return true;
    if ((myNumAdditions + myNumSubtractions)
      > (anotherCounter.getNumAdditions() + anotherCounter.getNumSubtractions()))
      return false;
    // at this point we know that they are equal in terms of ops
    return isMoreScarceThan(anotherCounter);
  } // end PreaccumulationCounter::hasFewerOpsThan()

  bool PreaccumulationCounter::operator<(const PreaccumulationCounter& anotherCounter) const {
    // this should prevent an infinite loop
    if (*this == anotherCounter)
      return false;
    // at this point we know that they are different in some way (ops, scarcity, or both)
    switch (ourPreaccumulationMetric) {
      case xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::SCARCITY_METRIC:
        return this->isMoreScarceThan(anotherCounter);
        break;
      case xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::OPERATIONS_METRIC:
        return this->hasFewerOpsThan(anotherCounter);
        break;
      default:
        THROW_LOGICEXCEPTION_MACRO("PreaccumulationCounter::operator<(const PreaccumulationCounter &b): ourPreaccumulationMetric is set to the unknown value "
                                   << xaifBoosterBasicBlockPreaccumulation::PreaccumulationMetric::toString(ourPreaccumulationMetric));
        break;
    } // end switch (ourPreaccumulationMetric)
  }

  bool PreaccumulationCounter::operator==(const PreaccumulationCounter& anotherCounter) const {
    // compare the relevant edge count
    switch (ourAwarenessLevel) {
      case xaifBoosterCrossCountryInterface::AwarenessLevel::NO_AWARENESS:
        if (this->getNumEdges() != anotherCounter.getNumEdges())
          return false;
        break;
      case xaifBoosterCrossCountryInterface::AwarenessLevel::UNIT_AWARENESS:
        if (this->getNumNonunitEdges() != anotherCounter.getNumNonunitEdges())
          return false;
        break;
      case xaifBoosterCrossCountryInterface::AwarenessLevel::CONSTANT_AWARENESS:
        if (this->getNumNonconstantEdges() != anotherCounter.getNumNonconstantEdges())
          return false;
        break;
      default:
        THROW_LOGICEXCEPTION_MACRO("PreaccumulationCounter::operator==(const PreaccumulationCounter& anotherCounter): ourAwarenessLevel is set to the unknown value "
                                   << xaifBoosterCrossCountryInterface::AwarenessLevel::toString(ourAwarenessLevel));
        break;
    } // end switch (ourAwarenessLevel)

    // compare operations counts
    if ((this->getNumMultiplications() + this->getNumDivisions())
     != (anotherCounter.getNumMultiplications() + anotherCounter.getNumDivisions()))
      return false;
    if ((this->getNumAdditions() + this->getNumSubtractions())
     != (anotherCounter.getNumAdditions() + anotherCounter.getNumSubtractions()))
      return false;
    return true;
  } // end PreaccumulationCounter::operator==()

  void PreaccumulationCounter::incrementBy(const PreaccumulationCounter &anotherCounter) {
    myNumMultiplications += anotherCounter.myNumMultiplications;
    myNumAdditions += anotherCounter.myNumAdditions;
    myNumDivisions += anotherCounter.myNumDivisions;
    myNumSubtractions += anotherCounter.myNumSubtractions;
    myNumEdges += anotherCounter.myNumEdges;
    myNumNonunitEdges += anotherCounter.myNumNonunitEdges;
    myNumNonconstantEdges += anotherCounter.myNumNonconstantEdges;
  } // end PreaccumulationCounter::incrementBy()

} 
