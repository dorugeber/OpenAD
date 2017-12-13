void XAIFBaseParserHelper::setAliasMapEntry(AliasMapEntry& whatToSet) {
if (myAliasMapEntry_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "AliasMapEntry" << " item already set");
myAliasMapEntry_p=&whatToSet;
}
AliasMapEntry& XAIFBaseParserHelper::getAliasMapEntry() const {
if (!myAliasMapEntry_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "AliasMapEntry" << " item not set");
return *myAliasMapEntry_p;
}
bool XAIFBaseParserHelper::hasAliasMapEntry() const {
return (myAliasMapEntry_p!=0);
}

void XAIFBaseParserHelper::setAliasMap(AliasMap& whatToSet) {
if (myAliasMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "AliasMap" << " item already set");
myAliasMap_p=&whatToSet;
}
AliasMap& XAIFBaseParserHelper::getAliasMap() const {
if (!myAliasMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "AliasMap" << " item not set");
return *myAliasMap_p;
}
bool XAIFBaseParserHelper::hasAliasMap() const {
return (myAliasMap_p!=0);
}

void XAIFBaseParserHelper::setArgumentList(ArgumentList& whatToSet) {
if (myArgumentList_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ArgumentList" << " item already set");
myArgumentList_p=&whatToSet;
}
ArgumentList& XAIFBaseParserHelper::getArgumentList() const {
if (!myArgumentList_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ArgumentList" << " item not set");
return *myArgumentList_p;
}
bool XAIFBaseParserHelper::hasArgumentList() const {
return (myArgumentList_p!=0);
}

void XAIFBaseParserHelper::setArrayAccess(ArrayAccess& whatToSet) {
if (myArrayAccess_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ArrayAccess" << " item already set");
myArrayAccess_p=&whatToSet;
}
ArrayAccess& XAIFBaseParserHelper::getArrayAccess() const {
if (!myArrayAccess_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ArrayAccess" << " item not set");
return *myArrayAccess_p;
}
bool XAIFBaseParserHelper::hasArrayAccess() const {
return (myArrayAccess_p!=0);
}

void XAIFBaseParserHelper::setAssignment(Assignment& whatToSet) {
if (myAssignment_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Assignment" << " item already set");
myAssignment_p=&whatToSet;
}
Assignment& XAIFBaseParserHelper::getAssignment() const {
if (!myAssignment_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Assignment" << " item not set");
return *myAssignment_p;
}
bool XAIFBaseParserHelper::hasAssignment() const {
return (myAssignment_p!=0);
}

void XAIFBaseParserHelper::setBaseLoop(BaseLoop& whatToSet) {
if (myBaseLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "BaseLoop" << " item already set");
myBaseLoop_p=&whatToSet;
}
BaseLoop& XAIFBaseParserHelper::getBaseLoop() const {
if (!myBaseLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "BaseLoop" << " item not set");
return *myBaseLoop_p;
}
bool XAIFBaseParserHelper::hasBaseLoop() const {
return (myBaseLoop_p!=0);
}

void XAIFBaseParserHelper::setBasicBlock(BasicBlock& whatToSet) {
if (myBasicBlock_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "BasicBlock" << " item already set");
myBasicBlock_p=&whatToSet;
}
BasicBlock& XAIFBaseParserHelper::getBasicBlock() const {
if (!myBasicBlock_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "BasicBlock" << " item not set");
return *myBasicBlock_p;
}
bool XAIFBaseParserHelper::hasBasicBlock() const {
return (myBasicBlock_p!=0);
}

void XAIFBaseParserHelper::setBranch(Branch& whatToSet) {
if (myBranch_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Branch" << " item already set");
myBranch_p=&whatToSet;
}
Branch& XAIFBaseParserHelper::getBranch() const {
if (!myBranch_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Branch" << " item not set");
return *myBranch_p;
}
bool XAIFBaseParserHelper::hasBranch() const {
return (myBranch_p!=0);
}

void XAIFBaseParserHelper::setCallGraph(CallGraph& whatToSet) {
if (myCallGraph_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "CallGraph" << " item already set");
myCallGraph_p=&whatToSet;
}
CallGraph& XAIFBaseParserHelper::getCallGraph() const {
if (!myCallGraph_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "CallGraph" << " item not set");
return *myCallGraph_p;
}
bool XAIFBaseParserHelper::hasCallGraph() const {
return (myCallGraph_p!=0);
}

void XAIFBaseParserHelper::setConcreteArgument(ConcreteArgument& whatToSet) {
if (myConcreteArgument_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ConcreteArgument" << " item already set");
myConcreteArgument_p=&whatToSet;
}
ConcreteArgument& XAIFBaseParserHelper::getConcreteArgument() const {
if (!myConcreteArgument_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ConcreteArgument" << " item not set");
return *myConcreteArgument_p;
}
bool XAIFBaseParserHelper::hasConcreteArgument() const {
return (myConcreteArgument_p!=0);
}

void XAIFBaseParserHelper::setCondition(Condition& whatToSet) {
if (myCondition_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Condition" << " item already set");
myCondition_p=&whatToSet;
}
Condition& XAIFBaseParserHelper::getCondition() const {
if (!myCondition_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Condition" << " item not set");
return *myCondition_p;
}
bool XAIFBaseParserHelper::hasCondition() const {
return (myCondition_p!=0);
}

void XAIFBaseParserHelper::setControlFlowGraph(ControlFlowGraph& whatToSet) {
if (myControlFlowGraph_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ControlFlowGraph" << " item already set");
myControlFlowGraph_p=&whatToSet;
}
ControlFlowGraph& XAIFBaseParserHelper::getControlFlowGraph() const {
if (!myControlFlowGraph_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ControlFlowGraph" << " item not set");
return *myControlFlowGraph_p;
}
bool XAIFBaseParserHelper::hasControlFlowGraph() const {
return (myControlFlowGraph_p!=0);
}

void XAIFBaseParserHelper::setDoMap(DoMap& whatToSet) {
if (myDoMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "DoMap" << " item already set");
myDoMap_p=&whatToSet;
}
DoMap& XAIFBaseParserHelper::getDoMap() const {
if (!myDoMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "DoMap" << " item not set");
return *myDoMap_p;
}
bool XAIFBaseParserHelper::hasDoMap() const {
return (myDoMap_p!=0);
}

void XAIFBaseParserHelper::setDuUdMap(DuUdMap& whatToSet) {
if (myDuUdMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "DuUdMap" << " item already set");
myDuUdMap_p=&whatToSet;
}
DuUdMap& XAIFBaseParserHelper::getDuUdMap() const {
if (!myDuUdMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "DuUdMap" << " item not set");
return *myDuUdMap_p;
}
bool XAIFBaseParserHelper::hasDuUdMap() const {
return (myDuUdMap_p!=0);
}

void XAIFBaseParserHelper::setExpression(Expression& whatToSet) {
if (myExpression_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Expression" << " item already set");
myExpression_p=&whatToSet;
}
Expression& XAIFBaseParserHelper::getExpression() const {
if (!myExpression_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Expression" << " item not set");
return *myExpression_p;
}
bool XAIFBaseParserHelper::hasExpression() const {
return (myExpression_p!=0);
}

void XAIFBaseParserHelper::setExpressionEdge(ExpressionEdge& whatToSet) {
if (myExpressionEdge_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ExpressionEdge" << " item already set");
myExpressionEdge_p=&whatToSet;
}
ExpressionEdge& XAIFBaseParserHelper::getExpressionEdge() const {
if (!myExpressionEdge_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ExpressionEdge" << " item not set");
return *myExpressionEdge_p;
}
bool XAIFBaseParserHelper::hasExpressionEdge() const {
return (myExpressionEdge_p!=0);
}

void XAIFBaseParserHelper::setForLoop(ForLoop& whatToSet) {
if (myForLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "ForLoop" << " item already set");
myForLoop_p=&whatToSet;
}
ForLoop& XAIFBaseParserHelper::getForLoop() const {
if (!myForLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "ForLoop" << " item not set");
return *myForLoop_p;
}
bool XAIFBaseParserHelper::hasForLoop() const {
return (myForLoop_p!=0);
}

void XAIFBaseParserHelper::setGoto(Goto& whatToSet) {
if (myGoto_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Goto" << " item already set");
myGoto_p=&whatToSet;
}
Goto& XAIFBaseParserHelper::getGoto() const {
if (!myGoto_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Goto" << " item not set");
return *myGoto_p;
}
bool XAIFBaseParserHelper::hasGoto() const {
return (myGoto_p!=0);
}

void XAIFBaseParserHelper::setIfStatement(IfStatement& whatToSet) {
if (myIfStatement_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "IfStatement" << " item already set");
myIfStatement_p=&whatToSet;
}
IfStatement& XAIFBaseParserHelper::getIfStatement() const {
if (!myIfStatement_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "IfStatement" << " item not set");
return *myIfStatement_p;
}
bool XAIFBaseParserHelper::hasIfStatement() const {
return (myIfStatement_p!=0);
}

void XAIFBaseParserHelper::setIndexTriplet(IndexTriplet& whatToSet) {
if (myIndexTriplet_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "IndexTriplet" << " item already set");
myIndexTriplet_p=&whatToSet;
}
IndexTriplet& XAIFBaseParserHelper::getIndexTriplet() const {
if (!myIndexTriplet_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "IndexTriplet" << " item not set");
return *myIndexTriplet_p;
}
bool XAIFBaseParserHelper::hasIndexTriplet() const {
return (myIndexTriplet_p!=0);
}

void XAIFBaseParserHelper::setInitialization(Initialization& whatToSet) {
if (myInitialization_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Initialization" << " item already set");
myInitialization_p=&whatToSet;
}
Initialization& XAIFBaseParserHelper::getInitialization() const {
if (!myInitialization_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Initialization" << " item not set");
return *myInitialization_p;
}
bool XAIFBaseParserHelper::hasInitialization() const {
return (myInitialization_p!=0);
}

void XAIFBaseParserHelper::setLabel(Label& whatToSet) {
if (myLabel_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Label" << " item already set");
myLabel_p=&whatToSet;
}
Label& XAIFBaseParserHelper::getLabel() const {
if (!myLabel_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Label" << " item not set");
return *myLabel_p;
}
bool XAIFBaseParserHelper::hasLabel() const {
return (myLabel_p!=0);
}

void XAIFBaseParserHelper::setPostLoop(PostLoop& whatToSet) {
if (myPostLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "PostLoop" << " item already set");
myPostLoop_p=&whatToSet;
}
PostLoop& XAIFBaseParserHelper::getPostLoop() const {
if (!myPostLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "PostLoop" << " item not set");
return *myPostLoop_p;
}
bool XAIFBaseParserHelper::hasPostLoop() const {
return (myPostLoop_p!=0);
}

void XAIFBaseParserHelper::setPreLoop(PreLoop& whatToSet) {
if (myPreLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "PreLoop" << " item already set");
myPreLoop_p=&whatToSet;
}
PreLoop& XAIFBaseParserHelper::getPreLoop() const {
if (!myPreLoop_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "PreLoop" << " item not set");
return *myPreLoop_p;
}
bool XAIFBaseParserHelper::hasPreLoop() const {
return (myPreLoop_p!=0);
}

void XAIFBaseParserHelper::setScope(Scope& whatToSet) {
if (myScope_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Scope" << " item already set");
myScope_p=&whatToSet;
}
Scope& XAIFBaseParserHelper::getScope() const {
if (!myScope_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Scope" << " item not set");
return *myScope_p;
}
bool XAIFBaseParserHelper::hasScope() const {
return (myScope_p!=0);
}

void XAIFBaseParserHelper::setScopes(Scopes& whatToSet) {
if (myScopes_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Scopes" << " item already set");
myScopes_p=&whatToSet;
}
Scopes& XAIFBaseParserHelper::getScopes() const {
if (!myScopes_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Scopes" << " item not set");
return *myScopes_p;
}
bool XAIFBaseParserHelper::hasScopes() const {
return (myScopes_p!=0);
}

void XAIFBaseParserHelper::setSideEffectList(SideEffectList& whatToSet) {
if (mySideEffectList_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "SideEffectList" << " item already set");
mySideEffectList_p=&whatToSet;
}
SideEffectList& XAIFBaseParserHelper::getSideEffectList() const {
if (!mySideEffectList_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "SideEffectList" << " item not set");
return *mySideEffectList_p;
}
bool XAIFBaseParserHelper::hasSideEffectList() const {
return (mySideEffectList_p!=0);
}

void XAIFBaseParserHelper::setStatementIdSetMap(StatementIdSetMap& whatToSet) {
if (myStatementIdSetMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "StatementIdSetMap" << " item already set");
myStatementIdSetMap_p=&whatToSet;
}
StatementIdSetMap& XAIFBaseParserHelper::getStatementIdSetMap() const {
if (!myStatementIdSetMap_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "StatementIdSetMap" << " item not set");
return *myStatementIdSetMap_p;
}
bool XAIFBaseParserHelper::hasStatementIdSetMap() const {
return (myStatementIdSetMap_p!=0);
}

void XAIFBaseParserHelper::setStatementIdSetMapEntry(StatementIdSetMapEntry& whatToSet) {
if (myStatementIdSetMapEntry_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "StatementIdSetMapEntry" << " item already set");
myStatementIdSetMapEntry_p=&whatToSet;
}
StatementIdSetMapEntry& XAIFBaseParserHelper::getStatementIdSetMapEntry() const {
if (!myStatementIdSetMapEntry_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "StatementIdSetMapEntry" << " item not set");
return *myStatementIdSetMapEntry_p;
}
bool XAIFBaseParserHelper::hasStatementIdSetMapEntry() const {
return (myStatementIdSetMapEntry_p!=0);
}

void XAIFBaseParserHelper::setSubroutineCall(SubroutineCall& whatToSet) {
if (mySubroutineCall_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "SubroutineCall" << " item already set");
mySubroutineCall_p=&whatToSet;
}
SubroutineCall& XAIFBaseParserHelper::getSubroutineCall() const {
if (!mySubroutineCall_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "SubroutineCall" << " item not set");
return *mySubroutineCall_p;
}
bool XAIFBaseParserHelper::hasSubroutineCall() const {
return (mySubroutineCall_p!=0);
}

void XAIFBaseParserHelper::setSymbol(Symbol& whatToSet) {
if (mySymbol_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Symbol" << " item already set");
mySymbol_p=&whatToSet;
}
Symbol& XAIFBaseParserHelper::getSymbol() const {
if (!mySymbol_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Symbol" << " item not set");
return *mySymbol_p;
}
bool XAIFBaseParserHelper::hasSymbol() const {
return (mySymbol_p!=0);
}

void XAIFBaseParserHelper::setSymbolReferenceProperty(SymbolReferenceProperty& whatToSet) {
if (mySymbolReferenceProperty_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "SymbolReferenceProperty" << " item already set");
mySymbolReferenceProperty_p=&whatToSet;
}
SymbolReferenceProperty& XAIFBaseParserHelper::getSymbolReferenceProperty() const {
if (!mySymbolReferenceProperty_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "SymbolReferenceProperty" << " item not set");
return *mySymbolReferenceProperty_p;
}
bool XAIFBaseParserHelper::hasSymbolReferenceProperty() const {
return (mySymbolReferenceProperty_p!=0);
}

void XAIFBaseParserHelper::setSymbolTable(SymbolTable& whatToSet) {
if (mySymbolTable_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "SymbolTable" << " item already set");
mySymbolTable_p=&whatToSet;
}
SymbolTable& XAIFBaseParserHelper::getSymbolTable() const {
if (!mySymbolTable_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "SymbolTable" << " item not set");
return *mySymbolTable_p;
}
bool XAIFBaseParserHelper::hasSymbolTable() const {
return (mySymbolTable_p!=0);
}

void XAIFBaseParserHelper::setUpdate(Update& whatToSet) {
if (myUpdate_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Update" << " item already set");
myUpdate_p=&whatToSet;
}
Update& XAIFBaseParserHelper::getUpdate() const {
if (!myUpdate_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Update" << " item not set");
return *myUpdate_p;
}
bool XAIFBaseParserHelper::hasUpdate() const {
return (myUpdate_p!=0);
}

void XAIFBaseParserHelper::setVariable(Variable& whatToSet) {
if (myVariable_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "Variable" << " item already set");
myVariable_p=&whatToSet;
}
Variable& XAIFBaseParserHelper::getVariable() const {
if (!myVariable_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "Variable" << " item not set");
return *myVariable_p;
}
bool XAIFBaseParserHelper::hasVariable() const {
return (myVariable_p!=0);
}

void XAIFBaseParserHelper::setVariableEdge(VariableEdge& whatToSet) {
if (myVariableEdge_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::set " << "VariableEdge" << " item already set");
myVariableEdge_p=&whatToSet;
}
VariableEdge& XAIFBaseParserHelper::getVariableEdge() const {
if (!myVariableEdge_p)
THROW_LOGICEXCEPTION_MACRO("XAIFBaseParserHelper" << " ::get " << "VariableEdge" << " item not set");
return *myVariableEdge_p;
}
bool XAIFBaseParserHelper::hasVariableEdge() const {
return (myVariableEdge_p!=0);
}

