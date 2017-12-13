#ifndef _EXPLICITJACOBIANPARTIAL_INCLUDE_
#define _EXPLICITJACOBIANPARTIAL_INCLUDE_
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

  class ExplicitJacobianPartial { 
    
  public: 

    ExplicitJacobianPartial();

    ~ExplicitJacobianPartial();

    static const std::string ourXAIFName;
    static const std::string our_myDep_XAIFName;
    static const std::string our_myInDep_XAIFName;
    static const std::string our_myPartial_XAIFName;

    void setInfo(unsigned short dep,
		 unsigned short indep,
		 unsigned short partial); 
    
    unsigned short getDep() const;
    unsigned short getInDep() const;
    unsigned short getPartial() const;

    const std::string& getDerivAction() const; 

    
  private:
    
    /**
     * no def
     */
    ExplicitJacobianPartial(const ExplicitJacobianPartial&);

    /**
     * no def
     */
    ExplicitJacobianPartial& operator=(const ExplicitJacobianPartial&);

    unsigned short myDep;

    unsigned short myInDep;

    unsigned short myPartial;

  }; 
  
} 

#endif
