/*! \file
  
  \brief Expression tree declarations
  
  \authors Nathan Tallent, Michelle Strout
  \version $Id: ExprTree.hpp,v 1.18 2007/10/08 19:19:12 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ExprTree_H
#define ExprTree_H
#include<set>
#include <map>

// OpenAnalysis headers
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/Utils/Tree.hpp>
//#include <OpenAnalysis/ReachConsts/Interface.hpp>
#include <OpenAnalysis/IRInterface/ConstValBasicInterface.hpp>


namespace OA {
using namespace std;

class ExprTree;

// to avoid circular reference in header files
class ExprTreeVisitor;

typedef std::set<OA_ptr<ExprTree> > ExprTreeSet;

/*!  Function that intersects sets of ExprTrees */
OA_ptr<std::set<OA_ptr<ExprTree> > >
intersectExprTreeSets(std::set<OA_ptr<ExprTree> >& set1,
                 std::set<OA_ptr<ExprTree> >& set2);

/*!  Function the unions sets of ExprTrees */
OA_ptr<std::set<OA_ptr<ExprTree> > >
    unionExprTreeSets(
        std::set<OA_ptr<ExprTree> >& set1,
        std::set<OA_ptr<ExprTree> >& set2);



//--------------------------------------------------------------------
/*! ExprTree is a Tree with enhanced nodes.  
    It represents expressions involving various IRHandles.
*/
  class ExprTree : public Tree {
public:
  class Node;
  class Edge;
private:
static const int sOrder = -200;

public:
  ExprTree();
  virtual ~ExprTree();
  
  OA_ptr<Node>  getRoot() { 
    OA_ptr<Tree::Node> n = Tree::getRoot();
    return n.convert<Node>(); 
  }


  //! visit root and then accept visitor on tree
  void acceptVisitor(ExprTreeVisitor& pVisitor);

  //-------------------------------------
  // construction
  //-------------------------------------
  void connect(OA_ptr<Node> src, OA_ptr<Node> dst) 
      { OA_ptr<Edge> e; e = new Edge(src,dst); addEdge(e); }
  void disconnect(OA_ptr<Edge> e) { removeEdge(e.convert<Tree::Edge>()); }
  void copyAndConnectSubTree(OA_ptr<Node> src, OA_ptr<ExprTree> subtree);
  
   //! an ordering for expression trees, needed for use within STL containers
  bool operator<(ExprTree &other);


   //! check if two memory references are equal at the level of
   //! accuracy provided by the MemRefExpr approximation
  bool operator==(ExprTree &other);
  virtual int getOrder() { return sOrder; }


  //--------------------------------------------------------
  class Node : public Tree::Node {
  public:
    Node()
      : Tree::Node() { }
    Node(const std::string& x)
      : Tree::Node(), mName(x) { }
    virtual ~Node() { }
    
    // A name for this node (useful for debugging)
    std::string& getName() { return mName; }

    //! return a copy of self
    virtual OA_ptr<Node> copy() = 0;

    //! isa methods, default for all is false
    virtual bool isaOpNode() { return false; }
    virtual bool isaCallNode() { return false; }
    virtual bool isaMemRefNode() { return false; }
    virtual bool isaConstSymNode() { return false; }
    virtual bool isaConstValNode() { return false; }

    //! pass visitor to nodes in postorder and then accept visitor on tree
    // NOTE: could give this a default implementation if wanted to
    // have ExprTree visitors have default implementation for nodes
    // they don't know about
    virtual void acceptVisitor(ExprTreeVisitor&) = 0;
    virtual void dump(std::ostream& os) { Tree::Node::dump(os); }
    virtual void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    virtual void output(IRHandlesIRInterface& ir) { Tree::Node::output(ir); }
   //! an ordering for expression trees, needed for use within STL containers
  virtual bool operator<(Node& other);

   //! check if two memory references are equal at the level of
   //! accuracy provided by the MemRefExpr approximation
  virtual bool operator==(Node& other);

  virtual int getOrder() { return sOrder; }


  private:
    static const int sOrder = -100;
    std::string mName;
    //std::string mAttr;
  }; // end of Node

  class OpNode : public Node {
  public:
    OpNode() : Node("OpNode") { }
    OpNode(OpHandle h) : Node("OpNode"), mHandle(h) {}
    virtual ~OpNode() { }

    //! return a copy of self
    OA_ptr<Node> copy() 
      { OA_ptr<Node> ret; ret = new OpNode(mHandle); return ret; }

    bool isaOpNode() { return true; }
    OpHandle getHandle() { return mHandle; }
    void dump(std::ostream& os) { Node::dump(os); }
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    void acceptVisitor(ExprTreeVisitor& pVisitor);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface& ir) ;
    //! an ordering for expression trees, needed for use within STL containers
   bool operator<(Node& other);

   //! check if two memory references are equal at the level of
   //! accuracy provided by the MemRefExpr approximation
   bool operator==(Node& other);
   virtual int getOrder() { return sOrder; }

  private:
    static const int sOrder = 100;
    OpHandle mHandle;

  };

  class CallNode : public Node {
  public:
    CallNode() : Node("CallNode") { }
    CallNode(CallHandle h) : Node("CallNode"), mHandle(h) {}
    virtual ~CallNode() { }

    //! return a copy of self
    OA_ptr<Node> copy() 
      { OA_ptr<Node> ret; ret = new CallNode(mHandle); return ret; }

    bool isaCallNode() { return true; }
    CallHandle getHandle() { return mHandle; }
    void dump(std::ostream& os) { Node::dump(os); }
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    void acceptVisitor(ExprTreeVisitor& pVisitor);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface& ir) ;
     //! an ordering for expression trees, needed for use within STL containers
    bool operator<(Node& other);

    //! check if two memory references are equal at the level of
    //! accuracy provided by the MemRefExpr approximation
    bool operator==(Node& other);
    virtual int getOrder() { return sOrder; }

  private:
    //Changed from ExprHandle to CallHandle by LMR. 6.8.06
     static const int sOrder = 500;
    CallHandle mHandle;
  };

  class MemRefNode : public Node {
  public:
    MemRefNode() : Node("MemRefNode") { }
    MemRefNode(MemRefHandle h) : Node("MemRefNode"), mHandle(h) {}
    virtual ~MemRefNode() { }

    //! return a copy of self
    OA_ptr<Node> copy() 
      { OA_ptr<Node> ret; ret = new MemRefNode(mHandle); return ret; }

    bool isaMemRefNode() { return true; }
    MemRefHandle getHandle() { return mHandle; }
    void dump(std::ostream& os) { Node::dump(os); }
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    void acceptVisitor(ExprTreeVisitor& pVisitor);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface& ir) ;
   //! an ordering for expression trees, needed for use within STL containers
   bool operator<(Node& other);

   //! check if two memory references are equal at the level of
   //! accuracy provided by the MemRefExpr approximation
   bool operator==(Node& other);
   virtual int getOrder() { return sOrder; }

  private:
   static const int sOrder = 200;
    MemRefHandle mHandle;
  };

  class ConstSymNode : public Node {
  public:
    ConstSymNode() : Node("ConstSymNode") { }
    ConstSymNode(ConstSymHandle h) : Node("ConstSymNode"), mHandle(h) {}
    virtual ~ConstSymNode() { }

    //! return a copy of self
    OA_ptr<Node> copy() 
      { OA_ptr<Node> ret; ret = new ConstSymNode(mHandle); return ret; }

    bool isaConstSymNode() { return true; }
    ConstSymHandle getHandle() { return mHandle; }
    void dump(std::ostream& os) { Node::dump(os); }
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    void acceptVisitor(ExprTreeVisitor& pVisitor);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface& ir) ;
     //! an ordering for expression trees, needed for use within STL containers
    bool operator<(Node& other);

    //! check if two memory references are equal at the level of
    //! accuracy provided by the MemRefExpr approximation
    bool operator==(Node& other);
    virtual int getOrder() { return sOrder; }

  private:
 static const int sOrder = 300;
    ConstSymHandle mHandle;
  };

  class ConstValNode : public Node {
  public:
    ConstValNode() : Node("ConstValNode") { }
    ConstValNode(ConstValHandle h) : Node("ConstValNode"), mHandle(h) {}
    virtual ~ConstValNode() { }

    //! return a copy of self
    OA_ptr<Node> copy() 
      { OA_ptr<Node> ret; ret = new ConstValNode(mHandle); return ret; }

    bool isaConstValNode() { return true; }
    ConstValHandle getHandle() { return mHandle; }
    void dump(std::ostream& os) { Node::dump(os); }
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

    void acceptVisitor(ExprTreeVisitor& pVisitor);

    //*****************************************************************
    // Annotation Interface
    //*****************************************************************
    void output(IRHandlesIRInterface& ir) ;
     //! an ordering for expression trees, needed for use within STL containers
   bool operator<(Node& other);

   //! check if two memory references are equal at the level of
   //! accuracy provided by the MemRefExpr approximation
   bool operator==(Node& other);
   virtual int getOrder() { return sOrder; }

  private:
 static const int sOrder = 400;
    ConstValHandle mHandle;
  };
  
  //--------------------------------------------------------
  class Edge : public Tree::Edge {
  public:
    Edge(OA_ptr<Node> n1, OA_ptr<Node> n2)
      : Tree::Edge(n1, n2) { }
    virtual ~Edge() { }
    
    // included here to return subclass node type
    OA_ptr<Node>  parent () const { 
      OA_ptr<Tree::Node> n = Tree::Edge::parent();
      return n.convert<Node>();
    }
    OA_ptr<Node>  source () const { return parent(); }
    OA_ptr<Node>  tail () const { return parent(); }
    OA_ptr<Node>  child () const { 
      OA_ptr<Tree::Node> n = Tree::Edge::child();
      return n.convert<Node>(); 
    }
    OA_ptr<Node>  sink () const { return child(); }
    OA_ptr<Node>  head () const { return child(); }

    void dump(std::ostream& os);
    void dump() { dump(std::cout); }

  private:
  };  
  //--------------------------------------------------------
  
  //------------------------------------------------------------------
  /*! The node iterator iterates over all the nodes in the tree in no 
      particular order.   
      Need one in this class as well as
      super class so that return value of current method is subclass
      specific.
  */
  class NodesIterator : public Tree::NodesIterator {
  public:
    NodesIterator (Tree& t) : Tree::NodesIterator(t) {}
    virtual ~NodesIterator () {}
    OA_ptr<Node>  current() const { 
      OA_ptr<Tree::Node> n = Tree::NodesIterator::current();
      return n.convert<Node>(); 
    }
  };
  
  //------------------------------------------------------------------
  /*! The edge iterator iterates over all the edges in the tree in no 
      particular order. 
      Need one in this class as well as
      super class so that return value of current method is subclass
      specific.
  */
  class EdgesIterator : public Tree::EdgesIterator {
  public:
    EdgesIterator (Tree& t) : Tree::EdgesIterator(t) {}
    virtual ~EdgesIterator () {}
    OA_ptr<Edge>  current() const { 
      OA_ptr<Tree::Edge> e = Tree::EdgesIterator::current();
      return e.convert<Edge>(); 
    }
  };

  //------------------------------------------------------------------
  /*! OutEdgesIterator iterates over all the non-null outgoing nodes 
      of a node. 
  */
  class OutEdgesIterator : public Tree::OutEdgesIterator {
  public:
    OutEdgesIterator (Node&  n) : Tree::OutEdgesIterator(n) {}
    virtual ~OutEdgesIterator () {}
    OA_ptr<Edge>  current() const { 
      OA_ptr<Tree::Edge> e = Tree::OutEdgesIterator::current();
      return e.convert<Edge>(); 
    }
  };
  
  //------------------------------------------------------------------
  /*! ChildNodesIterator iterates over all the children nodes of a node.  
   *  It skips the null edges.  Included here so that datatype information
   *  is not lost
   */
  class ChildNodesIterator : public Tree::ChildNodesIterator {
  public:
    ChildNodesIterator (Node&  n) : Tree::ChildNodesIterator(n) {}
    ~ChildNodesIterator () {}
    OA_ptr<Node>  current() const { 
      OA_ptr<Tree::Node> n = Tree::ChildNodesIterator::current();
      return n.convert<Node>();
    }
  };
 
  void str(std::ostream& os);
  
  void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);
  //void dump() { dump(std::cout); }

  //*****************************************************************
  // Annotation Interface
  //*****************************************************************
  void output(IRHandlesIRInterface& ir) ;

  //------------------------------------------------------------------
  /*! Pre-order iterator enumerates the nodes in pre-order (a node is 
      visited before all its sub-trees). 
  */
  class PreOrderIterator : public Tree::PreOrderIterator {
  public:
    PreOrderIterator (Tree& t) : Tree::PreOrderIterator(t) {}
    virtual ~PreOrderIterator () {}
    OA_ptr<Node>  current() const { 
      OA_ptr<Tree::Node> n = PreOrderIterator::current();
      return n.convert<Node>();
    }
  };
  //------------------------------------------------------------------
  /*! Post-order iterator enumerates the nodes in post-order 
      (a node's sub-trees are visited before the node). 
  */
  class PostOrderIterator : public Tree::PostOrderIterator {
  public:
    PostOrderIterator (Tree& t) : Tree::PostOrderIterator(t) {}
    virtual ~PostOrderIterator () {}
    OA_ptr<Node>  current() const { 
      OA_ptr<Tree::Node> n = PostOrderIterator::current();
      return n.convert<Node>();
    }
  };
  //------------------------------------------------------------------
  /*! Reverse post-order iterator, as the name suggests, enumerates 
      the nodes in the order that is reverse of post-order. 
  */
  class ReversePostOrderIterator : public Tree::ReversePostOrderIterator {
  public:
    ReversePostOrderIterator (Tree& t) : Tree::ReversePostOrderIterator(t) {}
    virtual ~ReversePostOrderIterator () {}
    OA_ptr<Node>  current() const { 
      OA_ptr<Tree::Node> n = Tree::ReversePostOrderIterator::current();
      return n.convert<Node>(); 
    }
  };
 //--------------------------------------------------------------------

};


} // end of OA namespace

#endif
