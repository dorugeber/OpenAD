#ifndef _ALGCONFIG_INCLUDE_
#define _ALGCONFIG_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>
#include <iostream>
#include "xaifBooster/utils/inc/CommandLineParser.hpp"

/**  
 * to be used in derived classes  for virtual single inheritance 
 * NAMING CONVENTION: ParentAlgConfigNameSpace = name space of the base class
 *                    OurNameSpace  = our own namespace
 */
#define DERIVED_V_ALG_CONFIG_DECL_MACRO(ParentAlgConfigNameSpace,OurNameSpace) \
  namespace OurNameSpace {						\
									\
    class AlgConfig : virtual public ParentAlgConfigNameSpace::AlgConfig{ \
									\
    public:								\
									\
      static void ourUsage();						\
      void myUsage();							\
									\
      static void ourConfig();						\
      void myConfig();							\
									\
    protected:								\
									\
      friend class AlgFactoryManager;					\
									\
      AlgConfig(int argc,						\
		char** argv,						\
		const std::string& buildStamp);				\
									\
    };									\
									\
  }

/** 
 * to be used in dervied classes for non-virtual single inheritance
 * NAMING CONVENTION: ParentAlgConfigNameSpace = name space of the base class
 *                    OurNameSpace  = our own namespace
 */
#define DERIVED_ALG_CONFIG_DECL_MACRO(ParentAlgConfigNameSpace,OurNameSpace) \
  namespace OurNameSpace {						\
									\
    class AlgConfig : public ParentAlgConfigNameSpace::AlgConfig{	\
									\
    public:								\
									\
      static void ourUsage();						\
      void myUsage();							\
									\
      static void ourConfig();						\
      void myConfig();							\
									\
    protected:								\
									\
      friend class AlgFactoryManager;					\
									\
      AlgConfig(int argc,						\
		char** argv,						\
		const std::string& buildStamp);				\
									\
    };									\
									\
  }

/** 
 * to be used in the derived classes implementation 
 */
#define STATIC_ALG_CONFIG_FUNC_DEF_MACRO				\
  void AlgConfig::ourConfig() {						\
    dynamic_cast<AlgFactoryManager*>(AlgFactoryManager::instance())->getAlgConfig()->myConfig(); \
  }									\
									\
  void AlgConfig::ourUsage() {						\
    dynamic_cast<AlgFactoryManager*>(AlgFactoryManager::instance())->getAlgConfig()->myUsage(); \
  }

namespace xaifBooster { 
  
  /** 
   * configuration and usage for this transformation 
   */
  class AlgConfig : public CommandLineParser { 

  public:

    virtual ~AlgConfig(){};

    void usage();

    /**
     * We separate the parsing/configuration 
     * step from the construction because 
     * we want to throw exceptions when 
     * something is not correctly specified
     * and we should not throw exceptions from the constructor.
     * On the other hand we have to avoid running config twice 
     * because we populate hashmaps etc.  We do have virtual 
     * inheritance though and if it wasn't for the need 
     * to throw exceptions it could easily be done in the 
     * constructor. Here we resort to a static guard 
     * to avoid running things twice. 
     */
    void config();

    const std::string& getInputFileName() const; 
    bool getInputValidationFlag() const; 
    const std::string& getIntrinsicsFileName() const; 
    const std::string& getNIIntrinsicsFileName() const; 
    const std::string& getSchemaPath() const; 
    const std::string& getOutFileName() const; 

  protected:

    friend class AlgFactoryManager; 
    
    /** 
     * to be instantiated by the respective AlgFactoryManager
     */
    AlgConfig(int argc, 
	      char** argv,
	      const std::string& buildStamp);

    std::string getSwitches();

    typedef void(*RegFP)();
    typedef std::list<RegFP> RegFPList;
    
    /** 
     * to contain the list of static ourConfig methods
     * according to class hierarchy (populated in registerIt)
     */
    RegFPList myConfigFPList;

    /** 
     * to contain the list of static ourUsage methods
     * according to class hierarchy (populated in registerIt)
     */
    RegFPList myUsageFPList;

    /**
     * collect all the commang line swithches here
     * done in registerIt
     */
    std::string mySwitches;
     
    /** 
     * for switches, configuration and usage 
     * we want to collect the contributations of this 
     * and all derived classes. However, switches should 
     * be collected only once per class and config and usage 
     * should be run only once per class even in the case 
     * of multiple virtual inheritance (i.e. diamond pattern).
     * So we mimic the behavior by adding static methods (ourUsage/ourConfig) 
     * that are reimplemented in every derived class in 
     * a list (does not work for our puropose with non-static methods)
     * via calling registerIt in the respective constructors. 
     * Each static method gets the respective AlgConfig instance from 
     * AlgFactoryManager and invokes the non-static myUsage/myConfig.
     * Thus, usage/config can invoke all the static methods by going through the 
     * the respective list and we have the once-per-class invocation 
     * that we want despite the diamond pattern. 
     */
    void registerIt(RegFP theConfig,
		    RegFP theUsage,
		    const std::string& switches);

    /** 
     * static method to be used with registerIt
     * calls myUsage on the instance supplied by AlgFactoryManager
     */
    static void ourUsage(); 

    void myUsage();

    /** 
     * static method to be used with registerIt
     * calls myConfig on the instance supplied by AlgFactoryManager
     */
    static void ourConfig();

    void myConfig();

  private: 

    std::string myInputFileName; 
    std::string myIntrinsicsFileName; 
    std::string myNIIntrinsicsFileName; 
    std::string mySchemaPath; 
    std::string myOutFileName;
    std::string myBuildStamp;
    bool myConfiguredFlag; 
    bool myInputValidationFlag; 
    
  }; 
  
} // end of namespace xaifBooster

#endif
