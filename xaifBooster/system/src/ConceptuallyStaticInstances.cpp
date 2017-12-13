// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"

namespace xaifBooster { 
  
  ConceptuallyStaticInstances* ConceptuallyStaticInstances::ourInstance_p(0);

  ConceptuallyStaticInstances::ConceptuallyStaticInstances() : 
    myCallGraph_p(0),
    myPrintVersion(PrintVersion::VIRTUAL) {
  }

  ConceptuallyStaticInstances::~ConceptuallyStaticInstances() {
    if (myCallGraph_p)
      delete myCallGraph_p;
  }


  ConceptuallyStaticInstances* 
  ConceptuallyStaticInstances::instance() { 
    if (!ourInstance_p)
      ourInstance_p=new ConceptuallyStaticInstances();
    return ourInstance_p;
  } 

  CallGraph& 
  ConceptuallyStaticInstances::getCallGraph() { 
    if (!myCallGraph_p)
      THROW_LOGICEXCEPTION_MACRO("ConceptuallyStaticInstances::getCallGraph: has not been created");
    return *myCallGraph_p;
  } 

  InlinableIntrinsicsCatalogue& 
  ConceptuallyStaticInstances::getInlinableIntrinsicsCatalogue() { 
    return myInlinableIntrinsicsCatalogue;
  }

  NonInlinableIntrinsicsCatalogue& 
  ConceptuallyStaticInstances::getNonInlinableIntrinsicsCatalogue() { 
    return myNonInlinableIntrinsicsCatalogue;
  }

  void ConceptuallyStaticInstances::createCallGraph(const std::string& aSchemaInstance,
						    const std::string& anXAIFInstance,
						    const std::string& aSchemaLocation,
						    const std::string& aPrefix) { 
    if (myCallGraph_p)
      THROW_LOGICEXCEPTION_MACRO("ConceptuallyStaticInstances::createCallGraph: already created");
    myCallGraph_p=new CallGraph(aSchemaInstance,
				anXAIFInstance,
				aSchemaLocation,
				aPrefix);
    myUniversalNameCreator.setBaseName(aPrefix+"Symbol");
    myTemporaryVariableNameCreator.setBaseName(aPrefix+"tmp");
    myTypeChangeVariableNameCreator.setBaseName(aPrefix+"tyc");
    myDelayVariableNameCreator.setBaseName(aPrefix+"dly");
    myAuxiliaryVariableNameCreator.setBaseName(aPrefix+"aux");
    myLinearizationVariableNameCreator.setBaseName(aPrefix+"lin");
    myAccumulationVariableNameCreator.setBaseName(aPrefix+"acc");
    myPropagationVariableNameCreator.setBaseName(aPrefix+"prp");
  } 

  PrintVersion::PrintVersion_E 
  ConceptuallyStaticInstances::getPrintVersion() const { 
    return myPrintVersion;
  }

  void ConceptuallyStaticInstances::setPrintVersion(PrintVersion::PrintVersion_E aPrintVersion) { 
    myPrintVersion=aPrintVersion;
  }

  const NameCreator&
  ConceptuallyStaticInstances::getUniversalNameCreator() const {
    return myUniversalNameCreator;
  } // end ConceptuallyStaticInstances::getUniversalNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getTemporaryVariableNameCreator() const { 
    return myTemporaryVariableNameCreator;
  } // end ConceptuallyStaticInstances::getTemporaryVariableNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getTypeChangeVariableNameCreator() const { 
    return myTypeChangeVariableNameCreator;
  } // end ConceptuallyStaticInstances::getTypeChangeVariableNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getDelayVariableNameCreator() const { 
    return myDelayVariableNameCreator;
  } // end ConceptuallyStaticInstances::getDelayVariableNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getAuxiliaryVariableNameCreator() const { 
    return myAuxiliaryVariableNameCreator;
  }

  const NameCreator&
  ConceptuallyStaticInstances::getLinearizationVariableNameCreator() const { 
    return myLinearizationVariableNameCreator;
  } // end ConceptuallyStaticInstances::getLinearizationVariableNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getAccumulationVariableNameCreator() const { 
    return myAccumulationVariableNameCreator;
  } // end ConceptuallyStaticInstances::getAccumulationVariableNameCreator()

  const NameCreator&
  ConceptuallyStaticInstances::getPropagationVariableNameCreator() const { 
    return myPropagationVariableNameCreator;
  } // end ConceptuallyStaticInstances::getPropagationVariableNameCreator()

  const TraversalStack& ConceptuallyStaticInstances::getTraversalStack() const {
    return myTraversalStack;
  } // end ConceptuallyStaticInstances::getTraversalStack() const

  TraversalStack& ConceptuallyStaticInstances::getTraversalStack() {
    return myTraversalStack;
  } // end ConceptuallyStaticInstances::getTraversalStack()

} // end namespace xaifBooster

