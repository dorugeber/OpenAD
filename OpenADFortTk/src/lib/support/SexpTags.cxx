// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#include <string>

#include "SexpTags.h"

namespace SexpTags {

  const char* WHIRL       = "whirl";
  const char* PU_FOREST   = "pu_forest";
  const char* PU_TREE     = "pu_tree";
  const char* PU          = "pu";
  
  const char* GBL_SYMTAB  = "gbl_symtab";
  const char* PU_SYMTAB   = "pu_symtab";
  
  const char* FILE_INFO   = "file_info";
  const char* ST_TAB      = "st_tab";
  const char* TY_TAB      = "ty_tab";
  const char* PU_TAB      = "pu_tab";
  const char* FLD_TAB     = "fld_tab";
  const char* ARB_TAB     = "arb_tab";
  const char* TYLIST_TAB  = "tylist_tab";
  const char* TCON_TAB    = "tcon_tab";
  const char* TCON_STR_TAB = "tcon_str_tab";
  const char* INITO_TAB   = "inito_tab";
  const char* INITV_TAB   = "initv_tab";
  const char* BLK_TAB     = "blk_tab";
  const char* ST_ATTR_TAB = "st_attr_tab";
  const char* STR_TAB     = "str_tab";
  const char* PREG_TAB    = "preg_tab";
  const char* LABEL_TAB   = "label_tab";
  
  const char* ST          = "st";
  const char* TY          = "ty";
  const char* FLG         = "flg";
  const char* OFLG        = "oflg";
  
};


static const char* BLANK_LINE =
";;\n";

static const char* GRAMMAR_SUMMARY =
";; *** Summary ***\n"
";; WHIRL      ::= (whirl GBL_SYMTAB PU_FOREST)\n"
";; GBL_SYMTAB ::= (gbl_symtab GBL_TAB1..GBL_TABn)        ; n is fixed\n"
";; PU_FOREST  ::= (pu_forest PU_TREE*)                   ; list of PU_TREEs\n"
";; PU_TREE    ::= (pu_tree PU PU_TREE*) | NULL\n"
";; PU         ::= (pu PU_SYM PU_SYMTAB WN_AST)\n"
";; PU_SYMTAB  ::= (pu_symtab PU_TAB1..PU_TABm)           ; m is fixed\n"
";; WN_AST     ::= (WN_OPR WN_ATTRS WN_AST*) | NULL\n";

static const char* GRAMMAR_SYMTAB_DETAILS =
";; *** Symbol Table Details ***\n"
";; GBL_SYMTAB ::= (gbl_symtab\n"
";;                   FILE_INFO       ; file information\n"
";;                   ST_TAB          ; symbol table (global)\n"
";;                     ST_ATTR_TAB   ;   misc. attributes for ST_TAB\n"
";;                   PU_TAB          ; program-unit table\n"
";;                   TY_TAB          ; type table\n"
";;                     FLD_TAB       ;   field table (structured types)\n"
";;                     ARB_TAB       ;   array-bound table\n"
";;                     TYLIST_TAB    ;   function prototypes\n"
";;                   TCON_TAB        ; constant table\n"
";;                     TCON_STR_TAB  ;   string constant table\n"
";;                   INITO_TAB       ; initial values for init'd objects\n"
";;                     INITV_TAB     ;   values for scalar components\n"
";;                   BLK_TAB         ; layout info for block of data \n"
";;                   STR_TAB         ; string table\n"
";;                   )\n"
";; PU_SYMTAB  ::= (pu_symtab\n"
";;                   ST_TAB          ; symbol table (local)\n"
";;                     ST_ATTR_TAB   ;   misc. attributes for ST_TAB\n"
";;                   LABEL_TAB       ; label table\n"
";;                   PREG_TAB        ; psuedo-register table\n"
";;                   INITO_TAB       ; initial values for init'd objects\n"
";;                   )\n"
";;\n"
";; FILE_INFO   ::= (file_info GP_GROUP FLAGS)\n"
";;\n"
";; ST_TAB      ::= (st_tab ENTRY*)\n"
";; ENTRY       ::= (IDX SYM_CLASS STORAGE_CLASS EXPORT_CLASS\n"
";;                  NAME_IDX/TCON TYPE/PU/BLK BASE_IDX OFFSET\n"
";;                  FLAGS FLAGS_EXT ST)\n"
";;\n"
";; ST_ATTR_TAB ::= (st_attr_tab ENTRY*)\n"
";; ENTRY       ::= (IDX ST KIND REG_ID/SECTION_NAME)\n"
";;\n"
";; PU_TAB      ::= (pu_tab ENTRY*)\n"
";; ENTRY       ::= (IDX PROTOTYPE LEXICAL_LEVEL GP_GROUP SRC_LANG\n"
";;                  TARGET_IDX FLAGS)\n"
";;\n"
";; TY_TAB      ::= (ty_tab ENTRY*)\n"
";; ENTRY       ::= (IDX KIND NAME_IDX MTYPE SIZE FLAGS\n"
";;                  ARB/FLD/TYLIST ETYPE/POINTED/PU_FLAGS)\n"
";;\n"
";; FLD_TAB     ::= (fld_tab ENTRY*)\n"
";; ENTRY       ::= (IDX NAME_IDX TYPE OFST, BSIZE, BOFST FLAGS ST)\n"
";;\n"
";; ARB_TAB     ::= (arb_tab ENTRY*)\n"
";; ENTRY       ::= (IDX FLAGS DIMENSION CO_DIMENSION\n"
";;                  LBND_VAL/LBND_VAR UBND_VAL/UBND_VAR\n"
";;                  STRIDE_VAL/STRIDE_VAR)\n"
";;\n"
";; TYLIST_TAB  ::= (tylist_tab ENTRY*)\n" 
";; ENTRY       ::= (IDX TY)\n"
";;\n"
";; TCON_TAB    ::= (tcon_tab ENTRY*)\n"
";; ENTRY       ::= (IDX TY FLAGS VALS CMPLXVAL)\n"
";;\n"
";; TCON_STR_TAB ::= (tcon_str_tab ENTRY*)\n"
";; ENTRY       ::= (IDX CHAR_ARRAY)\n"
";;\n"
";; INITO_TAB   ::= (inito_tab ENTRY*)\n"
";; ENTRY       ::= (IDX ST VAL)\n"
";;\n"
";; INITV_TAB   ::= (initv_tab ENTRY*)\n"
";; ENTRY       ::= (IDX NEXT KIND REPEAT1\n"
";;                  ST/LAB/LAB1/MTYPE/TC/BLK/PAD OFST/ST2/REPEAT2/UNUSED)\n"
";;\n"
";; BLK_TAB     ::= (blk_tab ENTRY*)\n"
";; ENTRY       ::= (IDX SIZE ALIGN FLAGS SECTION_IDX SCNINFO_IDX)\n"
";;\n"
";; STR_TAB     ::= (str_tab ENTRY*)\n"
";; ENTRY       ::= (IDX STRING)\n"
";;\n"
";; LABEL_TAB   ::= (label_tab ENTRY*)\n"
";; ENTRY       ::= (IDX NAME_IDX KIND FLAGS)\n"
";;\n"
";; PREG_TAB    ::= (preg_tab ENTRY*)\n"
";; ENTRY       ::= (IDX NAME_IDX)\n"
";;\n"
";;\n"
";; ST  ::= (st HSTR SCOPE_IDX IDX)\n"
";; TY  ::= (ty HSTR IDX ALIGN)\n"
";;\n"
";; HSTR (helper string) is a (possibly empty) quoted string.\n";

// Other notes:
// - indices aways begin with 1 (never 0)
// - 'entry' above is duplicated to make names easier to read
// - tylist-tab lists are null-terminated
// - strtab indices are special


static const char* GRAMMAR_AST_DETAILS =
";; *** AST Details ***\n"
";; WN_OPR     ::= OPERATOR RTYPE DTYPE\n"
";; WN_ATTRS   ::= ((ATTR1 ...) (ATTR2 ...) (ATTR3 ...) ...)\n";


const char* 
SexpTags::get_grammar_summary()
{
  return GRAMMAR_SUMMARY;
}


const char* 
SexpTags::get_grammar()
{
  static std::string str;
  if (str.size() == 0) {
    str += GRAMMAR_SUMMARY;
    str += BLANK_LINE;
    str += GRAMMAR_SYMTAB_DETAILS;
    str += BLANK_LINE;
    str += GRAMMAR_AST_DETAILS;
  }
  
  return str.c_str();
}
