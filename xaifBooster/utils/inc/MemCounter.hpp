#ifndef _MEMCOUNTER_INCLUDE_
#define _MEMCOUNTER_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>

namespace xaifBooster { 

  class MemCounter  {
    private:
    /**
     *  All counters are used to count the number of opperations of their type.
     *  The operations being counted are defined by their name.
     */
    int myIntScalar;
    int myRealScalar;
    int myIntVector;
    int myRealVector;
    int myIntMatrix;
    int myRealMatrix;

	  
  public:

    /**
     * Initializes the values of all counters to 0.
     */
    MemCounter();
    
    MemCounter(const MemCounter&);
    /**
     * Retrieves values from the data structure as specified by the function name.
     */
    int getIntScaValue() const;
    int getRealScaValue() const;
    int getIntVecValue() const;
    int getRealVecValue() const;
    int getIntMatValue() const;
    int getRealMatValue() const;
    
    /**
     * Increments the value of the counter specified by 1.
     */
    void intScaInc();
    void realScaInc();
    void intVecInc();
    void realVecInc();
    void intMatInc();
    void realMatInc();

    void intScaInc(int);
    void realScaInc(int);
    void intVecInc(int);
    void realVecInc(int);
    void intMatInc(int);
    void realMatInc(int);

    /**
     * Calls each counter specific reset function.
     */
    void reset();

    /**
     * Resets the value of the specified counter to 0.
     */
    void intScaReset();
    void realScaReset();
    void intVecReset();
    void realVecReset();
    void intMatReset();
    void realMatReset();

    /**
     * Prints all the counters values on a single line.
     */
    std::string debug() const ;

    /**
     * Calls every counter's print function and the Total Print function.
     */
    void print() const;

    /**
     * Prints the value of the specified counter.
     */
    void intScaPrint() const;
    void realScaPrint() const;
    void intVecPrint() const;
    void realVecPrint() const;
    void intMatPrint() const;
    void realMatPrint() const;

    /**
     * Prints out the sum total of all the counters.
     */
    void totalPrint() const;

    /**
     * Overloads the = operator so that two Counter types can be set equal to
     * each other.
     */
    MemCounter& operator=(const MemCounter &p);

     /**
     * Overloads the > operator so that counter1 > counter2 if it has a greater
     * multiply value.  If the nultiply values are equal then counter1 > counter2
     * if it has more additions.
     */
//    bool operator>(const MemCounter &b) const;
    
    /**
     * Overloads the < operator so that counter1 < counter2 if it has a lesser
     * multiply value.  If the nultiply values are equal then counter1 < counter2
     * if it has fewer additions.
     */
//    bool operator<(const MemCounter &b) const;

    /*
     * Overloads the + operator so that counter1 = counter2 + counter3. The add
     * and multiply values are summed up seperatly.
     */
    MemCounter operator+(MemCounter &b);
  }; 
} // end of namespace xaifBooster
                                                                     
#endif

