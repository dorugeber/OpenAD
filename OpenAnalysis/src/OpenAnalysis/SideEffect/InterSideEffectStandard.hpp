/*! \file
  
  \brief Definition of SideEffectStandard 

  \authors Michelle Strout
  \version $Id: InterSideEffectStandard.hpp,v 1.8 2007/10/08 19:19:15 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef InterSideEffectStandard_hpp
#define InterSideEffectStandard_hpp

#include <set>
#include <map>
#include <OpenAnalysis/Utils/OA_ptr.hpp>
#include <OpenAnalysis/IRInterface/IRHandles.hpp>
#include <OpenAnalysis/SideEffect/InterSideEffectInterface.hpp>
#include <OpenAnalysis/SideEffect/SideEffectStandard.hpp>
#include <OpenAnalysis/OABase/Annotation.hpp>
#include <OpenAnalysis/Utils/GenOutputTool.hpp>


namespace OA {
  namespace SideEffect {

/* 
   
*/
class InterSideEffectStandard : public InterSideEffectInterface,
			        public virtual Annotation
{
  public:
    InterSideEffectStandard();
    ~InterSideEffectStandard() {}

    //*****************************************************************
    // Interface Implementation
    //*****************************************************************
    
    //! Return an iterator over all procedures for which interprocedural
    //! side effect information is available 
    OA_ptr<ProcIterator> getKnownProcIterator();
    
    //*************************************************************************

    //! Return an iterator over all locations that may be
    //! modified locally within the called procedure.  
    //! Locations modified directly in the called procedure and not
    //! by a function call in the called procedure.
    OA_ptr<LocIterator> getLMODIterator(CallHandle call);

    //! Return an iterator over all locations that are may be
    //! modified within the called procedure or within a procedure
    //! called by the called procedure.  
    OA_ptr<LocIterator> getMODIterator(CallHandle call);

    //! Return an iterator over all locations that are definitely
    //! modified locally within the called procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLDEFIterator(CallHandle call);

    //! Return an iterator over all locations that are definitely
    //! modified within the called procedure or within a procedure
    //! called by the called procedure.  
    OA_ptr<LocIterator> getDEFIterator(CallHandle call);

    //! Return an iterator over all locations that are 
    //! used locally within the called procedure before being
    //! definitely modified.  
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLUSEIterator(CallHandle call);

    //! Return an iterator over all locations that may be 
    //! used before being defined within the called procedure or 
    //! within a procedure called by the called procedure.  
    OA_ptr<LocIterator> getUSEIterator(CallHandle call);

    //! Return an iterator over all locations that are 
    //! used locally within the called procedure.
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLREFIterator(CallHandle call);

    //! Return an iterator over all locations that are 
    //! used within the called procedure or within a procedure
    //! called by the called procedure.  
    OA_ptr<LocIterator> getREFIterator(CallHandle call);

    //************************************************************************
    
    //! Return an iterator over all locations that are may be
    //! modified locally within the given procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLMODIterator(ProcHandle p);

    //! Return an iterator over all locations that are may be
    //! modified within the given procedure or within a procedure
    //! called by the given procedure.  
    OA_ptr<LocIterator> getMODIterator(ProcHandle p);

    //! Return an iterator over all locations that are definitely
    //! modified locally within the given procedure.  
    //! Locations modified directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLDEFIterator(ProcHandle p);

    //! Return an iterator over all locations that are definitely
    //! modified within the given procedure or within a procedure
    //! called by the given procedure.  
    OA_ptr<LocIterator> getDEFIterator(ProcHandle p);

    //! Return an iterator over all locations that are 
    //! used locally within the given procedure before being
    //! definitely modified.  
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLUSEIterator(ProcHandle p);

    //! Return an iterator over all locations that may be 
    //! used before being defined within the given procedure or 
    //! within a procedure called by the given procedure.  
    OA_ptr<LocIterator> getUSEIterator(ProcHandle p);

    //! Return an iterator over all locations that are 
    //! used locally within the given procedure.
    //! Locations used directly in the procedure and not
    //! by a function call in the procedure.
    OA_ptr<LocIterator> getLREFIterator(ProcHandle p);

    //! Return an iterator over all locations that are 
    //! used within the given procedure or within a procedure
    //! called by the given procedure.  
    OA_ptr<LocIterator> getREFIterator(ProcHandle p);

    //*****************************************************************
    // Construction methods
    //*****************************************************************
    
    //! Associate the given procedure with the given intraprocedural
    //! SideEffect information
    void mapProcToSideEffect(ProcHandle proc, 
                         OA_ptr<OA::SideEffect::SideEffectStandard> sideEffect);
    
    //! Get the SideEffect information associated with the given procedure
    OA_ptr<OA::SideEffect::SideEffectStandard> 
        getSideEffectResults(ProcHandle proc);

    //! Associate the given call with the given intraprocedural
    //! SideEffect information
    void mapCallToSideEffect(CallHandle call, 
                         OA_ptr<OA::SideEffect::SideEffectStandard> sideEffect);
    
    
    //! Return count of locations in all sets for given procedure
    int getLocCount(ProcHandle proc);
    
    //! Insert a location into the LMOD set for the given call
    void insertLMOD(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the MOD set for the given call
    void insertMOD(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the LDEF set for the given call
    void insertLDEF(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the DEF set for the given call
    void insertDEF(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the LUSE set for the given call
    void insertLUSE(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the USE set for the given call
    void insertUSE(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the LREF set for the given call
    void insertLREF(CallHandle call, OA_ptr<Location> loc);
    
    //! Insert a location into the REF set for the given call
    void insertREF(CallHandle call, OA_ptr<Location> loc);

    //! Initialize the information for a particular call to have all
    //! empty sets
    void initCallSideEffect(CallHandle call);

   
    //*****************************************************************
    // Output
    //*****************************************************************
  public:

    //! will use OutputBuilder to generate output
    void output(IRHandlesIRInterface& ir);
    
    void dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir);

  private:

    OUTPUT 
    
    GENOUT typedef std::map<ProcHandle,OA_ptr<OA::SideEffect::SideEffectStandard> >  
            ProcToSideEffectMap;
    GENOUT typedef std::map<CallHandle,OA_ptr<OA::SideEffect::SideEffectStandard> >  
            CallToSideEffectMap;

    // mappings of procedures to various sets
    GENOUT ProcToSideEffectMap mProcToSideEffectMap;

    // mapping function calls to various sets
    GENOUT CallToSideEffectMap mCallToSideEffectMap;

    // default SideEffect results
    OA_ptr<OA::SideEffect::Interface> mDefaultSideEffect;

    //! Iterator over procedures in the mProcToSideEffectMap
    class InterSideEffectProcIter : public virtual ProcIterator,
                                    public IRHandleSetIterator<ProcHandle>
    {
      public:
        InterSideEffectProcIter(OA_ptr<std::set<ProcHandle> > pSet )
            : IRHandleSetIterator<ProcHandle>(pSet) {}
        ~InterSideEffectProcIter() { }

        ProcHandle current() const
          { return IRHandleSetIterator<ProcHandle>::current(); }
        bool isValid() const 
          { return IRHandleSetIterator<ProcHandle>::isValid(); }
        void operator++() { IRHandleSetIterator<ProcHandle>::operator++(); }
        void reset() { IRHandleSetIterator<ProcHandle>::reset(); }
    };


};


  } // end of SideEffect namespace
} // end of OA namespace

#endif

