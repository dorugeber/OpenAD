
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
      CALL sax(OpenAD_acc_0, __deriv__(X), __deriv__(A(1 : INT(SIZE(X))
     > )))
      CALL saxpy(OpenAD_acc_1, __deriv__(X), __deriv__(A(1 : INT(SIZE(X
     > )))))
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
      CALL sax(OpenAD_acc_2, __deriv__(X), __deriv__(B(1 : INT(SIZE(X))
     > )))
      CALL saxpy(OpenAD_lin_5, __deriv__(X(1 : 2)), __deriv__(B(1 : INT
     > (SIZE(X)))))
      CALL saxpy(OpenAD_lin_6, __deriv__(A(1 : INT(SIZE(X)))),
     >  __deriv__(B(1 : INT(SIZE(X)))))
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
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(__deriv__(OpenAD_prp_0), __deriv__(B(1
     >  : INT(SIZE(X)))))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(B(1 : INT(SIZE(X
     > )))))
      CALL sax(OpenAD_lin_7, __deriv__(OpenAD_prp_0), __deriv__(B(1 :
     >  INT(SIZE(X)))))
      CALL saxpy(OpenAD_acc_3, __deriv__(A(1 : INT(SIZE(X)))),
     >  __deriv__(B(1 : INT(SIZE(X)))))
      CALL saxpy(OpenAD_acc_4, __deriv__(X), __deriv__(B(1 : INT(SIZE(X
     > )))))
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
      CALL sax(OpenAD_acc_6, __deriv__(B), __deriv__(C(1 : INT(SIZE(X))
     > )))
      CALL saxpy(OpenAD_acc_7, __deriv__(A), __deriv__(C(1 : INT(SIZE(X
     > )))))
      CALL saxpy(OpenAD_acc_8, __deriv__(X), __deriv__(C(1 : INT(SIZE(X
     > )))))
      CALL saxpy(OpenAD_acc_9, __deriv__(A), __deriv__(C(1 : INT(SIZE(X
     > )))))
      CALL saxpy(OpenAD_lin_12, __deriv__(A(1 : INT(SIZE(X)))),
     >  __deriv__(C(1 : INT(SIZE(X)))))
      CALL saxpy(OpenAD_acc_10, __deriv__(A(1 : INT(SIZE(X)))),
     >  __deriv__(C(1 : INT(SIZE(X)))))
      __value__(Y(1 : 2)) = (__value__(A(1 : INT(SIZE(X)))) + __value__
     > (C(1 : INT(SIZE(X)))) * 5.0D00)
      CALL setderiv(__deriv__(Y(1 : 2)), __deriv__(A(1 : INT(SIZE(X))))
     > )
      CALL saxpy(5.0D00, __deriv__(C(1 : INT(SIZE(X)))), __deriv__(Y(1
     >  : 2)))
      END SUBROUTINE
