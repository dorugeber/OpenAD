// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/ArgumentAlgFactory.hpp"

namespace xaifBooster { 

  const std::string Argument::ourXAIFName("xaif:VariableReference");
  const std::string Argument::our_myId_XAIFName("vertex_id");

  Argument::Argument(bool makeAlgorithm) { 
    myExpressionVertexAlgBase_p=0;
    if (makeAlgorithm)
      myExpressionVertexAlgBase_p=ArgumentAlgFactory::instance()->makeNewAlg(*this);
  } 

  ExpressionVertex& Argument::createCopyOfMyself(bool withAlgorithm) const { 
    Argument* aNewArgument_p=new Argument(withAlgorithm);
    myVariable.copyMyselfInto(aNewArgument_p->myVariable);
    aNewArgument_p->setId(getId());
    return *aNewArgument_p;
  }
 
  void
  Argument::printXMLHierarchy(std::ostream& os) const {
    if (myExpressionVertexAlgBase_p
	&& 
	! ConceptuallyStaticInstances::instance()->getPrintVersion()==PrintVersion::SYSTEM_ONLY)
      getArgumentAlgBase().printXMLHierarchy(os);
    else
      printXMLHierarchyImpl(os);
  } // end of printXMLHierarchy

  void
  Argument::printXMLHierarchyImpl(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << ourXAIFName.c_str()
       << " " 
       << our_myId_XAIFName.c_str() 
       << "=\"" 
       << getId().c_str()
       << "\""
       << " ";
    myVariable.printVariableReferenceXMLAttributes(os);
    os << ">" 
       << std::endl;
    myVariable.printXMLHierarchy(os);
    os << pm.indent()
       << "</"
       << ourXAIFName
       << ">"
       << std::endl;
    pm.releaseInstance();
  } // end of Argument::printXMLHierarchy

  std::string Argument::debug () const { 
    std::ostringstream out;
    out << "Argument[" << ExpressionVertex::debug().c_str()
	<< ",myVariable=" << myVariable.debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of Argument::debug

  Variable& 
  Argument::getVariable() { 
    return myVariable;
  }

  const Variable& 
  Argument::getVariable() const { 
    return myVariable;
  }

  std::string Argument::equivalenceSignature() const { 
    return myVariable.equivalenceSignature();
  }

  ArgumentAlgBase&
  Argument::getArgumentAlgBase() const {
    if (!myExpressionVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("Argument::getArgumentAlgBase: not set");
    return dynamic_cast<ArgumentAlgBase&>(*myExpressionVertexAlgBase_p);
  } // end getArgumentAlgBase

  void Argument::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
    getArgumentAlgBase().genericTraversal(anAction_c);
  } // end traversalToChildren 

  bool Argument::isArgument() const { 
    return true;
  } // end Argument::isArgument 

} // end of namespace xaifBooster 
