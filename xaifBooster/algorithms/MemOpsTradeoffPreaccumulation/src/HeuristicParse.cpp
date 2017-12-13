// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================
#include "xaifBooster/algorithms/MemOpsTradeoffPreaccumulation/inc/HeuristicParse.hpp"
#include "xaifBooster/utils/inc/DbgLoggerManager.hpp"

using namespace MemOpsTradeoffPreaccumulation;

namespace MemOpsTradeoffPreaccumulation {

  ConceptuallyStaticInstances::Heuristic HeuristicParse::stringToEnum(const std::string& theHeuristic) {

    if(theHeuristic == "VERTEX"){
      return ConceptuallyStaticInstances::VERTEX;
    }// end if VERTEX
    else if(theHeuristic == "EDGE"){
      return ConceptuallyStaticInstances::EDGE;
    }// end if EDGE
    else if(theHeuristic == "FACE"){
      return ConceptuallyStaticInstances::FACE;
    }// end if FACE
    else if(theHeuristic == "FORWARD"){
      return ConceptuallyStaticInstances::FORWARD;
    }// end if FORWARD
    else if(theHeuristic == "REVERSE"){
      return ConceptuallyStaticInstances::REVERSE;
    }// end if REVERSE
    else if(theHeuristic == "MARKOWITZ"){
      return ConceptuallyStaticInstances::MARKOWITZ;
    }// end if MARKOWITZ
    else if(theHeuristic == "SIBLING"){
      return ConceptuallyStaticInstances::SIBLING;
    }// end if SIBLING
    else if(theHeuristic == "SIBLING2"){
      return ConceptuallyStaticInstances::SIBLING2;
    }// end if SIBLING2
    else if(theHeuristic == "SUCCPRED"){
      return ConceptuallyStaticInstances::SUCCPRED;
    }// end if SUCCPRED
    else if(theHeuristic == "ABSORB"){
      return ConceptuallyStaticInstances::ABSORB;
    }// end if ABSORB
    else{
      THROW_LOGICEXCEPTION_MACRO("Error: Unknown Heuristic Passed");
    }// end else

  }// end FromString

  void HeuristicParse::fromString(const std::string& theHeuristicString){

    ConceptuallyStaticInstances::HeuristicList& theHeuristicList = ConceptuallyStaticInstances::instance()->getList();

    boost::tokenizer<> tok(theHeuristicString);
    
    for(boost::tokenizer<>::iterator beg = tok.begin(); beg!=tok.end(); ++beg){
      switch(HeuristicParse::stringToEnum(*beg)){
      case ConceptuallyStaticInstances::VERTEX:
	if(theHeuristicList.empty()){theHeuristicList.push_back(ConceptuallyStaticInstances::VERTEX);}
	else{
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	break;
      case ConceptuallyStaticInstances::EDGE:
	if(theHeuristicList.empty()){theHeuristicList.push_back(ConceptuallyStaticInstances::EDGE);}
	else{
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	break;
      case ConceptuallyStaticInstances::FACE:
	if(theHeuristicList.empty()){theHeuristicList.push_back(ConceptuallyStaticInstances::FACE);}
	else{
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	break;
      case ConceptuallyStaticInstances::FORWARD:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::FORWARD);}
	break;
      case ConceptuallyStaticInstances::REVERSE:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::REVERSE);}
	break;
      case ConceptuallyStaticInstances::MARKOWITZ:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::MARKOWITZ);}
	break;
      case ConceptuallyStaticInstances::SIBLING:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::SIBLING);}
	break;
      case ConceptuallyStaticInstances::SIBLING2:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::SIBLING2);}
	break;
      case ConceptuallyStaticInstances::SUCCPRED:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::SUCCPRED);}
	break;
      case ConceptuallyStaticInstances::ABSORB:
	if(theHeuristicList.empty()){
	  THROW_LOGICEXCEPTION_MACRO("Error: Heuristics out of order");
	}
	else{theHeuristicList.push_back(ConceptuallyStaticInstances::ABSORB);}
      }//end switch
   
    }// end for all tokens

  }// end fromString

}// end namespace MemOpsTradeoffPreaccumulation
