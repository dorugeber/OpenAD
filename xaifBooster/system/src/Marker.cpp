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
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/Marker.hpp"
#include "xaifBooster/system/inc/MarkerAlgFactory.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"

namespace xaifBooster { 

  const std::string Marker::ourXAIFName("xaif:Marker");
  const std::string Marker::our_myId_XAIFName("statement_id");
  const std::string Marker::our_myAnnotation_XAIFName("annotation");

  Marker::Marker(bool makeAlgorithm) : myAnnotationFlag(false) { 
    if (makeAlgorithm)
      myBasicBlockElementAlgBase_p=MarkerAlgFactory::instance()->makeNewAlg(*this); 
  } // end of Marker::Marker

  Marker::~Marker() { 
  } 

  MarkerAlgBase& 
  Marker::getMarkerAlgBase() const { 
    if (!myBasicBlockElementAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Marker::getMarkerAlgBase: not set");
    return dynamic_cast<MarkerAlgBase&>(*myBasicBlockElementAlgBase_p);
  }

  std::string Marker::debug () const { 
    std::ostringstream out;
    out << "Marker[" << BasicBlockElement::debug().c_str()
	<< ",myAnnotation=" << myAnnotation.c_str() 
	<< ",myAnnotationFlag=" << myAnnotationFlag
	<< "]" << std::ends;  
    return out.str();
  } // end of Marker::debug

  void
  Marker::setAnnotation(const std::string& anAnnotation) { 
    if (myAnnotationFlag)
      THROW_LOGICEXCEPTION_MACRO("Marker::setAnnotation can't reset myAnnotation");
    myAnnotationFlag=true;
    myAnnotation=anAnnotation;
  } // end of Marker::setAnnotation

  const std::string&
  Marker::getAnnotation() const { 
    if (!myAnnotationFlag)
      THROW_LOGICEXCEPTION_MACRO("Marker::getAnnotation myAnnotation not set");
    return myAnnotation;
  } // end of Marker::getAnnotation

  void Marker::printXMLHierarchy(std::ostream& os) const {
    if (myBasicBlockElementAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getMarkerAlgBase().printXMLHierarchy(os);
    else 
      printXMLHierarchyImpl(os);
  }

  void
  Marker::printXMLHierarchyImpl(std::ostream& os) const {
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << ourXAIFName
       << " "
       << our_myId_XAIFName
       << "=\"" 
       << getId() 
       << "\" " 
       << our_myAnnotation_XAIFName
       << "=\"" 
       << getAnnotation() 
       << "\"/>" 
       << std::endl;
    pm.releaseInstance(); 
  } // end of Marker::printXMLHierarchyImpl

} 
