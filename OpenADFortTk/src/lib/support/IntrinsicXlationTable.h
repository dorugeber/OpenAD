// -*-Mode: C++;-*-
// $Header: /Volumes/cvsrep/developer/OpenADFortTk/src/IntrinsicXlationTable.h,v 1.8 2006/05/12 16:12:22 utke Exp $


#ifndef IntrinsicXlationTable_H 
#define IntrinsicXlationTable_H

#include <iostream>
#include <vector>

#include "Open64IRInterface/Open64BasicTypes.h"


namespace fortTkSupport { 

  /** 
   * IntrinsicXlationTable: There is not a trivial correspondence
   * between WHIRL and XAIF function/subroutine calls and intrinsics.
   * We use this table to determine how translation between WHIRL <->
   * XAIF should proceed.
   * 
   * These are the types of relationships that currently exist in
   * translation.  The starred (*) relationships are explicitly included
   * in the table.  
   * 
   *   WHIRL                          XAIF     
   *   -----                          ----     
   *   
   *                        /-------> SubCall (using WHIRL name)
   *                       /--------> FuncCall (using WHIRL name)
   *                      /
   * * OPR_CALL  <------------------> * Intrinsic, using XAIF name
   *
   * * OPR_INTRINSIC_CALL <---------> * Intrinsic, using XAIF name
   *
   * * OPR_INTRINSIC_OP <-----------> * Intrinsic, using XAIF name
   * 
   * * OPR_TRUNC <------------------> * Intrinsic, using XAIF name
   *   et al.
   * 
   * * OPR_EQ    <------------------> * BooleanOp, using XAIF name
   */
  class IntrinsicXlationTable {
  public:
  
    enum TableType { 
      W2X, // A table optimized for WHIRL->XAIF lookups
      X2W  // A table optimized for XAIF->WHIRL lookups
    };

    /**
     * WNOprClass: Three completely different kinds of WHIRL operators
     * are translated into an XAIF intrinsic and vice-versa.
     */
    enum WNOprClass {
      WNCall             =0, // A WHIRL function or subroutine call
      WNIntrinCall       =1, // A WHIRL intrinsics call
      WNIntrinOp         =2, // A WHIRL intrinsics operation
      WNExpr             =3, // A WHIRL expression
      WNOprClass_UNKNOWN =4  // special, reserved for searching table
    };

    static const std::string toString(const WNOprClass& oprcl);
  
    /**
     * XAIFOpr is the analog of the WHIRL 'OPERATOR'.  It is not really
     * needed at this point.
     */
    enum XAIFOpr {
      XAIFSubCall,
      XAIFFuncCall,
      XAIFIntrin,
      XAIFBoolOp,
      XAIFOpr_UNKNOWN   // special, reserved for searching table
    };

    static const std::string toString(const XAIFOpr& opr);

    struct WHIRLInfo {
      WNOprClass oprcl;   // class of the WN operator
      OPERATOR opr;       // the WN operator
      const char* name;   // string qualifier for calls
      unsigned int numop; // number of operands to intrinsic
      unsigned int numOptional; // number of optional arguments
      void dump(std::ostream& os = std::cerr) const;
      void ddump() const;
    };
  
    /** 
     * N.B.: 'key' serves two purposes at the moment.  First it
     * disambiguates multiple WHIRL constructs that translate into the
     * same XAIF intrinsic (e.g. calls to SIN or DSIN); during
     * WHIRL->XAIF, the key will be placed in the xaif:Intrinsic's
     * annotation attribute.  Second, during XAIF->WHIRL if an
     * ambiguous xaif:Intrinsic is found (e.g. sin_scal without its
     * annotation attribute), the key assigns a priority (based on
     * strcmp()) that defines the default WHIRL construct to use.
     */
    struct XAIFInfo {
      XAIFOpr opr;        // the XAIF 'operator'
      const char* name;   // intrinsic name
      const char* key;    // disambiguates otherwise identical xaif intrinsics
      unsigned int numop; // number of operands to intrinsic
      void dump(std::ostream& os = std::cerr) const;
      void ddump() const;
    };

    /** 
     * table entry
     */
    struct Entry {
      WHIRLInfo whirl_info;
      XAIFInfo xaif_info;
    };

  public:
    IntrinsicXlationTable(const TableType& tt);

    ~IntrinsicXlationTable();

    /**
     * since we can't have a std::pair with a reference type...
     */
    struct XAIFInfoPair {  
      XAIFInfoPair(bool aBoolean,
		   const XAIFInfo& anXAIFInfo);
      bool first;
      const XAIFInfo& second;
    private:
      /** 
       * no def
       */
      XAIFInfoPair();
    };

    /** 
     * first is true if we found it
     */
    XAIFInfoPair findXAIFInfo(OPERATOR opr, const char* name, bool mustFind=true);
    
    WHIRLInfo* findWHIRLInfo(XAIFOpr opr, const char* name, const char* key);

    void dump(std::ostream& os = std::cerr) const;
    void ddump() const;

  private:

    /** 
     * no def
     */
    IntrinsicXlationTable(const IntrinsicXlationTable& p);

    /** 
     * no def
     */
    IntrinsicXlationTable& operator=(const IntrinsicXlationTable& p);

    class LtSortedTable {
    public:
      LtSortedTable(TableType aTableType, bool ignoreXaifKey=false); 
      ~LtSortedTable();

      // return true if e1 < e2; false otherwise
      bool operator()(const Entry* e1, const Entry* e2) const;

      // required for binary_search(..), lower_bound(..) on GCC 3.3 (?!)
      bool operator()(Entry*& e1, const Entry& e2) {
	return operator()(dynamic_cast<Entry*>(e1), 
			  dynamic_cast<const Entry*>(&e2));
      }
      bool operator()(const Entry& e1, Entry*& e2) {
	return operator()(dynamic_cast<const Entry*>(&e1), 
			  dynamic_cast<Entry*>(e2));
      }
    
      // required for binary_search(..), lower_bound(..) on GCC 3.3 (?!)
      bool operator()(const Entry& e1, const Entry& e2) {
	return operator()(dynamic_cast<const Entry*>(&e1), 
			  dynamic_cast<const Entry*>(&e2));
      }
      bool operator()(const Entry*& e1, const Entry*& e2) {
	return operator()(dynamic_cast<const Entry*>(e1), 
			  dynamic_cast<const Entry*>(e2));
      }
    
    private:
      /** 
       * no def
       */
      LtSortedTable();
    
    private:
      TableType myTableType;         // sorting type
      bool      myIgnoreXaifKeyFlag; // only meaningful in X2W mode
    };
  
    typedef std::vector<Entry*>     SortedTable;
    typedef SortedTable::iterator   SortedTableIt;
    typedef SortedTable::value_type SortedTableVal;

  private:
  
    TableType   myTableType;
    SortedTable mySortedTable;
  
    static Entry        ourTable[];
    static unsigned int ourTableSize;

  };

} 

#endif

