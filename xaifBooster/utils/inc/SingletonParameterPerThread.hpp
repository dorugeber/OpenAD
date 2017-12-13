#ifndef _SINGLETONPARAMETERPERTHREAD_INCLUDE_
#define _SINGLETONPARAMETERPERTHREAD_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <stack>

namespace xaifBooster {  

  /** 
   * \todo incomplete implementation
   */
  template <class ParameterType>
  class SingletonParameterPerThread { 

  public:

    ParameterType& get();

    void set(ParameterType&);

    static SingletonParameterPerThread& instance();

  private:

    SingletonParameterPerThread();

    ~SingletonParameterPerThread();

    /** 
     * no def 
     */
    SingletonParameterPerThread(const SingletonParameterPerThread&);
 
    /** 
     * no def 
     */
    SingletonParameterPerThread& operator=(const SingletonParameterPerThread&);

    ParameterType* myParameter_p;

    /**
     * static requires unique instance, 
     * hence no implied instantiation of the 
     * implementation
     * would have to be replace by a 
     * map keyed with the thread it ...
     */
    static SingletonParameterPerThread* ourInstance_p;

  }; // end of class SingletonParameterPerThread

} // end of namespace 

#include "xaifBooster/utils/inc/TemplateImpl/SingletonParameterPerThread.cpp"

#endif
