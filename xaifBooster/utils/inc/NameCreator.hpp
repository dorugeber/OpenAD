#ifndef _NAMECREATOR_INCLUDE_
#define _NAMECREATOR_INCLUDE_
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

  /** 
   * simple class for name creation
   * Current use is, that all SymbolTable instances
   * share one instance of NameCreator
   */
  class NameCreator { 

  public:

    NameCreator ();

    ~NameCreator(){};
    
    /** 
     * returns a name consisting of 
     * myBaseName and the current value 
     * of myCounter appended
     */
    std::string makeName() const ;

    /**
     * aBaseName should itself be a valid name 
     * in the context where this is used, e.g. 
     * aBaseName should not start with a digit
     */
    void setBaseName(const std::string& aBaseName);

  private:

    /** 
     * no def
     */
    NameCreator(const NameCreator&);

    /** 
     * no def
     */
    NameCreator& operator=(const NameCreator&);

    /** 
     * my counter starts with 0
     */
    mutable unsigned int myCounter;
    
    /** 
     * the string to which the respective 
     * value of myCounter is appended
     */
    std::string myBaseName;

  }; // end of class NameCreator
 
} // end of namespace xaifBooster
                                                                     
#endif
