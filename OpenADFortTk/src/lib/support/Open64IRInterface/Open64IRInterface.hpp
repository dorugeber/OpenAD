// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef Open64IRInterface_H
#define Open64IRInterface_H

#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <cassert>

//************************ OpenAnalysis Include Files ***********************

#include <OpenAnalysis/IRInterface/CallGraphIRInterface.hpp>
#include <OpenAnalysis/IRInterface/CFGIRInterfaceDefault.hpp>
#include <OpenAnalysis/IRInterface/AliasIRInterfaceDefault.hpp>
#include <OpenAnalysis/IRInterface/ReachDefsIRInterface.hpp>
#include <OpenAnalysis/IRInterface/UDDUChainsIRInterface.hpp>
#include <OpenAnalysis/IRInterface/ConstValBasicInterface.hpp>
#include <OpenAnalysis/IRInterface/ConstValIntInterface.hpp>
#include <OpenAnalysis/IRInterface/ReachConstsIRInterface.hpp>
#include <OpenAnalysis/IRInterface/XAIFIRInterface.hpp>
#include <OpenAnalysis/IRInterface/SideEffectIRInterface.hpp>
#include <OpenAnalysis/IRInterface/CallGraphDFProblemIRInterface.hpp>
#include <OpenAnalysis/IRInterface/InterSideEffectIRInterfaceDefault.hpp>
#include <OpenAnalysis/IRInterface/ActivityIRInterface.hpp>
#include <OpenAnalysis/IRInterface/ParamBindingsIRInterface.hpp>
#include <OpenAnalysis/IRInterface/ICFGIRInterface.hpp>
#include <OpenAnalysis/IRInterface/LinearityIRInterface.hpp>


/*! Context-Sensitive and Flow-inSensitive Activity Analysis */
#include <OpenAnalysis/IRInterface/DUGIRInterface.hpp>

#include <OpenAnalysis/ExprTree/ExprTreeVisitor.hpp>
#include <OpenAnalysis/ExprTree/EvalToMemRefVisitor.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExpr.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExprVisitor.hpp>

// still needed for MemRefKludge
//#include <OpenAnalysis/MemRefExpr/MemRefExprBasic.hpp>

#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include "IntrinsicInfo.h"

// IRInterface types: Use OA_IRHANDLETYPE_UL
//   ProcHandle     - PU_Info*
//   StmtHandle     - WN*
//   ExprHandle     - WN*
//   OpHandle       - WN*
//   MemRefHandle   - WN*
//   LeafHandle     - WN*
//   StmtLabel      - INT32
//   SymHandle      - ST*
//   ConstSymHandle - ST*
//   ConstValHandle - WN*

#include "Open64BasicTypes.h"

#include "WhirlGlobalStateUtils.h"
#include "IFDiagnostics.h"

//typedef std::pair<char*,char*> fully_qualified_name;
struct fully_qualified_name {
    std::string mVar;
    std::string mContext;

    fully_qualified_name() 
        : mVar("garbage", "garbage") {}
    fully_qualified_name(const char* var, const char *context) 
        : mVar(var), mContext(context) {}

    bool operator< (const  fully_qualified_name& other) const
    {
        if (mVar < other.mVar
            || (mVar == other.mVar && mContext < other.mContext) )
        { return true; }
        else {return false; }
    }

    bool operator== (const  fully_qualified_name& other) const
    {
        if (mVar == other.mVar && mContext == other.mContext) 
        { return true; }
        else {return false; }
    }

};


//***************************************************************************
// Iterators, roughly organized by heirarchy (procedure, statement, etc.)
//***************************************************************************

//! Iterates in PU_Info* pu_forest in DFS order
class Open64IRProcIterator : public virtual OA::IRProcIterator {
public:
  Open64IRProcIterator(PU_Info* pu_forest);
  virtual ~Open64IRProcIterator();
  
  virtual OA::ProcHandle current() const 
    { return (OA::irhandle_t)(*pulist_iter); }
  virtual bool isValid () const { return (pulist.end() != pulist_iter); }
  virtual void operator++();
  void operator++(int) { ++*this; }  // postfix
  
  virtual void reset();

private:
  void prepare_current_pu();

  std::list<PU_Info*> pulist; // list of PUs (functions)
  std::list<PU_Info*>::iterator pulist_iter;
  void build_pu_list(PU_Info* pu);
};


//! 
class Open64IRRegionStmtIterator: public OA::IRRegionStmtIterator {
public:
  Open64IRRegionStmtIterator(WN* wn) : start_wn(wn), curr_wn(NULL) { reset(); }
  virtual ~Open64IRRegionStmtIterator() { }

  virtual OA::StmtHandle current () const { return (OA::irhandle_t)curr_wn; }
  virtual bool isValid () const { return (curr_wn != 0); }
  virtual void operator++ () 
  { curr_wn = WN_next(curr_wn) ? WN_next(curr_wn) : 0; }

  virtual void reset() { curr_wn = start_wn; }

private:
  WN* start_wn;
  WN*  curr_wn;
};


//! Enumerate all the statements in a program 
// This iterator DOES step into compound statements.
// 
// Note: control-flow statements are returned to represent any
// condition *expression* that may be embedded within;
// initialization/update statements will be part of the iteration as
// will any statements that occur within blocks of the control-flow
// statement.
class Open64IRStmtIterator : public OA::IRStmtIterator {
public:
  Open64IRStmtIterator(OA::ProcHandle h);
  Open64IRStmtIterator() { mValid = false; }
  virtual ~Open64IRStmtIterator();

  virtual OA::StmtHandle current() const ;
  virtual bool isValid() const { return (mValid && (mStmtIter != mEnd)); }
  virtual void operator++();

  virtual void reset();

private:
  void create(OA::ProcHandle h);

private:
  std::list<OA::StmtHandle> mStmtList;

  std::list<OA::StmtHandle>::iterator mEnd;
  std::list<OA::StmtHandle>::iterator mBegin;
  std::list<OA::StmtHandle>::iterator mStmtIter;
  bool mValid;
};

class Open64IRPtrAsgnIterator : public OA::IRStmtIterator {
public:
  Open64IRPtrAsgnIterator(OA::ProcHandle h);
  Open64IRPtrAsgnIterator() { mValid = false; }
  virtual ~Open64IRPtrAsgnIterator();

  virtual OA::StmtHandle current() const ;
  virtual bool isValid() const { return (mValid && (mStmtIter != mEnd)); }
  virtual void operator++();

  virtual void reset();

private:
  void create(OA::ProcHandle h);

private:
  std::list<OA::StmtHandle> mStmtList;

  std::list<OA::StmtHandle>::iterator mEnd;
  std::list<OA::StmtHandle>::iterator mBegin;
  std::list<OA::StmtHandle>::iterator mStmtIter;
  bool mValid;
};


//! Enumerate all the ExprHandle in a stmt
//! Used by a helper method
class Open64IRExprHandleIterator : public OA::ExprHandleIterator {
public:
  Open64IRExprHandleIterator(OA::StmtHandle h);
  Open64IRExprHandleIterator() { mValid = false; }
  virtual ~Open64IRExprHandleIterator() { };

  virtual OA::ExprHandle current() const;
  virtual bool isValid() const
    { return (mValid && (mExprIter!=mEnd)); }
  virtual void operator++();
  virtual void reset();
private:
  void create(OA::StmtHandle h);
private:
  std::list<OA::ExprHandle> mExprList;
  std::list<OA::ExprHandle>::iterator mEnd;
  std::list<OA::ExprHandle>::iterator mBegin;
  std::list<OA::ExprHandle>::iterator mExprIter;
  bool mValid;

};


//! 
class Open64IRCallsiteIterator : public OA::IRCallsiteIterator {
public:
  Open64IRCallsiteIterator(WN *wn);
  virtual ~Open64IRCallsiteIterator();
  
  virtual OA::CallHandle current() const  
    { return (OA::irhandle_t)(*wnlist_iter); }
  virtual bool isValid () const { return (wnlist.end() != wnlist_iter); }
  virtual void operator++() { ++wnlist_iter; }

  virtual void reset() { wnlist_iter = wnlist.begin(); }
  
private:
  std::list<WN*> wnlist; // a list of function call nodes
  std::list<WN*>::iterator wnlist_iter;
  void build_func_call_list(WN* wn);
};



// Enumerate all (actual) parameters within a callsite
// The iterator should contain the parameters in the same order in which
// they appear within the call itself
class Open64IRCallsiteParamIterator : public OA::IRCallsiteParamIterator {
public:
  Open64IRCallsiteParamIterator(WN* wn);
  virtual ~Open64IRCallsiteParamIterator() { }

  virtual OA::ExprHandle current() const {   // Returns the current item.
    return (OA::irhandle_t)(*wnlist_iter); 
  }

  virtual bool isValid () const {        // False when all items are exhausted. 
    return (wnlist.end() != wnlist_iter); 
  }

  virtual void operator++() { ++wnlist_iter; }
  void operator++(int) { ++*this; } // what is this for??? -BK

  virtual void reset() {wnlist_iter = wnlist.begin(); }

private:
  std::list<WN* > wnlist; // a list of function call nodes
  std::list<WN* >::iterator wnlist_iter;

};

//! Enumerate all the memory references in a stmt
//! Used by a helper method
class Open64IRMemRefIterator : public OA::MemRefHandleIterator {
public:
  Open64IRMemRefIterator(OA::StmtHandle h);
  Open64IRMemRefIterator() { mValid = false; }
  virtual ~Open64IRMemRefIterator() { };

  virtual OA::MemRefHandle current() const; 
  virtual bool isValid() const 
    { return (mValid && (mMemRefIter!=mEnd)); }        
  virtual void operator++();

  virtual void reset();

private:
  void create(OA::StmtHandle h);
private:
  std::list<OA::MemRefHandle> mMemRefList;
  
  std::list<OA::MemRefHandle>::iterator mEnd;
  std::list<OA::MemRefHandle>::iterator mBegin;
  std::list<OA::MemRefHandle>::iterator mMemRefIter;
  bool mValid;
  
};


//! 
class Open64IRSymIterator : public OA::IRSymIterator {
public:
  Open64IRSymIterator(PU_Info* pu);
  virtual ~Open64IRSymIterator() { }
  
  virtual OA::SymHandle current() const
    { return (OA::irhandle_t)(*symlist_iter); }
  virtual bool isValid () const { return (symlist.end() != symlist_iter); }
  virtual void operator++() { if (symlist_iter != symlist.end()) ++symlist_iter; }
  
  virtual void reset() { symlist_iter = symlist.begin(); }

private:
  void create(PU_Info* pu);
  
private:
  std::list<ST* > symlist; // a list of symbols
  std::list<ST* >::iterator symlist_iter;
};

/*
//! Not implemented yet
class Open64PtrAssignPairStmtIterator 
    : public OA::Alias::PtrAssignPairStmtIterator 
{
  public:
    Open64PtrAssignPairStmtIterator() {}
    ~Open64PtrAssignPairStmtIterator() {}

    //! right hand side
    OA::OA_ptr<OA::MemRefExpr> currentSource() const
      { OA::OA_ptr<OA::MemRefExpr> retval;  return retval; }
    //! left hand side
    OA::OA_ptr<OA::MemRefExpr> currentTarget() const
      { OA::OA_ptr<OA::MemRefExpr> retval;  return retval; }

    bool isValid() const  { return false; }
                    
    void operator++() {}
};
*/


class Open64PtrAssignPairStmtIterator
    : public OA::Alias::PtrAssignPairStmtIterator
{
 public:
  Open64PtrAssignPairStmtIterator() : mValid(false) { }
  Open64PtrAssignPairStmtIterator(OA::StmtHandle stmt)
  { create(stmt); reset(); mValid = true; }
  virtual ~Open64PtrAssignPairStmtIterator() { };

  //! left hand side
  virtual OA::OA_ptr<OA::MemRefExpr> currentTarget() const { return (*mIter).first; }
  //! right hand side
  virtual OA::OA_ptr<OA::MemRefExpr> currentSource() const { return (*mIter).second; }

  virtual bool isValid() const {
    return ( mValid && ( mIter != mEnd ) );
  }

  virtual void operator++() { if (isValid()) mIter++; }
  virtual void reset();

 private:
  void create(OA::StmtHandle h);

  //! FIXME
  //! Delayed Implemention
  //void createPtrAssignPairsFromReturnStmt(SgReturnStmt *returnStmt);

  //! FIXME
  //! Delayed Implementation
  //SgExpression *createPtrAssignPairsFromAssignment(SgNode *assign);

  std::list<std::pair<OA::OA_ptr<OA::MemRefExpr>, OA::OA_ptr<OA::MemRefExpr> > > mMemRefList;

  std::list<std::pair<OA::OA_ptr<OA::MemRefExpr>, OA::OA_ptr<OA::MemRefExpr> > >::iterator mEnd;
  std::list<std::pair<OA::OA_ptr<OA::MemRefExpr>, OA::OA_ptr<OA::MemRefExpr> > >::iterator mBegin;
  std::list<std::pair<OA::OA_ptr<OA::MemRefExpr>, OA::OA_ptr<OA::MemRefExpr> > >::iterator mIter;
  bool mValid;
};


class Open64ParamBindPtrAssignIterator 
    : public OA::Alias::ParamBindPtrAssignIterator {
  public:
    Open64ParamBindPtrAssignIterator() { mIter = mPairList.begin(); }
    ~Open64ParamBindPtrAssignIterator() {}

    //! right hand side
    OA::OA_ptr<OA::MemRefExpr> currentActual() const
      { OA::OA_ptr<OA::MemRefExpr> retval;  
        if (isValid()) { retval = mIter->second; }
        return retval; 
      }
    //! left hand side
    int currentFormalId() const { 
        if (isValid()) { return mIter->first; }
        else { return -1; }
    }

    bool isValid() const { return (mIter!=mPairList.end()); }

    void operator++() { if (isValid()) { mIter++; } }

    void reset() { mIter = mPairList.begin(); }

    // construction methods
    void insertParamBindPair(int formalID ,OA::OA_ptr<OA::MemRefExpr> actual)
    {
        mPairList.push_back(
            std::pair<int,OA::OA_ptr<OA::MemRefExpr> >(formalID, actual) );
        reset();
    }
  private:
    std::list<std::pair<int,OA::OA_ptr<OA::MemRefExpr> > > mPairList;
    std::list<std::pair<int,OA::OA_ptr<OA::MemRefExpr> > >::iterator mIter;
};



typedef std::pair<OA::MemRefHandle,OA::ExprHandle> AssignPair;
typedef std::list<AssignPair> AssignPairList;
class Open64AssignPairIterator 
    : public OA::AssignPairIterator {
  public:
    Open64AssignPairIterator(OA::OA_ptr<AssignPairList> pAssignList) 
        : mAssignList(pAssignList) { reset(); }
    virtual ~Open64AssignPairIterator() {}

    //! left hand side
    OA::MemRefHandle currentTarget() const 
      { if (isValid()) { return mIter->first; } 
        else { return OA::MemRefHandle(0); } }
    //! right hand side
    OA::ExprHandle currentSource() const 
      { if (isValid()) { return mIter->second; } 
        else { return OA::ExprHandle(0); } }

    bool isValid() const { return mIter!=mAssignList->end(); }
                    
    void operator++() { if (isValid()) mIter++; }
    void operator++(int) { ++*this; }

    void reset() { mIter = mAssignList->begin(); }
  private:
    OA::OA_ptr<AssignPairList> mAssignList;
    AssignPairList::iterator mIter;
};

class Open64MemRefHandleIterator 
    : public OA::IRHandleListIterator<OA::MemRefHandle>,
      public virtual OA::MemRefHandleIterator
{
  public:
    Open64MemRefHandleIterator (OA::OA_ptr<std::list<OA::MemRefHandle> > pList) 
        : OA::IRHandleListIterator<OA::MemRefHandle>(pList) {} 
    ~Open64MemRefHandleIterator () {}

    void operator ++ () 
        { OA::IRHandleListIterator<OA::MemRefHandle>::operator++(); }
                        
    //! is the iterator at the end
    bool isValid()  const
        { return OA::IRHandleListIterator<OA::MemRefHandle>::isValid(); }
                
    //! return current node
    OA::MemRefHandle current()  const
        { return OA::IRHandleListIterator<OA::MemRefHandle>::current(); }

    void reset()
        { return OA::IRHandleListIterator<OA::MemRefHandle>::reset(); }
};

class Open64MemRefExprIterator : public OA::MemRefExprIterator {
  public:
    Open64MemRefExprIterator(OA::OA_ptr<std::list<OA::OA_ptr<OA::MemRefExpr> > > pList)
        : mList(pList) { mIter = mList->begin(); }
    ~Open64MemRefExprIterator() {}

    OA::OA_ptr<OA::MemRefExpr> current() const 
      { return *mIter; } 

    bool isValid() const { return mIter!=mList->end(); }
                    
    void operator++() { if (isValid()) mIter++; }
    void operator++(int) { ++*this; }
    void reset() { mIter = mList->begin(); }
  private:
    OA::OA_ptr<std::list<OA::OA_ptr<OA::MemRefExpr> > > mList;
    std::list<OA::OA_ptr<OA::MemRefExpr> >::iterator mIter;
};


//***************************************************************************
// 
//***************************************************************************

class Open64ConstVal : public virtual OA::ConstValBasicInterface {
  public:
    Open64ConstVal() {}
    virtual ~Open64ConstVal() {}
     
    // Methods needed by OA, default behavior
    virtual bool operator==(OA::ConstValBasicInterface& x) { return false; }
    //virtual bool operator!=(OA::ConstValBasicInterface& x) { return false; }
    virtual bool operator!=(OA::ConstValBasicInterface& x) { return true; }
    virtual std::string toString() { return ""; }

    // Methods used by source IR, default behavior
    virtual bool isaInteger() const { return false; }
    virtual int getIntegerVal() const { return 0; } // FIXME: THROW EXCEPTION?
    virtual bool isaDouble() const { return false; } 
    virtual double getDoubleVal() const { return 0.0; } // FIXME: EXCEPTION?
    virtual bool isaChar() const { return false; }
    virtual char getCharVal() const { return '0'; } // FIXME: THROW EXCEPTION?
    // bool isaComplex() { return false; }
    // ...

    // eval: Given an operator and two operands (one being the current
    // object), return a new object representing the result.
    virtual OA::OA_ptr<ConstValBasicInterface> 
    eval(OPERATOR opr, const OA::OA_ptr<OA::ConstValBasicInterface> op2) const;
};  

class Open64IntegerConstVal 
  : public Open64ConstVal, 
    public virtual OA::ConstValIntInterface {
  public:
    Open64IntegerConstVal() {}
    Open64IntegerConstVal(int aVal) : Open64ConstVal(), mVal(aVal) {}
    ~Open64IntegerConstVal() {}

    // Methods used by OpenAnalysis
    bool operator<(OA::ConstValBasicInterface& other)
        { Open64ConstVal& otherRecast = dynamic_cast<Open64ConstVal&>(other);
          if (otherRecast.isaInteger()) {
              return (otherRecast.getIntegerVal() < mVal);
          }
          return false;
    }
    
    // Methods used by OpenAnalysis
    bool operator==(OA::ConstValBasicInterface& other) 
        { Open64ConstVal& otherRecast = dynamic_cast<Open64ConstVal&>(other);
          if (otherRecast.isaInteger()) {
              return (otherRecast.getIntegerVal() == mVal); 
          }
          return false;
        }
    bool operator!=(OA::ConstValBasicInterface& other)
        { Open64ConstVal& otherRecast = dynamic_cast<Open64ConstVal&>(other);
          if (otherRecast.isaInteger()) {
              return (otherRecast.getIntegerVal() != mVal); 
          }
          return true;
        }

    std::string toString() 
        { std::ostringstream oss; oss << mVal; return oss.str(); }

  
    // Methods used by source IR specific to this data type
    bool isaInteger() const { return true; }
    int getIntegerVal() const { return mVal; }

    // eval: Given an operator and two operands (one being the current
    // object), return a new object representing the result.
    virtual OA::OA_ptr<ConstValBasicInterface> 
    eval(OPERATOR opr, const OA::OA_ptr<OA::ConstValBasicInterface> op2) const;
  
    
  private:
    int mVal;
}; 


//***************************************************************************
// Abstract Interfaces
//***************************************************************************

class Open64IRInterface 
  : public virtual OA::IRHandlesIRInterface,
    public virtual OA::CallGraph::CallGraphIRInterface,
    public OA::CFG::CFGIRInterfaceDefault,
    public OA::Alias::AliasIRInterfaceDefault,
    public virtual OA::ReachDefs::ReachDefsIRInterface,
    public virtual OA::UDDUChains::UDDUChainsIRInterface,
    public virtual OA::ReachConsts::ReachConstsIRInterface,
    public virtual OA::XAIF::XAIFIRInterface,
    public virtual OA::SideEffect::SideEffectIRInterface,
    //public virtual OA::DataFlow::CallGraphDFProblemIRInterface,
    public OA::SideEffect::InterSideEffectIRInterfaceDefault,
    public virtual OA::DataFlow::ParamBindingsIRInterface,
    public virtual OA::ICFG::ICFGIRInterface,
    public virtual OA::Activity::ActivityIRInterface,
    public virtual OA::Linearity::LinearityIRInterface,
    public virtual OA::DUG::DUGIRInterface
{
public:
  Open64IRInterface();
  virtual ~Open64IRInterface();

  
  //-------------------------------------------------------------------------
  // IRHandlesIRInterface
  //-------------------------------------------------------------------------

  // create a string for the given handle, should be succinct
  // and there should be no newlines
  std::string toString(const OA::ProcHandle h);
  std::string toString(const OA::StmtHandle h);
  std::string toString(const OA::ExprHandle h);
  std::string toString(const OA::OpHandle h);
  std::string toString(const OA::MemRefHandle h);
  std::string toString(const OA::SymHandle h);
  std::string toString(const OA::ConstSymHandle h);
  std::string toString(const OA::ConstValHandle h);
  std::string toString(const OA::CallHandle h);

  // Given a statement, pretty-print it to the output stream os.
  void dump(OA::StmtHandle stmt, std::ostream& os);
  
  // Given a memory reference, pretty-print it to the output stream os.
  void dump(OA::MemRefHandle h, std::ostream& os);

  // Given a sym handle, pretty-print it to the output stream os.
  void dump(OA::SymHandle h, std::ostream& os);

  //-------------------------------------------------------------------------
  // CallGraphIRInterface
  //-------------------------------------------------------------------------

  //! Given a subprogram return an IRStmtIterator for the entire
  //! subprogram
  OA::OA_ptr<OA::IRStmtIterator> getStmtIterator(OA::ProcHandle h);

  OA::OA_ptr<OA::IRStmtIterator> getPtrAsgnIterator(OA::ProcHandle h);

  //! Return an iterator over all of the callsites in a given stmt
  OA::OA_ptr<OA::IRCallsiteIterator> getCallsites(OA::StmtHandle h);
  
  OA::SymHandle getProcSymHandle(OA::ProcHandle h);
  
  /*
  OA::SymHandle getSymHandle(OA::ExprHandle h) {
    WN* wn = (WN*)h.hval(); 
    ST* st = NULL;
    if (wn) {
      st = ((OPERATOR_has_sym(WN_operator(wn))) ? WN_st(wn) : NULL);
    }
    return (OA::irhandle_t)st;
  }
  */

 


  //-------------------------------------------------------------------------
  // CallGraphDFProblemIRInterface
  //-------------------------------------------------------------------------
  // !Get IRCallsiteParamIterator for a callsite.
  // !Don't assume parameters are visited in any particular order
  OA::OA_ptr<OA::IRCallsiteParamIterator> getCallsiteParams(OA::CallHandle h);

  //-------------------------------------------------------------------------
  // CFGIRInterface
  //-------------------------------------------------------------------------
  
  //! Given a ProcHandle, return an IRRegionStmtIterator* for the
  //! procedure. The user must free the iterator's memory via delete.
  OA::OA_ptr<OA::IRRegionStmtIterator> procBody(OA::ProcHandle h);

  // Statements: General
  //--------------------------------------------------------

  //! Are return statements allowed
  bool returnStatementsAllowed() { return true; }

  //! Given a statement, return its CFG::IRStmtType
  OA::CFG::IRStmtType getCFGStmtType(OA::StmtHandle h);

  OA::StmtLabel getLabel(OA::StmtHandle h);

  OA::OA_ptr<OA::IRRegionStmtIterator> getFirstInCompound(OA::StmtHandle h);

  // Loops
  //--------------------------------------------------------
  OA::OA_ptr<OA::IRRegionStmtIterator> loopBody(OA::StmtHandle h);
  OA::StmtHandle loopHeader(OA::StmtHandle h);
  OA::StmtHandle getLoopIncrement(OA::StmtHandle h);
  bool loopIterationsDefinedAtEntry(OA::StmtHandle h);
  OA::ExprHandle getLoopCondition(OA::StmtHandle h); //resurrected, used in
                                                     // MPICFGIRInterface

  // Structured two-way conditionals
  //--------------------------------------------------------
  OA::OA_ptr<OA::IRRegionStmtIterator> trueBody (OA::StmtHandle h);
  OA::OA_ptr<OA::IRRegionStmtIterator> elseBody (OA::StmtHandle h);
  OA::ExprHandle getCondition(OA::StmtHandle h); // resurrected, used in
                                                 // MPICFGIRInterface
  
  // Structured multiway conditionals
  //--------------------------------------------------------
  int numMultiCases(OA::StmtHandle h);
  OA::OA_ptr<OA::IRRegionStmtIterator> multiBody(OA::StmtHandle h, int bodyIndex);
  bool isBreakImplied(OA::StmtHandle multicond);
  bool isCatchAll(OA::StmtHandle h, int bodyIndex);
  OA::OA_ptr<OA::IRRegionStmtIterator> getMultiCatchall(OA::StmtHandle h);
  OA::ExprHandle getSMultiCondition (OA::StmtHandle h, int bodyIndex);
  OA::ExprHandle getSMultiTest(OA::StmtHandle h);  // used in MPICFGIRInterface

  // Unstructured two-way conditionals
  //--------------------------------------------------------
  OA::StmtLabel getTargetLabel(OA::StmtHandle h, int n);
  
  // Unstructured multi-way conditionals
  //--------------------------------------------------------
  int numUMultiTargets(OA::StmtHandle h);
  OA::StmtLabel getUMultiTargetLabel(OA::StmtHandle h, int targetIndex);
  OA::StmtLabel getUMultiCatchallLabel(OA::StmtHandle h);
  OA::ExprHandle getUMultiCondition(OA::StmtHandle h, int targetIndex);
  OA::ExprHandle getUMultiTest(OA::StmtHandle h);  // used in MPICFGInterface

  //-------------------------------------------------------------------------
  // AliasIRInterface
  //-------------------------------------------------------------------------

  class FindUseMREVisitor : public OA::MemRefExprVisitor {
      public:

         FindUseMREVisitor();
         ~FindUseMREVisitor();
         OA::OA_ptr<std::list<OA::OA_ptr<OA::MemRefExpr> > > getAllUseMREs();
         void visitNamedRef(OA::NamedRef& ref);
         void visitUnnamedRef(OA::UnnamedRef& ref);
         void visitUnknownRef(OA::UnknownRef& ref);
         void visitDeref(OA::Deref& ref);
         void visitAddressOf(OA::AddressOf& ref);
         void visitSubSetRef(OA::SubSetRef& ref);

      private:
         bool do_not_add_mre;
         OA::OA_ptr<std::list<OA::OA_ptr<OA::MemRefExpr> > > retList;
  };

  //! Get an Iterator over Use MREs
  OA::OA_ptr<OA::MemRefExprIterator> getUseMREs(OA::StmtHandle stmt);

  //! Get an Iterator over Def MREs
  OA::OA_ptr<OA::MemRefExprIterator> getDefMREs(OA::StmtHandle stmt);

  //! Get an Iterator over DiffUse MREs
  OA::OA_ptr<OA::MemRefExprIterator> getDiffUseMREs(OA::StmtHandle stmt);

  //! Given a MemRefHandle return an iterator over
  //! MemRefExprs that describe this memory reference
  OA::OA_ptr<OA::MemRefExprIterator> 
    getMemRefExprIterator(OA::MemRefHandle h);
 
  //! Given a procedure return associated SymHandle
  OA::SymHandle getSymHandle(OA::ProcHandle h) { return getProcSymHandle(h); }

  /*
  //! If this is a PTR_ASSIGN_STMT then return an iterator over MemRefHandle
  //! pairs where there is a source and target such that target
  //! FIXME: returning a bogus iterator with no pairs, will need this for F90
  OA::OA_ptr<OA::Alias::PtrAssignPairStmtIterator> 
      getPtrAssignStmtPairIterator(OA::StmtHandle stmt)
    {   OA::OA_ptr<OA::Alias::PtrAssignPairStmtIterator> retval;
        retval = new Open64PtrAssignPairStmtIterator;
        return retval;
    }
  */ 


  //! If this is a PTR_ASSIGN_STMT then return an iterator over MemRefHandle
  //! pairs where there is a source and target such that target
  OA::OA_ptr<OA::Alias::PtrAssignPairStmtIterator>
  getPtrAssignStmtPairIterator(OA::StmtHandle stmt);

  
  //! Return an iterator over <int, MemRefExpr> pairs
  //! where the integer represents which formal parameter 
  //! and the MemRefExpr describes the corresponding actual argument. 
  OA::OA_ptr<OA::Alias::ParamBindPtrAssignIterator>
      getParamBindPtrAssignIterator(OA::CallHandle call);

  //! Return the symbol handle for the nth formal parameter to proc
  //! Number starts at 0 and implicit parameters should be given
  //! a number in the order as well.  This number should correspond
  //! to the number provided in getParamBindPtrAssign pairs
  //! Should return SymHandle(0) if there is no formal parameter for 
  //! given num
  OA::SymHandle getFormalSym(OA::ProcHandle,int);

  //! Given a procedure call create a memory reference expression
  //! to describe that call.  For example, a normal call is
  //! a NamedRef.  A call involving a function ptr is a Deref.
  OA::OA_ptr<OA::MemRefExpr> getCallMemRefExpr(OA::CallHandle h);


  //! For the given symbol create a Location that indicates statically
  //! overlapping locations and information about whether the location
  //! is local or not, local means visible in only this procedure
  OA::OA_ptr<OA::Location> getLocation(OA::ProcHandle p, OA::SymHandle s);

  //-------------------------------------------------------------------------
  // ActivityIRInterface
  //-------------------------------------------------------------------------
  
  // Return an iterator over all independent locations for given proc
  //OA::OA_ptr<OA::LocIterator> getIndepLocIter(OA::ProcHandle h);
  //! Return an iterator over all independent MemRefExpr for given proc
  OA::OA_ptr<OA::MemRefExprIterator> getIndepMemRefExprIter(OA::ProcHandle h);
  
  // Return an iterator over all dependent locations for given proc
  //OA::OA_ptr<OA::LocIterator> getDepLocIter(OA::ProcHandle h);
  //! Return an iterator over all dependent MemRefExpr for given proc
  OA::OA_ptr<OA::MemRefExprIterator> getDepMemRefExprIter(OA::ProcHandle h);
 
  //! given a symbol return the size in bytes of that symbol
  int getSizeInBytes(OA::SymHandle h);

  //-------------------------------------------------------------------------
  // ReachDefsIRInterface
  //-------------------------------------------------------------------------

  //! Given a subprogram return an IRSymIterator for all
  //! symbols that are visible in the subprogram
  //OA::OA_ptr<OA::IRSymIterator> getVisibleSymIterator(OA::ProcHandle h);

  //! Given a subprogram return an IRSymIterator for all
  //! symbols that are referenced in the subprogram
  OA::OA_ptr<OA::IRSymIterator> getRefSymIterator(OA::ProcHandle h);

  // getStmtIterator(ProcHandle h)

  //! Return a list of all the memory reference handles that appear
  //! in the given statement.
  OA::OA_ptr<OA::MemRefHandleIterator> getAllMemRefs(OA::StmtHandle stmt);

  //! Return a list of all the target memory reference handles that appear
  //! in the given statement.
  OA::OA_ptr<OA::MemRefHandleIterator> getDefMemRefs(OA::StmtHandle stmt);

  //-------------------------------------------------------------------------
  // UDDUChainsIRInterface
  //-------------------------------------------------------------------------

  //! Return a list of all the source and sub memory reference handles 
  //! that appear in the given statement.
  OA::OA_ptr<OA::MemRefHandleIterator> getUseMemRefs(OA::StmtHandle stmt);

  //-------------------------------------------------------------------------
  // ReachConstsIRInterface
  //-------------------------------------------------------------------------

  //! Given a statement return a list to the pairs of 
  //! target MemRefHandle, ExprHandle where
  //! target = expr
  OA::OA_ptr<OA::AssignPairIterator> 
      getAssignPairIterator(OA::StmtHandle h); 
 
  // Iterator over Expressions in the given Statement
  OA::OA_ptr<OA::ExprHandleIterator>
      getExprHandleIterator(OA::StmtHandle stmt);
      
  //! Given an OpHandle and two operands (unary ops will just
  //! use the first operand and the second operand should be NULL)
  //! return a ConstValBasicInterface 
  OA::OA_ptr<OA::ConstValBasicInterface> evalOp(OA::OpHandle op, 
      OA::OA_ptr<OA::ConstValBasicInterface> operand1, 
      OA::OA_ptr<OA::ConstValBasicInterface> operand2);
  
  //! Given a ConstSymHandle return an abstraction representing the 
  //! constant value
  OA::OA_ptr<OA::ConstValBasicInterface> getConstValBasic(OA::ConstSymHandle c);
  
  //! Given a ConstValHandle return an abstraction representing the 
  //! constant value
  //! User must free the ConstValBasicInterface
  OA::OA_ptr<OA::ConstValBasicInterface> getConstValBasic(OA::ConstValHandle c);
  
  //! Return a stmt handle for the given memory reference handle
  OA::StmtHandle getStmtFromMemRef(OA::MemRefHandle h);

  //! Temporary routine for testing MPICFG  // given a ConstValBasicInterface, print out value if any
  std::string toString(OA::OA_ptr<OA::ConstValBasicInterface> cvPtr);

  //! Temporary routine for testing ReachConsts
  // Given an unsigned int, return a ConstValBAsicInterface for it
  OA::OA_ptr<OA::ConstValBasicInterface> getConstValBasic (unsigned int val);

  //! FIXME: temporary routine to find things out
  //! should be removed after testing
  int returnOpEnumValInt(OA::OpHandle op);

  //---------------------------------------------------------------------------
  // LinearityIRInterface.hpp
  //---------------------------------------------------------------------------

  //! get the operation type and returns a LinOpType
  OA::Linearity::LinOpType getLinearityOpType(OA::OpHandle op);

  //-------------------------------------------------------------------------
  // InterSideEffectIRInterface
  //-------------------------------------------------------------------------

  OA::OA_ptr<OA::SideEffect::SideEffectStandard> 
      getSideEffect(OA::ProcHandle callerProc, OA::SymHandle calleeSym);

  //-------------------------------------------------------------------------
  // ParamBindingsIRInterface
  //-------------------------------------------------------------------------

  //! Given an ExprHandle, return an ExprTree* 
  OA::OA_ptr<OA::ExprTree> getExprTree(OA::ExprHandle h);
  
  //! returns true if given symbol is a parameter 
  bool isParam(OA::SymHandle);
  
  //-------------------------------------------------------------------------
  // ICFGIRInterface
  //-------------------------------------------------------------------------
  //! Given the callee symbol returns the callee proc handle
  OA::ProcHandle getProcHandle(OA::SymHandle sym);
  
  //-------------------------------------------------------------------------
  // XAIFIRInterface
  //-------------------------------------------------------------------------

  //-------------------------------------------------------------------------

  // for Activity lookup
  OA::OA_ptr<OA::MemRefExpr>
   convertSymToMemRefExpr(OA::SymHandle sym);

  //***************************************************************************
  // Helpers
  //***************************************************************************
  static void DumpWN(WN* wn, ostream& os);

private:
  
  // assumption is that StmtHandles and MemRefHandles are unique across
  // different program and procedure contexts for which analysis is being
  // currently performed
  static std::map<OA::StmtHandle,std::set<OA::MemRefHandle> > 
      sStmt2allMemRefsMap;

  static std::map<OA::StmtHandle,std::set<OA::ExprHandle> >
      mStmt2allExprsMap;

  static std::map<OA::MemRefHandle,OA::StmtHandle> sMemRef2StmtMap;

  static std::map<OA::MemRefHandle,set<OA::OA_ptr<OA::MemRefExpr> > > 
      sMemref2mreSetMap;

  // mapping of call symbols to procedure handles
  static std::map<OA::SymHandle,OA::ProcHandle> sCallSymToProc;

  // mapping of fully qualified global variable names to a set of SymHandles
  // that all have that fully qualified name
  // the first symbol in the set will be used to represent all the others
  static std::map<fully_qualified_name,std::set<OA::SymHandle> > sGlobalVarMap;
  // the fully qualified name and procedure mapped to the localy symbol
  // for the particular procedure
  static std::map<fully_qualified_name,std::map<OA::ProcHandle,OA::SymHandle> >
      sFQNToProcToLocalSymMap;
  // mapping of each symbol handle to its fully qualified name
  static std::map<OA::SymHandle,fully_qualified_name> sSymToFQNMap;

  // mapping of symbol handles to strings used in getSideEffect
  static std::map<OA::SymHandle,std::string> sSymToVarStringMap;

  // mapping of actual params (PR_PARMs WNs) to CallHandles
  static std::map<OA::ExprHandle,OA::CallHandle> sParamToCallMap;

  // mapping of proc handles to a set of referenced symbols
  static std::map<OA::ProcHandle,std::set<OA::SymHandle> > 
     sProcToSymRefSetMap;

public:  
  //! Given a statement return an iterator over all memory references
  //! During its creation it also sets up sStmt2allMemRefs 
  //! and memRefs2mreSetMap.
  OA::OA_ptr<OA::MemRefHandleIterator> getMemRefIterator(OA::StmtHandle h); 

private:
  // helper functions for getMemRefIterator
  void findAllMemRefsAndMapToMemRefExprs(OA::StmtHandle stmt,
    WN* wn, unsigned lvl);

  /*
  void createAndMapDerefs(OA::StmtHandle stmt, WN* wn, WN* subMemRef, 
                          bool isAddrOf, bool fullAccuracy, 
                          OA::MemRefExpr::MemRefType hty);
  void createAndMapNamedRef(OA::StmtHandle stmt, WN* wn, ST*, bool isAddrOf,
    bool fullAccuracy, OA::MemRefExpr::MemRefType hty);
    */

  void createAndMapNamedRef(OA::StmtHandle stmt, WN* wn,
                            ST* st, OA::MemRefExpr::MemRefType hty);

  void createAndMapDerefs(OA::StmtHandle stmt, WN* wn, WN* subMemRef);

  bool isPassByReference(WN*);
  ST* findBaseSymbol(WN*);

  void currentProc(OA::ProcHandle p) {
    assert(p!=OA::ProcHandle(0));
    PU_Info* pu = (PU_Info*)p.hval();
    PU_SetGlobalState(pu);
  }

  //! use this to appropriately get a string for a symbol
  static const char* createCharStarForST(ST*);

  static void DumpWNMemRef(WN* wn, ostream& os);

  static void DumpWNLeaf(WN* wn, ostream & os);

  static void DumpWNMemRefLeaf(WN* wn, ostream& os);

  OA::MemRefHandle findTopMemRefHandle(WN *wn);

  bool ExprTree_hack_for_MPI(OA::MemRefHandle h, OA::OA_ptr<OA::ExprTree> tree);

  OA::OA_ptr<OA::ExprTree> createExprTree(WN* wn);

  OA::OA_ptr<OA::ExprTree::Node> 
  createExprTree(OA::OA_ptr<OA::ExprTree> tree, WN* wn);

//  OA::OA_ptr<OA::AssignPairIterator> findAssignPairs(WN* wn);

  OA::OA_ptr<OA::ConstValBasicInterface> getConstValBasicFromST(ST* st);

  // maintaining the context of all handles in terms of Program and Procedure
  // For now just allowing one program context
  static std::map<OA::IRHandle,OA::ProcHandle> sProcContext;
  static PU_Info* sProgContext;
  //static PU_Info* sCurrentProc; // current proc context in Open64 datastructures
  static bool sContextInit;
  static void initProcContext(PU_Info* pu_forest, 
                              Open64IRProcIterator &procIter);
  static void setCurrentProcToProcContext(OA::IRHandle h);
  static OA::ProcHandle getCurrentProcContext();
  static void setCurrentProcContext(OA::ProcHandle);

  static fully_qualified_name create_fqn(OA::SymHandle sym);

  // mapping of calls to procs
  static void initCallSymToProcMap(Open64IRProcIterator &procIter);

  // getting set of symbols that are referenced within a particul procedure
  static void initProcToSymRefSetMap(Open64IRProcIterator &procIter);

  // context information is initialized and used within some of the iterators
  // so they need to be friends
  friend class Open64IRProcIterator;
  friend class Open64IRMemRefIterator;
  friend class Open64IRExprHandleIterator;
  friend class InitContextVisitor;

private:

  // if this is true we do not supply  
  // call handles or statement handles 
  // for any calls to black box routines
  static bool ourIgnoreBlackBoxRoutines; 
 
public:

  static void setIgnoreBlackBoxRoutines(); 

  static bool ignoreBlackBoxRoutines(); 

  // true if the passed in WN 
  // is a call node for which we 
  // have a definition in sCallSymToProc
  static bool haveDefinition(WN* wn);

  //! return the formal parameter that an actual parameter is associated with.
  //! 'call' is a handle to the call node; 'param' is the actual
  //! parameter within the call node that we want info about.
  // BK: I need this to stay public for use in ManagerMPI_ICFG
  OA::SymHandle getFormalForActual(OA::ProcHandle caller, OA::CallHandle call, 
                                   OA::ProcHandle callee, OA::ExprHandle param);
  //! get callee SymHandle from CallHandle 
  //! (outermost or first, if multiple calls)
  // BK: I need this to stay public for use in ManagerMPI_ICFG
  OA::SymHandle getSymHandle(OA::CallHandle h);

  //! returns true if given symbol is a pass by reference parameter 
  // JU: I need this to stay public because I use it in FortTk
  bool isRefParam(OA::SymHandle);
               
  //! User is responsible for doing this.  It should not be part of
  // the Open64IRProcIterator
  static void initContextState(PU_Info* pu_forest);

  //! add a new IRHandle to the given procedure after context has
  //! already been initialized with initProcContext
  static void setContext(OA::IRHandle h, OA::ProcHandle proc) {
      sProcContext[h] = proc;
  }

};  



#endif

