/*! \file
  
  \brief Implementation of ExprTree.

  \authors Nathan Tallent, Michelle Strout
  \version $Id: NewExprTree.cpp,v 1.1 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/
// standard headers

#include "NewExprTree.hpp"

// included here instead of in hpp file to remove circular reference
#include <OpenAnalysis/NewExprTree/NewExprTreeVisitor.hpp>
#include <OpenAnalysis/NewExprTree/OutputNewExprTreeVisitor.hpp>

namespace OA {

int debug = false;


//--------------------------------------------------------------------
NewExprTree::NewExprTree() 
{
    OA_DEBUG_CTRL_MACRO("DEBUG_NewExprTree:ALL", debug);
}

NewExprTree::~NewExprTree()
{
}

//! visit nodes in require order and then accept visitor on tree
void NewExprTree::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitExprTreeBefore(*this); 
    getRoot()->acceptVisitor(pVisitor);
    pVisitor.visitExprTreeAfter(*this); 
}

/*!
   Copies the given subtree into this tree as a child of the given node
*/


//FOR NOW COMMENTED	
void NewExprTree::copyAndConnectSubTree(OA_ptr<Node> src, OA_ptr<NewExprTree> subtree)
{
    std::map<OA_ptr<Node>,OA_ptr<Node> > oldToNewNodeMap;

    // iterate over all nodes to get copy
    NodesIterator nIter(*subtree);
    for ( ; nIter.isValid(); ++nIter) {
      oldToNewNodeMap[nIter.current()] = nIter.current()->copy();
    }

    // iterate over all edges of the subtree
    EdgesIterator eIter(*subtree);
    for ( ; eIter.isValid(); ++eIter ) {
        OA_ptr<Edge> edge = eIter.current();
        connect( oldToNewNodeMap[edge->source()],
                 oldToNewNodeMap[edge->sink()] );
    }
    connect(src,oldToNewNodeMap[subtree->getRoot()]);
} 


void
NewExprTree::dump (std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
  os << "===== ExprTree: =====" << std::endl;
  
  // print the contents of all the nodes
  //PreOrderIterator nodes_iter(*this);
  NodesIterator nodes_iter(*this);
  for ( ; nodes_iter.isValid(); ++nodes_iter) {
    OA_ptr<NewExprTree::Node> node = nodes_iter.current();
    node->dump(os,ir);
  }
  
  os << "====================" << std::endl << std::endl;
}

bool NewExprTree::operator<(NewExprTree &rhs) {
    if(debug) { cout << "NewExprTree < NewExprTree\n"; }

    OA_ptr<Node> rootLHS, rootRHS;

    rootLHS = getRoot();
    rootRHS = rhs.getRoot();

    return compareTreesRootedAt(rootLHS, rootRHS);
}

bool NewExprTree::compareTreesRootedAt(OA_ptr<NewExprTree::Node> rootLHS,
    OA_ptr<NewExprTree::Node> rootRHS)
{
    int nChildrenLHS, nChildrenRHS;
    OA_ptr<Tree::Node> bChildLHS, bChildRHS;
    OA_ptr<NewExprTree::Node> childLHS, childRHS;

    if(debug) {
        cout << "Compare Trees Rooted at: " << rootLHS->getName()
             << " and " << rootRHS->getName() << endl;
    }

    nChildrenLHS = rootLHS->num_children();
    nChildrenRHS = rootRHS->num_children();
    
    // First compare by the root nodes, if they're the same
    // then ordering by the number of children.
    if(*rootLHS < *rootRHS) { return true; }
    else if(*rootRHS < *rootLHS) { return false; }
    else if(nChildrenLHS < nChildrenRHS) { return true; }
    else if(nChildrenRHS < nChildrenLHS) { return false; }

    //iterate through children. At this point we know nChildrenLHS == nChildrenRHS
    for(int i = 0; i < nChildrenLHS; i++) 
    { 
        bChildLHS = rootLHS->child(i);
        bChildRHS = rootRHS->child(i);
        childLHS = bChildLHS.convert<NewExprTree::Node>();
        childRHS = bChildRHS.convert<NewExprTree::Node>();

        if(compareTreesRootedAt(childLHS, childRHS) == true) {
            return true;
        }
        else if(compareTreesRootedAt(childRHS, childLHS) == true) {
            return false;
        }
    }

    // if execution get's here than the LHS tree and RHS side trees are equivelent
    // so (LHS < RHS) == false.
    return false;
}

bool NewExprTree::operator==(NewExprTree &rhs) {
    // Determine equality by ensuring that the LHS < RHS is false
    // and RHS < LHS is false.
    if(!(*this < rhs) && !(rhs < *this)) { return true; }
    return false;
}




void
NewExprTree::output(IRHandlesIRInterface& ir)
{
  
  NodesIterator nodes_iter(*this);

  sOutBuild->graphStart("EXPRESSION TREE");
  
  OutputNewExprTreeVisitor OutputTreeVisitor(ir);
  acceptVisitor(OutputTreeVisitor);

  sOutBuild->graphEnd("Expression Tree");

}
//---------------------------------------------------------------
//Node
//---------------------------------------------------------------

//-------------------------------------------------------------------
// OpNode
//-------------------------------------------------------------------

void NewExprTree::OpNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
    Node::dump(os,ir);  
    os << "handle: " << mOpInterface->toString() << std::endl;
}

void NewExprTree::OpNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET OpNode ( " << mOpInterface->toString() << " )";
  sOutBuild->outputString( label.str() );
}

void NewExprTree::OpNode::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitOpNode(*this); 
}

bool NewExprTree::OpNode::operator<(Node& rhs) {
    if(debug) { cout << "OpNode < Node\n"; }

    // First check for a less-than relationship by comparing the nodes'
    // ordering constants. If that constants are the same than both nodes
    // are the same type.
    if(getOrder() < rhs.getOrder()) { return true; }
    else if(rhs.getOrder() < getOrder()) { return false; }

    return *this < (*(OpNode*)&rhs);
}

bool NewExprTree::OpNode::operator<(OpNode& rhs) {
    if(debug) { cout << "OpNode < OpNode\n"; }

    return *getOpInterface() < *rhs.getOpInterface();
}

//-------------------------------------------------------------------
// CallNode
//-------------------------------------------------------------------
void NewExprTree::CallNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void NewExprTree::CallNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET CallNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void NewExprTree::CallNode::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitCallNode(*this); 
}

bool NewExprTree::CallNode::operator<(Node& rhs) {
    if(debug) { cout << "CallNode < Node\n"; }

    // First check for a less-than relationship by comparing the nodes'
    // ordering constants. If that constants are the same than both nodes
    // are the same type.
    if(getOrder() < rhs.getOrder()) { return true; }
    else if(rhs.getOrder() < getOrder()) { return false; }

    return *this < (*(CallNode*)&rhs);
}

bool NewExprTree::CallNode::operator<(CallNode& rhs) {
    if(debug) { cout << "CallNode < CallNode\n"; }

    return getHandle() < rhs.getHandle();
}

//-------------------------------------------------------------------
// MemRefNode
//-------------------------------------------------------------------
void NewExprTree::MemRefNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
    assert(false); // function needs implementation
//  Node::dump(os,ir);  
//  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void NewExprTree::MemRefNode::output(IRHandlesIRInterface& ir)
{
    assert(false); // function needs implementation
//  ostringstream label;
//  label << "ET MemRefNode ( " << ir.toString(mHandle) << " )";
//  sOutBuild->outputString( label.str() );
}

void NewExprTree::MemRefNode::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitMemRefNode(*this); 
}

bool NewExprTree::MemRefNode::operator<(Node& rhs) {
    if(debug) { cout << "MemRefNode < Node\n"; }

    // First check for a less-than relationship by comparing the nodes'
    // ordering constants. If that constants are the same than both nodes
    // are the same type.
    if(getOrder() < rhs.getOrder()) { return true; }
    else if(rhs.getOrder() < getOrder()) { return false; }

    return *this < (*(MemRefNode*)&rhs);
}

bool NewExprTree::MemRefNode::operator<(MemRefNode& rhs) {
    if(debug) { cout << "MemRefNode < MemRefNode\n"; }

    return *getMRE() < *rhs.getMRE();
}

//-------------------------------------------------------------------
// ConstSymNode
//-------------------------------------------------------------------
void NewExprTree::ConstSymNode::dump(std::ostream& os, 
                                  OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void NewExprTree::ConstSymNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET ConstSymNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void NewExprTree::ConstSymNode::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitConstSymNode(*this); 
}

bool NewExprTree::ConstSymNode::operator<(Node& rhs) {
    if(debug) { cout << "ConstSymNode < Node\n"; }

    // First check for a less-than relationship by comparing the nodes'
    // ordering constants. If that constants are the same than both nodes
    // are the same type.
    if(getOrder() < rhs.getOrder()) { return true; }
    else if(rhs.getOrder() < getOrder()) { return false; }

    return *this < (*(ConstSymNode*)&rhs);
}

bool NewExprTree::ConstSymNode::operator<(ConstSymNode& rhs) {
    if(debug) { cout << "ConstSymNode < ConstSymNode\n"; }

    return getHandle() < rhs.getHandle();
}

//-------------------------------------------------------------------
// ConstValNode
//-------------------------------------------------------------------
void NewExprTree::ConstValNode::dump(std::ostream& os, 
                                  OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << mConstValInter->toString() << std::endl; 
}

void NewExprTree::ConstValNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET ConstValNode ( " << mConstValInter->toString() << " )";
  sOutBuild->outputString( label.str() );
}

void NewExprTree::ConstValNode::acceptVisitor(NewExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitConstValNode(*this); 
}

bool NewExprTree::ConstValNode::operator<(Node& rhs) {
    if(debug) { cout << "ConstValNode < Node\n"; }

    // First check for a less-than relationship by comparing the nodes'
    // ordering constants. If that constants are the same than both nodes
    // are the same type.
    if(getOrder() < rhs.getOrder()) { return true; }
    else if(rhs.getOrder() < getOrder()) { return false; }

    return *this < (*(ConstValNode*)&rhs);
}

bool NewExprTree::ConstValNode::operator<(ConstValNode& rhs) {
    if(debug) { cout << "ConstValNode < ConstValNode\n"; }

    return *getConstValInterface() < *rhs.getConstValInterface();
}


//====================================================================
//====================================================================
//====================================================================

//--------------------------------------------------------------------
void NewExprTree::Edge::dump(std::ostream& os)
{
  std::cout << "{Edge: parent: " << parent() << " child: " << child()
	    << "}" << std::endl;
}

//--------------------------------------------------------------------
void
NewExprTree::Node::dump (std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    os << "{Node: "
	<< "  " << mName;
	//<< ", attr:  " << mAttr;
}

//--------------------------------------------------------------------
/*
void
ExprTree::Node::str(ostream& os){
  if (isSym()) {
    os << symHndl << ":";
  }
  if (isConst()) {
    os << constHndl << ":";
  }
  if (isAttr()) {
    os << attr << ":";
  }
}

//-----------------------------------------------------------------
void ExprTree::str(ostream &os){
  PreOrderIterator nodes_iter(*this);
  for ( ; (bool)nodes_iter; ++nodes_iter) {
    ExprTree::Node* node = 
      dynamic_cast<ExprTree::Node*>((Tree::Node*)nodes_iter);
    node->str(os);
  }
}
*/

} // end of OA namespace
