// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <cctype>
#include <algorithm>

#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"
#include "xaifBooster/utils/inc/PrintManager.hpp"

#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/SubroutineCall.hpp"
#include "xaifBooster/system/inc/ArgumentList.hpp"
#include "xaifBooster/system/inc/ArrayAccess.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"
#include "xaifBooster/system/inc/BasicBlock.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"
#include "xaifBooster/system/inc/SubroutineNotFoundException.hpp"

#include "xaifBooster/algorithms/InlinableXMLRepresentation/inc/InlinableSubroutineCall.hpp"

#include "xaifBooster/algorithms/TypeChange/inc/SubroutineCallAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ConcreteArgumentAlg.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/MissingSubroutinesReport.hpp"
#include "xaifBooster/algorithms/TypeChange/inc/ControlFlowGraphAlg.hpp"

namespace xaifBoosterTypeChange {  

  std::list<std::string> SubroutineCallAlg::ourWrapperSubRoutineNameList;
  const std::string SubroutineCallAlg::ourConversionRoutineName("oad_convert");

  SubroutineCallAlg::SubroutineCallAlg(const SubroutineCall& theContainingSubroutineCall) : 
    SubroutineCallAlgBase(theContainingSubroutineCall) { 
  }

  SubroutineCallAlg::~SubroutineCallAlg() { 
  }

  void SubroutineCallAlg::printXMLHierarchy(std::ostream& os) const { 
    printXMLHierarchyImplWithAdjustments(os);
  }

  void SubroutineCallAlg::printXMLHierarchyImplWithAdjustments(std::ostream& os) const {
    mySaveValuesAcrossForTypeChange.printXMLHierarchy(os);
    for (PlainBasicBlock::BasicBlockElementList::const_iterator priorI=myPriorAdjustmentsList.begin();
	 priorI!=myPriorAdjustmentsList.end();
	 ++priorI) { 
      (*priorI)->printXMLHierarchy(os);
    } 
    printXMLHierarchyImpl(os);
    for (PlainBasicBlock::BasicBlockElementList::const_iterator postI=myPostAdjustmentsList.begin();
	 postI!=myPostAdjustmentsList.end();
	 ++postI) { 
      (*postI)->printXMLHierarchy(os);
    } 
  }

  void SubroutineCallAlg::printXMLHierarchyImpl(std::ostream& os) const { 
    // figure out the replacement symbol if needed
    const SymbolAlg& theSymbolAlg(dynamic_cast<const SymbolAlg&>(getContainingSubroutineCall().
								 getSymbolReference().getSymbol().getSymbolAlgBase()));
    const SymbolReference* theSymbolReference_p;
    if (theSymbolAlg.hasReplacementSymbolReference()) 
      theSymbolReference_p=&(theSymbolAlg.getReplacementSymbolReference());
    else
      theSymbolReference_p=&(getContainingSubroutineCall().getSymbolReference());
    PrintManager& pm=PrintManager::getInstance();
    os << pm.indent() 
       << "<"
       << SubroutineCall::ourXAIFName.c_str() 
       << " " 
       << SubroutineCall::our_myId_XAIFName.c_str() 
       << "=\"" 
       << getContainingSubroutineCall().getId().c_str()
       << "\" " 
       << SubroutineCall::our_symbolId_XAIFName.c_str() 
       << "=\"" 
       << theSymbolReference_p->getSymbol().getId().c_str()
       << "\" " 
       << SubroutineCall::our_scopeId_XAIFName.c_str() 
       << "=\"" 
       << theSymbolReference_p->getScope().getId().c_str()
       << "\" " 
       << ActiveUseType::our_attribute_XAIFName.c_str() 
       << "=\"" 
       << ActiveUseType::toString(getContainingSubroutineCall().getActiveUse()).c_str()
       << "\" " 
       << SubroutineCall::our_myFormalArgCount_XAIFName.c_str() 
       << "=\"" 
       << getContainingSubroutineCall().getFormalArgCount()
       << "\">" 
       << std::endl;
    for (SubroutineCall::ConcreteArgumentPList::const_iterator i=
	   getContainingSubroutineCall().getConcreteArgumentPList().begin();
	 i!=getContainingSubroutineCall().getConcreteArgumentPList().end();
	 ++i)
      (*i)->printXMLHierarchy(os);
    os << pm.indent() 
       << "</"
       << SubroutineCall::ourXAIFName.c_str() 
       << ">" 
       << std::endl;
    pm.releaseInstance();
  } 

  std::string 
  SubroutineCallAlg::debug() const { 
    std::ostringstream out;
    out << "xaifBoosterTypeChange::SubroutineCallAlg["
	<< this 
	<< ","
 	<< SubroutineCallAlgBase::debug().c_str()
	<< "]" << std::ends;  
    return out.str();
  }

  void SubroutineCallAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) { 
  } 

  void SubroutineCallAlg::initExternalCall(SymbolAlg& aSymbolAlg) {
    // we need to set it all up 
    aSymbolAlg.setExternal();
    // now try to figure out if this has hand written wrappers:
    for (std::list<std::string>::const_iterator aNameListI=ourWrapperSubRoutineNameList.begin();
	 aNameListI!=ourWrapperSubRoutineNameList.end();
	 ++aNameListI) { 
      // we have 2 issues, 
      // first case sensitivity
      std::string anInputName(*aNameListI);
      std::transform(anInputName.begin(),
		     anInputName.end(), 
		     anInputName.begin(), 
		     tolower);
      std::string aSymbolName(aSymbolAlg.getContaining().getId());
      std::transform(aSymbolName.begin(),
		     aSymbolName.end(), 
		     aSymbolName.begin(), 
		     tolower);
      // second the appendices added by the 
      // fortran front end.
      if (aSymbolName.find(anInputName)!=0)
	continue;
      // the tail should just be empty or contain underscores followed by digits
      std::string aSymbolNameTail(aSymbolName.substr(anInputName.size()));
      unsigned int position=0;
      while(position<aSymbolNameTail.size() && aSymbolNameTail[position]=='_')
	position++;
      while(position<aSymbolNameTail.size() && std::isdigit(aSymbolNameTail[position]))
	position++;
      if (position!=aSymbolNameTail.size())
	// this doesn't have the proper appendix
	continue;
      // now we we know this has a handwritten wrapper.
      aSymbolAlg.setHandCodedWrapper(getContainingSubroutineCall().getSymbolReference());
      aSymbolAlg.getActivityPattern().setSize(getContainingSubroutineCall().
					      getConcreteArgumentPList().
					      size());
      for (SubroutineCall::ConcreteArgumentPList::const_iterator concreteArgumentPI=
	     getContainingSubroutineCall().getConcreteArgumentPList().begin();
	   concreteArgumentPI!=getContainingSubroutineCall().getConcreteArgumentPList().end();
	   ++concreteArgumentPI) { 
	if (((*concreteArgumentPI)->isArgument())?
	    (*concreteArgumentPI)->getArgument().getVariable().getActiveType():
	    false) { 
	  aSymbolAlg.getActivityPattern().trackAt((*concreteArgumentPI)->getPosition());
	} 
      }// end for iterating through all concrete arguments 
    } // end for iterating through the list if hand written wrappers 
  } 

  void SubroutineCallAlg::handleExternalCall(const BasicBlock& theBasicBlock) { 
    // get the symbol's algorithm object
    SymbolAlg& theSymbolAlg(dynamic_cast<SymbolAlg&>(getContainingSubroutineCall().
						     getSymbolReference().getSymbol().getSymbolAlgBase()));
    if(!theSymbolAlg.isExternal()) { 
      initExternalCall(theSymbolAlg);
    } // end if - initial setup.
    else { 
      // this was initialized before
      // we do a consistency check if this is a handwritten wrapper
      if (theSymbolAlg.hasHandCodedWrapper()) { 
	// make the ActivityPattern for this call
	SignaturePattern aNewPattern;
	aNewPattern.setSize(getContainingSubroutineCall().
			    getConcreteArgumentPList().
			    size());
	if (aNewPattern.getSize()!=theSymbolAlg.getActivityPattern().getSize()) { 
	  THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::algorithm_action_1: argument count inconsistent between calls for "
				     << getContainingSubroutineCall().getSymbolReference().debug().c_str());
	} 
	for (SubroutineCall::ConcreteArgumentPList::const_iterator concreteArgumentPI=
	       getContainingSubroutineCall().getConcreteArgumentPList().begin();
	     concreteArgumentPI!=getContainingSubroutineCall().getConcreteArgumentPList().end();
	     ++concreteArgumentPI) { 
	  if (((*concreteArgumentPI)->isArgument())?
	      (*concreteArgumentPI)->getArgument().getVariable().getActiveType():
	      false) { 
	    aNewPattern.trackAt((*concreteArgumentPI)->getPosition());
	  } 
	}// end for iterating through all concrete arguments 
	if (aNewPattern!=theSymbolAlg.getActivityPattern()) { 
	  THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::handleExternalCall: inconsistent activity patterns in position(s) "
				     << aNewPattern.discrepancyPositions(theSymbolAlg.getActivityPattern()).c_str()
				     << " for call to subroutine " 
				     << getContainingSubroutineCall().getSymbolReference().debug().c_str());
	}
      }
    }
    if (!theSymbolAlg.hasHandCodedWrapper()) { 
      // we may need to add conversion routines
      for (SubroutineCall::ConcreteArgumentPList::const_iterator concreteArgumentPI=
	     getContainingSubroutineCall().getConcreteArgumentPList().begin();
	   concreteArgumentPI!=getContainingSubroutineCall().getConcreteArgumentPList().end();
	   ++concreteArgumentPI) { 
	if (((*concreteArgumentPI)->isArgument())?
	    (*concreteArgumentPI)->getArgument().getVariable().getActiveType():
	    false) {
	  MissingSubroutinesReport::reportConversion(getContainingSubroutineCall().getSymbolReference());
       	  // we need conversion to passive
	  addExternalConversion(**concreteArgumentPI,
				theBasicBlock);
	} 
      }
    }
  }

  void SubroutineCallAlg::algorithm_action_1() { 
    replaceArguments(true);
  }

  void SubroutineCallAlg::replaceArguments(bool withCopy) {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterTypeChange::SubroutineCallAlg::replaceArguments() " << debug().c_str());
    const ArgumentList::ArgumentSymbolReferencePList* anArgumentSymbolReferencePList_p(0); 
    const ControlFlowGraph* aCFG_p(0); 
    const BasicBlock& theBasicBlock(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentBasicBlockInstance());
    try { 
      // get the formal argument list; 
      anArgumentSymbolReferencePList_p=
	&((aCFG_p=&(ConceptuallyStaticInstances::instance()->
		    getCallGraph().
		    getSubroutineBySymbolReference(getContainingSubroutineCall().getSymbolReference()).
		    getControlFlowGraph()))->
	  getArgumentList().
	  getArgumentSymbolReferencePList());
    } 
    catch (const SubroutineNotFoundException& e) {
      MissingSubroutinesReport::report(e);
      handleExternalCall(theBasicBlock);
      return;
    } // end catch
    if (anArgumentSymbolReferencePList_p->size()!=getContainingSubroutineCall().getFormalArgCount())
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::algorithm_action_1: argument count mismatch "
				 << anArgumentSymbolReferencePList_p->size() 
				 << " (formal argument list length) vs. "
				 << getContainingSubroutineCall().getFormalArgCount()
				 << " (formalArgCount given for  the call) for "
				 << getContainingSubroutineCall().getSymbolReference().debug().c_str());
    ControlFlowGraphAlg& theControlFlowGraphAlg(dynamic_cast<ControlFlowGraphAlg&>(aCFG_p->getControlFlowGraphAlgBase()));
    for (SubroutineCall::ConcreteArgumentPList::const_iterator concreteArgumentPI=
	   getContainingSubroutineCall().getConcreteArgumentPList().begin();
	 concreteArgumentPI!=getContainingSubroutineCall().getConcreteArgumentPList().end();
	 ++concreteArgumentPI) { 
      if ((*concreteArgumentPI)->getPosition()>getContainingSubroutineCall().getFormalArgCount() 
	  || 
	  (*concreteArgumentPI)->getPosition()<1) { 
	THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::algorithm_action_1: argument position "
				   << (*concreteArgumentPI)->getPosition()
				   << " out of range [1,"
				   << getContainingSubroutineCall().getFormalArgCount()
				   << "] in the call to "
				   << getContainingSubroutineCall().getSymbolReference().debug().c_str());
      }
      bool concreteArgumentActive=((*concreteArgumentPI)->isArgument())?
	(*concreteArgumentPI)->getArgument().getVariable().getActiveType():false;
      bool formalArgumentActive=false; 
      ArgumentList::ArgumentSymbolReferencePList::const_iterator formalArgumentPI;
      for (formalArgumentPI=anArgumentSymbolReferencePList_p->begin();
	   formalArgumentPI!=anArgumentSymbolReferencePList_p->end();
	   ++formalArgumentPI) { 
	if((*concreteArgumentPI)->getPosition()==(*formalArgumentPI)->getPosition()) 
	  break;
      }
      if (formalArgumentPI==anArgumentSymbolReferencePList_p->end()) { 
	THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::algorithm_action_1: cannot find formal argument for position "
				   << (*concreteArgumentPI)->getPosition()
				   << " for "
				   << getContainingSubroutineCall().getSymbolReference().debug().c_str());
      }
      formalArgumentActive=(*formalArgumentPI)->getSymbol().getActiveTypeFlag();
      if (concreteArgumentActive!=formalArgumentActive) { 
	addConversion(**concreteArgumentPI,
		      **formalArgumentPI,
		      theBasicBlock,
		      withCopy);
      } 
      if ((*concreteArgumentPI)->isConstant()
	  && 
	  !formalArgumentActive)
	// if the argument is constant but the formal is active we 
	// need a conversion so it no longer is constant
	theControlFlowGraphAlg.getSomewhereConstPattern().trackAt((*concreteArgumentPI)->getPosition());
      if (!(*concreteArgumentPI)->isConstant())
	theControlFlowGraphAlg.getSomewhereVariablePattern().trackAt((*concreteArgumentPI)->getPosition());
    }// end for 
  } 
  
  void SubroutineCallAlg::addAllocation(const Variable& toBeAllocated,
					const ConcreteArgument& argumentToMatch,
					bool allocateSliceOfargumentToMatch) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSRCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_AllocateMatching");
    theSRCall_p->setId("TypeChange::SubroutineCallAlg::addAllocation");
    myPriorAdjustmentsList.push_back(theSRCall_p);
    // first argument
    toBeAllocated.copyMyselfInto(theSRCall_p->addConcreteArgument(1).getArgument().getVariable());
    // second argument 
    // if allocateSliceOfargumentToMatch is true this is deep copy and preserves the array indices
    argumentToMatch.copyMyselfInto(theSRCall_p->addConcreteArgument(2),allocateSliceOfargumentToMatch);
  }

  void SubroutineCallAlg::addShapeTest(const Variable& toBeAllocated,
				       const ConcreteArgument& argumentToMatch,
				       bool testSliceOfargumentToMatch) {
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* theSRCall_p=new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall("oad_ShapeTest");
    theSRCall_p->setId("TypeChange::SubroutineCallAlg::addShapeTest");
    myPostAdjustmentsList.push_back(theSRCall_p);
    // first argument
    toBeAllocated.copyMyselfInto(theSRCall_p->addConcreteArgument(1).getArgument().getVariable());
    // second argument 
    // if testSliceOfargumentToMatch is true this is deep copy and preserves the array indices
    argumentToMatch.copyMyselfInto(theSRCall_p->addConcreteArgument(2),testSliceOfargumentToMatch);
  }

  void SubroutineCallAlg::addConversion(const ConcreteArgument& theConcreteArgument,
					const ArgumentSymbolReference& aFormalArgumentSymbolReference,
					const BasicBlock& theBasicBlock,
					bool withCopy) { 
    SymbolShape::SymbolShape_E formalArgumentSymbolShape(aFormalArgumentSymbolReference.getSymbol().getSymbolShape());
    SymbolShape::SymbolShape_E tmpArgumentSymbolShape(formalArgumentSymbolShape); // to init
    bool passingPointer=false;
    bool passingSlice=false;
    // we handle the following cases (this is in effect FORTRAN specific until we recognize an agreed upon C/C++ array type)
    //  actual            | formal
    // ---------------------------------------------------------
    //  SCALAR            | SCALAR (copy scalar)
    //  SCALAR deref      | SCALAR (copy scalar)
    //  SCALAR deref      | non-SCALAR (implies pointer passing, copy entire array)
    //  F90 array (slice) | array of matching shape (copy slice, if possible)
    //
    // in the case of passing a pointer we have the following cases:
    // 1. concrete has lesser shape than the formal,
    //     call foo(vector,...) 
    //     subroutine foo(matrix,...)
    //       and the dimension is somehow known in foo
    //       and would refer to matrix(i,1) to reflect that only a vector was passed
    // 2. formal has lesser shape than the concrete,
    //    a: a  call foo (vector(i))  for a scalar concrete argument
    //    b: implicit reshaping, e.g. a call foo (matrix) for a vector concrete argument
    //       where it is assumed that 'matrix' occupies a consecutive 'n x m' bit in memory that can 
    //       be addressed by vector indices up to n*m
    //    in case a: we just convert the reduced data, i.e. vector(i)
    //    in case b: we need to convert all concrete data
    if (theConcreteArgument.isArgument()) {
      SymbolShape::SymbolShape_E concreteArgumentSymbolShape(theConcreteArgument.
						             getArgument().
						             getVariable().
						             getVariableSymbolReference().
						             getSymbol().
						             getSymbolShape());
      SymbolShape::SymbolShape_E concreteArgumentEffectiveShape(theConcreteArgument.
								getArgument().
								getVariable().
								getEffectiveShape());
      if (concreteArgumentEffectiveShape==SymbolShape::SCALAR) { 
	if (formalArgumentSymbolShape!=SymbolShape::SCALAR) {
	  passingPointer=true;
	  DBG_MACRO(DbgGroup::DATA, "SubroutineCallAlg::addConversion: passing to formal argument of shape "
		    << SymbolShape::toString(formalArgumentSymbolShape).c_str()
		    << " the concrete argument "
		    << theConcreteArgument.getArgument().getVariable().getVariableSymbolReference().getSymbol().plainName().c_str()
		    << " dererferenced to a SCALAR on line "
		    << getContainingSubroutineCall().getLineNumber());	
	  tmpArgumentSymbolShape=concreteArgumentSymbolShape;
	}
	else { 
	  tmpArgumentSymbolShape=concreteArgumentEffectiveShape; // always SCALAR here
	}
      }
      else{
	passingSlice=true; // also works when entire arrays are passed
	tmpArgumentSymbolShape=concreteArgumentEffectiveShape; 
      }
    }
    // this is the extra temporary that replaces the original argument
    Variable theTempVar;
    bool doAllocation=makeTempSymbol(theConcreteArgument,
				     aFormalArgumentSymbolReference.getSymbol(),
				     aFormalArgumentSymbolReference.getScope(),
				     theTempVar,
				     tmpArgumentSymbolShape,
				     false);
    ConcreteArgumentAlg& theConcreteArgumentAlg(dynamic_cast<ConcreteArgumentAlg&>(theConcreteArgument.getConcreteArgumentAlgBase()));
    theConcreteArgumentAlg.makeReplacement(theTempVar,passingPointer);
    if (doAllocation) {
      addAllocation(theTempVar,theConcreteArgument,passingSlice);
    }
    if (!withCopy)
      return; 
    // Pre-call conversion
    // skip if the concrete argument has intent out
    if (aFormalArgumentSymbolReference.getIntent() != IntentType::OUT_ITYPE) {
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* 
	thePriorCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(ourConversionRoutineName));
      myPriorAdjustmentsList.push_back(thePriorCall_p);
      thePriorCall_p->setId("SubroutineCallAlg::addConversion prior");
      theTempVar.copyMyselfInto(thePriorCall_p->addConcreteArgument(1).getArgument().getVariable());
      ConcreteArgument& theSecondPriorConcreteArg(thePriorCall_p->addConcreteArgument(2));
      theConcreteArgument.copyMyselfInto(theSecondPriorConcreteArg,!passingPointer);
      theConcreteArgumentAlg.setPriorConversionConcreteArgument(theSecondPriorConcreteArg);
      // may have to adjust upper bounds
      //       if (theConcreteArgument.isArgument() && !passingPointer) {
      // 	theSecondPriorConcreteArg.getArgument().getVariable().adjustUpperBounds((int)(tmpArgumentSymbolShape));
      //       }
    }

    // skip post-call back-conversion if
    // 1) the concrete argument is a constant; or
    // 2) the formal argument (callee context) has intent in; or
    // 3) the concrete argument is a formal argument (in caller context) with intent in
    //
    //1):
    if (!theConcreteArgument.isArgument())
      return;
    //2):
    if (aFormalArgumentSymbolReference.getIntent() == IntentType::IN_ITYPE)
      return;
    //3):
    const ControlFlowGraph& theCFG(ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph());
    ControlFlowGraph::FormalResult theCallerFormalResult = theCFG.hasFormal(theConcreteArgument.getArgument().getVariable().getVariableSymbolReference());
    if (theCallerFormalResult.first) { // this concrete argument happens to be a formal argument in the caller context
      // get the positional argument  from the list 
      for (ArgumentList::ArgumentSymbolReferencePList::const_iterator listI=theCFG.getArgumentList().getArgumentSymbolReferencePList().begin();
	   listI!=theCFG.getArgumentList().getArgumentSymbolReferencePList().end();
	   ++listI) {
	if (theCallerFormalResult.second==(*listI)->getPosition() // match the position
	    && 
	    IntentType::IN_ITYPE==(*listI)->getIntent()) { // look at its itype
	  return;
	}
      }
    }
    // if we successfully get to this point, then create the post-call conversion
    if (doAllocation) {
      addShapeTest(theTempVar,theConcreteArgument,passingSlice);
    } 
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* 
      thePostCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(ourConversionRoutineName));
    myPostAdjustmentsList.push_back(thePostCall_p);
    thePostCall_p->setId("SubroutineCallAlg::addConversion post");
    ConcreteArgument& theFirstPostConcreteArg(thePostCall_p->addConcreteArgument(1));
    theConcreteArgumentAlg.setPostConversionConcreteArgument(theFirstPostConcreteArg);
    Variable& theInlineVariablePostRes(theFirstPostConcreteArg.getArgument().getVariable());
    theConcreteArgument.getArgument().getVariable().copyMyselfInto(theInlineVariablePostRes,!passingPointer);
    //     if (!passingPointer)
    //       theInlineVariablePostRes.adjustUpperBounds((int)(tmpArgumentSymbolShape));
    Variable& theInlineVariablePostArg(thePostCall_p->addConcreteArgument(2).getArgument().getVariable());
    theTempVar.copyMyselfInto(theInlineVariablePostArg);
    if (!passingPointer && theConcreteArgument.getArgument().getVariable().hasArrayAccess()) {
      handleArrayAccessIndices(theConcreteArgument,
			       theBasicBlock);
    }
  } // end SubroutineCallAlg::addConversion()
  

  void SubroutineCallAlg::addExternalConversion(const ConcreteArgument& theConcreteArgument,
						const BasicBlock& theBasicBlock) { 
    const SymbolReference& theActualSymbolReference(theConcreteArgument.getArgument().getVariable().
						    getVariableSymbolReference());

    // prior call
    xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* 
      thePriorCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(ourConversionRoutineName));
    thePriorCall_p->setId("SubroutineCallAlg::addExternalConversion prior");
    // this is the extra temporary that replaces the original argument
    Variable& theTempVar(thePriorCall_p->addConcreteArgument(1).getArgument().getVariable());
    bool doAllocation=makeTempSymbol(theConcreteArgument,
				     theActualSymbolReference.getSymbol(),
				     theActualSymbolReference.getScope(),
				     theTempVar,
				     theActualSymbolReference.getSymbol().getSymbolShape(),
				     true);
    if (doAllocation) {
      addAllocation(theTempVar,theConcreteArgument,false);
    }
    // now that we have the allocation call - if needed - we can push the conversion to the list
    myPriorAdjustmentsList.push_back(thePriorCall_p);
    ConcreteArgument& theSecondPriorConcreteArg(thePriorCall_p->addConcreteArgument(2));
    theConcreteArgument.copyMyselfInto(theSecondPriorConcreteArg,false);
    ConcreteArgumentAlg& theConcreteArgumentAlg(dynamic_cast<ConcreteArgumentAlg&>(theConcreteArgument.getConcreteArgumentAlgBase()));
    theConcreteArgumentAlg.makeReplacement(theTempVar,true);
    theConcreteArgumentAlg.setPriorConversionConcreteArgument(theSecondPriorConcreteArg);
    if (theConcreteArgument.isArgument()) {
      if (doAllocation) {
	addShapeTest(theTempVar,theConcreteArgument,false);
      } 
      // post call only if not a constant
      xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall* 
	thePostCall_p(new xaifBoosterInlinableXMLRepresentation::InlinableSubroutineCall(ourConversionRoutineName));
      myPostAdjustmentsList.push_back(thePostCall_p);
      thePostCall_p->setId("SubroutineCallAlg::addExternalConversion post");
      ConcreteArgument& theFirstPostConcreteArg(thePostCall_p->addConcreteArgument(1));
      theConcreteArgumentAlg.setPostConversionConcreteArgument(theFirstPostConcreteArg);
      Variable& theInlineVariablePostRes(theFirstPostConcreteArg.getArgument().getVariable());
      theConcreteArgument.getArgument().getVariable().copyMyselfInto(theInlineVariablePostRes,false);
      Variable& theInlineVariablePostArg(thePostCall_p->addConcreteArgument(2).getArgument().getVariable());
      theTempVar.copyMyselfInto(theInlineVariablePostArg);
      if (theConcreteArgument.getArgument().getVariable().hasArrayAccess()) {
	handleArrayAccessIndices(theConcreteArgument,
				 theBasicBlock);
      }
    }
  } 
  
  bool SubroutineCallAlg::makeTempSymbol(const ConcreteArgument& theConcreteArgument,
					 const Symbol& formalArgumentSymbol,
					 const Scope&, // formalArgumentScope  when we finally get around it
					 Variable& aVariable,
					 SymbolShape::SymbolShape_E tmpSymbolShape,
					 bool forcePassive) { 
    Scope& theCurrentCfgScope (ConceptuallyStaticInstances::instance()->getTraversalStack().getCurrentCallGraphVertexInstance().getControlFlowGraph().getScope());
    // create a new symbol and add a new VariableSymbolReference in the Variable
    Symbol& theNewVariableSymbol (theCurrentCfgScope.getSymbolTable().addUniqueSymbol(ConceptuallyStaticInstances::instance()->getTypeChangeVariableNameCreator(),
										      SymbolKind::VARIABLE,
										      formalArgumentSymbol.getSymbolType(),
										      tmpSymbolShape,
										      (forcePassive) ? false
										      : formalArgumentSymbol.getActiveTypeFlag())
				  );
    theNewVariableSymbol.setFrontEndType(formalArgumentSymbol.getFrontEndType());
    VariableSymbolReference* theNewVariableSymbolReference_p(new VariableSymbolReference(theNewVariableSymbol,
											 theCurrentCfgScope));
    short addedDimBounds=0;
    if (theConcreteArgument.isArgument()){
      short skipDimBoundsCount=SymbolShape::difference(theConcreteArgument.getArgument().getVariable().
						       getVariableSymbolReference().getSymbol().getSymbolShape(),
						       tmpSymbolShape);
      // preserve dimension information from the concrete argument if any:
      const Symbol& theConcreteArgumentSymbol(theConcreteArgument.getArgument().getVariable().getVariableSymbolReference().getSymbol());
      if (theConcreteArgumentSymbol.hasDimensionBounds()) { 
	const Symbol::DimensionBoundsPList& aDimensionBoundsPList(theConcreteArgumentSymbol.getDimensionBoundsPList());
	switch(DimensionBounds::getIndexOrder()) { 
	case IndexOrder::ROWMAJOR: // c and c++
	  for (Symbol::DimensionBoundsPList::const_iterator li=aDimensionBoundsPList.begin();
	       (li!=aDimensionBoundsPList.end());
	       ++li, --skipDimBoundsCount) { 
	    if (skipDimBoundsCount<=0){
	      // e.g. the difference between a three-tensor and a vector is 2 so we skip over the 2 leftmost dimensions.
	      theNewVariableSymbol.addDimensionBounds((*li)->getLower(),
						      (*li)->getUpper());
              addedDimBounds++;
            }
	  }
	  break;
	case IndexOrder::COLUMNMAJOR: { // fortran
	  for (Symbol::DimensionBoundsPList::const_iterator li=aDimensionBoundsPList.begin();
	       (li!=aDimensionBoundsPList.end());
	       ++li) { 
	    if (addedDimBounds<tmpSymbolShape) { 
	      theNewVariableSymbol.addDimensionBounds((*li)->getLower(),
						      (*li)->getUpper());
	      addedDimBounds++;
	    }
	  }
	  break;
	}     
	default:
	  THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::makeTempSymbol: no logic for "
				     << IndexOrder::toString(DimensionBounds::getIndexOrder()).c_str());
	  break;
	}
      }
    }
    theNewVariableSymbolReference_p->setId("1");
    theNewVariableSymbolReference_p->setAnnotation("xaifBoosterTypeChange::SubroutineCallAlg::makeTempVariable");
    aVariable.supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
    aVariable.getAliasMapKey().setTemporary();
    aVariable.getDuUdMapKey().setTemporary();
    return tmpSymbolShape!=addedDimBounds;
  } // end SubroutineCallAlg::makeTempSymbol()

  void SubroutineCallAlg::addWrapperNames(const std::string& theSpaceSeparatedNames) { 
    if (theSpaceSeparatedNames.empty())
      THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::addWrapperNames: no names given ");
    std::string::size_type startPosition=0,endPosition=0;
    std::string::size_type totalSize(theSpaceSeparatedNames.size());
    while (startPosition<=totalSize && endPosition<=totalSize) { 
      startPosition=theSpaceSeparatedNames.find_first_not_of(' ',startPosition);
      endPosition=theSpaceSeparatedNames.find_first_of(' ',startPosition);
      ourWrapperSubRoutineNameList.push_back(theSpaceSeparatedNames.substr(startPosition,
									   endPosition));
      startPosition=endPosition;
    } 
  }

  bool
  SubroutineCallAlg::hasExpression(const Expression& anExpression) const {
    // check myPriorAdjustmentsList
    for (PlainBasicBlock::BasicBlockElementList::const_iterator priorI = myPriorAdjustmentsList.begin();
	 priorI != myPriorAdjustmentsList.end(); ++priorI)
      if ((*priorI)->hasExpression(anExpression))
	return true;
    // check myPostAdjustmentsList
    for (PlainBasicBlock::BasicBlockElementList::const_iterator postI = myPostAdjustmentsList.begin();
	 postI != myPostAdjustmentsList.end(); ++postI)
      if ((*postI)->hasExpression(anExpression))
	return true;
    // check values saved across
    if (mySaveValuesAcrossForTypeChange.hasExpression(anExpression))
      return true;
    // pass on to the alg base
    return xaifBooster::SubroutineCallAlgBase::hasExpression(anExpression);
  } // end SubroutineCallAlg::hasExpression()

  void SubroutineCallAlg::handleArrayAccessIndices(const ConcreteArgument& theConcreteArgument,
						   const BasicBlock& theBasicBlock) {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterTypeChange::SubroutineCallAlg::handleArrayAccessIndices() " << debug().c_str());
    // get the argument algorithm instance 
    ConcreteArgumentAlg& theConcreteArgumentAlg(dynamic_cast<ConcreteArgumentAlg&>(theConcreteArgument.getConcreteArgumentAlgBase()));
    if (!theConcreteArgumentAlg.hasPostConversionConcreteArgument()) { 
      // no post conversion, nothing further to be done here
      return;
    } 
    const ArrayAccess::IndexTripletListType& theIndexTripletList(theConcreteArgument.getArgument().getVariable().getArrayAccess().getIndexTripletList());
    for (ArrayAccess::IndexTripletListType::const_iterator anIndexTripletListTypeCI=theIndexTripletList.begin();
	 anIndexTripletListTypeCI!=theIndexTripletList.end();
	 ++anIndexTripletListTypeCI) { 
      for (IndexTriplet::IndexPairList::const_iterator anIndexPairListCI=(*anIndexTripletListTypeCI)->getIndexPairList().begin();
	   anIndexPairListCI!=(*anIndexTripletListTypeCI)->getIndexPairList().end();
	   ++anIndexPairListCI) { 
	// now we have two cases, essentially the expression is a single vertex with a constant 
	// (this discounts constant expressions, this is a todo which might be dealt with later or 
	// it may be completly superceded by a TBR analysis)
	const Expression& theIndexExpression(*((*anIndexPairListCI).second));
	if (!theIndexExpression.isConstant()) {
	  Expression::CArgumentPList listToBeAppended;
	  theIndexExpression.appendArguments(listToBeAppended);
	  for (Expression::CArgumentPList::const_iterator argumentI=listToBeAppended.begin();
	       argumentI!=listToBeAppended.end();
	       ++argumentI) { 
	    if (!mySaveValuesAcrossForTypeChange.isSavedAcross(*((*argumentI).first))
		&& 
		(dynamic_cast<const SubroutineCall&>(getContaining())).overwrites((*argumentI).first->getVariable())) {
	      if (theBasicBlock.getReversalType()==ForLoopReversalType::EXPLICIT) { 
		// for sanity check if we a re about to change a known loop variable 
		// in this call which we forbid
		const ControlFlowGraphVertex::VariablePList& theKnownLoopVariables(theBasicBlock.getKnownLoopVariables());
		for (ControlFlowGraphVertex::VariablePList::const_iterator knownVarsI=theKnownLoopVariables.begin();
		     knownVarsI!=theKnownLoopVariables.end();
		     ++knownVarsI) {
		  if ((*argumentI).first->getVariable().equivalentTo(**knownVarsI))
		    THROW_LOGICEXCEPTION_MACRO("SubroutineCallAlg::handleArrayAccessIndices: analysis determines overwrite of simple loop variable "
					       << (*argumentI).first->getVariable().getVariableSymbolReference().getSymbol().plainName().c_str()
					       << " in call to "
					       << getContainingSubroutineCall().getSymbolReference().getSymbol().plainName().c_str());
		}
	      }
	      // save the value of the Argument before the subroutine call
	      mySaveValuesAcrossForTypeChange.saveValue(*((*argumentI).first),theBasicBlock);
	    }
	  }
	}
      }
    }
    // now we are done going through all the index expressions. 
    // we need to replace all the saved variable values in the post conversion. 
    if (theConcreteArgumentAlg.getPostConversionConcreteArgument().getArgument().getVariable().hasArrayAccess()) { 
      ArrayAccess::IndexTripletListType& thePostReplacementIndexTripletList(theConcreteArgumentAlg.getPostConversionConcreteArgument().getArgument().getVariable().getArrayAccess().getIndexTripletList());
      //populate the list of replacement pairs
      Expression::VariablePVariableSRPPairList theReplacementPairsList;
      mySaveValuesAcrossForTypeChange.populateReplacementPairsList(theReplacementPairsList);
      for (ArrayAccess::IndexTripletListType::iterator thePostReplacementIndexTripletListI=thePostReplacementIndexTripletList.begin();
	   thePostReplacementIndexTripletListI!=thePostReplacementIndexTripletList.end();
	   ++thePostReplacementIndexTripletListI) {
	for (IndexTriplet::IndexPairList::iterator anIndexPairListI=(*thePostReplacementIndexTripletListI)->getIndexPairList().begin();
	     anIndexPairListI!=(*thePostReplacementIndexTripletListI)->getIndexPairList().end();
	     ++anIndexPairListI) {
	  Expression& theIndexExpression(*((*anIndexPairListI).second));
	  theIndexExpression.replaceVariables(theReplacementPairsList);
	}
      }
    }
  } // end of SubroutineCallAlg::handleArrayAccessIndices

} // end namespace xaifBoosterTypeChange

