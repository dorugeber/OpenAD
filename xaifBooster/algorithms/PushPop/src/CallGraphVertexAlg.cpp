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

#include "xaifBooster/utils/inc/PrintManager.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

#include "xaifBooster/system/inc/CallGraph.hpp"
#include "xaifBooster/system/inc/ConceptuallyStaticInstances.hpp"
#include "xaifBooster/system/inc/VariableSymbolReference.hpp"

#include "xaifBooster/algorithms/PushPop/inc/BasicBlockAlg.hpp"
#include "xaifBooster/algorithms/PushPop/inc/CallGraphVertexAlg.hpp"

using namespace xaifBooster;

namespace xaifBoosterPushPop { 

  CallGraphVertexAlg::CallGraphVertexAlg(CallGraphVertex& theContaining) : 
    CallGraphVertexAlgBase(theContaining),
    xaifBoosterAddressArithmetic::CallGraphVertexAlg(theContaining) {
  } // end CallGraphVertexAlg::CallGraphVertexAlg()

  CallGraphVertexAlg::~CallGraphVertexAlg() {
  } // end CallGraphVertexAlg::~CallGraphVertexAlg()

  std::string
  CallGraphVertexAlg::debug() const {
    std::ostringstream out;
    out << "xaifBoosterPushPop::CallGraphVertexAlg[" << xaifBoosterAddressArithmetic::CallGraphVertexAlg::debug()
        << ",myRequiredValueSet=" << myRequiredValueSet.debug()
        << "]" << std::ends;
    return out.str();
  } // end CallGraphVertexAlg::debug()

  void CallGraphVertexAlg::traverseToChildren(const GenericAction::GenericAction_E anAction_c) {
  } // end CallGraphVertexAlg::traverseToChildren()

  void CallGraphVertexAlg::algorithm_action_6_TD() {
    DBG_MACRO(DbgGroup::CALLSTACK, "xaifBoosterPushPop::CallGraphVertexAlg::algorithm_action_6(place pushes and pops) called for: " << debug().c_str());
    // create a map from CFG vertices to their respective sets of required values
    CFGVertexP2RequiredValuePListMap theCFGVertexP2RequiredValuePListMap;
    const xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList& theRequiredValuesPList (myRequiredValueSet.getRequiredValuesPList());
    for (xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList::const_iterator reqValI = theRequiredValuesPList.begin();
         reqValI != theRequiredValuesPList.end(); ++reqValI) {
      // add this value to the map
      theCFGVertexP2RequiredValuePListMap[&(*reqValI)->getControlFlowGraphVertex()].push_back(*reqValI);
    } // end iterate over required variables

    // build a list of all CFG vertices (the keys of the map)
    std::list<const ControlFlowGraphVertex*> theCFGVerticesCPList;
    for (CFGVertexP2RequiredValuePListMap::const_iterator mapI = theCFGVertexP2RequiredValuePListMap.begin();
         mapI != theCFGVertexP2RequiredValuePListMap.end(); ++mapI) {
      theCFGVerticesCPList.push_back(mapI->first);
    } // end for all map pairs

    // iterate over CFG vertices
    for (std::list<const ControlFlowGraphVertex*>::const_iterator cfgI = theCFGVerticesCPList.begin();
         cfgI != theCFGVerticesCPList.end(); ++cfgI) {
      const ControlFlowGraphVertex& theCFGVertex (**cfgI);
      const xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList&
       cfgRequiredValuePList (theCFGVertexP2RequiredValuePListMap[&theCFGVertex]);

      // determine the type of the CFGVertex in which the variable occurs
      switch (theCFGVertex.getKind()) {
        case ControlFlowGraphVertexKind::ENTRY_VKIND:
          break;
        case ControlFlowGraphVertexKind::EXIT_VKIND:
          break;
        case ControlFlowGraphVertexKind::IF_VKIND:
          break;
        case ControlFlowGraphVertexKind::FORLOOP_VKIND:
          break;
        case ControlFlowGraphVertexKind::PRELOOP_VKIND:
          break;
        case ControlFlowGraphVertexKind::POSTLOOP_VKIND:
          break;
        case ControlFlowGraphVertexKind::BASICBLOCK_VKIND: {
          const BasicBlock& theBasicBlock (dynamic_cast<const BasicBlock&>(theCFGVertex));
          xaifBoosterPushPop::BasicBlockAlg& theBasicBlockAlg (dynamic_cast<xaifBoosterPushPop::BasicBlockAlg&>(theBasicBlock.getBasicBlockAlgBase()));
          //const xaifBoosterPushPop::BasicBlockAlg& theBasicBlockAlg (dynamic_cast< const xaifBoosterPushPop::BasicBlockAlg&>(theBasicBlock.getBasicBlockAlgBase()));

          // partition the required values into equivalence classes according to their definition
          // FIXME: we have to delete the lists in this list at some point...is this the best idea?
          RequiredValuePListPList theDefEquivalenceClassList;
          for (xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList::const_iterator reqValI = cfgRequiredValuePList.begin();
               reqValI != cfgRequiredValuePList.end(); ++reqValI) {
            // we skip expressions with more than one argument for the time being.
            // \todo descend into expressions to get at all their arguments
            if (!(*reqValI)->isArgument()) continue;
            // find the appropriate equivalance class
            RequiredValuePListPList::iterator eqI = theDefEquivalenceClassList.begin();
            for (; eqI != theDefEquivalenceClassList.end(); ++eqI) {
              if (ConceptuallyStaticInstances::instance()->getCallGraph().getDuUdMap().sameDefinition(
                    (*reqValI)->getArgument().getVariable().getDuUdMapKey(),
                    (*eqI)->front()->getArgument().getVariable().getDuUdMapKey()
                  )) {
                DBG_MACRO(DbgGroup::CALLSTACK,(*reqValI)->debug() << " is part of an EXISTING EQUIVALENCE CLASS for this basic block");
                (*eqI)->push_back(*reqValI);
                break;
              }
            } // end for all def equivalance classes
            if (eqI == theDefEquivalenceClassList.end()) {
              // no definition equivalence class was found for this value,
              // so we make a new one and push this value to it
              DBG_MACRO(DbgGroup::CALLSTACK,(*reqValI)->debug() << " REQUIRES A NEW equivalence class");
              theDefEquivalenceClassList.push_back(new xaifBoosterRequiredValues::RequiredValueSet::RequiredValuePList);
              theDefEquivalenceClassList.back()->push_back(*reqValI);
            }
          } // end for all values required in this basic block
          // push one value from each equivalence class
          for (RequiredValuePListPList::const_iterator eqCI = theDefEquivalenceClassList.begin();
               eqCI != theDefEquivalenceClassList.end(); ++eqCI) {
            theBasicBlockAlg.pushSupremum(**eqCI);
          } // end for all definition equivalence classes
          // deallocate the list
          for (RequiredValuePListPList::const_iterator eqI = theDefEquivalenceClassList.begin();
               eqI != theDefEquivalenceClassList.end(); ++eqI)
            if (*eqI)
              delete *eqI;
	  break;
        } // end BASICBLOCK_VKIND
        case ControlFlowGraphVertexKind::ENDLOOP_VKIND:
	  break;
        case ControlFlowGraphVertexKind::ENDBRANCH_VKIND:
	  break;
        case ControlFlowGraphVertexKind::BRANCH_VKIND:
	  break;
        case ControlFlowGraphVertexKind::LABEL_VKIND:
	  break;
        case ControlFlowGraphVertexKind::GOTO_VKIND:
	  break;
        case ControlFlowGraphVertexKind::UNDEF_VKIND:
          THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::CallGraphVertexAlg::algorithm_action_6:"
                                     << "CFG vertex " << theCFGVertex.debug() << " has kind UNDEF_VKIND");
	  break;
	default:
          THROW_LOGICEXCEPTION_MACRO("xaifBoosterPushPop::CallGraphVertexAlg::algorithm_action_6:"
                        << "CFG vertex " << theCFGVertex.debug()
                        << " has an unrecognized kind "<< ControlFlowGraphVertexKind::toString(theCFGVertex.getKind()));
	  break;

      } // end switch on CFGVertex kind

      //const Expression& theExpression ((*reqValI)->getExpression());
    } // end for all CFG vertices
  } // end CallGraphVertexAlg::algorithm_action_6()

} // end namespace xaifBoosterPushPop

