/*! \file
  
  \brief Default implementation for interprocedural side effect analysis
         IR interface
  
  \authors Michelle Strout
  \version $Id: InterSideEffectIRInterfaceDefault.hpp,v 1.4 2007/10/08 19:19:13 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef InterSideEffectIRInterfaceDefault_h
#define InterSideEffectIRInterfaceDefault_h

#include <OpenAnalysis/IRInterface/InterSideEffectIRInterface.hpp>
#include <OpenAnalysis/SideEffect/SideEffectStandard.hpp>

namespace OA {

  namespace SideEffect {


class InterSideEffectIRInterfaceDefault : public virtual InterSideEffectIRInterface
{
 public:
  InterSideEffectIRInterfaceDefault() { }
  virtual ~InterSideEffectIRInterfaceDefault() { }

  //! Returns the default conservative SideEffectStandard results 
  virtual OA_ptr<SideEffect::SideEffectStandard> 
  getSideEffect(ProcHandle caller, SymHandle calleesym)
    { OA_ptr<SideEffect::SideEffectStandard> retval;
      retval = new SideEffect::SideEffectStandard;
      return retval;
    }

};  

  } // end of namespace SideEffect
} // end of namespace OA

#endif 
