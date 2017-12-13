// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/ConcreteArgument.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"

namespace xaifBoosterTypeChange {  

  ConcreteArgumentAlg::ConcreteArgumentAlg(const ConcreteArgument& theContainingConcreteArgument) : 
    ConcreteArgumentAlgBase(theContainingConcreteArgument),
    myReplacement_p(0),
    myPriorConversionConcreteArgument_p(0),
    myPostConversionConcreteArgument_p(0) { 
  }

  ConcreteArgumentAlg::~ConcreteArgumentAlg() { 
    if (myReplacement_p)
      delete myReplacement_p;
  } 

  void ConcreteArgumentAlg::printXMLHierarchy(std::ostream& os) const { 
    if (myReplacement_p)
      myReplacement_p->printXMLHierarchyImpl(os);
    else
      getContaining().printXMLHierarchyImpl(os);
  }

  std::string 
  ConcreteArgumentAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterTypeChange::ConcreteArgumentAlg["
	<< this 
	<< ","
 	<< ConcreteArgumentAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void ConcreteArgumentAlg::makeReplacement(const Variable& aVariable,
					    bool entireArrayCopied) { 
    if(myReplacement_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::makeReplacement : already have a replacement");
    myReplacement_p=new ConcreteArgument(getContaining().getPosition());
    if (getContaining().annotationIsSet())
      myReplacement_p->setAnnotation(getContaining().getAnnotation());
    aVariable.copyMyselfInto(myReplacement_p->getArgument().getVariable());
    if (entireArrayCopied) { 
      // this means we have some shape discrepancy
      // and we need to attach the subscript of the original 
      // argument to the replacement if there are any subscripts
      if (getContaining().isArgument() 
	  && 
	  getContaining().getArgument().getVariable().hasArrayAccess()) {
	if (aVariable.hasArrayAccess())
	  THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::makeReplacement : temp variable already has indices");
	VariableVertex& newVariableVertex(getContaining().getArgument().getVariable().getArrayAccess().createCopyOfMyself());
	myReplacement_p->getArgument().getVariable().supplyAndAddVertexInstance(newVariableVertex);
	VariableEdge& theEdge(myReplacement_p->
			      getArgument().
			      getVariable().addEdge(dynamic_cast<const VariableVertex&>(myReplacement_p->getArgument().getVariable().getVariableSymbolReference()),
						    newVariableVertex));
	theEdge.setId(1);
      }
    }
  }
  
  bool ConcreteArgumentAlg::hasReplacement() const { 
    return (myReplacement_p)?true:false;
  } 
  
  ConcreteArgument& ConcreteArgumentAlg::getReplacement() { 
    if(!myReplacement_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::getReplacement : has no replacement");
    return *myReplacement_p;
  } 

  bool ConcreteArgumentAlg::hasPriorConversionConcreteArgument() const { 
    return (myPriorConversionConcreteArgument_p)?true:false;
  } 

  ConcreteArgument& ConcreteArgumentAlg::getPriorConversionConcreteArgument() { 
    if(!myPriorConversionConcreteArgument_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::getPriorConversionConcreteArgument:  nothing there ");
    return *myPriorConversionConcreteArgument_p;
  } 

  void ConcreteArgumentAlg::setPriorConversionConcreteArgument(ConcreteArgument& aConcreteArgument) { 
    if(myPriorConversionConcreteArgument_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::setPriorConversionConcreteArgument: already set");
    myPriorConversionConcreteArgument_p=&aConcreteArgument;
  } 
  
  bool ConcreteArgumentAlg::hasPostConversionConcreteArgument() const { 
    return (myPostConversionConcreteArgument_p)?true:false;
  } 

  ConcreteArgument& ConcreteArgumentAlg::getPostConversionConcreteArgument() { 
    if(!myPostConversionConcreteArgument_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::getPostConversionConcreteArgument:  nothing there ");
    return *myPostConversionConcreteArgument_p;
  } 

  void ConcreteArgumentAlg::setPostConversionConcreteArgument(ConcreteArgument& aConcreteArgument) { 
    if(myPostConversionConcreteArgument_p)
      THROW_LOGICEXCEPTION_MACRO("ConcreteArgumentAlg::setPostConversionConcreteArgument: already set");
    myPostConversionConcreteArgument_p=&aConcreteArgument;
  } 
  
} // end of namespace 
