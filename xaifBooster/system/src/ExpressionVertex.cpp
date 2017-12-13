// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include <sstream>
#include "xaifBooster/system/inc/ExpressionVertex.hpp"

namespace xaifBooster { 

  ExpressionVertex::ExpressionVertex() : 
    myExpressionVertexAlgBase_p(0)  {
  } 

  ExpressionVertex::~ExpressionVertex(){
    if (myExpressionVertexAlgBase_p) delete myExpressionVertexAlgBase_p;
  }
  
  ExpressionVertexAlgBase&
  ExpressionVertex::getExpressionVertexAlgBase() const {
    if (!myExpressionVertexAlgBase_p)
      THROW_LOGICEXCEPTION_MACRO("ExpressionVertex::getExpressionVertexAlgBase: not set");
    return *myExpressionVertexAlgBase_p;
  } // end getExpressionVertexAlgBase

  std::string ExpressionVertex::debug () const { 
    std::ostringstream out;
    out << "ExpressionVertex[" << Vertex::debug().c_str()
        << "]" << std::ends;
    return out.str();
  } // end debug

  const InlinableIntrinsicsCatalogueItem& 
  ExpressionVertex::getInlinableIntrinsicsCatalogueItem() const { 
    THROW_LOGICEXCEPTION_MACRO("ExpressionVertex::getInlinableIntrinsicsCatalogueItem: is not valid for this instance");
    // make up a dummy to satisfy the compiler
    // we never reach this, so...
    return *(new InlinableIntrinsicsCatalogueItem(1,false,ShapeChange::NO_CHANGE));
  } 

  const NonInlinableIntrinsicsCatalogueItem&
  ExpressionVertex::getNonInlinableIntrinsicsCatalogueItem() const {
    THROW_LOGICEXCEPTION_MACRO("ExpressionVertex::getNonInlinableIntrinsicsCatalogueItem: is not valid for this instance");
    // make up a dummy to satisfy the compiler
    // we never reach this, so...
    return *(new NonInlinableIntrinsicsCatalogueItem(false));
  }

  bool ExpressionVertex::isArgument() const { 
    return false;
  }

  bool ExpressionVertex::isIntrinsic() const {
    return false;
  }

} // end of namespace xaifBooster 
