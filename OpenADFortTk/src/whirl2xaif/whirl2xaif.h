// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2xaif/whirl2xaif.h,v 1.10 2007-10-08 18:28:34 utke Exp $


#ifndef whirl2xaif_h
#define whirl2xaif_h

#include <fstream>

#include "Open64IRInterface/Open64BasicTypes.h"

#include "xmlostream.h"
#include "IntrinsicXlationTable.h"
#include "OAMaps.h"
#include "ScalarizedRefTab.h"
#include "WhirlIDMaps.h"
#include "Open64IRInterface/stab_attr.h"
#include "Open64IRInterface/wn_attr.h"
#include "Diagnostics.h"
#include "PUXlationContext.h"

namespace whirl2xaif {
  
  class Whirl2Xaif {
  public : 
    static void translateIR(std::ostream& os, 
			    PU_Info* pu_forest,
			    const char* tmpVarPrefix);

    static fortTkSupport::IntrinsicXlationTable& getIntrinsicXlationTable();
    static fortTkSupport::InterOAInfoMap& getOAAnalMap();
    static fortTkSupport::ScalarizedRefTabMap_W2X& getScalarizedRefTableMap();
    static fortTkSupport::WNToWNIdTabMap& getWNToWNIdTableMap();

  private:

    /** 
     * no def
     */
    Whirl2Xaif();

    static fortTkSupport::IntrinsicXlationTable ourIntrinsicTable;
    static const std::string ourDividerComment;
    static fortTkSupport::InterOAInfoMap ourOAAnalMap;
    static fortTkSupport::ScalarizedRefTabMap_W2X ourScalarizedRefTableMap;
    static fortTkSupport::WNToWNIdTabMap ourWNToWNIdTableMap;

    static void translateScopeHierarchy(xml::ostream& xos, 
					PU_Info* pu_forest, 
					PUXlationContext& ctxt);

    static void translateScopeHierarchyPU(xml::ostream& xos, 
					  PU_Info* pu, 
					  UINT32 parentId, 
					  PUXlationContext& ctxt);

    static void translateAnalMaps(xml::ostream& xos, 
				  PU_Info* pu_forest, 
				  PUXlationContext& ctxt);

    static void translatePU(xml::ostream& xos, 
			    OA::OA_ptr<OA::CallGraph::Node> n, 
			    UINT32 vertexId, 
			    PUXlationContext& ctxt);

    static void translatePU(xml::ostream& xos, 
			    PU_Info *pu, 
			    UINT32 vertexId,
			    PUXlationContext& ctxt);

    static void translateWNPU(xml::ostream& xos, 
			      WN* pu, 
			      PUXlationContext& ctxt);

    static void dumpTranslationHeaderComment(xml::ostream& xos);

  };

}

#endif 
