/*! \file
  
  \brief Definition of the AnnotationManager that generates a CallGraphStandard.

  \authors Arun Chauhan (2001 as part of Mint), Nathan Tallent, Michelle Strout, Brian White
  \version $Id: ManagerCallGraph.cpp,v 1.1 2007/10/08 19:19:11 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

// standard headers

#ifdef NO_STD_CHEADERS
# include <stdlib.h>
# include <string.h>
# include <assert.h>
#else
# include <cstdlib>
# include <cstring>
# include <cassert>
using namespace std; // For compatibility with non-std C headers
#endif

#include <iostream>

#include "ManagerCallGraph.hpp"

namespace OA {
  namespace CallGraph {

static bool debug = false;

ManagerCallGraphStandard::ManagerCallGraphStandard(OA_ptr<CallGraphIRInterface> _ir) : mIR(_ir)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_ManagerCallGraph:ALL", debug);
}

//--------------------------------------------------------------------
OA_ptr<CallGraph> ManagerCallGraphStandard::performAnalysis (
        OA_ptr<IRProcIterator> procIter, 
        OA_ptr<Alias::InterAliasInterface> interAlias 
) 
{
  mCallGraph = new CallGraph();
  mInterAlias = interAlias;

  build_graph(procIter);

  // document CallHandle to ProcHandle Set Map

  OA_ptr<EdgesIteratorInterface> edgeIter;
  edgeIter = mCallGraph->getCallGraphEdgesIterator();
  for (; edgeIter->isValid(); edgeIter->operator++()) {
    OA_ptr<EdgeInterface> edge = edgeIter->currentCallGraphEdge();
    CallHandle call = edge->getCallHandle();
    OA_ptr<NodeInterface> dest = edge->getCallGraphSink();
    ProcHandle proc = dest->getProc();
    
    if (debug) {
      std::cout << "Inserting call:'" << mIR->toString(call) << "'"
                << " to proc:'" << mIR->toString(proc) << "'\n";
    }

    mCallGraph->addToCallProcSetMap(call, proc);
  }


  return mCallGraph;
}

//--------------------------------------------------------------------
void
ManagerCallGraphStandard::build_graph(OA_ptr<IRProcIterator> procIter)
{
  // Iterate over all the procedures in the program
  for ( procIter->reset(); procIter->isValid(); ++(*procIter)) { 
    
    // Create a node for this procedure
    ProcHandle currProc = procIter->current();
    SymHandle currProcSym = mIR->getProcSymHandle(currProc);
    if (debug) {
      std::cout << "currProc = " << mIR->toString(currProc);
      std::cout << ", currProcSym = " << mIR->toString(currProcSym) 
                << std::endl;
    }

    OA_ptr<Alias::Interface> alias = mInterAlias->getAliasResults(currProc);

    OA_ptr<Node> currProcNode 
      = mCallGraph->findOrAddNode(currProcSym);
    currProcNode->add_def(currProc); 

  if (debug) {
      std::cout << "After currProc = " << mIR->toString(currProc);
      std::cout << ", After currProcSym = " << mIR->toString(currProcSym)
                << std::endl;
    }

    
    // Iterate over the statements of this procedure
    OA_ptr<IRStmtIterator> stmtIterPtr = mIR->getStmtIterator(currProc);
    // Iterate over the statements of this block adding procedure references
    for ( ; stmtIterPtr->isValid(); ++(*stmtIterPtr)) {
      StmtHandle stmt = stmtIterPtr->current();

      // Iterate over procedure calls of a statement
      OA_ptr<IRCallsiteIterator> callsiteItPtr = mIR->getCallsites(stmt);
      if (debug) {
        if (callsiteItPtr->isValid()) {
          std::cout << "\n***\n  Call Statement:\n     <'";
          std::cout << mIR->toString(stmt) << "'>";
        }
      }
      for ( ; callsiteItPtr->isValid(); ++(*callsiteItPtr)) {
        CallHandle call = callsiteItPtr->current();

        if (debug) {
          std::cout << "\n        Call: ["
                    << mIR->toString(call) << "]";
        }
        // get the mre for the function call (eg. NamedRef('foo'))
        OA_ptr<MemRefExpr> callMRE = mIR->getCallMemRefExpr(call);

        // iterate over the may locs for this callMRE
        OA_ptr<LocIterator> mayCallItPtr = alias->getMayLocs(*callMRE,
                                                              currProc);
        for(mayCallItPtr->reset(); mayCallItPtr->isValid(); ++(*mayCallItPtr)) {
           OA_ptr<Location> loc = mayCallItPtr->current();

           if (loc->isaNamed()) {
             OA_ptr<NamedLoc> namedProc;
             namedProc = loc.convert<NamedLoc>();

             // get the symbol for the function
             SymHandle sym = namedProc->getSymHandle();

             // Add a node (if nonexistent) and edge
             OA_ptr<Node> node
               = mCallGraph->findOrAddNode(sym);
             node->add_call(call);

             if(debug)
             {
                 std::cout << "ManagerCallGraphStandard currProcNode";
                 std::cout << currProcNode->getId();
                 std::cout << std::endl;
             }
            
             if(debug)
             {
                 std::cout << "ManagerCallGraphStandard Node";
                 std::cout << node->getId();
                 std::cout << std::endl;
             }

             
             mCallGraph->connect(currProcNode, node,
                                 NORMAL_EDGE, call);

           } // if may loc isaNamed()

        } // iterating over may locs for the callMRE

      } // iterating over calls of a statement

      if (debug) {
        callsiteItPtr->reset();
        if (callsiteItPtr->isValid()) {
          std::cout << "\n***\n";
        }
      }

    } // iterating over statements

  } // iterating over procedures

}

  } // end CallGraph namespace
} // end OA namespace
