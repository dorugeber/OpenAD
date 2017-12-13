/*! \file
  
  \brief Implementation of OutputExprTreeVisitor class.
  
  \authors Luis Ramos
  \version $Id: OutputNewExprTreeVisitor.cpp,v 1.1 2008/02/06 19:58:48 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include <OpenAnalysis/NewExprTree/OutputNewExprTreeVisitor.hpp>

static bool debug = false;

namespace OA {


OutputNewExprTreeVisitor::OutputNewExprTreeVisitor(IRHandlesIRInterface& ir)
	:mIR(ir)
{
  mId = 0;
}

//! if we don't know how to handle a node then the default action
//! is to change the eval result to NULL
void OutputNewExprTreeVisitor::visitNode(NewExprTree::Node&)
{
    std::cout << "visitNode\n";
}

void OutputNewExprTreeVisitor::visitOpNode(NewExprTree::OpNode& n)
{
  int Id = mId;
  ostringstream label;
  label << "ET OpNode ( " << n.getOpInterface()->toString() << " )";
  sOutBuild->graphNodeStart(mId);
  sOutBuild->graphNodeLabel(label.str());
  mId++;


  if(n.num_children() >= 1)
  {
    // if so then visit each child
    OA_ptr<NewExprTree::Node> cetNodePtr;
    NewExprTree::ChildNodesIterator cNodesIter(n);

    for(;cNodesIter.isValid(); ++cNodesIter, ++mId) {
      cetNodePtr = cNodesIter.current();
 
      sOutBuild->graphEdgeStart();
        sOutBuild->graphEdgeLabelStart();
        sOutBuild->graphEdgeSourceNode(Id);
          cetNodePtr->acceptVisitor(*this);
        sOutBuild->graphEdgeLabelEnd();
    }
    
  } else {
      assert(0);
  }

  mId--;
}

void OutputNewExprTreeVisitor::visitCallNode(NewExprTree::CallNode& n)
{
    CallHandle call = n.getHandle();
    ostringstream label;
    label << "ET CallNode ( " << mIR.toString(call) << " )";

    sOutBuild->graphNodeStart(mId);
    sOutBuild->graphNodeLabel(label.str());
}

void OutputNewExprTreeVisitor::visitMemRefNode(NewExprTree::MemRefNode& n)
{
    ostringstream label;
    label << "ET MemRefNode ( " << " )";

    sOutBuild->graphNodeStart(mId);
    sOutBuild->graphNodeLabel(label.str());
}

void OutputNewExprTreeVisitor::visitConstSymNode(NewExprTree::ConstSymNode& n)
{
    ConstSymHandle simnode = n.getHandle();
    ostringstream label;
    label << "ET ConstSymNode ( " << mIR.toString(simnode) << " )";

    sOutBuild->graphNodeStart(mId);
    sOutBuild->graphNodeLabel(label.str());
}

void OutputNewExprTreeVisitor::visitConstValNode(NewExprTree::ConstValNode& n)
{
    ostringstream label;
    label << "ET ConstValNode ( " << n.getConstValInterface() << " )";

    sOutBuild->graphNodeStart(mId);
    sOutBuild->graphNodeLabel(label.str());
}

void OutputNewExprTreeVisitor::output(IRHandlesIRInterface&)
{
}



} // end of OA namespace

