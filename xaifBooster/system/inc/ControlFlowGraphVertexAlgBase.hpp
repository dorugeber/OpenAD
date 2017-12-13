#ifndef _CONTROLFLOWGRAPHVERTEXALGBASE_INCLUDE_
#define _CONTROLFLOWGRAPHVERTEXALGBASE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/AlgBase.hpp"
#include "xaifBooster/system/inc/Expression.hpp"

namespace xaifBooster {  
  
  class ControlFlowGraphVertex;
  
  class ControlFlowGraphVertexAlgBase : public AlgBase<ControlFlowGraphVertex> {

  public:

    ControlFlowGraphVertexAlgBase(const ControlFlowGraphVertex& theContaining);

    virtual ~ControlFlowGraphVertexAlgBase();

    /// simply returns false (designed to be redefined by the classes that inherit from this)
    virtual bool hasExpression(const Expression& anExpression) const;

  private:

    /// no def
    ControlFlowGraphVertexAlgBase();

    /// no def
    ControlFlowGraphVertexAlgBase(const ControlFlowGraphVertexAlgBase&);

    /// no def
    ControlFlowGraphVertexAlgBase& operator=(const ControlFlowGraphVertexAlgBase&);

  }; // end class ControlFlowGraphVertexAlgBase

} // end namespace xaifBooster

#endif
