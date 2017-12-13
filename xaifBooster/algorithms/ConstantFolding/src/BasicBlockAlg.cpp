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
//#include <iostream.h>
#include "xaifBooster/system/inc/ControlFlowGraphVertex.hpp"
//#include "xaifBooster/system/inc/GraphVizDisplay.hpp"
#include "xaifBooster/algorithms/BasicBlockPreaccumulation/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraph.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphAlgFactory.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphEdgeAlgFactory.hpp"
#include "xaifBooster/algorithms/ConstantFolding/inc/EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory.hpp"

namespace xaifBoosterConstantFolding { 
  BasicBlockAlg::BasicBlockAlg( BasicBlock& theContaining ) :
    BasicBlockAlgBase(theContaining), xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg(theContaining) {
  }
  
  void BasicBlockAlg::algorithm_action_3() {
    SequencePList SpL = getUniqueSequencePList();
    for (SequencePList::iterator i=(SpL).begin();i!=(SpL).end();++i) {
      EnhancedPrivateLinearizedComputationalGraph& g=(dynamic_cast <EnhancedPrivateLinearizedComputationalGraph&> (*(*i)->myFlattenedSequence_p));
//      GraphVizDisplay::show(g,"flattened");
      g.RunParallelEdgeAddition();
//      GraphVizDisplay::show(g,"flattened");
//      cout<<"Parallel Addition Complete\n";
      g.RunConstantCheckForwardBackward();
//      GraphVizDisplay::show(g,"flattened");
//      cout<<"ConstantCheck Complete\n";
      g.RunTrivialCheckForward();
//      GraphVizDisplay::show(g,"flattened");
//      cout<<"Trivial CheckForward Complete\n";
      g.RunTrivialCheckBackwards();
//      GraphVizDisplay::show(g,"flattened");
      g.RunTrivialCheckForwardAgain();
//      cout<<"Trivial CheckForward Complete\n";
//      GraphVizDisplay::show(g,"flattened");
      g.RunTrivialCheckBackwardsAgain();
//      cout<<"PLCG is Constant Folded\n";
//      GraphVizDisplay::show(g,"flattened");
    }
//    cout<<"\n\nConstantFolding is Done, so the error is in Basic Block or Lower\n\n";
    xaifBoosterBasicBlockPreaccumulation::BasicBlockAlg::algorithm_action_3();
  }
  void BasicBlockAlg::init() {
    setPrivateLinearizedComputationalGraphAlgFactory(EnhancedPrivateLinearizedComputationalGraphAlgFactory::instance());
    setPrivateLinearizedComputationalGraphEdgeAlgFactory(EnhancedPrivateLinearizedComputationalGraphEdgeAlgFactory::instance());
    setPrivateLinearizedComputationalGraphVertexAlgFactory(EnhancedPrivateLinearizedComputationalGraphVertexAlgFactory::instance());
  } 

} // end of namespace xaifBoosterAngelInterfaceAlgorithms 
