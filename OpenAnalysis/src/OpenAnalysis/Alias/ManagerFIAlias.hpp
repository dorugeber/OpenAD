/*! \file
  
  \brief Declarations of the AnnotationManager that implements FIAlias.

  \authors Michelle Strout
  \version $Id: ManagerFIAlias.hpp,v 1.2 2007/10/08 19:19:09 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef ManagerFIAlias_H
#define ManagerFIAlias_H

//--------------------------------------------------------------------
#include <cassert>
#include <map>
#include <set>

// OpenAnalysis headers
#include <OpenAnalysis/IRInterface/AliasIRInterface.hpp>
#include <OpenAnalysis/Utils/UnionFindUniverse.hpp>
#include <OpenAnalysis/MemRefExpr/MemRefExprVisitor.hpp>
#include <OpenAnalysis/Alias/Interface.hpp>
#include <OpenAnalysis/Location/Locations.hpp>

namespace OA {
  namespace Alias {

/*!
   Visitor over memory reference expressions that determines a
   a fixed location for a memory reference based on the current
   set of alias results for possible sub MREs.
 */
class FixedLocationVisitor : public virtual MemRefExprVisitor {
  public:
    FixedLocationVisitor(OA_ptr<AliasIRInterface> ir, ProcHandle proc,
                         OA_ptr<Interface> aliasInterface)
      : mIR(ir), mProc(proc), mDirectRef(false),
        mAliasInterface(aliasInterface) 
    {
       mDirectRefLocations = new LocSet;
    }
    ~FixedLocationVisitor() {}

    OA_ptr<LocSetIterator> getDirectRefLocIterator();

    void visitNamedRef(NamedRef& ref); 
    void visitUnnamedRef(UnnamedRef& ref); 
    void visitUnknownRef(UnknownRef& ref);
    void visitAddressOf(AddressOf& ref);
    void visitDeref(Deref& ref);
    void visitFieldAccess(FieldAccess& ref); 
    // default handling of more specific SubSet specificiations
    void visitSubSetRef(SubSetRef& ref); 

  private:

    OA_ptr<LocSet> mDirectRefLocations;

    OA_ptr<Alias::Interface> mAliasInterface;

    void notDirect();

    bool mDirectRef;
    OA_ptr<Location> mLoc;
    OA_ptr<AliasIRInterface> mIR;
    ProcHandle mProc;
};

/*!
   Visitor over memory reference expressions that determines if
   a memory reference is some level of dereference involving a formal
   parameter or a symbol that is visible outside of the current
   procedure and inside the current proc.  If so then an 
   InvisibleLoc is created.
 */
class InvisibleLocationVisitor : public virtual MemRefExprVisitor {
  public:
    InvisibleLocationVisitor(OA_ptr<AliasIRInterface> ir, ProcHandle proc,
                             std::set<SymHandle>& procFormalSet) 
        : mIR(ir),mProc(proc),mInvisibleRef(false), 
          mProcFormalSet(procFormalSet) {}
    ~InvisibleLocationVisitor() {}

    bool isInvisibleRef();

    OA_ptr<Location> getInvisibleRefLoc();

    //! if NamedRef is the base of a derference then we need to know
    //! whether the NamedRef is a formal or not local
    void visitNamedRef(NamedRef& ref);

    void visitUnnamedRef(UnnamedRef& ref);
    void visitUnknownRef(UnknownRef& ref);
    void visitDeref(Deref& ref);
    void visitAddressOf(AddressOf& ref);

    // default handling of more specific SubSet specificiations
    // nothing more specific than this is needed because if
    // a deref to a global or formal was involved then this ref will 
    // become the mre in the invisible location.  Doesn't matter what 
    // subclass the ref is
    void visitSubSetRef(SubSetRef& ref);
    void visitFieldAccess(FieldAccess& ref);
    void visitIdxAccess(IdxAccess& ref);
    void visitIdxExprAccess(IdxExprAccess& ref);

  private:
    void notInvisible();

    bool mInvisibleRef;
    bool mBaseIsNotLocal;
    bool mBaseIsFormal;
    OA_ptr<Location> mLoc;
    OA_ptr<AliasIRInterface> mIR;
    ProcHandle mProc;
    std::set<SymHandle>& mProcFormalSet;

};

/*!
   Visitor over memory reference expressions that determines if 
   the innermost MRE is visible within the given procedure.
 */
class VisibleBaseVisitor : public virtual MemRefExprVisitor {
  public:
    VisibleBaseVisitor(OA_ptr<AliasIRInterface> ir, ProcHandle proc)
      : mIR(ir), mProc(proc),
        mBaseVisible(true)
    { }
    ~VisibleBaseVisitor() {}

    bool isBaseVisible() { return mBaseVisible; }

    void visitNamedRef(NamedRef& ref); 
    void visitUnnamedRef(UnnamedRef& ref); 
    void visitUnknownRef(UnknownRef& ref);
    void visitDeref(Deref& ref);
    void visitAddressOf(AddressOf& ref);
    // default handling of more specific SubSet specificiations
    void visitSubSetRef(SubSetRef& ref); 

  private:
    bool mBaseVisible;
    OA_ptr<AliasIRInterface> mIR;
    ProcHandle mProc;
};

class AnalyzedProcIterator : public IRProcIterator { 
public: 
  AnalyzedProcIterator(std::set<ProcHandle> analyzedProcs) :
    mAnalyzedProcs(analyzedProcs), mIt(analyzedProcs.begin())
  { } 
  ~AnalyzedProcIterator() { }; 
 
  virtual ProcHandle current() const; // Returns the current item. 
  virtual bool isValid() const;        // False when all items are exhausted. 
         
  virtual void operator++();     // prefix 
  void operator++(int) { ++*this; }  // postfix 

  virtual void reset(); 
private:
  std::set<ProcHandle>           mAnalyzedProcs;
  std::set<ProcHandle>::iterator mIt;

}; 

class RecordMREsVisitor;

// FIAlias may analyze all procedures defined in the program or  
// only those (iteratively) determined to be reachable.  
// The latter is our modification to Ryder's algorithm.
enum FIAliasImplement { ALL_PROCS = 0, REACHABLE_PROCS };

/*! 
    Creates a union-find universe holding sets of aliased MREs.
    Uses the FIAlias algorithm described in the Ryder2001 journal paper.
    Note that ManagerFIAlias no longer satisfies the Alias::Interface.hpp
    interface.  Returning a concrete representation of alias
    relations (via performAnalysis) is now left to subclasses 
    of ManagerFIAlias, such as ManagerFIAliasEquivSets and 
    ManagerFIAliasAliasMap.
*/
class ManagerFIAlias 
{ 
public:
  ManagerFIAlias(OA_ptr<AliasIRInterface> _ir);
  ~ManagerFIAlias () {}

  OA_ptr<UnionFindUniverse>
  performFIAlias( OA_ptr<IRProcIterator> procIter,
                  FIAliasImplement implement = ALL_PROCS );

  OA_ptr<IRProcIterator> getAnalyzedProcIter();

  void mergeSubSetRefs(OA_ptr<UnionFindUniverse> ufset);

protected:
  OA_ptr<AliasIRInterface> mIR;

  //===== helper datastructures and routines
  // mapping of MemRefExpr to unique ids
  std::map<OA_ptr<MemRefExpr>,int> mMREToID;
  // use a counter to assign unique ids to each MemRefExpr
  int mCount;

  // mapping of MemRefExpr to set of MemRefHandles that can
  // be expressed by said MemRefExpr
  std::map<OA_ptr<MemRefExpr>,std::set<MemRefHandle> > mMREToMemRefHandles;

  // mapping of MemRefExpr to procedures it shows up in
  std::map<OA_ptr<MemRefExpr>,std::set<ProcHandle> > mMREToProcs;

  // mapping of MemRefHandles to procedures, each MemRefHandle is only in
  // one procedure
  std::map<MemRefHandle,ProcHandle> mMemRefHandleToProc;

  //! set of formal symbols associated with each procedure
  std::map<ProcHandle,std::set<SymHandle> > mProcToFormalSet;

  //! maps mre to the given proc and memref, also assigns mre a unique ID
  void recordMRE( OA_ptr<MemRefExpr> mre, ProcHandle proc, MemRefHandle memref);
  void recordMRE( OA_ptr<MemRefExpr> mre, ProcHandle proc );

  OA_ptr<MemRefExpr> createDeref( OA_ptr<MemRefExpr> mre );

  std::set<OA_ptr<MemRefExpr> > 
    allMemRefExprsInSameSet( OA_ptr<MemRefExpr> pMRE, 
                             OA_ptr<UnionFindUniverse> ufset);

  //! records all memory reference expressions in a given procedure
  void initMemRefExprs( ProcHandle proc );

  //! records all memory reference expressions in all procedures
  void initMemRefExprs( OA_ptr<IRProcIterator> procIter );

  //! increment versions of FIAlias use a worklist.  this does not, so this is a noop.
  void addProcToWorkList(ProcHandle proc);

  //! perform Ryder phase 1 on stmt.
  void doPhase1Iteration(StmtHandle stmt, ProcHandle currProc, OA_ptr<UnionFindUniverse> ufset);

  //! perform Ryder phase 2
  bool doPhase2Iteration(OA_ptr<UnionFindUniverse> ufset, std::map<int,std::map<OA_ptr<MemRefExpr>,int> > & map);

  //! perform Ryder phase 3 on the callsite call invoked from caller currProc.
  void doPhase3Iteration(CallHandle call, ProcHandle currProc,OA_ptr<UnionFindUniverse> ufset, std::map<int,std::map<OA_ptr<MemRefExpr>,int> > & map);

  //! A set of reachable procs that have been analyzed.
  std::set<ProcHandle> mAnalyzedProcs;

  //! Perform FIAlias using Ryder's original algorithm that analyzes all procs.
  OA_ptr<UnionFindUniverse>
  performFIAliasAllProcs( OA_ptr<IRProcIterator> procIter );

  //! Perform FIAlias using a modification that only visits reachable procs.
  OA_ptr<UnionFindUniverse>
  performFIAliasReachableProcs( OA_ptr<IRProcIterator> procIter );

  friend class RecordMREsVisitor;

private: // helper functions
void outputMREsInSet(int setID, 
        OA_ptr<UnionFindUniverse> ufset, 
        std::map<int,std::map<OA_ptr<MemRefExpr>,int> > & map  );
void merge(int part1, int part2, OA_ptr<UnionFindUniverse> ufset, 
           std::map<int,std::map<OA_ptr<MemRefExpr>,int> > & map  );

  //! The list of procedures to be analyzed.
  std::set<ProcHandle> mWorklist;

  //! The flavor of FIAlias--examine all procs or only reachable procs.
  FIAliasImplement mImplement;

};


  } // end of Alias namespace
} // end of OA namespace

#endif
