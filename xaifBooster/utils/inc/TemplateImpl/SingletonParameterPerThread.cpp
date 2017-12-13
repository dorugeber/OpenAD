// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#ifndef  _SINGLETONPARAMETERPERTHREAD_INCLUDE_
#include "xaifBooster/utils/inc/SingletonParameterPerThread.hpp"
#endif

#include "xaifBooster/utils/inc/LogicException.hpp"

namespace xaifBooster {  

  template <class ParameterType> 
  SingletonParameterPerThread<ParameterType>* SingletonParameterPerThread<ParameterType>::ourInstance_p=0;

  template <class ParameterType>
  SingletonParameterPerThread<ParameterType>::SingletonParameterPerThread() : 
    myParameter_p(0){ 
  } 

  template <class ParameterType>
  SingletonParameterPerThread<ParameterType>::~SingletonParameterPerThread() {}

  template <class ParameterType>
  SingletonParameterPerThread<ParameterType>& SingletonParameterPerThread<ParameterType>::instance() { 
    if (!ourInstance_p)
      ourInstance_p=new SingletonParameterPerThread();
    return *ourInstance_p;
  } 

  template <class ParameterType>
  ParameterType& 
  SingletonParameterPerThread<ParameterType>::get() { 
    if (!myParameter_p)
      THROW_LOGICEXCEPTION_MACRO("SingletonParameterPerThread<ParameterType>::get: not set");
    return *myParameter_p;
  } 

  template <class ParameterType>
  void
  SingletonParameterPerThread<ParameterType>::set(ParameterType& theParameter) { 
    myParameter_p=&theParameter;
  }

}
