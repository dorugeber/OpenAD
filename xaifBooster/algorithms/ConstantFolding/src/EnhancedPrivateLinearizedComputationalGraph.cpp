// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
//let's begin by assuming we have the edge we want to remove...
//so we need to:
//1. find all edges that go to j^@
//2. make all edges that don't goto j now goto j
//3. make them have the right value
#include <sstream>
#include<list>
#include<iterator>
#include<iostream.h>
#include "xaifBooster/system/inc/Argument.hpp"

#include "xaifBooster/system/inc/Intrinsic.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/system/inc/ExpressionEdge.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/PrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/Linearization/inc/ExpressionEdgeAlg.hpp"
#include "xaifBooster/algorithms/Linearization/inc/IntrinsicAlg.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphEdge.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphVertex.hpp"
#include "xaifBooster/system/inc/Assignment.hpp"

using namespace xaifBooster;

namespace xaifBoosterConstantFolding {

  void EnhancedPrivateLinearizedComputationalGraph::back_elim(EnhancedPrivateLinearizedComputationalGraphEdge& e) {
    cout<<"back_elim";
    //1.find all i' depen i but not a depend of j
    //2.do the mult
    //3.find all i' depn j, and depend i
    //4.do the math
    bool Expression1exists=true,Expression2exists=true;
    EnhancedPrivateLinearizedComputationalGraphVertex& vo = (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( getTargetOf(e)));
    EnhancedPrivateLinearizedComputationalGraphVertex& vi = (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( getSourceOf(e)));
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIteratorPair vii(getInEdgesOf(vi));
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIterator currentEdge=vii.first,endEdge=vii.second;
    bool alreadyconnected;
    for(;currentEdge!=endEdge;++currentEdge) {
      alreadyconnected=false;
      EnhancedPrivateLinearizedComputationalGraphVertex& viii= (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( getSourceOf(*currentEdge)));
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair viiio (getOutEdgesOf(viii));
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator nowEdge=viiio.first,thenEdge=viiio.second;
      EnhancedPrivateLinearizedComputationalGraphEdge* conn1;
      EnhancedPrivateLinearizedComputationalGraphEdge* conn2;
      for(;nowEdge!=thenEdge;++nowEdge) {
        if(&vo == &(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( getTargetOf(*nowEdge)))) {
          alreadyconnected=true;
          conn2 = &(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *nowEdge));
        }
        if(&vi == &(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( getTargetOf(*nowEdge)))) {
          conn1 = &(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *nowEdge));
        }
      }

      xaifBoosterLinearization::ExpressionEdgeAlg* tempee1;
      Assignment* tempAssignment1;
      Expression* tempExpression1;

      xaifBoosterLinearization::ExpressionEdgeAlg* tempee2;
      Assignment* tempAssignment2;
      Expression* tempExpression2; 

      if(!e.isDirectCopyEdge()) {
        if (e.hasConstantFoldedAssignment()) {
          tempAssignment1=e.getConstantFoldedAssignment();
          tempExpression1=&(tempAssignment1->getRHS());
        }
        else {
          tempee1 = &(dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(e.getLinearizedExpressionEdge().getExpressionEdgeAlgBase()));      
          //      ExpressionEdge* tempee1=e.getLinearizedExpressionEdge ();
          tempAssignment1=(&(*tempee1).getConcretePartialAssignment());
          tempExpression1=(&(*tempAssignment1).getRHS());
        }
      }
      else {
	tempExpression1=new Expression();
	Expression1exists=false;
      }
      if (!(*conn1).isDirectCopyEdge()) {
        if((*conn1).hasConstantFoldedAssignment()) {
          tempAssignment2=(*conn1).getConstantFoldedAssignment();
          tempExpression2=&(tempAssignment2->getRHS());
        }
        else {
          tempee2 = &(dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>((*conn1).getLinearizedExpressionEdge().getExpressionEdgeAlgBase())); 
          //    ExpressionEdge tempee2=conn1.getLinearizedExpressionEdge ();
          tempAssignment2=&((*tempee2).getConcretePartialAssignment ());
          tempExpression2=&((*tempAssignment2).getRHS());
        }
      }
      else {
	tempExpression2=new Expression();
	Expression2exists=false;
      }
      if(alreadyconnected) {
        Expression* Expression3;
        Assignment* Assignment3;
        if ((*conn2).hasConstantFoldedAssignment()) {
          Assignment3 = (*conn2).getConstantFoldedAssignment();
          Expression3= &(Assignment3->getRHS());
        }
        else {
          xaifBoosterLinearization::ExpressionEdgeAlg& tempee3 =  dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>((*conn2).getLinearizedExpressionEdge().getExpressionEdgeAlgBase()); 
          //    ExpressionEdge tempee3=conn2.getLinearizedExpressionEdge ();
          Assignment& tempAssignment3=tempee3.getConcretePartialAssignment ();
          Expression3= &(tempAssignment3.getRHS());
        }
        //find maximal vertex (Ie: vertex that has no out edges)
        Expression::VertexIteratorPair vit3 ((*Expression3).vertices());
        Expression::VertexIterator startVertex3=vit3.first,endVertex3=vit3.second;
        ExpressionVertex* v3;
        for (;startVertex3!=endVertex3;++startVertex3) {
          if((*Expression3).numOutEdgesOf((dynamic_cast <ExpressionVertex&>( *startVertex3)))==0) {
            v3 = &(dynamic_cast <ExpressionVertex&>( *startVertex3));
          }
        }  

	Intrinsic* I1 = (new Intrinsic ("mul_scal_scal"));
	if(Expression1exists&&Expression2exists) {
	  (*I1).setId((*Expression3).getNextVertexId());
	}
        Intrinsic* I2 = (new Intrinsic ("add_scal_scal"));
        (*I2).setId((*Expression3).getNextVertexId());
	if(Expression1exists)
	  (*tempExpression1).copyMyselfInto(*Expression3,true,false);
	if(Expression2exists)
	  (*tempExpression2).copyMyselfInto(*Expression3,true,false);
	
	ExpressionVertex* v(0);
	if(Expression1exists) {
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit ((*Expression3).vertices());
	  Expression::VertexIterator startVertex =vit.first,endVertex =vit.second;
	  for (;startVertex!=endVertex;++startVertex) {
	    if((*Expression3).numOutEdgesOf((dynamic_cast <ExpressionVertex&>( *startVertex)))==0 && (&(*startVertex)) != v3) {
	      v = &(dynamic_cast <ExpressionVertex&>( *startVertex));
	      break;
	    }
	  }
	}

	ExpressionVertex* v2(0);
	if(Expression2exists) {
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit2 ((*Expression3).vertices());
	  Expression::VertexIterator startVertex2=vit2.first, endVertex2=vit2.second;
	  for (;startVertex2!=endVertex2;++startVertex2) {
	    if((*Expression3).numOutEdgesOf((dynamic_cast <ExpressionVertex&>( *startVertex2)))==0 &&  (!Expression1exists ||(&(*startVertex2)) != v) && (&(*startVertex2)) != v3) {
	      v2 = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
	      break;
	    }
	  }
	}
	if(Expression1exists&&Expression2exists) 
	  (*Expression3).supplyAndAddVertexInstance(*I1);
        (*Expression3).supplyAndAddVertexInstance(*I2);
	if(Expression1exists&&Expression2exists) {
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*I1,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE3=(*Expression3).addEdge(*v,*I1);
	  ExpresE3.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE4=(*Expression3).addEdge(*v2,*I1);
	  ExpresE4.setId((*Expression3).getNextEdgeId());
	}
	else if (Expression1exists) {
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	}
	else if (Expression2exists) {
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v2,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	}
        ExpressionEdge& ExpresE2=(*Expression3).addEdge(*v3,*I2);
        ExpresE2.setId((*Expression3).getNextEdgeId());
	
      }
      else {
        EnhancedPrivateLinearizedComputationalGraphEdge* E2 = new EnhancedPrivateLinearizedComputationalGraphEdge();
        Assignment* Assignment3 = new Assignment(false);
        Expression* Expression3 =(new Expression());
	if(Expression1exists && Expression2exists) {
	  Intrinsic* I= (new Intrinsic("mul_scal_scal"));
	  (*I).setId((*Expression3).getNextVertexId());
	  //set the intrinsic as a vertex
	  (*tempExpression1).copyMyselfInto((*Expression3),true,false);
	  (*tempExpression2).copyMyselfInto((*Expression3),true,false);
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit ((*Expression3).vertices());
	  Expression::VertexIterator startVertex =vit.first,endVertex =vit.second;
	  ExpressionVertex* v;
	  for (;startVertex!=endVertex;++startVertex) {
	    if((*Expression3).numOutEdgesOf((dynamic_cast <ExpressionVertex&>( *startVertex)))==0 && (&(*startVertex)) != I) {
	      v = &(dynamic_cast <ExpressionVertex&>( *startVertex));
	      break;
	    }
	  }
	  
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit2 ((*Expression3).vertices());
	  Expression::VertexIterator startVertex2=vit2.first, endVertex2=vit2.second;
	  ExpressionVertex* v2;
	  for (;startVertex2!=endVertex2;++startVertex2) {
	    if((*Expression3).numOutEdgesOf((dynamic_cast <ExpressionVertex&>( *startVertex2)))==0 && (&(*startVertex2)) != v  && (&(*startVertex2)) != I ) {
	      v2 = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
	      break;
	    }
	  }

	  (*Expression3).supplyAndAddVertexInstance(*I);
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v,*I);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE2=(*Expression3).addEdge(*v2,*I);
	  ExpresE2.setId((*Expression3).getNextEdgeId());

	  //Insert the Expression into the Edge  //ADDASSIGNMENTSTUFFHERE
	
	//	  theDelayVertex_p->getVariable().copyMyselfInto(theReplacementAssignment.getLHS());theDelayVertex_p->getVariable().copyMyselfInto(theReplacementAssignment.getLHS());
	
	// make a temporary Variable on the RHS:
	Argument* theDelayVertex_p=new Argument();
	Assignment3->setId("back_elim,ConstantFolding");
	theDelayVertex_p->setId(1);
	// add it to the RHS of the temp assignment
	//          Assignment3->getRHS().supplyAndAddVertexInstance(*theDelayVertex_p);
	// set the alias key to temporary:
	theDelayVertex_p->getVariable().getAliasMapKey().setTemporary();
	theDelayVertex_p->getVariable().getDuUdMapKey().setTemporary();
	// get the global scope
	Scope& theGlobalScope(ConceptuallyStaticInstances::instance()->
			      getCallGraph().getScopeTree().getGlobalScope());
	// create a new symbol and add a new VariableSymbolReference in the Variable
	VariableSymbolReference* theNewVariableSymbolReference_p=
	  new VariableSymbolReference(theGlobalScope.
				      getSymbolTable().
				      addUniqueAuxSymbol(SymbolKind::VARIABLE,
							 SymbolType::REAL_STYPE, 
							 SymbolShape::SCALAR,
							 false),
				      theGlobalScope);
	theNewVariableSymbolReference_p->setId("1");
	theDelayVertex_p->getVariable().
	  supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
	Expression3->copyMyselfInto(Assignment3->getRHS(),false,false);
	theDelayVertex_p->getVariable().copyMyselfInto(Assignment3->getLHS());
	  //connect the two previously maximal vertices to the intrinsic, so that it can now be the new maximal vertex.
	}
	else if (Expression1exists) {
	  tempExpression1->copyMyselfInto(Assignment3->getRHS(),false,false);
	  tempAssignment1->getLHS().copyMyselfInto(Assignment3->getLHS());
	}
	else if (Expression2exists) {
	  tempExpression2->copyMyselfInto(Assignment3->getRHS(),false,false);
	  tempAssignment2->getLHS().copyMyselfInto(Assignment3->getLHS());
	}
	(*E2).setConstantFoldedAssignment((*Assignment3));
	supplyAndAddEdgeInstance(*E2,viii,vo);
	//              new edge(viii,vo,conn1*e);
      }
    }
    removeAndDeleteEdge(e);
    cout<<"end of back elim\n";
  }
  
  void EnhancedPrivateLinearizedComputationalGraph::front_elim(EnhancedPrivateLinearizedComputationalGraphEdge& e) {
    cout<<"front_elim";
    //1.find all j such that j depend j' but i not depen j'/
    //2.do the mult
    //3.find all j such that j depend j' and i depend i'
    //4.do the math...
    bool Expression1exists=true,Expression2exists=true;
    EnhancedPrivateLinearizedComputationalGraphVertex& vo= (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>(getTargetOf(e)));
    EnhancedPrivateLinearizedComputationalGraphVertex& vi= (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>(getSourceOf(e))) ;
    EnhancedPrivateLinearizedComputationalGraphEdge *conn1;
    EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair voo (getOutEdgesOf(vo));
    EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator currentEndOutEdge=voo.first,finalEndOutEdge=voo.second;
    bool alreadyconnected;
    for(;currentEndOutEdge!=finalEndOutEdge;++currentEndOutEdge) {
      alreadyconnected=false;
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair vio (getOutEdgesOf(vi));
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator currentStartEdge=vio.first, finalStartEdge=vio.second;
      for(;currentStartEdge!=finalStartEdge;++currentStartEdge) {
        EnhancedPrivateLinearizedComputationalGraphEdge& startEdge =(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentStartEdge));
        EnhancedPrivateLinearizedComputationalGraphEdge& endEdge =(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEndOutEdge));
        if(&getTargetOf(startEdge)==&getTargetOf(endEdge)) {
          alreadyconnected=true;
          conn1 = &(dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( startEdge)); 
        }
      }   


      xaifBoosterLinearization::ExpressionEdgeAlg* tempee1;
      Assignment* tempAssignment1;
      Expression* tempExpression1;

      xaifBoosterLinearization::ExpressionEdgeAlg* tempee2;
      Assignment* tempAssignment2;
      Expression* tempExpression2;

      if(!e.isDirectCopyEdge()) {
        if (e.hasConstantFoldedAssignment()) {
          tempAssignment1=e.getConstantFoldedAssignment();
          tempExpression1=&(tempAssignment1->getRHS());
        }
        else {
          tempee1 =  &(dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(e.getLinearizedExpressionEdge().getExpressionEdgeAlgBase()));      
          //      ExpressionEdge* tempee1=e.getLinearizedExpressionEdge ();
          tempAssignment1=&((*tempee1).getConcretePartialAssignment());
          tempExpression1=&((*tempAssignment1).getRHS());
        }
      }
      else{
	tempExpression1=new Expression();
	Expression1exists=false;
      }
      EnhancedPrivateLinearizedComputationalGraphEdge& conn2 = dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&> (*currentEndOutEdge);
      if (!(conn2.isDirectCopyEdge())) {
        if (conn2.hasConstantFoldedAssignment()) {
          tempAssignment2=conn2.getConstantFoldedAssignment();
          tempExpression2=&(tempAssignment2->getRHS());
        }
        else {
          tempee2 =  &(dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>((conn2).getLinearizedExpressionEdge().getExpressionEdgeAlgBase())); 
          //    ExpressionEdge tempee2=conn1.getLinearizedExpressionEdge ();
          tempAssignment2=&((*tempee2).getConcretePartialAssignment ());
          tempExpression2=&((*tempAssignment2).getRHS());
        }
      }
      else {
	tempExpression2=new Expression();
	Expression2exists=false;
      }

      Expression* Expression3;
      Assignment* Assignment3;
      if(alreadyconnected) {
	if(conn1->isDirectCopyEdge()) {
	  cout<<"SomeThing is VERY WRONG HERE\n";
	}
        //(*conn1).hasConstantFoldedAssignment()) 
	Assignment3 = &(conn1->getAssignmentFromEdge());
	Expression3 = &(Assignment3->getRHS());
	
	cout<<Expression3->numVertices()<<": This had BETTER NOT BE ZERO!!\n";
        //find maximal vertex (Ie: vertex that has no out edges)
        Expression::VertexIteratorPair vit3((*Expression3).vertices());
        Expression::VertexIterator startVertex3=vit3.first;
        Expression::VertexIterator endVertex3=vit3.second;
        ExpressionVertex* v3;

        for (;startVertex3!=endVertex3;++startVertex3) {
          if((*Expression3).numOutEdgesOf(*startVertex3)==0) {
            v3 = &(dynamic_cast <ExpressionVertex&>( *startVertex3));
            break;
          }
        }
	Intrinsic* I1 = new Intrinsic("mul_scal_scal");
	if(Expression1exists&&Expression2exists) {
	  (*I1).setId((*Expression3).getNextVertexId());
	}
        Intrinsic* I2 = new Intrinsic("add_scal_scal");
        (*I2).setId((*Expression3).getNextVertexId());
	if(Expression1exists) 
	  (*tempExpression1).copyMyselfInto(*Expression3,true,false);
	if(Expression2exists) 
	  (*tempExpression2).copyMyselfInto(*Expression3,true,false);
	ExpressionVertex* v(0);
	if(Expression1exists) {
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit((*Expression3).vertices());
	  Expression::VertexIterator startVertex=vit.first,endVertex=vit.second;
	  for (;startVertex!=endVertex;++startVertex) {
	    if((*Expression3).numOutEdgesOf(*startVertex)==0 && (&(*startVertex)) != v3 && (&(*startVertex)) != I1 && (&(*startVertex)) != I2) {
	      v = &(dynamic_cast <ExpressionVertex&>( *startVertex));
	      break;
	    }
	  }
	}
	ExpressionVertex* v2(0);
	if(Expression2exists) {
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit2((*Expression3).vertices());
	  Expression::VertexIterator startVertex2=vit2.first;
	  Expression::VertexIterator endVertex2=vit2.second;
	  for (;startVertex3!=endVertex2;++startVertex2) {
	    if((*Expression3).numOutEdgesOf(*startVertex2)==0 && (&(*startVertex2)) != v3 && (!Expression1exists || (&(*startVertex2)) != v) && (&(*startVertex2)) != I1 && (&(*startVertex2)) != I2) {
	      v2 = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
	      break;
	    }
	  }
	} 
	(*Expression3).supplyAndAddVertexInstance(*I2);
	if(Expression1exists && Expression2exists) {
	  (*Expression3).supplyAndAddVertexInstance(*I1);
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*I1,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE3=(*Expression3).addEdge(*v,*I1);
	  ExpresE3.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE4=(*Expression3).addEdge(*v2,*I1);
	  ExpresE4.setId((*Expression3).getNextEdgeId());
	}
	else if(Expression1exists) {
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	}
	else if (Expression2exists) {
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v2,*I2);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	}
        ExpressionEdge& ExpresE2=Expression3->addEdge(*v3,*I2);
        ExpresE2.setId((*Expression3).getNextEdgeId());

        //      conn1+=voo.first*e;
      }
      else {
        EnhancedPrivateLinearizedComputationalGraphEdge* E2 = new EnhancedPrivateLinearizedComputationalGraphEdge();
	Assignment3 = new Assignment(false);
        Expression3 = (new Expression());
	if(Expression1exists&&Expression2exists) {
	  Intrinsic* I = new Intrinsic("mul_scal_scal");
	  I->setId((*Expression3).getNextVertexId());
	  //set the intrinsic as a vertex
	  Expression3->supplyAndAddVertexInstance(*I);
	  tempExpression1->copyMyselfInto(*Expression3,true,false);
	  tempExpression2->copyMyselfInto(*Expression3,true,false);
	  
	  //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit((*Expression3).vertices());
	  Expression::VertexIterator startVertex=vit.first,endVertex=vit.second;
	  ExpressionVertex* v;
	  for (;startVertex!=endVertex;++startVertex) {
	    if((*Expression3).numOutEdgesOf(*startVertex)==0 && (&(*startVertex)) != I) {
	      v = &(dynamic_cast <ExpressionVertex&>( *startVertex));
	    }
	  }
	  
        //find maximal vertex (Ie: vertex that has no out edges)
	  Expression::VertexIteratorPair vit2((*Expression3).vertices());
	  Expression::VertexIterator startVertex2=vit2.first;
	  Expression::VertexIterator endVertex2=vit2.second;
	  ExpressionVertex* v2;
	  for (;startVertex2!=endVertex2;++startVertex2) {
	    if((*Expression3).numOutEdgesOf(*startVertex2)==0  && (&(*startVertex2)) != v && (&(*startVertex2)) != I) {
	      v2 = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
	    }
	  }
	  
	  ExpressionEdge& ExpresE1=(*Expression3).addEdge(*v,*I);
	  ExpresE1.setId((*Expression3).getNextEdgeId());
	  ExpressionEdge& ExpresE2=(*Expression3).addEdge(*v2,*I);
	  ExpresE2.setId((*Expression3).getNextEdgeId());
	  //connect the two previously maximal vertices to the intrinsic, so that it can now be the new maximal vertex.
	  //Insert the Expression into the Edge  //add ASSIGNMENTSTUFF here
	  
          // make a temporary Variable on the RHS:
          Argument* theDelayVertex_p=new Argument();
          theDelayVertex_p->setId(1);
	  Assignment3->setId("front_elim,ConstantFolding");
          // add it to the RHS of the temp assignment
	  //          Assignment3->getRHS().supplyAndAddVertexInstance(*theDelayVertex_p);
          // set the alias key to temporary:
          theDelayVertex_p->getVariable().getAliasMapKey().setTemporary();
          theDelayVertex_p->getVariable().getDuUdMapKey().setTemporary();
          // get the global scope
          Scope& theGlobalScope(ConceptuallyStaticInstances::instance()->
                                getCallGraph().getScopeTree().getGlobalScope());
          // create a new symbol and add a new VariableSymbolReference in the Variable
          VariableSymbolReference* theNewVariableSymbolReference_p=
            new VariableSymbolReference(theGlobalScope.
                                        getSymbolTable().
                                        addUniqueAuxSymbol(SymbolKind::VARIABLE,
                                                           SymbolType::REAL_STYPE,
                                                           SymbolShape::SCALAR,
                                                           false),
                                        theGlobalScope);
          theNewVariableSymbolReference_p->setId("1");
          theDelayVertex_p->getVariable().
            supplyAndAddVertexInstance(*theNewVariableSymbolReference_p);
          Expression3->copyMyselfInto(Assignment3->getRHS(),false,false);
	  theDelayVertex_p->getVariable().copyMyselfInto(Assignment3->getLHS());  
	}
	else if (Expression1exists) {
	  tempExpression1->copyMyselfInto(Assignment3->getRHS(),false,false);
	  tempAssignment1->getLHS().copyMyselfInto(Assignment3->getLHS());
	}
	else if (Expression2exists) {
	  tempExpression2->copyMyselfInto(Assignment3->getRHS(),false,false);
	  tempAssignment2->getLHS().copyMyselfInto(Assignment3->getLHS());
	}

 
        E2->setConstantFoldedAssignment((*Assignment3));
        supplyAndAddEdgeInstance(*E2,vi,getTargetOf((dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEndOutEdge))));

        //      new edge(vi,getTargetOf(voo.first),voo.first*e);
      }
    }
    removeAndDeleteEdge(e);
    cout<<"end of front elim\n";
  }



  void 
EnhancedPrivateLinearizedComputationalGraph::CheckInEdgesForTriviality(EnhancedPrivateLinearizedComputationalGraphVertex& 
testingVertex) {
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIteratorPair theInEdges (getInEdgesOf(testingVertex));
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIterator currentEdge = theInEdges.first, endEdge=theInEdges.second;
    cout<<"numInEdgesOf(j)="<<numInEdgesOf(testingVertex)<<"\n";
    if (numInEdgesOf(testingVertex)==1) {
      VertexPointerList Dependants=getDependentList();
      VertexPointerList::iterator currentVertex=Dependants.begin(),endVertex=Dependants.end();
      for(;currentVertex!=endVertex;++currentVertex) 
	if(&testingVertex==(*currentVertex))
	  return;
      EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>(*currentEdge));
      cout<<"0";
      if(!(testingEdge.isDirectCopyEdge())) {
	if(testingEdge.hasConstantFoldedAssignment()) {
	  cout<<"testingEdge has a ConstantFolded Assignment\n";
	  return;
	}
	cout<<"1";
	xaifBoosterLinearization::ExpressionEdgeAlg& EE= dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
	xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = EE.getPartialDerivativeKind ();
	cout<<"2";
	if (!(PDK==PartialDerivativeKind::LINEAR_ONE||PDK==PartialDerivativeKind::LINEAR_MINUS_ONE)) {
	  return;
	}
      }
      cout<<"front_elim::trivialedges";
      front_elim(testingEdge);
      removeAndDeleteVertex(testingVertex);
    }
  }
  
  void 
  EnhancedPrivateLinearizedComputationalGraph::CheckOutEdgesForTriviality(EnhancedPrivateLinearizedComputationalGraphVertex& 
									  testingVertex) {
    cout<<"numOutEdgesOf(j)="<<numOutEdgesOf(testingVertex)<<"\n"; 
    if (numOutEdgesOf(testingVertex)==1) { 
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair theOutEdges (getOutEdgesOf(testingVertex));
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator currentEdge = theOutEdges.first, endEdge=theOutEdges.second;
      EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge =dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEdge);
      cout<<"0";
      if(!(testingEdge.isDirectCopyEdge())) {
	if(testingEdge.hasConstantFoldedAssignment()) {
          cout<<"testingEdge has a ConstantFolded Assignment\n";
          return;
        }
        cout<<"1";
        xaifBoosterLinearization::ExpressionEdgeAlg& EE=dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
        xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = EE.getPartialDerivativeKind ();
        cout<<"2";
        if (!(PDK==PartialDerivativeKind::LINEAR_ONE||PDK==PartialDerivativeKind::LINEAR_MINUS_ONE)) {
	  return;
        }
      }
      cout<<"back_elim::trivialedges";
      back_elim(testingEdge);
      VertexPointerList Dependants=getDependentList();
      VertexPointerList::iterator currentVertex=Dependants.begin(),endVertex=Dependants.end();
      for(;currentVertex!=endVertex;++currentVertex) 
	if(&testingVertex==(*currentVertex))
	  return;
      VertexPointerList Independants=getIndependentList();
      VertexPointerList::iterator currentVertex2=Independants.begin(),endVertex2=Independants.end();
      for(;currentVertex2!=endVertex2;++currentVertex2) 
	if(&testingVertex==(*currentVertex2))
	  return;
      removeAndDeleteVertex(testingVertex);
    }
  }
  
  void 
  EnhancedPrivateLinearizedComputationalGraph::CheckInEdgesForTrivialitySecondRun(EnhancedPrivateLinearizedComputationalGraphVertex& 
										  testingVertex) {
    cout<<"numInEdgesOf(j)="<<numInEdgesOf(testingVertex)<<"\n"; 
    if (numOutEdgesOf(testingVertex)==1) { 
      //      if(getDependentList().contains(testingVertex)) 
      VertexPointerList Dependants=getDependentList();
      VertexPointerList::iterator currentVertex=Dependants.begin(),endVertex=Dependants.end();
      for(;currentVertex!=endVertex;++currentVertex) 
	if(&testingVertex==(*currentVertex))
	  return;
      EnhancedPrivateLinearizedComputationalGraph::InEdgeIteratorPair theInEdges (getInEdgesOf(testingVertex));
      EnhancedPrivateLinearizedComputationalGraph::InEdgeIterator currentEdge = theInEdges.first, endEdge=theInEdges.second;
      for(;currentEdge!=endEdge;) {
	EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge =dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEdge);
	++currentEdge;
	cout<<"0";
	if(!(testingEdge.isDirectCopyEdge())) {
	  if(testingEdge.hasConstantFoldedAssignment()) {
	    cout<<"testingEdge has a ConstantFolded Assignment\n";
	  }
	  else {
	    cout<<"1";
	    xaifBoosterLinearization::ExpressionEdgeAlg& EE=dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
	    xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = EE.getPartialDerivativeKind ();
	    cout<<"2";
	    if ((PDK==PartialDerivativeKind::LINEAR_ONE||PDK==PartialDerivativeKind::LINEAR_MINUS_ONE)) {
	      cout<<"front_elim::trivialedges";
	      front_elim(testingEdge);
	    }
	  }
	}
      }
      if(numOutEdgesOf(testingVertex)==0 || numInEdgesOf(testingVertex)==0) 
      	removeAndDeleteVertex(testingVertex);
    }
  }
  
void 
EnhancedPrivateLinearizedComputationalGraph::CheckOutEdgesForTrivialitySecondRun(EnhancedPrivateLinearizedComputationalGraphVertex& 
										     testingVertex) {
    cout<<"numOutEdgesOf(j)="<<numOutEdgesOf(testingVertex)<<"\n"; 
    if (numInEdgesOf(testingVertex)==1) { 
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair theOutEdges (getOutEdgesOf(testingVertex));
      EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator currentEdge = theOutEdges.first, endEdge=theOutEdges.second;
      for(;currentEdge!=endEdge;) {
	EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge =dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEdge);
	++currentEdge;
	cout<<"0";
	if(!(testingEdge.isDirectCopyEdge())) {
	  if(testingEdge.hasConstantFoldedAssignment()) {
	    cout<<"testingEdge has a ConstantFolded Assignment\n";
	  }
	  else {
	    cout<<"1";
	    xaifBoosterLinearization::ExpressionEdgeAlg& EE=dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
	    xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = EE.getPartialDerivativeKind ();
	    cout<<"2";
	    if ((PDK==PartialDerivativeKind::LINEAR_ONE||PDK==PartialDerivativeKind::LINEAR_MINUS_ONE)) {
	      cout<<"back_elim::trivialedges";
	      back_elim(testingEdge);
	    }
	  }
	}
      }
      VertexPointerList Dependants=getDependentList();
      VertexPointerList::iterator currentVertex=Dependants.begin(),endVertex=Dependants.end();
      for(;currentVertex!=endVertex;++currentVertex) 
	if(&testingVertex==(*currentVertex))
	  return;
      if(numOutEdgesOf(testingVertex)==0 || numInEdgesOf(testingVertex)==0) 
      removeAndDeleteVertex(testingVertex);
    }
  }
  

  bool 
EnhancedPrivateLinearizedComputationalGraph::CheckInEdgesForConstants(EnhancedPrivateLinearizedComputationalGraphVertex& 
testingVertex){
    cout<<"numInEdgesOf(j)="<<numInEdgesOf(testingVertex)<<"\n";
    if (numInEdgesOf(testingVertex)) {
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIteratorPair theInEdges ( getInEdgesOf(testingVertex)); 
    EnhancedPrivateLinearizedComputationalGraph::InEdgeIterator currentEdge=theInEdges.first, endEdge=theInEdges.second;
    while( currentEdge!=endEdge) {
      EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEdge));
      if (!(testingEdge.isDirectCopyEdge())) {
        if(testingEdge.hasConstantFoldedAssignment()) {
          cout<<"testingEdge has a ConstantFolded Assignment\n";
          return false;
        }
        xaifBoosterLinearization::ExpressionEdgeAlg& currentExpressionEdge = dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
        const xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = currentExpressionEdge.getPartialDerivativeKind(); 
        if (!( PDK==PartialDerivativeKind::LINEAR_ONE || PDK==PartialDerivativeKind::LINEAR_MINUS_ONE || PDK==PartialDerivativeKind::LINEAR))
          {
            return false;
          }
      }
      ++currentEdge;
    }
    cout<<"inedges are constant";
    return true;
    }
    return false;
  }
  
  bool EnhancedPrivateLinearizedComputationalGraph::CheckOutEdgesForConstants  (EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex){
    cout<<"numOutEdgesOf(j)="<<numOutEdgesOf(testingVertex)<<"\n";
    if(numOutEdgesOf(testingVertex)) {
    EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair theOutEdges ( getOutEdgesOf(testingVertex));
    EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator currentEdge=theOutEdges.first, endEdge=theOutEdges.second;
    while(currentEdge!=endEdge) {
      EnhancedPrivateLinearizedComputationalGraphEdge& testingEdge (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *currentEdge));
      if (!(testingEdge.isDirectCopyEdge())) {
        if(testingEdge.hasConstantFoldedAssignment()) {
          cout<<"testingEdge has a ConstantFolded Assignment\n";
          return false;
        }
        xaifBoosterLinearization::ExpressionEdgeAlg& currentExpressionEdge =  dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>(testingEdge.getLinearizedExpressionEdge().getExpressionEdgeAlgBase());
        const xaifBooster::PartialDerivativeKind::PartialDerivativeKind_E PDK = currentExpressionEdge.getPartialDerivativeKind();
        if (( PDK==PartialDerivativeKind::LINEAR_ONE || PDK==PartialDerivativeKind::LINEAR_MINUS_ONE || PDK==PartialDerivativeKind::LINEAR)==false)
        {
            return false;
        }
      }
      ++currentEdge;
    }
    cout<<"outedges are constant";
    return true;
    }
    return false;
  }
  
  void EnhancedPrivateLinearizedComputationalGraph::RunTrivialCheckBackwards() {
    cout<<"trivialcheckbackwards\n";
    EnhancedPrivateLinearizedComputationalGraph::VertexIteratorPair completeVertex ( vertices( ) );
    EnhancedPrivateLinearizedComputationalGraph::VertexIterator currentVertex = completeVertex.first , endVertex = completeVertex.second;
    while(currentVertex!=endVertex) {
      EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( *currentVertex));
      ++currentVertex;
      if(numOutEdgesOf(testingVertex)&&numInEdgesOf(testingVertex))
	CheckOutEdgesForTriviality(testingVertex);
      }
  }

  void EnhancedPrivateLinearizedComputationalGraph::RunTrivialCheckForward() {
    cout<<"trivialCheckFroward\n";
    EnhancedPrivateLinearizedComputationalGraph::VertexIteratorPair completeVertex ( vertices( ) );
    EnhancedPrivateLinearizedComputationalGraph::VertexIterator currentVertex = completeVertex.first, endVertex = completeVertex.second;
    while(currentVertex!=endVertex) {
      EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( *currentVertex));
      ++currentVertex;
      if(numOutEdgesOf(testingVertex)&&numInEdgesOf(testingVertex))
	CheckInEdgesForTriviality(testingVertex);
      }
  }
    
  void EnhancedPrivateLinearizedComputationalGraph::RunTrivialCheckBackwardsAgain() {
    cout<<"trivialcheckbackwardsagain\n";
    EnhancedPrivateLinearizedComputationalGraph::VertexIteratorPair completeVertex ( vertices( ) );
    EnhancedPrivateLinearizedComputationalGraph::VertexIterator currentVertex = completeVertex.first , endVertex = completeVertex.second;
    while(currentVertex!=endVertex) {
      EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( *currentVertex));
      ++currentVertex;
      if(numOutEdgesOf(testingVertex)&&numInEdgesOf(testingVertex))
	CheckOutEdgesForTrivialitySecondRun(testingVertex);
      }
  }

  void EnhancedPrivateLinearizedComputationalGraph::RunTrivialCheckForwardAgain() {
    cout<<"trivialCheckFrowardagain\n";
    EnhancedPrivateLinearizedComputationalGraph::VertexIteratorPair completeVertex ( vertices( ) );
    EnhancedPrivateLinearizedComputationalGraph::VertexIterator currentVertex = completeVertex.first, endVertex = completeVertex.second;
    while(currentVertex!=endVertex) {
      EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( *currentVertex));
      ++currentVertex;
      if(numOutEdgesOf(testingVertex)&&numInEdgesOf(testingVertex))
	CheckInEdgesForTrivialitySecondRun(testingVertex);
      }
  }
  void EnhancedPrivateLinearizedComputationalGraph::RunConstantCheckForwardBackward() {
    /*    cout<<"constantChecking forward/backwards\n";
    EnhancedPrivateLinearizedComputationalGraph::VertexIteratorPair completeVertex ( vertices( ) );
    EnhancedPrivateLinearizedComputationalGraph::VertexIterator currentVertex = completeVertex.first, endVertex =completeVertex.second,nextVertex=completeVertex.first;
    while(currentVertex != endVertex) {
      EnhancedPrivateLinearizedComputationalGraphVertex& testingVertex (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphVertex&>( *currentVertex));
      ++currentVertex;
      cout<<"numInEdgesOf(v)="<<numInEdgesOf(testingVertex)<<"\n";
      cout<<"numOutEdgesOf(v)="<<numOutEdgesOf(testingVertex)<<"\n";
      if(numOutEdgesOf(testingVertex)&&numInEdgesOf(testingVertex))
	if (CheckOutEdgesForConstants(testingVertex) && CheckInEdgesForConstants(testingVertex)) {
	  if (numInEdgesOf(testingVertex)==1) {
	    VertexPointerList Dependants=getDependentList();
	    VertexPointerList::iterator currentVertex2=Dependants.begin(),endVertex2=Dependants.end();
	    for(;currentVertex2!=endVertex2;++currentVertex2) 
	      if(&testingVertex==(*currentVertex2))
		return;
	    EnhancedPrivateLinearizedComputationalGraph::InEdgeIteratorPair InEI ( getInEdgesOf(testingVertex));
	    EnhancedPrivateLinearizedComputationalGraph::InEdgeIterator Ief = InEI.first;
	    EnhancedPrivateLinearizedComputationalGraphEdge& eliminatedEdgeIn (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *Ief));
	    cout<<"front_elim::ConstantCheck";
	    front_elim(eliminatedEdgeIn);

	    //	    removeAndDeleteVertex(testingVertex);
	  } 
	  else if (numOutEdgesOf(*currentVertex)==1) {
	  EnhancedPrivateLinearizedComputationalGraph::OutEdgeIteratorPair OnEI ( getOutEdgesOf(testingVertex));
	    EnhancedPrivateLinearizedComputationalGraph::OutEdgeIterator Oef = OnEI.first;
	    EnhancedPrivateLinearizedComputationalGraphEdge& eliminatedEdgeOut (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>( *Oef));
	    cout<<"back_elim::ConstantCheck";
	    back_elim(eliminatedEdgeOut);
	    VertexPointerList Dependants=getDependentList();
	    VertexPointerList::iterator currentVertex2=Dependants.begin(),endVertex2=Dependants.end();
	    for(;currentVertex2!=endVertex2;++currentVertex2) 
	    if(&testingVertex==(*currentVertex2)){
	    return;}
	    VertexPointerList Independants=getIndependentList();
		VertexPointerList::iterator currentVertex3=Independants.begin(),endVertex3=Independants.end();
	    for(;currentVertex3!=endVertex3;++currentVertex3) 
	    if(&testingVertex==(*currentVertex3))

		return;
		removeAndDeleteVertex(testingVertex);
	  }
	}
    }*/
  }
  void EnhancedPrivateLinearizedComputationalGraph::RunParallelEdgeAddition() {
    EnhancedPrivateLinearizedComputationalGraph::EdgeIteratorPair allEdges = edges();
    EnhancedPrivateLinearizedComputationalGraph::EdgeIterator currentEdge=allEdges.first, endEdge=allEdges.second;
    for (;currentEdge!=endEdge;++currentEdge) {
      EnhancedPrivateLinearizedComputationalGraphEdge& thePrivateEdge = (dynamic_cast <EnhancedPrivateLinearizedComputationalGraphEdge&>(*currentEdge));
      //      EnhancedPrivateLinearizedComputationalGraphEdge::ExpressionEdgePList parallEledges = thisEdge.getParallels();
      //      Assignment* TrueEdge = thisEdge.getAssignmentFromEdge();
      //      Expression FinalExpression = thisEdge.getRHS();
      //      ListIterator startIterator=parallelEdge.begin,endIterator=parallelEdges.end;
      //      for(;startIterator!=endIterator;++startIterator) {
      //	ExpressionEdge parallelExpression= (dynamic_cat <ExpressionEdge&> (*startIterator));
      //	/*So I've got an Expression Edge and I need to get an Expression...*/
      //	        const PrivateLinearizedComputationalGraphEdge& thePrivateEdge(dynamic_cast<const PrivateLinearizedComputationalGraphEdge&>(theVertex.getExternalReference()));
      if(!(thePrivateEdge.isDirectCopyEdge())) {
	Expression& finalExpression = thePrivateEdge.getAssignmentFromEdge().getRHS();
	ExpressionVertex* v;
	//find maximal vertex (Ie: vertex that has no out edges)
	Expression::VertexIteratorPair vit2 (finalExpression.vertices());
	Expression::VertexIterator startVertex2=vit2.first, endVertex2=vit2.second;
	for (;startVertex2!=endVertex2;++startVertex2) {
	  if(finalExpression.numOutEdgesOf( *startVertex2)==0) {
	    v = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
	    break;
	  }
	} 
	
	if(thePrivateEdge.getParallels().size()) {
	  unsigned int position;
	  Intrinsic* theIntrinsic_p=0;
	  for (EnhancedPrivateLinearizedComputationalGraphEdge::ExpressionEdgePList::const_iterator i=thePrivateEdge.getParallels().begin();i!=thePrivateEdge.getParallels().end();++i) {
	    //	  Argument* theExternalArgument_p=new Argument();

	    dynamic_cast<xaifBoosterLinearization::ExpressionEdgeAlg&>((*i)->getExpressionEdgeAlgBase()).getConcretePartialAssignment().getRHS().copyMyselfInto(finalExpression,true,false);
	    ExpressionVertex* v2;
	    Expression::VertexIteratorPair vit2 (finalExpression.vertices());
	    Expression::VertexIterator startVertex2=vit2.first, endVertex2=vit2.second;
	    for (;startVertex2!=endVertex2;++startVertex2) {
	      if(finalExpression.numOutEdgesOf(*startVertex2)==0 &&  (&(*startVertex2)) != v) {
		v2 = &(dynamic_cast <ExpressionVertex&>( *startVertex2));
		break;
	      }
	    }
	    if (theIntrinsic_p) { // have one addition already
	      ExpressionEdge& theNewEdge(finalExpression.addEdge(*v2,*theIntrinsic_p));
	      // make up an ID
	      theNewEdge.setId(finalExpression.getNextEdgeId());
	      theNewEdge.setPosition(position++);
	    }
	    theIntrinsic_p=new Intrinsic("add_scal_scal");
	    position=1; // start a new addition   
	    theIntrinsic_p->setId(finalExpression.getNextVertexId());
	    finalExpression.supplyAndAddVertexInstance(*theIntrinsic_p);
	    ExpressionEdge& theNewEdge(finalExpression.addEdge(*v2,*theIntrinsic_p));
	    // make up an ID
	    theNewEdge.setId(finalExpression.getNextEdgeId());
	    theNewEdge.setPosition(position++);
	  } // end for
	}
      }
    }
  }
}

   
  //constantPropigation...
  /*Do stuff, make it work...
    I like working code, it is nice...*/
