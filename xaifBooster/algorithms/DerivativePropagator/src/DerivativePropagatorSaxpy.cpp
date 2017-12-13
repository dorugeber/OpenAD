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

#include "xaifBooster/system/inc/Argument.hpp"
#include "xaifBooster/system/inc/Constant.hpp"

#include "xaifBooster/algorithms/DerivativePropagator/inc/DerivativePropagatorSaxpy.hpp"

using namespace xaifBooster;

namespace xaifBoosterDerivativePropagator { 

  const std::string DerivativePropagatorSaxpy::ourXAIFName("xaif:Saxpy");
  const std::string DerivativePropagatorSaxpy::our_SAX_XAIFName("xaif:Sax");
  const std::string DerivativePropagatorSaxpy::our_myA_XAIFName("xaif:A");
  const std::string DerivativePropagatorSaxpy::our_myX_XAIFName("xaif:X");
  const std::string DerivativePropagatorSaxpy::our_myTarget_XAIFName("xaif:Y");
  const std::string DerivativePropagatorSaxpy::our_myAX_XAIFName("xaif:AX");

  DerivativePropagatorSaxpy::DerivativePropagatorSaxpy(const Variable& theA,
						       const Variable& theX,
						       const Variable& theY) : 
    DerivativePropagatorEntry(theY),
    useAsSaxFlag(false) { 
    AX* theAX_p=new AX();
    myAXPList.push_back(theAX_p);
    Argument* theArgument_p=new Argument();
    theArgument_p->setId(1);
    theAX_p->myA.supplyAndAddVertexInstance(*theArgument_p);
    theA.copyMyselfInto(theArgument_p->getVariable());
    theX.copyMyselfInto(theAX_p->myX);
    theAX_p->myX.setId(1);
    theAX_p->myX.setDerivFlag();
  }

  void DerivativePropagatorSaxpy::addAX(const Variable& theA,
					const Variable& theX) { 
    AX* theAX_p=new AX();
    myAXPList.push_back(theAX_p);
    Argument* theArgument_p=new Argument();
    theArgument_p->setId(1);
    theAX_p->myA.supplyAndAddVertexInstance(*theArgument_p);
    theA.copyMyselfInto(theArgument_p->getVariable());
    theX.copyMyselfInto(theAX_p->myX);
    theAX_p->myX.setId(1);
    theAX_p->myX.setDerivFlag();
  } 

  DerivativePropagatorSaxpy::DerivativePropagatorSaxpy(const Constant& theA,
						       const Variable& theX,
						       const Variable& theY) : 
    DerivativePropagatorEntry(theY),
    useAsSaxFlag(false) { 
    AX* theAX_p=new AX();
    myAXPList.push_back(theAX_p);
    ExpressionVertex& theConstant(theA.createCopyOfMyself());
    theAX_p->myA.supplyAndAddVertexInstance(theConstant);
    theX.copyMyselfInto(theAX_p->myX);
    theAX_p->myX.setId(1);
    theAX_p->myX.setDerivFlag();
  }


  void DerivativePropagatorSaxpy::addAX(const Constant& theA,
					const Variable& theX) { 
    AX* theAX_p=new AX();
    myAXPList.push_back(theAX_p);
    ExpressionVertex& theConstant(theA.createCopyOfMyself());
    theAX_p->myA.supplyAndAddVertexInstance(theConstant);
    theX.copyMyselfInto(theAX_p->myX);
    theAX_p->myX.setId(1);
    theAX_p->myX.setDerivFlag();
  } 
  
  DerivativePropagatorSaxpy::~DerivativePropagatorSaxpy() { 
    for (AXPList::iterator i=myAXPList.begin();
	 i!=myAXPList.end();
	 ++i) 
      if (*i)
	delete *i;
  } 

  void DerivativePropagatorSaxpy::useAsSax() { 
    useAsSaxFlag=true;
  }

  void
  DerivativePropagatorSaxpy::printXMLHierarchy(std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"; 
    if (useAsSaxFlag) 
      os << our_SAX_XAIFName.c_str();
    else
      os << ourXAIFName.c_str(); 
    os << ">" 
       << std::endl; 
    for (AXPList::const_iterator i=myAXPList.begin();
	 i!=myAXPList.end();
	 ++i) 
      printAXMemberXMLHierarchy(**i,os);
    printMemberXMLHierarchy(myTarget,our_myTarget_XAIFName,os);
    os << pm.indent() 
       << "</";
    if (useAsSaxFlag) 
      os << our_SAX_XAIFName.c_str();
    else
      os << ourXAIFName.c_str(); 
    os << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagatorSaxpy::printXMLHierarchy

  void
  DerivativePropagatorSaxpy::printAXMemberXMLHierarchy(const AX& theAX,
						       std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << our_myAX_XAIFName.c_str()
       << ">" 
       << std::endl; 
    printMemberXMLHierarchy(theAX.myA,our_myA_XAIFName,os);
    printMemberXMLHierarchy(theAX.myX,our_myX_XAIFName,os);
    os << pm.indent() 
       << "</"
       << our_myAX_XAIFName.c_str()
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagatorSaxpy::printXMLHierarchy

  void
  DerivativePropagatorSaxpy::printMemberXMLHierarchy(const Variable& theVariable,
						     const std::string& aName,
						     std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << aName.c_str() 
       << " "
       << Variable::our_myDerivFlag_XAIFName.c_str()
       << "=\""
       << theVariable.getDerivFlag()
       << "\""
       << ">" 
       << std::endl; 
    theVariable.printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << aName.c_str()
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagatorSaxpy::printXMLHierarchy

  void
  DerivativePropagatorSaxpy::printMemberXMLHierarchy(const Expression& theA,
						     const std::string& aName,
						     std::ostream& os) const { 
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent()
       << "<"
       << aName.c_str() 
       << ">" 
       << std::endl; 
    theA.printXMLHierarchyImpl(os);
    os << pm.indent() 
       << "</"
       << aName.c_str()
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } // end of DerivativePropagatorSaxpy::printXMLHierarchy

  std::string DerivativePropagatorSaxpy::debug () const { 
    std::ostringstream out;
    out << "DerivativePropagatorSaxpy[" << this 
	<< DerivativePropagatorEntry::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  } // end of DerivativePropagatorSaxpy::debug

  void  DerivativePropagatorSaxpy::getFactors(FactorList& theFactorList) const { 
    for (AXPList::const_iterator i=myAXPList.begin();
	 i!=myAXPList.end();
	 ++i) { 
      // get the Expression
      const Expression& theA((*i)->myA);
      if(theA.numVertices()!=1) 
	THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorSaxpy::getVariables: not defined for factor expressions");
      const ExpressionVertex& theVertex(*(theA.vertices().first));
      Factor aFactor;
      aFactor.setSource((*i)->myX);
      if (theVertex.isArgument()) { 
	aFactor.setVariable(dynamic_cast<const Argument&>(theVertex).getVariable());
	theFactorList.push_back(aFactor);
      }
      else { 
	// This cast shouldn't fail or something else is seriously wrong
	aFactor.setConstant(dynamic_cast<const Constant&>(theVertex));
	theFactorList.push_back(aFactor);
      } 
    }
  } 

  bool DerivativePropagatorSaxpy::isIncremental()const { 
    return !useAsSaxFlag;
  }  

  bool
  DerivativePropagatorSaxpy::hasExpression(const Expression& anExpression) const {
    for (AXPList::const_iterator axPI = myAXPList.begin(); axPI != myAXPList.end(); ++axPI)
      if ((*axPI)->myA.hasExpression(anExpression)
       || (*axPI)->myX.hasExpression(anExpression))
        return true;
    return DerivativePropagatorEntry::hasExpression(anExpression);
  }

  const xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall&
  DerivativePropagatorSaxpy::asInlinableSubroutineCall() const {
	  if (!myInlinableSubroutineCall_p) {
		  if (isIncremental()) {
			  myInlinableSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("saxpy");
		  }
		  else {
			  myInlinableSubroutineCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("sax");
		  }
		  myInlinableSubroutineCall_p->setId("asInlinableSubroutineCall");
		  unsigned short position=1;
		  std::string suffix;
     	  bool inlinable=true;
		  for (AXPList::const_iterator it=myAXPList.begin();
				  it!=myAXPList.end(); ++it) {
			  ConcreteArgument& theA=myInlinableSubroutineCall_p->addConcreteArgument(position++);
			  Expression::ReturnOfSingleVariable sVar((*it)->myA.singleVariable());
			  if (sVar.hasIt()) {
				  sVar.getIt().copyMyselfInto(theA.getArgument().getVariable());
				  SymbolShape::SymbolShape_E effShape=theA.getArgument().getVariable().getEffectiveShape();
				  if (effShape!=SymbolShape::SCALAR && sVar.getIt().hasArrayAccess())
					  inlinable=false;
				  suffix+="_"+SymbolShape::toShortString(effShape);
			  }
			  else {
				  Expression::ReturnOfSingleConstant sCon((*it)->myA.singleConstant());
				  if (sCon.hasIt()) {
					  Constant& aConst(theA.makeConstant(sCon.getIt().getType()));
					  aConst.setFromString(sCon.getIt().toString());
					  suffix+="_"+SymbolShape::toShortString(SymbolShape::SCALAR);
				  }
				  else {
					  THROW_LOGICEXCEPTION_MACRO("DerivativePropagatorSaxpy::asInlinableSubroutineCall: no logic to handle factor expression");
				  }
			  }
			  ConcreteArgument& theX=myInlinableSubroutineCall_p->addConcreteArgument(position++);
			  (*it)->myX.copyMyselfInto(theX.getArgument().getVariable());
			  SymbolShape::SymbolShape_E effShape=theX.getArgument().getVariable().getEffectiveShape();
			  if (effShape!=SymbolShape::SCALAR && (*it)->myX.hasArrayAccess())
				  inlinable=false;
			  suffix+="_"+SymbolShape::toShortString(effShape);
		  }
		  ConcreteArgument& target=myInlinableSubroutineCall_p->addConcreteArgument(position);
		  getTarget().copyMyselfInto(target.getArgument().getVariable());
		  SymbolShape::SymbolShape_E effShape=getTarget().getEffectiveShape();
		  if (effShape!=SymbolShape::SCALAR && getTarget().hasArrayAccess())
			  inlinable=false;
		  suffix+="_"+SymbolShape::toShortString(effShape);
     	  if (inlinable)
     		  myInlinableSubroutineCall_p->appendSuffix(suffix);
	  }
	  return *myInlinableSubroutineCall_p;
  }

}

