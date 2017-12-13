// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/MemCounter.hpp"
#include <iostream>
#include <sstream>

namespace xaifBooster { 
 
  MemCounter::MemCounter()
  : myIntScalar(0), myRealScalar(0), myIntVector(0), myRealVector(0), myIntMatrix(0), myRealMatrix(0)
  {
  }
  
  int MemCounter::getIntMatValue() const {
    return myIntMatrix;
  }
  
  int MemCounter::getRealMatValue() const {
    return myRealMatrix;
  }

  int MemCounter::getIntVecValue() const {
    return myIntVector;
  }
  
  int MemCounter::getRealVecValue() const {
    return myRealVector;
  }

  int MemCounter::getIntScaValue() const {
    return myIntScalar;
  }
  
  int MemCounter::getRealScaValue() const {
    return myRealScalar;
  }

  void MemCounter::intMatInc() {
    myIntMatrix++;
  }

  void MemCounter::realMatInc() {
    myRealMatrix++;
  }
  
  void MemCounter::intVecInc() {
    myIntVector++;
  }
  
  void MemCounter::realVecInc() {
    myRealVector++;
  }

  void MemCounter::intScaInc() {
    myIntScalar++;
  }

  void MemCounter::realScaInc() {
    myRealScalar++;
  }

  void MemCounter::intScaInc(int count) {
      myIntScalar += count;
  }

  void MemCounter::realScaInc(int count) {
      myRealScalar += count;
  }

  void MemCounter::intVecInc(int count) {
      myIntVector += count;
  }
  
  void MemCounter::realVecInc(int count) {
      myRealVector += count;
  }

  void MemCounter::intMatInc(int count) {
      myIntMatrix += count;
  }
      
  void MemCounter::realMatInc(int count) {
      myRealMatrix += count;
  }
  
  void MemCounter::reset() {
    intScaReset();
    realScaReset();
    intVecReset();
    realVecReset();
    intMatReset();
    realMatReset();
  }
  
  void MemCounter::intMatReset() {
    myIntMatrix = 0;
  }

  void MemCounter::realMatReset() {
    myRealMatrix = 0;
  }
  
  void MemCounter::intVecReset() {
    myIntVector = 0;
  }
  
  void MemCounter::realVecReset() {
    myRealVector = 0;
  }
  
  void MemCounter::intScaReset() {
    myIntScalar = 0;
  }
  
  void MemCounter::realScaReset() {
    myRealScalar = 0;
  }

  std::string MemCounter::debug () const {
    std::ostringstream out;
    out << "Ints Sca=" << myIntScalar
        << " Vec=" << myIntVector
        << " Mat=" << myIntMatrix
        << " Reals Sca=" << myRealScalar
        << " Vec=" << myRealVector
        << " Mat=" << myRealMatrix
        << "]" << std::ends;
    return out.str();
  } // end of Symbol::debug


  void MemCounter::print() const {
    intScaPrint(); 
    realScaPrint();
    intVecPrint();
    realVecPrint(); 
    intMatPrint(); 
    realMatPrint();
    totalPrint();
  }
  
  void MemCounter::intScaPrint() const {
    std::cout << "The number of integer scalars was: " << myIntScalar << std::endl;
  }

  void MemCounter::realScaPrint() const {
    std::cout << "The number of real scalars was: " << myRealScalar << std::endl;
  }
  
  void MemCounter::intVecPrint() const {
    std::cout << "The number of integer vectors was: " << myIntVector << std::endl;
  }

  void MemCounter::realVecPrint() const {
    std::cout << "The number of real vectors was: " << myRealVector << std::endl;
  }
  
  void MemCounter::intMatPrint() const {
    std::cout << "The number of integer matricies was: " << myIntMatrix << std::endl;
  }
  
  void MemCounter::realMatPrint() const {
    std::cout << "The number of real matricies was: " << myRealMatrix << std::endl;
  }

  void MemCounter::totalPrint() const {
    std::cout << "The total number of memory uses was: " << myIntMatrix + myRealMatrix + myIntVector +myRealVector + myIntScalar + myRealScalar << std::endl;
  }
  
  MemCounter& MemCounter::operator=(const MemCounter &in) {
    myIntMatrix = in.myIntMatrix;
    myIntVector = in.myIntVector;
    myIntScalar = in.myIntScalar;
    myRealMatrix = in.myRealMatrix;
    myRealVector = in.myRealVector;
    myRealScalar = in.myRealScalar;
    return *this;    // Return ref for multiple assignment
  }

  

/*  bool Counter::operator>(const Counter &b) const
  { 
    if(this->getMulValue() > b.getMulValue())//check if mults greater
    {
      return true;
    }
    else if(this->getMulValue() < b.getMulValue()) //check if mults less
    {
      return false; 
    } 
    else //mults equal
    {
      if(this->getAddValue() > b.getAddValue()) //check if adds greater
      {
	return true;
      }
      else
      {
	return false;
      }
    }
  }
  
  bool Counter::operator<(const Counter &b) const
  {
    if(this->getMulValue() < b.getMulValue()) //check if mults less
    {
      return true;
    }
    else if(this->getMulValue() > b.getMulValue()) //check if mults greater
    {
      return false;
    }
    else
    {
      if(this->getAddValue() < b.getAddValue()) //check if adds less
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }*/

  MemCounter MemCounter::operator+(MemCounter &b)
  {
	  MemCounter temp;
	  temp.myIntScalar = myIntScalar + b.myIntScalar;
          temp.myRealScalar = myRealScalar + b.myRealScalar;
	  temp.myIntVector = myIntVector + b.myIntVector;
          temp.myRealVector = myRealVector + b.myRealVector;
	  temp.myIntMatrix = myIntMatrix + b.myIntMatrix;
          temp.myRealMatrix = myRealMatrix + b.myRealMatrix;
	  return temp;
  }
} 
