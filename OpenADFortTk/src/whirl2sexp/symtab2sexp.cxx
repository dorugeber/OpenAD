// -*-Mode: C++;-*-
// $Header: /m_home/m_utkej/Argonne/cvs2svn/cvs/OpenADFortTk/src/whirl2sexp/symtab2sexp.cxx,v 1.13 2007-10-08 18:28:34 utke Exp $

#include "Open64IRInterface/Open64BasicTypes.h"

#include "quad.h"
#include "SexpTags.h"

#include "wn2sexp.h"
#include "symtab2sexp.h"


using namespace whirl2sexp;
using namespace sexp; // for sexp::ostream, etc


//***************************************************************************
// Translate symbol tables
//***************************************************************************

void 
whirl2sexp::TranslateGlobalSymbolTables(sexp::ostream& sos, int flags)
{
  // Cf. symtab_defs.h, Write_Global_Info(), Read_Global_Info()
  //
  // GLOBAL_SYMTAB_TABLES: There are 13 tables in the global symtab.
  //   FILE_INFO        (File_info)
  //   ST_TAB           (St_Table)      [global and local]
  //     ST_ATTR_TABLE  (St_Attr_Table) [global and local]
  //   PU_TAB           (Pu_Table)
  //   TYPE_TABLE       (Ty_Table)
  //     FLD_TAB        (Fld_Table)
  //     ARB_TAB        (Arb_Table)
  //     TYLIST_TAB     (Tylist_Table)
  //   TCON_TAB         (Tcon_Table) 
  //     TCON_STR_TAB   (TCON_strtab)
  //   INITO_TABLE      (Inito_Table)   [global and local]
  //     INITV_TAB      (Initv_Table)
  //   BLK_TAB          (Blk_Table)
  // String table
  //   STR_TABLE   (Str_Table) [source code strings, not string constants]
  
  sos << BegList << Atom(SexpTags::GBL_SYMTAB) << EndLine;

  xlate_FILE_INFO(sos);
  sos << EndLine;
  
  xlate_ST_TAB(sos, GLOBAL_SYMTAB);
  sos << EndLine;

  xlate_ST_ATTR_TAB(sos, GLOBAL_SYMTAB);
  sos << EndLine;
  
  xlate_PU_TAB(sos);
  sos << EndLine;

  xlate_TY_TAB(sos);
  sos << EndLine;

  xlate_FLD_TAB(sos);
  sos << EndLine;

  xlate_ARB_TAB(sos);
  sos << EndLine;

  xlate_TYLIST_TAB(sos);
  sos << EndLine;
  
  xlate_TCON_TAB(sos);
  sos << EndLine;

  xlate_TCON_STR_TAB(sos);
  sos << EndLine;
  
  xlate_INITO_TAB(sos, GLOBAL_SYMTAB);
  sos << EndLine;

  xlate_INITV_TAB(sos);
  sos << EndLine;

  xlate_BLK_TAB(sos);
  sos << EndLine;
  
  xlate_STR_TAB(sos);
  
  sos << EndList;
}


void 
whirl2sexp::TranslateLocalSymbolTables(sexp::ostream& sos, SYMTAB_IDX stab_lvl,
				       int flags)
{
  // Cf. symtab_defs.h and Read_Local_Info()
  // 
  // LOCAL_SYMTAB_TABLES: There are 5 tables in the local symtab. 
  //   ST_TAB
  //     ST_ATTR_TAB
  //   LABEL_TABLE (Label_Table)
  //   PREG_TABLE  (Preg_Table)
  //   INITO_TAB
  
  sos << BegList << Atom(SexpTags::PU_SYMTAB) << EndLine;
  
  xlate_ST_TAB(sos, stab_lvl);
  sos << EndLine;

  xlate_ST_ATTR_TAB(sos, stab_lvl);
  sos << EndLine;
  
  xlate_LABEL_TAB(sos, stab_lvl);
  sos << EndLine;

  xlate_PREG_TAB(sos, stab_lvl);
  sos << EndLine;
  
  xlate_INITO_TAB(sos, stab_lvl);
  
  sos << EndList;
}


//***************************************************************************
// Translate individual tables
//***************************************************************************

// Note: 'For_all' applies 'operator()' to every entry of the
// respective table.

void 
whirl2sexp::xlate_FILE_INFO(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::FILE_INFO);
  
  // gp_group
  UINT gp = (UINT)FILE_INFO_gp_group(File_info);
  sos << Atom(gp);
  
  // flags
  const char* flg_str = FILE_INFO_FLAGS_To_Str(File_info.flags);
  sos << GenSexpFlg(flg_str);
  
  sos << EndList;
}


void 
whirl2sexp::xlate_ST_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl)
{
  sos << BegList << Atom(SexpTags::ST_TAB) << EndLine;
  For_all(St_Table, stab_lvl, xlate_SYMTAB_entry_op<ST>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_ST_ATTR_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl)
{
  sos << BegList << Atom(SexpTags::ST_ATTR_TAB) << EndLine;
  For_all(St_Attr_Table, stab_lvl, xlate_SYMTAB_entry_op<ST_ATTR>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_ST_TAB(sexp::ostream& sos, const SCOPE& scope)
{
  sos << BegList << Atom(SexpTags::ST_TAB) << EndLine;
  For_all(*scope.st_tab, xlate_SYMTAB_entry_op<ST>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_PU_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::PU_TAB) << EndLine;
  For_all(Pu_Table, xlate_SYMTAB_entry_op<PU>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_TY_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::TY_TAB) << EndLine;
  For_all(Ty_Table, xlate_SYMTAB_entry_op<TY>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_FLD_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::FLD_TAB) << EndLine;
  For_all(Fld_Table, xlate_SYMTAB_entry_op<FLD>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_ARB_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::ARB_TAB) << EndLine;
  For_all(Arb_Table, xlate_SYMTAB_entry_op<ARB>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_TYLIST_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::TYLIST_TAB) << EndLine;
  For_all(Tylist_Table, xlate_SYMTAB_entry_op<TYLIST>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_TCON_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::TCON_TAB) << EndLine;
  For_all(Tcon_Table, xlate_SYMTAB_entry_op<TCON>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_TCON_STR_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::TCON_STR_TAB) << EndLine; 

  // We must manually find the string boundaries, knowing
  // implementation details. Yuck.  

  // Details: Each char-array is preceeded by size info.  If the
  // char-array is less than 0xff bytes, the first byte contains the
  // size.  Otherwise the first byte is 0xff and the next 4 bytes hold
  // the size (UINT32).  The index points to the first byte in the
  // string!
  // E.g.: -xxx0-yyy0 [where - is size info; xxx and yyy are strings]
  const char* buf = Index_to_char_array(0); // the whole buffer
  UINT32 last_idx = TCON_strtab_size() - 1;
  for (UINT32 idx = 0; idx < last_idx; /* */) {
    // always at the beginning of a string
    if ((unsigned char)buf[idx] == 0xff) {
      xlate_SYMTAB_entry_TCON_STR(sos, idx+5);
    }
    else {
      xlate_SYMTAB_entry_TCON_STR(sos, idx+1);
    }
    idx += strlen(buf+idx) + 1;
    sos << sexp::EndLine;
  }
  
  sos << EndList;
}


void 
whirl2sexp::xlate_INITO_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl)
{
  sos << BegList << Atom(SexpTags::INITO_TAB) << EndLine;
  For_all(Inito_Table, stab_lvl, xlate_SYMTAB_entry_op<INITO>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_INITV_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::INITV_TAB) << EndLine;
  For_all(Initv_Table, xlate_SYMTAB_entry_op<INITV>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_BLK_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::BLK_TAB) << EndLine;
  For_all(Blk_Table, xlate_SYMTAB_entry_op<BLK>(sos));
  sos << EndList;
}


void 
whirl2sexp::xlate_STR_TAB(sexp::ostream& sos)
{
  sos << BegList << Atom(SexpTags::STR_TAB) << EndLine; 
  
  // We must manually find the string boundaries, knowing
  // implementation details. Yuck.  

  // Details: The first entry in the buffer is NULL and thus every
  // string is preceeded by a NULL.  The index points to the first
  // byte in the string!
  // E.g: 0xxx0yyy0zzz0  [where xxx, yyy, and zzz are strings]
  const char* buf = Index_To_Str(0); // the whole buffer
  STR_IDX last_idx = STR_Table_Size() - 1;
  for (STR_IDX idx = 0; idx < last_idx; ++idx) {
    if (buf[idx] == '\0') {
      xlate_SYMTAB_entry_STR(sos, idx+1);
      sos << sexp::EndLine;
    }
  }  
  
  sos << EndList;
}


void 
whirl2sexp::xlate_LABEL_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl)
{
  sos << BegList << Atom(SexpTags::LABEL_TAB) << EndLine;
  // osprey1.0/common/com/symtab_utils.h defines this without 'stab_lvl' arg!
  //For_all(Label_Table, stab_lvl, xlate_SYMTAB_entry_op<LABEL>(sos));
  For_all_entries(*Scope_tab[stab_lvl].label_tab, 
		  xlate_SYMTAB_entry_op<LABEL>(sos), 1);
  sos << EndList;
}


void 
whirl2sexp::xlate_PREG_TAB(sexp::ostream& sos, SYMTAB_IDX stab_lvl)
{
  sos << BegList << Atom(SexpTags::PREG_TAB) << EndLine;
  // osprey1.0/common/com/symtab_utils.h defines this without 'stab_lvl' arg!
  //For_all(Preg_Table, stab_lvl, xlate_SYMTAB_entry_op<PREG>(sos));
  For_all_entries(*Scope_tab[stab_lvl].preg_tab, 
		  xlate_SYMTAB_entry_op<PREG>(sos), 1);
  sos << EndList;
}


//***************************************************************************
// Function objects to translate individual table entries
//***************************************************************************

void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ST* st)
{
  using namespace sexp::IOFlags;
  
  sos << BegList << Atom(idx);
  
  // sym_class, storage_class, export_class
  ST_CLASS stclass = ST_class(st);
  const char* stclass_nm = Class_Name(stclass);
  sos << Atom(stclass_nm);
  
  ST_SCLASS stsclass = ST_sclass(st);
  const char* stsclass_nm = Sclass_Name(stsclass);
  sos << Atom(stsclass_nm);
  
  ST_EXPORT stexport = ST_export(st);
  const char* stexport_nm = Export_Name(stexport);
  sos << Atom(stexport_nm);
  
  // name_idx/tcon
  STR_IDX nmidx = ST_name_idx(st); // or TCON_IDX  
  sos << BegList << GenSexpSymNm(st) << Atom(nmidx) << EndList;
  
  // type/pu/blk
  if (stclass == CLASS_FUNC) {
    PU_IDX stpu = ST_pu(st);
    sos << Atom(stpu); // FIXME add tag?
  }
  else if (stclass == CLASS_BLOCK) {
    BLK_IDX stblk = ST_blk(st);
    sos << Atom(stblk); // FIXME add tag?
  }
  else {
    TY_IDX sttype = ST_type(st);
    sos << GenSexpTy(sttype);
  }
  
  // base_idx, offset
  ST_IDX stbase_idx = ST_base_idx(st);
  UINT64 oset = ST_ofst(st);
  sos << GenSexpSym(stbase_idx) << Atom(oset);
  
  // flags/flags_ext
  const char* flg_str = ST_FLAGS_To_Str(st->flags);
  const char* flgext_str = ST_EXT_FLAGS_To_Str(st->flags_ext);
  sos << GenSexpFlg(flg_str) << GenSexpFlg(flgext_str);
  
  // st_idx
  ST_IDX st_idx = ST_st_idx(st);
  sos << GenSexpSym(st_idx);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ST_ATTR* sta)
{
  sos << BegList << Atom(idx);
  
  // st_idx
  ST_IDX st_idx = ST_ATTR_st_idx(*sta);
  sos << GenSexpSym(st_idx);
  
  // kind
  ST_ATTR_KIND knd = ST_ATTR_kind(*sta);
  const char* knd_nm = ST_ATTR_Kind_Name(knd);
  sos << Atom(knd_nm);
  
  // reg_id/section_name
  PREG_NUM val = ST_ATTR_reg_id(*sta);
  sos << Atom(val);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, PU* pu)
{
  sos << BegList << Atom(idx);
  
  // prototype
  TY_IDX ty_idx = PU_prototype(*pu);
  sos << GenSexpTy(ty_idx);
  
  // lexical_level
  UINT lvl = (UINT)PU_lexical_level(*pu);
  sos << Atom(lvl);
  
  // gp_group
  UINT gp = (UINT)PU_gp_group(*pu);
  sos << Atom(gp);
  
  // src_lang
  UINT64 srclang = PU_src_lang(*pu);
  const char* srclang_str = PU_SRC_LANG_FLAGS_To_Str(srclang);
  sos << GenSexpFlg(srclang_str);
  
  // target_idx
  TARGET_INFO_IDX targidx = PU_target_idx(*pu);
  sos << Atom(targidx);

  // flags
  const char* flg_str = PU_FLAGS_To_Str(pu->flags);
  sos << GenSexpFlg(flg_str);

  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TY* typ)
{
  using namespace sexp::IOFlags;

  TY& ty = *typ;
  
  sos << BegList << Atom(idx);
  
  // kind
  TY_KIND knd = TY_kind(ty);
  const char* knd_nm = Kind_Name(knd);
  sos << Atom(knd_nm);
  
  // name_idx
  STR_IDX nmidx = TY_name_idx(ty);
  const char* nm = TY_name(ty);
  sos << BegList << Atom(A_DQUOTE, nm) << Atom(nmidx) << EndList;
  
  // mtype, size
  TYPE_ID mty = TY_mtype(ty);
  UINT64 sz   = TY_size(ty);
  const char* mty_nm = Mtype_Name(mty);
  sos << Atom(mty_nm) << Atom(sz);
  
  // flags
  UINT16 flg = TY_flags(ty);
  const char* flg_str = TY_FLAGS_To_Str(flg);
  sos << GenSexpFlg(flg_str);
  
  // arb/fld/tylist:         ARRAY, STRUCT, FUNCTION  (respectively)
  // etype/pointed/pu_flags: ARRAY, POINTER, FUNCTION (respectively)
  sos << BegList;
  if (knd == KIND_ARRAY) {
    ARB_HANDLE arb = TY_arb(ty); // FIXME add arb tag?
    TY_IDX ety = TY_etype(ty);
    sos << Atom(arb.Idx()) << GenSexpTy(ety);
  }
  else if (knd == KIND_STRUCT) {
    FLD_HANDLE fld = TY_fld(ty); // FIXME add tag?
    sos << Atom(fld.Idx());
  }
  else if (knd == KIND_POINTER) {
    TY_IDX basety = TY_pointed(ty);
    sos << GenSexpTy(basety);
  } 
  else if (knd == KIND_FUNCTION) {
    TYLIST_IDX tyl = TY_tylist(ty);
    const char* pu_flg_str = TY_PU_FLAGS_To_Str(ty.Pu_flags());
    sos << Atom(tyl) << GenSexpFlg(pu_flg_str);
  }
  sos << EndList;
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, FLD* fld)
{
  using namespace sexp::IOFlags;
  
  FLD_HANDLE fldh(fld, (FLD_IDX)idx);
  
  sos << BegList << Atom(idx);
  
  // name_idx
  STR_IDX nmidx = FLD_name_idx(fldh);
  const char* nm = FLD_name(fldh);
  sos << BegList << Atom(A_DQUOTE, nm) << Atom(nmidx) << EndList;
  
  // type
  TY_IDX ty_idx = fld->type; // FLD_type(fldh) is not a simple accessor!
  sos << GenSexpTy(ty_idx);
  
  // ofst, bsize, bofst
  UINT64 ofst = FLD_ofst(fldh);
  UINT8 bsz = FLD_bsize(fldh);
  UINT8 bofst = FLD_bofst(fldh);
  sos << Atom(ofst) << Atom(bsz) << Atom(bofst);
  
  // flags
  UINT16 flg = FLD_flags(fldh);
  const char* flg_str = FLD_FLAGS_To_Str(flg);
  sos << GenSexpFlg(flg_str);
  
  // st
  ST_IDX st_idx = FLD_st(fldh);
  sos << GenSexpSym(st_idx);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, ARB* arb)
{
  using namespace sexp::IOFlags;
  
  ARB_HANDLE arbh(arb, (ARB_IDX)idx);
  
  sos << BegList << Atom(idx);
  
  // flags, dimension, co_dimension
  UINT16 flg = ARB_flags(arbh);
  const char* flg_str = ARB_FLAGS_To_Str(flg);
  UINT16 dim = ARB_dimension(arbh);
  UINT16 codim = ARB_co_dimension(arbh);
  sos << GenSexpFlg(flg_str) << Atom(dim) << Atom(codim);
  
  // lbnd_val/(lbnd_var, lbnd_unused)
  if (ARB_const_lbnd(arbh)) {
    INT64 val = ARB_lbnd_val(arbh);
    sos << BegList << Atom("const") << Atom(val) << EndList;
  } 
  else {
    ST_IDX st_idx = ARB_lbnd_var(arbh);
    sos << GenSexpSym(st_idx);
  }
  
  // ubnd_val/(ubnd_var, ubnd_unused)
  if (ARB_const_ubnd(arbh)) {
    INT64 val = ARB_ubnd_val(arbh);
    sos << BegList << Atom("const") << Atom(val) << EndList;
  }
  else {
    ST_IDX st_idx = ARB_ubnd_var(arbh);
    sos << GenSexpSym(st_idx);
  }
  
  // stride_val/(stride_var, stride_unused)
  if (ARB_const_stride(arbh)) {
    INT64 val = ARB_stride_val(arbh);
    sos << BegList << Atom("const") << Atom(val) << EndList;
  }
  else {
    ST_IDX st_idx = ARB_stride_var(arbh);
    sos << GenSexpSym(st_idx);
  }
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TYLIST* tyl)
{
  sos << BegList << Atom(idx);

  // ty_idx
  TY_IDX ty_idx = TYLIST_type(*tyl);
  sos << GenSexpTy(ty_idx);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, TCON* tcon)
{
  // see osprey1.0/common/com/targ_const.h
  using namespace sexp::IOFlags;

  sos << BegList << Atom(idx);
  
  FortTk::uint128_t qd; // 16 byte value, a tcon has two of these
  
  // ty
  TYPE_ID mty = TCON_ty(*tcon);
  const char* mty_nm = Mtype_Name(mty);
  sos << Atom(mty_nm);

  // flags
  UINT32 flg = tcon->flags;
  const char* flg_str = TCONFlags_To_Str(flg);
  sos << GenSexpFlg(flg_str);
  
  // vals [quad] (FIXME: abstract these values from the union)
  FortTk::assign(qd, tcon->vals.qval);
  sos << BegList << Atom(A_HEX, qd.hi) << Atom(A_HEX, qd.lo) << EndList;
  
  // cmplxval [quad]
  FortTk::assign(qd, tcon->cmplxval.qival);
  sos << BegList << Atom(A_HEX, qd.hi) << Atom(A_HEX, qd.lo) << EndList;
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, INITO* inito)
{
  // see osprey1.0/common/com/irbdata_defs.h
  sos << BegList << Atom(idx);
  
  // st_idx
  sos << GenSexpSym(inito->st_idx);
  
  // val
  sos << Atom(inito->val);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, INITV* initv)
{
  // see osprey1.0/common/com/irbdata_defs.h
  sos << BegList << Atom(idx);
  
  // next
  sos << Atom(initv->next);

  // kind
  const char* initvknd_nm = InitvKind_Name(initv->kind);
  sos << Atom(initvknd_nm);
  
  // repeat1
  sos << Atom(initv->repeat1);
  
  // st/lab/lab1/mtype/tc/blk/pad
  sos << Atom(initv->St());

  // ofst/st2/repeat2/unused
  sos << Atom(initv->Ofst());  
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, BLK* blk)
{
  sos << BegList << Atom(idx);

  // size
  sos << Atom(blk->Size());

  // align
  sos << Atom(blk->Align());

  // flags
  const char* flgext_str = BLK_FLAGS_To_Str(blk->Flags());
  sos << GenSexpFlg(flgext_str);
  
  // section_idx
  sos << Atom(blk->Section_idx());

  // scninfo_idx
  sos << Atom(blk->Scninfo_idx());

  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, LABEL* lbl)
{
  using namespace sexp::IOFlags;

  sos << BegList << Atom(idx);
  
  // name_idx
  STR_IDX nmidx = LABEL_name_idx(*lbl);
  const char* nm = LABEL_name(*lbl);
  sos << BegList << Atom(A_DQUOTE, nm) << Atom(nmidx) << EndList;
    
  // kind
  LABEL_KIND knd = LABEL_kind(*lbl);
  const char* knd_nm = LABEL_Kind_Name(knd);
  sos << Atom(knd_nm);
  
  // flags
  UINT32 flg = (UINT32)lbl->flags;
  const char* flg_str = LABEL_FLAGS_To_Str(flg);
  sos << GenSexpFlg(flg_str);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry(sexp::ostream& sos, UINT32 idx, PREG* preg)
{
  using namespace sexp::IOFlags;
  
  sos << BegList << Atom(idx);
  
  // name_idx
  STR_IDX nmidx = PREG_name_idx(*preg);
  const char* nm = PREG_name(*preg);
  sos << BegList << Atom(A_DQUOTE, nm) << Atom(nmidx) << EndList;
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry_TCON_STR(sexp::ostream& sos, UINT32 idx)
{
  using namespace sexp::IOFlags;
  
  sos << BegList << Atom(idx);
  
  // char_array
  const char* str = Index_to_char_array(idx);
  sos << Atom(A_DQUOTE, str);
  
  sos << EndList;
}


void
whirl2sexp::xlate_SYMTAB_entry_STR(sexp::ostream& sos, UINT32 idx)
{
  using namespace sexp::IOFlags;
  
  sos << BegList << Atom(idx);
  
  // string
  const char* str = Index_To_Str(idx);
  sos << Atom(A_DQUOTE, str);
  
  sos << EndList;
}


//***************************************************************************

