// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/LogicException.hpp"
#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"

namespace xaifBooster { 

  const std::string ObjectWithAnnotation::our_myAnnotation_XAIFName("annotation");

  ObjectWithAnnotation::ObjectWithAnnotation() : myAnnotationFlag(false) { 
  } // end of ObjectWithAnnotation::ObjectWithAnnotation

  std::string ObjectWithAnnotation::debug () const { 
    std::ostringstream out;
    out << "ObjectWithAnnotation[" << this 
	<< ",myAnnotation=" << myAnnotation.c_str() 
	<< ",myAnnotationFlag=" << myAnnotationFlag
	<< "]" << std::ends;  
    return out.str();
  } // end of ObjectWithAnnotation::debug

  bool
  ObjectWithAnnotation::annotationIsSet() const { 
    return myAnnotationFlag;
  }

  void
  ObjectWithAnnotation::setAnnotation(const std::string& anAnnotation) { 
    if (myAnnotationFlag)
      THROW_LOGICEXCEPTION_MACRO("ObjectWithAnnotation::setAnnotation can't reset myAnnotation for "
				 << debug().c_str());
    myAnnotationFlag=true;
    myAnnotation=anAnnotation;
  } // end of ObjectWithAnnotation::ObjectWithAnnotation

  const std::string&
  ObjectWithAnnotation::getAnnotation() const { 
    if (!myAnnotationFlag)
      THROW_LOGICEXCEPTION_MACRO("ObjectWithAnnotation::getAnnotation myAnnotation not set for "
				 << debug().c_str());
    return myAnnotation;
  } // end of ObjectWithAnnotation::ObjectWithAnnotation

} 
