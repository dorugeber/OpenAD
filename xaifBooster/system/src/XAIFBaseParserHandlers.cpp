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

#include "xaifBooster/utils/inc/XMLParserMessage.hpp"
#include "xaifBooster/utils/inc/StringConversions.hpp"

#include "xaifBooster/system/inc/XAIFBaseParserHandlers.hpp"
#include "xaifBooster/system/inc/XMLParser.hpp"


namespace xaifBooster {
  
  void 
  XAIFBaseParserHandlers::onPreLoop(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onPreLoop");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    PreLoop* theNewPreLoop_p=new PreLoop();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewPreLoop_p);
    theNewPreLoop_p->setId(XMLParser::getAttributeValueByName(PreLoop::our_myId_XAIFName));
    theNewPreLoop_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    theNewPreLoop_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    passingOut.setBaseLoop(dynamic_cast<BaseLoop&>(*theNewPreLoop_p));
  }

  void 
  XAIFBaseParserHandlers::onPostLoop(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onPostLoop");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    PostLoop* theNewPostLoop_p=new PostLoop();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewPostLoop_p);
    theNewPostLoop_p->setId(XMLParser::getAttributeValueByName(PostLoop::our_myId_XAIFName));
    theNewPostLoop_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    theNewPostLoop_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    passingOut.setBaseLoop(dynamic_cast<BaseLoop&>(*theNewPostLoop_p));
  }

  void 
  XAIFBaseParserHandlers::onForLoop(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onForLoop");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    ForLoop* theNewForLoop_p=new ForLoop(ForLoopReversalType::fromString(XMLParser::getAttributeValueByName(ForLoopReversalType::our_attribute_XAIFName)));
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewForLoop_p);
    theNewForLoop_p->setId(XMLParser::getAttributeValueByName(ForLoop::our_myId_XAIFName));
    theNewForLoop_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    theNewForLoop_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    passingOut.setBaseLoop(dynamic_cast<BaseLoop&>(*theNewForLoop_p));
    passingOut.setForLoop(*theNewForLoop_p);
  }

  void 
  XAIFBaseParserHandlers::onIfStatement(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onIfStatement");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    IfStatement* theNewIfStatement_p=new IfStatement();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewIfStatement_p);
    theNewIfStatement_p->setId(XMLParser::getAttributeValueByName(IfStatement::our_myId_XAIFName));
    theNewIfStatement_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    passingOut.setCondition(theNewIfStatement_p->getCondition());
    theNewIfStatement_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onBranch(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onBranch");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Branch* theNewBranch_p=new Branch();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewBranch_p);
    theNewBranch_p->setId(XMLParser::getAttributeValueByName(Branch::our_myId_XAIFName));
    theNewBranch_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    passingOut.setCondition(theNewBranch_p->getCondition());
    theNewBranch_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onBasicBlock(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onBasicBlock");
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Scope& theScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
		    getScopeById(XMLParser::getAttributeValueByName(BasicBlock::our_myScopeId_XAIFName)));
    BasicBlock* theNewBasicBlock_p=new BasicBlock(theScope);
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewBasicBlock_p);
    theNewBasicBlock_p->setId(XMLParser::getAttributeValueByName(BasicBlock::our_myId_XAIFName));
    passingOut.setBasicBlock(*theNewBasicBlock_p);
    theNewBasicBlock_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onControlFlowGraph(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onControlFlowGraph" ); 
    const Scope& theScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
			  getScopeById(XMLParser::getAttributeValueByName(ControlFlowGraphCommonAttributes::our_mySymbolReferenceScopeId_XAIFName)));
    const Symbol& theSymbol(theScope.getSymbolTable().
			    getSymbol(XMLParser::getAttributeValueByName(ControlFlowGraphCommonAttributes::our_mySymbolReferenceSymbolId_XAIFName)));
    const Scope& theCFGScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
			     getScopeById(XMLParser::getAttributeValueByName(ControlFlowGraphCommonAttributes::our_myScope_XAIFName)));
    CallGraphVertex* theCallGraphVertex_p=new CallGraphVertex(theSymbol,
							      theScope,
							      theCFGScope,
							      XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(ControlFlowGraph::our_myActiveFlag_XAIFName)));
    ConceptuallyStaticInstances::instance()->
      getCallGraph().supplyAndAddVertexInstance(*theCallGraphVertex_p);
    passingOut.setControlFlowGraph(theCallGraphVertex_p->getControlFlowGraph());
    theCallGraphVertex_p->getControlFlowGraph().setId(XMLParser::getAttributeValueByName(ControlFlowGraph::our_myId_XAIFName));
    theCallGraphVertex_p->getControlFlowGraph().setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    theCallGraphVertex_p->getControlFlowGraph().setStructured(XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(ControlFlowGraph::our_myStructuredFlag_XAIFName)));
  }

  void 
  XAIFBaseParserHandlers::onControlFlowGraphEnd(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onControlFlowGraphEnd" ); 
    passingIn.getControlFlowGraph().augmentGraphInfo();
  }

  void 
  XAIFBaseParserHandlers::onSymbol(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSymbol" ); 
    SymbolTable& theSymbolTable(passingIn.getSymbolTable());
    Symbol& theNewSymbol(theSymbolTable.addSymbol(XMLParser::getAttributeValueByName(Symbol::our_myId_XAIFName),
						  SymbolKind::fromString(XMLParser::getAttributeValueByName(SymbolKind::our_attribute_XAIFName)),
						  SymbolType::fromString(XMLParser::getAttributeValueByName(SymbolType::our_attribute_XAIFName)),	
						  SymbolShape::fromString(XMLParser::getAttributeValueByName(SymbolShape::our_attribute_XAIFName)),
						  XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(Symbol::our_myActiveTypeFlag_XAIFName)),
						  XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(Symbol::our_myTempFlag_XAIFName))));
    theNewSymbol.setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    theNewSymbol.setFrontEndType(XMLParser::getAttributeValueByName(FrontEndType::our_attribute_XAIFName));
    passingOut.setSymbol(theNewSymbol);
  }


  void 
  XAIFBaseParserHandlers::onSymbolReferenceProperty(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSymbolReferenceProperty" ); 
  }

  void 
  XAIFBaseParserHandlers::onDimensionBounds(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onDimensionBounds" ); 
    Symbol& theSymbol(passingIn.getSymbol());
    theSymbol.addDimensionBounds(StringConversions::convertToInt(XMLParser::getAttributeValueByName(DimensionBounds::our_myLower_XAIFName)),
				 StringConversions::convertToInt(XMLParser::getAttributeValueByName(DimensionBounds::our_myUpper_XAIFName)));
  }

  void 
  XAIFBaseParserHandlers::onScopeContainment(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onScopeContainment" ); 
    Scopes& theScopes(passingIn.getScopes());
    std::string sourceId=XMLParser::getAttributeValueByName(ScopeContainment::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(ScopeContainment::our_target_XAIFName);
    Scopes::VertexIteratorPair p=theScopes.vertices();
    Scopes::VertexIterator beginIt(p.first),endIt(p.second);
    Scope *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHandlers::onScopeContainment: can't find source scope " 
				 << sourceId.c_str()
				 << " or target scope "
				 << targetId.c_str());
    ScopeContainment& theScopeContainment(theScopes.addEdge(*theSource_p,
							    *theTarget_p));
    theScopeContainment.setId(XMLParser::getAttributeValueByName(ScopeContainment::our_myId_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onSymbolTable(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSymbolTable" ); 
    Scope& theScope(passingIn.getScope());
    SymbolTable& theSymbolTable(theScope.getSymbolTable());
    passingOut.setSymbolTable(theSymbolTable);
  }

  void 
  XAIFBaseParserHandlers::onScope(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onScope" ); 
    Scopes& theScopes(passingIn.getScopes());
    Scope& theScope(theScopes.addVertex());
    theScope.setId(XMLParser::getAttributeValueByName(Scope::our_myId_XAIFName));
    passingOut.setScope(theScope);
    theScope.setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onAliasMap(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAliasMap" ); 
    AliasMap& theAliasMap(passingIn.getAliasMap());
    passingOut.setAliasMap(theAliasMap);
  }

  void 
  XAIFBaseParserHandlers::onAliasMapEntry(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAliasMapEntry" ); 
    AliasMap& theAliasMap(passingIn.getAliasMap());
    AliasMapEntry& theAliasMapEntry=theAliasMap.
      addAliasMapEntry(XMLParser::getAttributeValueByName(AliasMapEntry::our_myKey_XAIFName));
    passingOut.setAliasMapEntry(theAliasMapEntry);
  }

  void 
  XAIFBaseParserHandlers::onAliasRange(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAliasRange" ); 
    AliasSet& theAliasSet(passingIn.getAliasMapEntry().getAliasSet());
    theAliasSet.addAlias(StringConversions::convertToInt(XMLParser::getAttributeValueByName(AliasRange::our_myLowerAddress_XAIFName)),
			 StringConversions::convertToInt(XMLParser::getAttributeValueByName(AliasRange::our_myUpperAddress_XAIFName)),
			 XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(AliasRange::our_myPartial_XAIFName)));
  }

  void 
  XAIFBaseParserHandlers::onDuUdMap(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onDuUdMap" ); 
    DuUdMap& theDuUdMap(passingIn.getDuUdMap());
    passingOut.setStatementIdSetMap(theDuUdMap);
  }

  void 
  XAIFBaseParserHandlers::onDoMap(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onDoMap" ); 
    DoMap& theDoMap(passingIn.getDoMap());
    passingOut.setStatementIdSetMap(theDoMap);
  }

  void 
  XAIFBaseParserHandlers::onStatementIdSetMapEntry(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onStatementIdSetMapEntry" ); 
    StatementIdSetMap& theStatementIdSetMap(passingIn.getStatementIdSetMap());
    StatementIdSetMapEntry& theStatementIdSetMapEntry=theStatementIdSetMap.
      addEntry(StringConversions::convertToInt(XMLParser::getAttributeValueByName(StatementIdSetMapEntry::our_myKey_XAIFName)));
    passingOut.setStatementIdSetMapEntry(theStatementIdSetMapEntry);
  }

  void 
  XAIFBaseParserHandlers::onStatementId(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onStatementId" ); 
    StatementIdSetMapEntry& theStatementIdSetMapEntry(passingIn.getStatementIdSetMapEntry());
    theStatementIdSetMapEntry.insert(XMLParser::getAttributeValueByName(StatementIdSetMapEntry::our_IdRef_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onScopes(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onScopes" ); 
    Scopes& theScopes(passingIn.getScopes());
    passingOut.setScopes(theScopes);
  }

  void 
  XAIFBaseParserHandlers::onCallGraph(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onCallGraph" ); 
    ConceptuallyStaticInstances::instance()->
      createCallGraph(XMLParser::getAttributeValueByName(CallGraph::our_mySchemaInstance_XAIFName),
		      XMLParser::getAttributeValueByName(CallGraph::our_myXAIFInstance_XAIFName),
		      XMLParser::getAttributeValueByName(CallGraph::our_mySchemaLocation_XAIFName),
		      XMLParser::getAttributeValueByName(CallGraph::our_myPrefix_XAIFName));
    CallGraph& theCallGraph=ConceptuallyStaticInstances::instance()->getCallGraph();
    theCallGraph.setProgramName(XMLParser::getAttributeValueByName(CallGraph::our_myProgramName_XAIFName));
    passingOut.setScopes(theCallGraph.getScopeTree());
    passingOut.setAliasMap(theCallGraph.getAliasMap());
    passingOut.setDuUdMap(theCallGraph.getDuUdMap());
    passingOut.setDoMap(theCallGraph.getDoMap());
  }

  void 
  XAIFBaseParserHandlers::onAssignment(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAssignment" ); 
    BasicBlock& theBasicBlock(passingIn.getBasicBlock());
    Assignment* theAssignment_p=new Assignment;
    theAssignment_p->setId(XMLParser::getAttributeValueByName(Assignment::our_myId_XAIFName));
    theAssignment_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    theBasicBlock.supplyAndAddBasicBlockElementInstance(*theAssignment_p);
    theAssignment_p->getDoMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Assignment::our_myDoMapKey_XAIFName)));
    passingOut.setAssignment(*theAssignment_p);
  }

  void 
  XAIFBaseParserHandlers::onAssignmentLHS(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAssignmentLHS" ); 
    Assignment& theAssignment(passingIn.getAssignment());
    theAssignment.getLHS().getAliasMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myAliasMapKey_XAIFName)));
    theAssignment.getLHS().getDuUdMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myDuUdMapKey_XAIFName)));
    theAssignment.getLHS().
      setActiveUseType(ActiveUseType::fromString(XMLParser::getAttributeValueByName(ActiveUseType::our_attribute_XAIFName).c_str()));
    if (XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(Variable::our_myConstantUseFlag_XAIFName)))
      theAssignment.getLHS().setConstantUseFlag();
    passingOut.setVariable(theAssignment.getLHS());
  }

  void 
  XAIFBaseParserHandlers::onAssignmentRHS(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onAssignmentRHS" ); 
    Assignment& theAssignment(passingIn.getAssignment());
    passingOut.setExpression(theAssignment.getRHS());
  }

  void 
  XAIFBaseParserHandlers::onConstant(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onConstant" ); 
    if (passingIn.hasExpression()) { // comes from an expression
      Expression& theExpression(passingIn.getExpression());
      Constant* theConstant_p=new Constant(SymbolType::fromString(XMLParser::getAttributeValueByName(SymbolType::our_attribute_XAIFName)));
      theConstant_p->setId(XMLParser::getAttributeValueByName(Constant::our_myId_XAIFName));
      theConstant_p->setFromString(XMLParser::getAttributeValueByName(Constant::our_myValue_XAIFName));
      theConstant_p->setFrontEndType(XMLParser::getAttributeValueByName(FrontEndType::our_attribute_XAIFName));
      theExpression.supplyAndAddVertexInstance(*theConstant_p);
    }
    else { // comes from a ConcreteArgument
      ConcreteArgument& theConcreteArgument(passingIn.getConcreteArgument());
      Constant& theConstant=theConcreteArgument.makeConstant(SymbolType::fromString(XMLParser::getAttributeValueByName(SymbolType::our_attribute_XAIFName)));
      theConstant.setFromString(XMLParser::getAttributeValueByName(Constant::our_myValue_XAIFName));
      theConstant.setFrontEndType(XMLParser::getAttributeValueByName(FrontEndType::our_attribute_XAIFName));
    }
  }

  void 
  XAIFBaseParserHandlers::onBooleanOperation(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onBooleanOperation" ); 
    Expression& theExpression(passingIn.getExpression());
    BooleanOperation* theBooleanOperation_p=
      new BooleanOperation(BooleanOperationType::fromString(XMLParser::getAttributeValueByName(BooleanOperation::our_myType_XAIFName)));
    theBooleanOperation_p->setId(XMLParser::getAttributeValueByName(BooleanOperation::our_myId_XAIFName));
    theExpression.supplyAndAddVertexInstance(*theBooleanOperation_p);
  }

  void 
  XAIFBaseParserHandlers::onArgument(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onArgument" ); 
    Variable* theNewVariable_p(0);
    if (passingIn.hasExpression()) { // comes from an expression
      Expression& theExpression(passingIn.getExpression());
      Argument* theArgument_p=new Argument();
      theArgument_p->setId(XMLParser::getAttributeValueByName(Argument::our_myId_XAIFName));
      theExpression.supplyAndAddVertexInstance(*theArgument_p);
      theNewVariable_p=&(theArgument_p->getVariable());
    }
    else { // comes from a ConcreteArgument
      ConcreteArgument& theConcreteArgument(passingIn.getConcreteArgument());
      theNewVariable_p=&(theConcreteArgument.getArgument().getVariable());
    }
    theNewVariable_p->getAliasMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myAliasMapKey_XAIFName)));
    theNewVariable_p->getDuUdMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myDuUdMapKey_XAIFName)));
    theNewVariable_p->
      setActiveUseType(ActiveUseType::fromString(XMLParser::getAttributeValueByName(ActiveUseType::our_attribute_XAIFName).c_str()));
    if (XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(Variable::our_myConstantUseFlag_XAIFName)))
      theNewVariable_p->setConstantUseFlag();
    passingOut.setVariable(*theNewVariable_p);
  }

  void 
  XAIFBaseParserHandlers::onVariableEdge(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onVariableEdge" ); 
    Variable& theVariable(passingIn.getVariable());
    std::string sourceId=XMLParser::getAttributeValueByName(VariableEdge::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(VariableEdge::our_target_XAIFName);
    Variable::VertexIteratorPair p=theVariable.vertices();
    Variable::VertexIterator beginIt(p.first),endIt(p.second);
    VariableVertex *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHandlers::onVariableEdge: can't find source  " 
				 << sourceId.c_str()
				 << " or target "
				 << targetId.c_str());
    VariableEdge& theVariableEdge(theVariable.addEdge(*theSource_p, *theTarget_p));
    theVariableEdge.setId(XMLParser::getAttributeValueByName(VariableEdge::our_myId_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onIntrinsic(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onIntrinsic" ); 
    Expression& theExpression(passingIn.getExpression());
    Intrinsic* theIntrinsic_p=new Intrinsic(XMLParser::getAttributeValueByName(Intrinsic::our_myName_XAIFName));
    theIntrinsic_p->setId(XMLParser::getAttributeValueByName(Intrinsic::our_myId_XAIFName));
    theIntrinsic_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    theExpression.supplyAndAddVertexInstance(*theIntrinsic_p);
  }

  void 
  XAIFBaseParserHandlers::onExpressionEdge(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onExpressionEdge" ); 
    Expression& theExpression(passingIn.getExpression());
    std::string sourceId=XMLParser::getAttributeValueByName(ExpressionEdge::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(ExpressionEdge::our_target_XAIFName);
    Expression::VertexIteratorPair p=theExpression.vertices();
    Expression::VertexIterator beginIt(p.first),endIt(p.second);
    ExpressionVertex *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHandlers::onExpressionEdge: can't find source  " 
				 << sourceId.c_str()
				 << " or target "
				 << targetId.c_str());
    ExpressionEdge& theExpressionEdge(theExpression.addEdge(*theSource_p,
							    *theTarget_p));
    theExpressionEdge.setId(XMLParser::getAttributeValueByName(ExpressionEdge::our_myId_XAIFName));
    theExpressionEdge.setPosition(XMLParser::getAttributeValueByName(ExpressionEdge::our_myPosition_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onControlFlowGraphEdge(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onControlFlowGraphEdge" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    std::string sourceId=XMLParser::getAttributeValueByName(ControlFlowGraphEdge::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(ControlFlowGraphEdge::our_target_XAIFName);
    ControlFlowGraph::VertexIteratorPair p=theControlFlowGraph.vertices();
    ControlFlowGraph::VertexIterator beginIt(p.first),endIt(p.second);
    ControlFlowGraphVertex *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHandlers::onControlFlowGraphEdge: can't find source  " 
				 << sourceId.c_str()
				 << " or target "
				 << targetId.c_str());
    ControlFlowGraphEdge& theControlFlowGraphEdge(theControlFlowGraph.addEdge(*theSource_p,
									      *theTarget_p));
    theControlFlowGraphEdge.setId(XMLParser::getAttributeValueByName(ControlFlowGraphEdge::our_myId_XAIFName));
    if (XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(ControlFlowGraphEdge::our_myConditionValueFlag_XAIFName))) 
      theControlFlowGraphEdge.setConditionValue(StringConversions::convertToInt(XMLParser::getAttributeValueByName(ControlFlowGraphEdge::our_myConditionValue_XAIFName)));
  }

  void 
  XAIFBaseParserHandlers::onCallGraphEdge(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onCallGraphEdge" ); 
    std::string sourceId=XMLParser::getAttributeValueByName(CallGraphEdge::our_source_XAIFName);
    std::string targetId=XMLParser::getAttributeValueByName(CallGraphEdge::our_target_XAIFName);
    CallGraph& theCallGraph=ConceptuallyStaticInstances::instance()->getCallGraph();
    CallGraph::VertexIteratorPair p=theCallGraph.vertices();
    CallGraph::VertexIterator beginIt(p.first),endIt(p.second);
    CallGraphVertex *theSource_p(0), *theTarget_p(0);
    for (; (beginIt!=endIt)
	   && !(theSource_p && theTarget_p)
	   ;++beginIt) { 
      if (!theSource_p && sourceId==(*beginIt).getControlFlowGraph().getId())
	theSource_p=&(*beginIt);
      if (!theTarget_p && targetId==(*beginIt).getControlFlowGraph().getId())
	theTarget_p=&(*beginIt);
    }
    if (!theTarget_p || 
	!theSource_p)
      THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHandlers::onCallGraphEdge: can't find source  " 
				 << sourceId.c_str()
				 << " or target "
				 << targetId.c_str());
    CallGraphEdge& theCallGraphEdge(theCallGraph.addEdge(*theSource_p,
							 *theTarget_p));
    theCallGraphEdge.setId(XMLParser::getAttributeValueByName(CallGraphEdge::our_myId_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onSymbolReference(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSymbolReference" ); 
    Variable& theVariable(passingIn.getVariable());
    const Scope& theScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
			  getScopeById(XMLParser::getAttributeValueByName(VariableSymbolReference::our_scopeId_XAIFName)));
    const Symbol& theSymbol(theScope.getSymbolTable().
			    getSymbol(XMLParser::getAttributeValueByName(Symbol::our_myId_XAIFName)));
    VariableSymbolReference* theVariableSymbolReference_p=new VariableSymbolReference(theSymbol,
										      theScope);
    theVariable.supplyAndAddVertexInstance(*theVariableSymbolReference_p);
    theVariableSymbolReference_p->setId(XMLParser::getAttributeValueByName(VariableSymbolReference::our_myId_XAIFName));
    theVariableSymbolReference_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onArrayAccess(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onArrayAccess" ); 
    Variable& theVariable(passingIn.getVariable());
    ArrayAccess* theNewArrayAccess_p = new ArrayAccess();
    theVariable.supplyAndAddVertexInstance(*theNewArrayAccess_p);
    theNewArrayAccess_p->setId(XMLParser::getAttributeValueByName(ArrayAccess::our_myId_XAIFName));
    passingOut.setArrayAccess(*theNewArrayAccess_p);
  }

  void 
  XAIFBaseParserHandlers::onIndexTriplet(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onIndexTriplet" ); 
    ArrayAccess& theArrayAccess(passingIn.getArrayAccess());
    IndexTriplet& theIndexTriplet(theArrayAccess.addIndexTriplet());
    passingOut.setIndexTriplet(theIndexTriplet);
  }

  void 
  XAIFBaseParserHandlers::onIndex(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onIndex" ); 
    IndexTriplet& theIndexTriplet(passingIn.getIndexTriplet());
    passingOut.setExpression(theIndexTriplet.addExpression(IndexTriplet::IT_INDEX));
  }

  void 
  XAIFBaseParserHandlers::onBound(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onBound" ); 
    IndexTriplet& theIndexTriplet(passingIn.getIndexTriplet());
    passingOut.setExpression(theIndexTriplet.addExpression(IndexTriplet::IT_BOUND));
  }

  void 
  XAIFBaseParserHandlers::onStride(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onStride" ); 
    IndexTriplet& theIndexTriplet(passingIn.getIndexTriplet());
    passingOut.setExpression(theIndexTriplet.addExpression(IndexTriplet::IT_STRIDE));
  }

  void 
  XAIFBaseParserHandlers::onEntry(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onEntry" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Entry* theNewEntry_p=new Entry();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewEntry_p);
    theNewEntry_p->setId(XMLParser::getAttributeValueByName(Entry::our_myId_XAIFName));
    theNewEntry_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onExit(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onExit" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Exit* theNewExit_p=new Exit();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewExit_p);
    theNewExit_p->setId(XMLParser::getAttributeValueByName(Exit::our_myId_XAIFName));
    theNewExit_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onEndBranch(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onEndBranch" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    EndBranch* theNewEndBranch_p=new EndBranch();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewEndBranch_p);
    theNewEndBranch_p->setId(XMLParser::getAttributeValueByName(EndBranch::our_myId_XAIFName));
    theNewEndBranch_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }
 
  void 
  XAIFBaseParserHandlers::onEndLoop(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onEndLoop" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    EndLoop* theNewEndLoop_p=new EndLoop();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewEndLoop_p);
    theNewEndLoop_p->setId(XMLParser::getAttributeValueByName(EndLoop::our_myId_XAIFName));
    theNewEndLoop_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onSubroutineCall(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSubroutineCall" ); 
    BasicBlock& theBasicBlock(passingIn.getBasicBlock());
    const Scope& theScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
			  getScopeById(XMLParser::getAttributeValueByName(SubroutineCall::our_scopeId_XAIFName)));
    const Symbol& theSymbol(theScope.getSymbolTable().
			    getSymbol(XMLParser::getAttributeValueByName(SubroutineCall::our_symbolId_XAIFName)));
    SubroutineCall* theNewSubroutineCall_p=
      new SubroutineCall(theSymbol,
			 theScope,
			 ActiveUseType::fromString(XMLParser::getAttributeValueByName(ActiveUseType::our_attribute_XAIFName).c_str()),
			 StringConversions::convertToUInt(XMLParser::getAttributeValueByName(SubroutineCall::our_myFormalArgCount_XAIFName).c_str()));
    theBasicBlock.supplyAndAddBasicBlockElementInstance(*theNewSubroutineCall_p);
    theNewSubroutineCall_p->setId(XMLParser::getAttributeValueByName(SubroutineCall::our_myId_XAIFName));
    theNewSubroutineCall_p->setLineNumber(StringConversions::convertToUInt(XMLParser::getAttributeValueByName(ObjectWithLineNumber::our_myLineNumber_XAIFName)));
    passingOut.setSubroutineCall(*theNewSubroutineCall_p);
  }

  void 
  XAIFBaseParserHandlers::onMarker(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onMarker" ); 
    BasicBlock& theBasicBlock(passingIn.getBasicBlock());
    Marker* theMarker_p=new Marker();
    theMarker_p->setId(XMLParser::getAttributeValueByName(Marker::our_myId_XAIFName));
    theMarker_p->setAnnotation(XMLParser::getAttributeValueByName(Marker::our_myAnnotation_XAIFName));
    theBasicBlock.supplyAndAddBasicBlockElementInstance(*theMarker_p);
  }

  void 
  XAIFBaseParserHandlers::onConcreteArgument(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onConcreteArgument" ); 
    SubroutineCall& theSubroutineCall(passingIn.getSubroutineCall());
    ConcreteArgument* theNewConcreteArgument_p=
      new ConcreteArgument(StringConversions::convertToInt(XMLParser::getAttributeValueByName(ConcreteArgument::our_myPosition_XAIFName)));
    theNewConcreteArgument_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    theSubroutineCall.getConcreteArgumentPList().push_back(theNewConcreteArgument_p);
    passingOut.setConcreteArgument(*theNewConcreteArgument_p);
  }

  void 
  XAIFBaseParserHandlers::onArgumentList(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onArgumentList"); 
    // pass all set elements through
    passingOut=passingIn;
    return;
  }

  void 
  XAIFBaseParserHandlers::onArgumentSymbolReference(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onArgumentSymbolReference"); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    const Scope& theScope(ConceptuallyStaticInstances::instance()->getCallGraph().getScopeTree().
			  getScopeById(XMLParser::getAttributeValueByName(ArgumentSymbolReference::our_scopeId_XAIFName)));
    const Symbol& theSymbol(theScope.getSymbolTable().
			    getSymbol(XMLParser::getAttributeValueByName(ArgumentSymbolReference::our_symbolId_XAIFName)));
    // this will be deleted in the dtor of theControlFlowGraph
    ArgumentSymbolReference* theNewArgumentSymbolReference_p=
      new ArgumentSymbolReference(theSymbol,
				  theScope,
				  StringConversions::convertToInt(XMLParser::getAttributeValueByName(ArgumentSymbolReference::our_myPosition_XAIFName)),
				  IntentType::fromString(XMLParser::getAttributeValueByName(ArgumentSymbolReference::our_myIntent_XAIFName)));
    theNewArgumentSymbolReference_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
    theControlFlowGraph.getArgumentList().getArgumentSymbolReferencePList().push_back(theNewArgumentSymbolReference_p);
  }

  void 
  XAIFBaseParserHandlers::onInitialization(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onInitialization"); 
    Initialization& theInitialization=passingIn.getForLoop().makeInitialization();
    theInitialization.getAssignment().setId(XMLParser::getAttributeValueByName(Assignment::our_myId_XAIFName));
    theInitialization.getAssignment().getDoMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Assignment::our_myDoMapKey_XAIFName)));
    passingOut.setAssignment(theInitialization.getAssignment());;
  }

  void 
  XAIFBaseParserHandlers::onCondition(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onCondition");
    if (passingIn.hasCondition())
      passingOut.setExpression(passingIn.getCondition().getExpression());
    else
      passingOut.setExpression(passingIn.getBaseLoop().makeCondition().getExpression());
  }

  void 
  XAIFBaseParserHandlers::onUpdate(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onUpdate"); 
    Update& theUpdate=passingIn.getForLoop().makeUpdate();
    theUpdate.getAssignment().setId(XMLParser::getAttributeValueByName(Assignment::our_myId_XAIFName));
    theUpdate.getAssignment().getDoMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Assignment::our_myDoMapKey_XAIFName)));
    passingOut.setAssignment(theUpdate.getAssignment());;
  }

  void 
  XAIFBaseParserHandlers::onMod(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onMod"); 
    passingOut.setSideEffectList(passingIn.getControlFlowGraph().getSideEffectList(SideEffectListType::MOD_LIST));
    return;
  }

  void 
  XAIFBaseParserHandlers::onModLocal(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onMod"); 
    passingOut.setSideEffectList(passingIn.getControlFlowGraph().getSideEffectList(SideEffectListType::MOD_LOCAL_LIST));
    return;
  }

  void
  XAIFBaseParserHandlers::onOnEntry(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onOnEntry"); 
    passingOut.setSideEffectList(passingIn.getControlFlowGraph().getSideEffectList(SideEffectListType::ON_ENTRY_LIST));
    return;
  }

  void 
  XAIFBaseParserHandlers::onRead(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onRead"); 
    passingOut.setSideEffectList(passingIn.getControlFlowGraph().getSideEffectList(SideEffectListType::READ_LIST));
    return;
  }

  void
  XAIFBaseParserHandlers::onReadLocal(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onRead"); 
    passingOut.setSideEffectList(passingIn.getControlFlowGraph().getSideEffectList(SideEffectListType::READ_LOCAL_LIST));
    return;
  }

  void 
  XAIFBaseParserHandlers::onSideEffectReference(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onSideEffectReference"); 
    Variable& theVariable(passingIn.getSideEffectList().addSideEffectReference());
    theVariable.getAliasMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myAliasMapKey_XAIFName)));
    theVariable.getDuUdMapKey().
      setReference(StringConversions::convertToInt(XMLParser::getAttributeValueByName(Variable::our_myDuUdMapKey_XAIFName)));
    theVariable.
      setActiveUseType(ActiveUseType::fromString(XMLParser::getAttributeValueByName(ActiveUseType::our_attribute_XAIFName).c_str()));
    if (XMLParser::convertToBoolean(XMLParser::getAttributeValueByName(Variable::our_myConstantUseFlag_XAIFName)))
      theVariable.setConstantUseFlag();
    passingOut.setVariable(theVariable);
  }

  void 
  XAIFBaseParserHandlers::onDummy(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, 
	      "in XAIFBaseParserHandlers::onDummy"); 
    // pass all set elements through
    passingOut=passingIn;
    return;
  }

  void 
  XAIFBaseParserHandlers::onGoto(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onGoto" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Goto* theNewGoto_p=new Goto();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewGoto_p);
    theNewGoto_p->setId(XMLParser::getAttributeValueByName(Goto::our_myId_XAIFName));
    theNewGoto_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

  void 
  XAIFBaseParserHandlers::onLabel(const XAIFBaseParserHelper& passingIn, XAIFBaseParserHelper& passingOut) {
    DBG_MACRO(DbgGroup::CALLSTACK, "in XAIFBaseParserHandlers::onLabel" ); 
    ControlFlowGraph& theControlFlowGraph(passingIn.getControlFlowGraph());
    Label* theNewLabel_p=new Label();
    theControlFlowGraph.supplyAndAddVertexInstance(*theNewLabel_p);
    theNewLabel_p->setId(XMLParser::getAttributeValueByName(Label::our_myId_XAIFName));
    theNewLabel_p->setAnnotation(XMLParser::getAttributeValueByName(ObjectWithAnnotation::our_myAnnotation_XAIFName));
  }

} // end of namespace 
