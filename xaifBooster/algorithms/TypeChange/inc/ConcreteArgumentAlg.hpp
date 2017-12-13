#ifndef _XAIFBOOSTERTYPECHANGE_CONCRETEARGUMENTALG_INCLUDE_
#define _XAIFBOOSTERTYPECHANGE_CONCRETEARGUMENTALG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/ConcreteArgumentAlgBase.hpp"

using namespace xaifBooster;

namespace xaifBooster { 
  class ConcreteArgument;
}

namespace xaifBoosterTypeChange {  

  /** 
   * class to hold a replaced argument for 
   * cases of enforced conversions between active 
   * and passive types when there is a mismatch 
   * of types between the actual and the formal 
   * argument
   */
  class ConcreteArgumentAlg : public ConcreteArgumentAlgBase {

  public:
    
    ConcreteArgumentAlg(const ConcreteArgument& theContainingConcreteArgument);

    virtual ~ConcreteArgumentAlg();

    virtual void printXMLHierarchy(std::ostream& os) const;

    virtual std::string debug() const ;

    /**
     * replaces the Variable in this instance
     * \param aVariable is the replacement
     * \param entireArrayCopied if true, then 
     * the replacement will have all subscripts 
     * that are on the original instance 
     */
    void makeReplacement(const Variable& aVariable,
			 bool entireArrayCopied);

    bool hasReplacement() const;

    ConcreteArgument& getReplacement();

    bool hasPriorConversionConcreteArgument() const;

    ConcreteArgument& getPriorConversionConcreteArgument();

    void setPriorConversionConcreteArgument(ConcreteArgument& aConcreteArgument);

    bool hasPostConversionConcreteArgument() const;

    ConcreteArgument& getPostConversionConcreteArgument();

    void setPostConversionConcreteArgument(ConcreteArgument& aConcreteArgument);

  private: 

    /** 
     * no def
     */
    ConcreteArgumentAlg();

    /** 
     * no def
     */
    ConcreteArgumentAlg(const ConcreteArgumentAlg&);

    /** 
     * no def
     */
    ConcreteArgumentAlg& operator=(const ConcreteArgumentAlg&);

    /** 
     * my replacement, i.e. the converted argument
     * in the orginal call (not necessarily identical 
     * to the arguments in the conversion routines
     */
    ConcreteArgument* myReplacement_p;

    /** 
     * a reference to the old argument used in the prior conversion
     * we want to keep track of those in case of a later need 
     * to modify them, e.g. for storing/restoring indices
     */
    ConcreteArgument* myPriorConversionConcreteArgument_p;

    /** 
     * a reference to the old argument used in the post conversion
     * we want to keep track of those in case of a later need 
     * to modify them, e.g. for storing/restoring indices
     */
    ConcreteArgument* myPostConversionConcreteArgument_p;
    
  }; // end of class ConcreteArgumentAlg
 
} 
                                                                     
#endif
