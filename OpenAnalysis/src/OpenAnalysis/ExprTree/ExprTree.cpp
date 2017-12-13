/*! \file
  
  \brief Implementation of ExprTree.

  \authors Nathan Tallent, Michelle Strout
  \version $Id: ExprTree.cpp,v 1.16 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/
// standard headers

#include "ExprTree.hpp"

// included here instead of in hpp file to remove circular reference
#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/ExprTree/OutputExprTreeVisitor.hpp>

namespace OA {

//--------------------------------------------------------------------
ExprTree::ExprTree() 
{
}

ExprTree::~ExprTree()
{
}

OA_ptr<std::set<OA_ptr<ExprTree> > >
intersectExprTreeSets(std::set<OA_ptr<ExprTree> >& set1,
                 std::set<OA_ptr<ExprTree> >& set2)
{
    OA_ptr<set<OA_ptr<ExprTree> > > temp;
    temp = new set<OA_ptr<ExprTree> >;
    std::set_intersection(set1.begin(), set1.end(),
                          set2.begin(), set2.end(),
                          std::inserter(*temp,temp->end()));
    return temp;
}


OA_ptr<std::set<OA_ptr<ExprTree> > >
unionExprTreeSets(std::set<OA_ptr<ExprTree> >& set1, std::set<OA_ptr<ExprTree> >& set2)
{
  OA_ptr<std::set<OA_ptr<ExprTree> > > temp;
  temp = new std::set<OA_ptr<ExprTree> >;

  std::set<OA_ptr<ExprTree> >::iterator setIter;
  int count1=0;
  for (setIter=set1.begin(); setIter!=set1.end(); setIter++) {
      OA_ptr<ExprTree> loc = *setIter;
      count1++;
  }

  int count2=0;
  for (setIter=set2.begin(); setIter!=set2.end(); setIter++) {
      OA_ptr<ExprTree> loc = *setIter;
      count2++;
  }

  std::set_union(set1.begin(), set1.end(),
                 set2.begin(), set2.end(),
                 std::inserter(*temp,temp->end()));
  return temp;
}






//! visit nodes in require order and then accept visitor on tree
void ExprTree::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitExprTreeBefore(*this); 
    getRoot()->acceptVisitor(pVisitor);
    pVisitor.visitExprTreeAfter(*this); 
}

/*!
   Copies the given subtree into this tree as a child of the given node
*/


//FOR NOW COMMENTED	
void ExprTree::copyAndConnectSubTree(OA_ptr<Node> src, OA_ptr<ExprTree> subtree)
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
ExprTree::dump (std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
  os << "===== ExprTree: =====" << std::endl;
  
  // print the contents of all the nodes
  //PreOrderIterator nodes_iter(*this);
  NodesIterator nodes_iter(*this);
  for ( ; nodes_iter.isValid(); ++nodes_iter) {
    OA_ptr<ExprTree::Node> node = nodes_iter.current();
    node->dump(os,ir);
  }
  
  os << "====================" << std::endl << std::endl;
}

bool ExprTree::operator<(ExprTree &other) 
{

  return getRoot() < other.getRoot(); 
}

 

bool ExprTree::operator==(ExprTree &other) 
{
  return getRoot() == other.getRoot();
}

void
ExprTree::output(IRHandlesIRInterface& ir)
{
  
  NodesIterator nodes_iter(*this);

  sOutBuild->graphStart("EXPRESSION TREE");
  
  OutputExprTreeVisitor OutputTreeVisitor(ir);
  acceptVisitor(OutputTreeVisitor);
    
/*  for ( ; nodes_iter.isValid(); ++nodes_iter) {
    OA_ptr<ExprTree::Node> node = nodes_iter.current();
    ostringstream align;
    align << indt;
    sOutBuild->outputString( align.str() );
    
    ostringstream pushIndent;
    pushIndent << pushIndt;
    sOutBuild->outputString( pushIndent.str() );

    //node->output(ir);
    node->acceptVisitor(OutputTreeVisitor);
  
    //ostringstream popIndent;
    //popIndent << popIndt << indt;
    //sOutBuild->outputString( popIndent.str() );
  }
*/

  sOutBuild->graphEnd("Expression Tree");

}
//---------------------------------------------------------------
//Node
//---------------------------------------------------------------

bool ExprTree::Node::operator<(Node& other)
{
    bool retval = false;
    if(getOrder() < other.getOrder()) { retval = true; }
    return retval;


   /* if(getOrder() < other.getOrder()) { return true; }
    else if(getOrder() > other.getOrder()) { return false; }

    // execution will reach here of two NamedRef objects are being compared
    Node& ref = static_cast<Node&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }*/

}

bool ExprTree::Node::operator==(Node& other)
{
    bool retval = false;
    if(getOrder() == other.getOrder()) { retval = true; }
    return retval;

   /* if(getOrder() != other.getOrder()) { return false; }

    // execution reaches here if two NamedRef objects are being compared
    Node& ref = static_cast<Node&>(other);

    if( getHandle() == ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }*/


}






//-------------------------------------------------------------------
// OpNode
//-------------------------------------------------------------------

void ExprTree::OpNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
    Node::dump(os,ir);  
    os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void ExprTree::OpNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET OpNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void ExprTree::OpNode::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitOpNode(*this); 
}

bool ExprTree::OpNode::operator<(Node& other)
{
   if(getOrder() < other.getOrder()) { return true; }
    else if(getOrder() > other.getOrder()) { return false; }

    // execution will reach here of two NamedRef objects are being compared
    OpNode& ref = static_cast<OpNode&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

} 

bool ExprTree::OpNode::operator==(Node& other)
{
  if(getOrder() != other.getOrder()) { return false; }

    // execution reaches here if two NamedRef objects are being compared
    OpNode& ref = static_cast<OpNode&>(other);

    if( getHandle() == ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

} 

//-------------------------------------------------------------------
// CallNode
//-------------------------------------------------------------------
void ExprTree::CallNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void ExprTree::CallNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET CallNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void ExprTree::CallNode::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitCallNode(*this); 
}

bool ExprTree::CallNode::operator<(Node& other)
{
    if(getOrder() < other.getOrder()) { return true; }
    else if(getOrder() > other.getOrder()) { return false; }

    return false;
    // execution will reach here of two UnnamedRef objects are being compared
    CallNode& ref = static_cast<CallNode&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }


} 

bool ExprTree::CallNode::operator==(Node& other)
{
        return false;
} 


//-------------------------------------------------------------------
// MemRefNode
//-------------------------------------------------------------------
void ExprTree::MemRefNode::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void ExprTree::MemRefNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET MemRefNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void ExprTree::MemRefNode::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitMemRefNode(*this); 
}
bool ExprTree::MemRefNode::operator<(Node& other)
{
   if(getOrder() < other.getOrder()) { return true; }
    else if(getOrder() > other.getOrder()) { return false; }

    // execution will reach here of two Deref objects are being compared
    MemRefNode& ref = static_cast<MemRefNode&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}

bool ExprTree::MemRefNode::operator==(Node& other)
{
   if(getOrder() != other.getOrder()) { return false; }

    MemRefNode& ref = static_cast<MemRefNode&>(other);

    if( getHandle() == ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}

//-------------------------------------------------------------------
// ConstSymNode
//-------------------------------------------------------------------
void ExprTree::ConstSymNode::dump(std::ostream& os, 
                                  OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void ExprTree::ConstSymNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET ConstSymNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void ExprTree::ConstSymNode::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitConstSymNode(*this); 
}
bool ExprTree::ConstSymNode::operator<(Node& other)
{
    if(getOrder() < other.getOrder()) { return true; }
    else if(getOrder() > other.getOrder()) { return false; }

    // execution will reach here of two Deref objects are being compared
    ConstSymNode& ref = static_cast<ConstSymNode&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}

bool ExprTree::ConstSymNode::operator==(Node& other)
{
    if(getOrder() != other.getOrder()) { return false; }

    ConstSymNode& ref = static_cast<ConstSymNode&>(other);

    if( getHandle() == ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}

//-------------------------------------------------------------------
// ConstValNode
//-------------------------------------------------------------------
void ExprTree::ConstValNode::dump(std::ostream& os, 
                                  OA_ptr<IRHandlesIRInterface> ir)
{ 
  Node::dump(os,ir);  
  os << "handle: " << ir->toString(mHandle) << std::endl; 
}

void ExprTree::ConstValNode::output(IRHandlesIRInterface& ir)
{
  ostringstream label;
  label << "ET ConstValNode ( " << ir.toString(mHandle) << " )";
  sOutBuild->outputString( label.str() );
}

void ExprTree::ConstValNode::acceptVisitor(ExprTreeVisitor& pVisitor)
{ 
    pVisitor.visitConstValNode(*this); 
}
bool ExprTree::ConstValNode::operator<(Node& other)
{
  bool retval = false;
   if(getOrder() < other.getOrder()) { return true; }
    else 
      if(getOrder() > other.getOrder()) { return false; }

    // execution will reach here of two Deref objects are being compared
    ConstValNode& ref = static_cast<ConstValNode&>(other);

    if( getHandle() < ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}

bool ExprTree::ConstValNode::operator==(Node& other)
{
   if(getOrder() != other.getOrder()) { return false; }
    ConstValNode& ref = static_cast<ConstValNode&>(other);
    if( getHandle() == ref.getHandle() )
    {
        return true;
    } else {
        return false;
    }

}


//--------------------------------------------------------------------
void ExprTree::Edge::dump(std::ostream& os)
{
  std::cout << "{Edge: parent: " << parent() << " child: " << child()
	    << "}" << std::endl;
}

//--------------------------------------------------------------------
void
ExprTree::Node::dump (std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
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
