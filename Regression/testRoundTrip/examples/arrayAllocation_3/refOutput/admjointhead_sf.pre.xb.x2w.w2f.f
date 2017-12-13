
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) A(1 : INT(SIZE(__value__(X))))
      TYPE (OpenADTy_active) B(1 : INT(SIZE(__value__(X))))
      TYPE (OpenADTy_active) C(1 : INT(SIZE(__value__(X))))
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_17
      INTEGER(w2f__i8) OpenAD_Symbol_19
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_20
      INTEGER(w2f__i8) OpenAD_Symbol_21
      INTEGER(w2f__i8) OpenAD_Symbol_23
      INTEGER(w2f__i8) OpenAD_Symbol_24
      INTEGER(w2f__i8) OpenAD_Symbol_25
      INTEGER(w2f__i8) OpenAD_Symbol_26
      INTEGER(w2f__i8) OpenAD_Symbol_27
      INTEGER(w2f__i8) OpenAD_Symbol_28
      REAL(w2f__8) OpenAD_Symbol_29(:)
      ALLOCATABLE OpenAD_Symbol_29
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_30
      REAL(w2f__8) OpenAD_Symbol_32(:)
      ALLOCATABLE OpenAD_Symbol_32
      INTEGER(w2f__i8) OpenAD_Symbol_33
      REAL(w2f__8) OpenAD_Symbol_35(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_36(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_37(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_38(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_39
      INTEGER(w2f__i8) OpenAD_Symbol_40
      INTEGER(w2f__i8) OpenAD_Symbol_41
      INTEGER(w2f__i8) OpenAD_Symbol_42
      INTEGER(w2f__i8) OpenAD_Symbol_43
      INTEGER(w2f__i8) OpenAD_Symbol_44
      INTEGER(w2f__i8) OpenAD_Symbol_45
      INTEGER(w2f__i8) OpenAD_Symbol_46
      REAL(w2f__8) OpenAD_Symbol_47(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_48(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_49(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_50
      INTEGER(w2f__i8) OpenAD_Symbol_51
      INTEGER(w2f__i8) OpenAD_Symbol_52
      INTEGER(w2f__i8) OpenAD_Symbol_53
      INTEGER(w2f__i8) OpenAD_Symbol_54
      REAL(w2f__8) OpenAD_Symbol_55(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_56(:)
      ALLOCATABLE OpenAD_Symbol_56
      INTEGER(w2f__i8) OpenAD_Symbol_57
      REAL(w2f__8) OpenAD_Symbol_59(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_60
      INTEGER(w2f__i8) OpenAD_Symbol_61
      INTEGER(w2f__i8) OpenAD_Symbol_62
      INTEGER(w2f__i8) OpenAD_Symbol_63
      REAL(w2f__8) OpenAD_Symbol_64(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_65(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_66
      INTEGER(w2f__i8) OpenAD_Symbol_67
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_acc_1(1 : 2)
      REAL(w2f__8) OpenAD_acc_10(:)
      ALLOCATABLE OpenAD_acc_10
      REAL(w2f__8) OpenAD_acc_2(1 : 2)
      REAL(w2f__8) OpenAD_acc_3(1 : 2)
      REAL(w2f__8) OpenAD_acc_4(1 : 2)
      REAL(w2f__8) OpenAD_acc_5(1 : 2)
      REAL(w2f__8) OpenAD_acc_6(1 : 2)
      REAL(w2f__8) OpenAD_acc_7(1 : 2)
      REAL(w2f__8) OpenAD_acc_8(1 : 2)
      REAL(w2f__8) OpenAD_acc_9(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_1(1 : 2)
      REAL(w2f__8) OpenAD_aux_2(1 : 2)
      REAL(w2f__8) OpenAD_aux_3(1 : 2)
      REAL(w2f__8) OpenAD_aux_4(1 : 2)
      REAL(w2f__8) OpenAD_aux_5(:)
      ALLOCATABLE OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6(1 : 2)
      REAL(w2f__8) OpenAD_aux_7(:)
      ALLOCATABLE OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8(:)
      ALLOCATABLE OpenAD_aux_8
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(1 : 2)
      REAL(w2f__8) OpenAD_lin_10(:)
      ALLOCATABLE OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_11(1 : 2)
      REAL(w2f__8) OpenAD_lin_12(:)
      ALLOCATABLE OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_13(:)
      ALLOCATABLE OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_14(:)
      ALLOCATABLE OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_15(1 : 2)
      REAL(w2f__8) OpenAD_lin_16(:)
      ALLOCATABLE OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_17(1 : 2)
      REAL(w2f__8) OpenAD_lin_18(:)
      ALLOCATABLE OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_19(:)
      ALLOCATABLE OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_20(:)
      ALLOCATABLE OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_21(:)
      ALLOCATABLE OpenAD_lin_21
      REAL(w2f__8) OpenAD_lin_3(1 : 2)
      REAL(w2f__8) OpenAD_lin_4(1 : 2)
      REAL(w2f__8) OpenAD_lin_5(:)
      ALLOCATABLE OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6(1 : 2)
      REAL(w2f__8) OpenAD_lin_7(1 : 2)
      REAL(w2f__8) OpenAD_lin_8(:)
      ALLOCATABLE OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9(1 : 2)
      TYPE (OpenADTy_active) OpenAD_prp_0(:)
      ALLOCATABLE OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(A(1 : INT(SIZE(X)))) = (TAN(__value__(X)) / SIN(
     > __value__(X)))
      __value__(B(1 : INT(SIZE(X)))) = (COS(__value__(X) * 8.0D00) +
     >  __value__(X(1 : 2)) * __value__(A(1 : INT(SIZE(X)))))
      __value__(B(1 : INT(SIZE(X)))) = (__value__(B(1 : INT(SIZE(X))))
     >  * __value__(A(1 : INT(SIZE(X)))) * TAN(__value__(X)))
      __value__(C(1 : INT(SIZE(X)))) = ((__value__(A(1 : INT(SIZE(X))))
     >  **(4.0D00 / __value__(A(1 : INT(SIZE(X)))))) - SQRT(__value__(X
     > ) * __value__(A) + COS(__value__(A)) * TAN(__value__(B))))
      __value__(Y(1 : 2)) = (__value__(A(1 : INT(SIZE(X)))) + __value__
     > (C(1 : INT(SIZE(X)))) * 5.0D00)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = TAN(__value__(X))
      OpenAD_aux_1 = SIN(__value__(X))
      OpenAD_lin_2 = (OpenAD_aux_0 * OpenAD_aux_0 + INT(1_w2f__i8))
      OpenAD_lin_0 = (INT(1_w2f__i8) / OpenAD_aux_1)
      OpenAD_lin_3 = COS(__value__(X))
      OpenAD_lin_1 = (-(OpenAD_aux_0 /(OpenAD_aux_1 * OpenAD_aux_1)))
      __value__(A(1 : INT(SIZE(X)))) = (OpenAD_aux_0 / OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_1)
      OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_0)
      OpenAD_Symbol_0 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_1)
      OpenAD_Symbol_1 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
      OpenAD_aux_2 = (__value__(X) * 8.0D00)
      OpenAD_lin_4 = (- SIN(OpenAD_aux_2))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5, __deriv__(A(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_5 = __value__(A(1 : INT(SIZE(X))))
      OpenAD_lin_6 = __value__(X(1 : 2))
      __value__(B(1 : INT(SIZE(X)))) = (COS(OpenAD_aux_2) + __value__(X
     > (1 : 2)) * __value__(A(1 : INT(SIZE(X)))))
      OpenAD_acc_2 = (8.0D00 * OpenAD_lin_4)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_2)
      OpenAD_Symbol_2 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
      OpenAD_Symbol_3 = SIZE(OpenAD_lin_5, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_5)
      OpenAD_Symbol_5 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_6)
      OpenAD_Symbol_6 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_7)
      OpenAD_aux_4 = TAN(__value__(X))
      OpenAD_aux_3 = (__value__(A(1 : INT(SIZE(X)))) * OpenAD_aux_4)
      OpenAD_lin_7 = OpenAD_aux_3
      OpenAD_lin_9 = OpenAD_aux_4
      OpenAD_lin_11 = (OpenAD_aux_4 * OpenAD_aux_4 + INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_10, __deriv__(A(1 : INT(SIZE
     > (X)))))
      OpenAD_lin_10 = __value__(A(1 : INT(SIZE(X))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_8, __deriv__(B(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_8 = __value__(B(1 : INT(SIZE(X))))
      __value__(B(1 : INT(SIZE(X)))) = (__value__(B(1 : INT(SIZE(X))))
     >  * OpenAD_aux_3)
      OpenAD_acc_3 = (OpenAD_lin_9 * OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_11 * OpenAD_lin_10 * OpenAD_lin_8)
      OpenAD_Symbol_8 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_7)
      OpenAD_Symbol_9 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_3)
      OpenAD_Symbol_10 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_10)
      OpenAD_Symbol_11 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_4)
      OpenAD_Symbol_12 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_5, __deriv__(A(1 : INT(SIZE(
     > X)))))
      OpenAD_aux_5 = (4.0D00 / __value__(A(1 : INT(SIZE(X)))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_7, __deriv__(A))
      OpenAD_aux_7 = COS(__value__(A))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_8, __deriv__(B))
      OpenAD_aux_8 = TAN(__value__(B))
      OpenAD_aux_6 = SQRT(__value__(X) * __value__(A) + OpenAD_aux_7 *
     >  OpenAD_aux_8)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_12, OpenAD_aux_5)
      OpenAD_lin_12 = (OpenAD_aux_5 *(__value__(A(1 : INT(SIZE(X)))) **
     > (OpenAD_aux_5 - INT(1_w2f__i8))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_14, __deriv__(A(1 : INT(SIZE
     > (X)))))
      OpenAD_lin_14 = (-(4.0D00 /(__value__(A(1 : INT(SIZE(X)))) *
     >  __value__(A(1 : INT(SIZE(X)))))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_13, OpenAD_aux_5)
      OpenAD_lin_13 = (LOG(__value__(A(1 : INT(SIZE(X))))) *(__value__(
     > A(1 : INT(SIZE(X)))) ** OpenAD_aux_5))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_16, __deriv__(A))
      OpenAD_lin_16 = __value__(A)
      OpenAD_lin_17 = __value__(X)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_20, __deriv__(A))
      OpenAD_lin_20 = (- SIN(__value__(A)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_18, OpenAD_aux_8)
      OpenAD_lin_18 = OpenAD_aux_8
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_21, OpenAD_aux_8)
      OpenAD_lin_21 = (OpenAD_aux_8 * OpenAD_aux_8 + INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_19, OpenAD_aux_7)
      OpenAD_lin_19 = OpenAD_aux_7
      OpenAD_lin_15 = (5.0D-01 / OpenAD_aux_6)
      __value__(C(1 : INT(SIZE(X)))) = ((__value__(A(1 : INT(SIZE(X))))
     >  ** OpenAD_aux_5) - OpenAD_aux_6)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_acc_10, OpenAD_lin_13)
      OpenAD_acc_5 = (OpenAD_lin_15 * INT((-1_w2f__i8)))
      OpenAD_acc_6 = (OpenAD_lin_21 * OpenAD_lin_19 * OpenAD_acc_5)
      OpenAD_acc_7 = (OpenAD_lin_20 * OpenAD_lin_18 * OpenAD_acc_5)
      OpenAD_acc_8 = (OpenAD_lin_16 * OpenAD_acc_5)
      OpenAD_acc_9 = (OpenAD_lin_17 * OpenAD_acc_5)
      OpenAD_acc_10 = (OpenAD_lin_14 * OpenAD_lin_13)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_6)
      OpenAD_Symbol_13 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_13)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_7)
      OpenAD_Symbol_14 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_8)
      OpenAD_Symbol_15 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_15)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_9)
      OpenAD_Symbol_16 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_16)
      OpenAD_Symbol_17 = SIZE(OpenAD_lin_12, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_17)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_12)
      OpenAD_Symbol_19 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_19)
      OpenAD_Symbol_20 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_20)
      OpenAD_Symbol_21 = SIZE(OpenAD_acc_10, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_21)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_10)
      OpenAD_Symbol_23 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_23)
      OpenAD_Symbol_24 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_24)
      __value__(Y(1 : 2)) = (__value__(A(1 : INT(SIZE(X)))) + __value__
     > (C(1 : INT(SIZE(X)))) * 5.0D00)
      OpenAD_Symbol_25 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_25)
      OpenAD_Symbol_26 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_26)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_27)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(5.0D00, __deriv__(Y(1 : 2)), __deriv__(C(1 : INT(
     > OpenAD_Symbol_27))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_28)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 2)), __deriv__(A(1 : INT(
     > OpenAD_Symbol_28))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_30)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_29, OpenAD_Symbol_30)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_29)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_33)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_32, OpenAD_Symbol_33)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_32)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_35)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_36)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_37)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_38)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_39)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_40)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_29, __deriv__(C(1 : INT(OpenAD_Symbol_39
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_40))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_41)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_42)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_32, __deriv__(C(1 : INT(OpenAD_Symbol_41
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_42))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_43)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_35, __deriv__(C(1 : INT(OpenAD_Symbol_43
     > ))), __deriv__(A))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_44)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_36, __deriv__(C(1 : INT(OpenAD_Symbol_44
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_45)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_37, __deriv__(C(1 : INT(OpenAD_Symbol_45
     > ))), __deriv__(A))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_46)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_38, __deriv__(C(1 : INT(OpenAD_Symbol_46
     > ))), __deriv__(B))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(C(1 : INT(OpenAD_Symbol_46))))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_47)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_48)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_49)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(__deriv__(OpenAD_prp_0), __deriv__(B(1
     >  : INT(SIZE(X)))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_50)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_47, __deriv__(B(1 : INT(OpenAD_Symbol_50
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_51)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_52)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_48, __deriv__(B(1 : INT(OpenAD_Symbol_51
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_52))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_53)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_49, __deriv__(B(1 : INT(OpenAD_Symbol_53
     > ))), __deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(B(1 : INT(OpenAD_Symbol_53))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_54)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(B(1 : INT(
     > OpenAD_Symbol_54))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_55)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_57)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_56, OpenAD_Symbol_57)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_56)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_59)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_60)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_61)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_55, __deriv__(B(1 : INT(OpenAD_Symbol_60
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_61))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_62)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_56, __deriv__(B(1 : INT(OpenAD_Symbol_62
     > ))), __deriv__(X(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_63)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_59, __deriv__(B(1 : INT(OpenAD_Symbol_63
     > ))), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(B(1 : INT(OpenAD_Symbol_63))))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_64)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_65)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_66)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_64, __deriv__(A(1 : INT(OpenAD_Symbol_66
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_67)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_65, __deriv__(A(1 : INT(OpenAD_Symbol_67
     > ))), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(A(1 : INT(OpenAD_Symbol_67))))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = TAN(__value__(X))
      OpenAD_aux_1 = SIN(__value__(X))
      OpenAD_lin_2 = (OpenAD_aux_0 * OpenAD_aux_0 + INT(1_w2f__i8))
      OpenAD_lin_0 = (INT(1_w2f__i8) / OpenAD_aux_1)
      OpenAD_lin_3 = COS(__value__(X))
      OpenAD_lin_1 = (-(OpenAD_aux_0 /(OpenAD_aux_1 * OpenAD_aux_1)))
      __value__(A(1 : INT(SIZE(X)))) = (OpenAD_aux_0 / OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_1)
      OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_0)
      OpenAD_Symbol_0 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_1)
      OpenAD_Symbol_1 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
      OpenAD_aux_2 = (__value__(X) * 8.0D00)
      OpenAD_lin_4 = (- SIN(OpenAD_aux_2))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5, __deriv__(A(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_5 = __value__(A(1 : INT(SIZE(X))))
      OpenAD_lin_6 = __value__(X(1 : 2))
      __value__(B(1 : INT(SIZE(X)))) = (COS(OpenAD_aux_2) + __value__(X
     > (1 : 2)) * __value__(A(1 : INT(SIZE(X)))))
      OpenAD_acc_2 = (8.0D00 * OpenAD_lin_4)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_2)
      OpenAD_Symbol_2 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
      OpenAD_Symbol_3 = SIZE(OpenAD_lin_5, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_5)
      OpenAD_Symbol_5 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_6)
      OpenAD_Symbol_6 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_7)
      OpenAD_aux_4 = TAN(__value__(X))
      OpenAD_aux_3 = (__value__(A(1 : INT(SIZE(X)))) * OpenAD_aux_4)
      OpenAD_lin_7 = OpenAD_aux_3
      OpenAD_lin_9 = OpenAD_aux_4
      OpenAD_lin_11 = (OpenAD_aux_4 * OpenAD_aux_4 + INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_10, __deriv__(A(1 : INT(SIZE
     > (X)))))
      OpenAD_lin_10 = __value__(A(1 : INT(SIZE(X))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_8, __deriv__(B(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_8 = __value__(B(1 : INT(SIZE(X))))
      __value__(B(1 : INT(SIZE(X)))) = (__value__(B(1 : INT(SIZE(X))))
     >  * OpenAD_aux_3)
      OpenAD_acc_3 = (OpenAD_lin_9 * OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_11 * OpenAD_lin_10 * OpenAD_lin_8)
      OpenAD_Symbol_8 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_7)
      OpenAD_Symbol_9 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_3)
      OpenAD_Symbol_10 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_10)
      OpenAD_Symbol_11 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_4)
      OpenAD_Symbol_12 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_5, __deriv__(A(1 : INT(SIZE(
     > X)))))
      OpenAD_aux_5 = (4.0D00 / __value__(A(1 : INT(SIZE(X)))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_7, __deriv__(A))
      OpenAD_aux_7 = COS(__value__(A))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_aux_8, __deriv__(B))
      OpenAD_aux_8 = TAN(__value__(B))
      OpenAD_aux_6 = SQRT(__value__(X) * __value__(A) + OpenAD_aux_7 *
     >  OpenAD_aux_8)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_12, OpenAD_aux_5)
      OpenAD_lin_12 = (OpenAD_aux_5 *(__value__(A(1 : INT(SIZE(X)))) **
     > (OpenAD_aux_5 - INT(1_w2f__i8))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_14, __deriv__(A(1 : INT(SIZE
     > (X)))))
      OpenAD_lin_14 = (-(4.0D00 /(__value__(A(1 : INT(SIZE(X)))) *
     >  __value__(A(1 : INT(SIZE(X)))))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_13, OpenAD_aux_5)
      OpenAD_lin_13 = (LOG(__value__(A(1 : INT(SIZE(X))))) *(__value__(
     > A(1 : INT(SIZE(X)))) ** OpenAD_aux_5))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_16, __deriv__(A))
      OpenAD_lin_16 = __value__(A)
      OpenAD_lin_17 = __value__(X)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_20, __deriv__(A))
      OpenAD_lin_20 = (- SIN(__value__(A)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_18, OpenAD_aux_8)
      OpenAD_lin_18 = OpenAD_aux_8
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_21, OpenAD_aux_8)
      OpenAD_lin_21 = (OpenAD_aux_8 * OpenAD_aux_8 + INT(1_w2f__i8))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_19, OpenAD_aux_7)
      OpenAD_lin_19 = OpenAD_aux_7
      OpenAD_lin_15 = (5.0D-01 / OpenAD_aux_6)
      __value__(C(1 : INT(SIZE(X)))) = ((__value__(A(1 : INT(SIZE(X))))
     >  ** OpenAD_aux_5) - OpenAD_aux_6)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_acc_10, OpenAD_lin_13)
      OpenAD_acc_5 = (OpenAD_lin_15 * INT((-1_w2f__i8)))
      OpenAD_acc_6 = (OpenAD_lin_21 * OpenAD_lin_19 * OpenAD_acc_5)
      OpenAD_acc_7 = (OpenAD_lin_20 * OpenAD_lin_18 * OpenAD_acc_5)
      OpenAD_acc_8 = (OpenAD_lin_16 * OpenAD_acc_5)
      OpenAD_acc_9 = (OpenAD_lin_17 * OpenAD_acc_5)
      OpenAD_acc_10 = (OpenAD_lin_14 * OpenAD_lin_13)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_6)
      OpenAD_Symbol_13 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_13)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_7)
      OpenAD_Symbol_14 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_8)
      OpenAD_Symbol_15 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_15)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_9)
      OpenAD_Symbol_16 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_16)
      OpenAD_Symbol_17 = SIZE(OpenAD_lin_12, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_17)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_12)
      OpenAD_Symbol_19 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_19)
      OpenAD_Symbol_20 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_20)
      OpenAD_Symbol_21 = SIZE(OpenAD_acc_10, 1_w2f__i8)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_21)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_10)
      OpenAD_Symbol_23 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_23)
      OpenAD_Symbol_24 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_24)
      __value__(Y(1 : 2)) = (__value__(A(1 : INT(SIZE(X)))) + __value__
     > (C(1 : INT(SIZE(X)))) * 5.0D00)
      OpenAD_Symbol_25 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_25)
      OpenAD_Symbol_26 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_26)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_27)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(5.0D00, __deriv__(Y(1 : 2)), __deriv__(C(1 : INT(
     > OpenAD_Symbol_27))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_28)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 2)), __deriv__(A(1 : INT(
     > OpenAD_Symbol_28))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_30)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_29, OpenAD_Symbol_30)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_29)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_33)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_32, OpenAD_Symbol_33)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_32)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_35)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_36)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_37)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_38)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_39)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_40)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_29, __deriv__(C(1 : INT(OpenAD_Symbol_39
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_40))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_41)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_42)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_32, __deriv__(C(1 : INT(OpenAD_Symbol_41
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_42))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_43)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_35, __deriv__(C(1 : INT(OpenAD_Symbol_43
     > ))), __deriv__(A))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_44)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_36, __deriv__(C(1 : INT(OpenAD_Symbol_44
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_45)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_37, __deriv__(C(1 : INT(OpenAD_Symbol_45
     > ))), __deriv__(A))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_46)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_38, __deriv__(C(1 : INT(OpenAD_Symbol_46
     > ))), __deriv__(B))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(C(1 : INT(OpenAD_Symbol_46))))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_47)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_48)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_49)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(__deriv__(OpenAD_prp_0), __deriv__(B(1
     >  : INT(SIZE(X)))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_50)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_47, __deriv__(B(1 : INT(OpenAD_Symbol_50
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_51)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_52)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_48, __deriv__(B(1 : INT(OpenAD_Symbol_51
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_52))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_53)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_49, __deriv__(B(1 : INT(OpenAD_Symbol_53
     > ))), __deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(B(1 : INT(OpenAD_Symbol_53))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_54)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(B(1 : INT(
     > OpenAD_Symbol_54))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_55)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_57)
C     $OpenAD$ INLINE oad_AllocateShape(subst,subst)
      CALL oad_AllocateShape(OpenAD_Symbol_56, OpenAD_Symbol_57)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_56)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_59)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_60)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_61)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_55, __deriv__(B(1 : INT(OpenAD_Symbol_60
     > ))), __deriv__(A(1 : INT(OpenAD_Symbol_61))))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_62)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_56, __deriv__(B(1 : INT(OpenAD_Symbol_62
     > ))), __deriv__(X(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_63)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_59, __deriv__(B(1 : INT(OpenAD_Symbol_63
     > ))), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(B(1 : INT(OpenAD_Symbol_63))))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_64)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_65)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_66)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_64, __deriv__(A(1 : INT(OpenAD_Symbol_66
     > ))), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_67)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_65, __deriv__(A(1 : INT(OpenAD_Symbol_67
     > ))), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(A(1 : INT(OpenAD_Symbol_67))))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
