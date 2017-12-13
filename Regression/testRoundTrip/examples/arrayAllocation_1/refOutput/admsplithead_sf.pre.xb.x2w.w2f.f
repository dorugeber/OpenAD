
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
      TYPE (OpenADTy_active) Z(1 : INT(SIZE(__value__(X))))
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_3(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_4(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_5(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_Symbol_7(1 : 2)
      REAL(w2f__8) OpenAD_Symbol_8(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_acc_1(1 : 2)
      REAL(w2f__8) OpenAD_acc_2(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_1(1 : 2)
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(:)
      ALLOCATABLE OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_3(1 : 2)
      REAL(w2f__8) OpenAD_lin_4(1 : 2)
      REAL(w2f__8) OpenAD_lin_5(:)
      ALLOCATABLE OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6(1 : 2)
      TYPE (OpenADTy_active) OpenAD_prp_0(1 : 2)
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
      __value__(Z(1 : INT(SIZE(X)))) = __value__(X(1 : 2))
      __value__(Y(1 : 2)) = (__value__(Z(1 : INT(SIZE(X)))) * COS(
     > __value__(X)))
      __value__(Y(1 : 2)) = ((__value__(X(1 : 2)) * __value__(Z(1 : INT
     > (SIZE(X))))) / __value__(Y(1 : 2)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Z(1 : INT(SIZE(X)))) = __value__(X(1 : 2))
      OpenAD_Symbol_0 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
      OpenAD_aux_0 = COS(__value__(X))
      OpenAD_lin_0 = OpenAD_aux_0
      OpenAD_lin_2 = (- SIN(__value__(X)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_1 = __value__(Z(1 : INT(SIZE(X))))
      __value__(Y(1 : 2)) = (__value__(Z(1 : INT(SIZE(X)))) *
     >  OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_0)
      OpenAD_Symbol_1 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_0)
      OpenAD_aux_1 = (__value__(X(1 : 2)) * __value__(Z(1 : INT(SIZE(X)
     > ))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_5 = __value__(Z(1 : INT(SIZE(X))))
      OpenAD_lin_6 = __value__(X(1 : 2))
      OpenAD_lin_3 = (INT(1_w2f__i8) / __value__(Y(1 : 2)))
      OpenAD_lin_4 = (-(OpenAD_aux_1 /(__value__(Y(1 : 2)) * __value__(
     > Y(1 : 2)))))
      __value__(Y(1 : 2)) = (OpenAD_aux_1 / __value__(Y(1 : 2)))
      OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
      OpenAD_acc_2 = (OpenAD_lin_6 * OpenAD_lin_3)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_2)
      OpenAD_Symbol_2 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_4)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_5)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y(1 : 2)), __deriv__(Z(1 :
     >  INT(OpenAD_Symbol_6))))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y(1 : 2)), __deriv__(X(1 :
     >  2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y(1 : 2)), __deriv__(
     > OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_7)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_8)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(Y(1 : 2)), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(Y(1 : 2)), __deriv__(Z(1 :
     >  INT(OpenAD_Symbol_9))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_10)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Z(1 : INT(OpenAD_Symbol_10))), __deriv__(
     > X(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Z(1 : INT(OpenAD_Symbol_10))))
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
      __value__(Z(1 : INT(SIZE(X)))) = __value__(X(1 : 2))
      OpenAD_Symbol_0 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_0)
      OpenAD_aux_0 = COS(__value__(X))
      OpenAD_lin_0 = OpenAD_aux_0
      OpenAD_lin_2 = (- SIN(__value__(X)))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_1 = __value__(Z(1 : INT(SIZE(X))))
      __value__(Y(1 : 2)) = (__value__(Z(1 : INT(SIZE(X)))) *
     >  OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_0)
      OpenAD_Symbol_1 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_0)
      OpenAD_aux_1 = (__value__(X(1 : 2)) * __value__(Z(1 : INT(SIZE(X)
     > ))))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5, __deriv__(Z(1 : INT(SIZE(
     > X)))))
      OpenAD_lin_5 = __value__(Z(1 : INT(SIZE(X))))
      OpenAD_lin_6 = __value__(X(1 : 2))
      OpenAD_lin_3 = (INT(1_w2f__i8) / __value__(Y(1 : 2)))
      OpenAD_lin_4 = (-(OpenAD_aux_1 /(__value__(Y(1 : 2)) * __value__(
     > Y(1 : 2)))))
      __value__(Y(1 : 2)) = (OpenAD_aux_1 / __value__(Y(1 : 2)))
      OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
      OpenAD_acc_2 = (OpenAD_lin_6 * OpenAD_lin_3)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s1(subst)
      CALL push_s1(OpenAD_acc_2)
      OpenAD_Symbol_2 = SIZE(X)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_4)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_5)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y(1 : 2)), __deriv__(Z(1 :
     >  INT(OpenAD_Symbol_6))))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y(1 : 2)), __deriv__(X(1 :
     >  2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y(1 : 2)), __deriv__(
     > OpenAD_prp_0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_7)
C     $OpenAD$ INLINE pop_s1(subst)
      CALL pop_s1(OpenAD_Symbol_8)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(Y(1 : 2)), __deriv__(X))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(Y(1 : 2)), __deriv__(Z(1 :
     >  INT(OpenAD_Symbol_9))))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_10)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Z(1 : INT(OpenAD_Symbol_10))), __deriv__(
     > X(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Z(1 : INT(OpenAD_Symbol_10))))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
