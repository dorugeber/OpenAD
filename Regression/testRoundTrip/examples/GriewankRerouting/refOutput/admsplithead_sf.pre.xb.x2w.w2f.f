
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
      SUBROUTINE ad_reroute(X1, X2, Y1, Y2)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X1
      TYPE (OpenADTy_active) X2
      TYPE (OpenADTy_active) Y1
      TYPE (OpenADTy_active) Y2
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) T1
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) T3
      TYPE (OpenADTy_active) T4
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X1)
C$OPENAD INDEPENDENT(X2)
C$OPENAD DEPENDENT(Y1)
C$OPENAD DEPENDENT(Y2)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(T1) = __value__(X1)
      __value__(T2) = __value__(X2)
      __value__(T3) = (__value__(T1) / __value__(T2))
      __value__(T4) = (__value__(T1) * __value__(T2))
      __value__(Y1) = (__value__(T3) * __value__(T4))
      __value__(Y2) = (__value__(T3) / __value__(T4))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(T1) = __value__(X1)
      __value__(T2) = __value__(X2)
      OpenAD_lin_0 = (INT(1_w2f__i8) / __value__(T2))
      OpenAD_lin_1 = (-(__value__(T1) /(__value__(T2) * __value__(T2)))
     > )
      __value__(T3) = (__value__(T1) / __value__(T2))
      OpenAD_lin_2 = __value__(T2)
      OpenAD_lin_3 = __value__(T1)
      __value__(T4) = (__value__(T1) * __value__(T2))
      OpenAD_lin_4 = __value__(T4)
      OpenAD_lin_5 = __value__(T3)
      __value__(Y1) = (__value__(T3) * __value__(T4))
      OpenAD_lin_6 = (INT(1_w2f__i8) / __value__(T4))
      OpenAD_lin_7 = (-(__value__(T3) /(__value__(T4) * __value__(T4)))
     > )
      __value__(Y2) = (__value__(T3) / __value__(T4))
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_5 + OpenAD_lin_1 *
     >  OpenAD_lin_4)
      OpenAD_acc_1 = (OpenAD_lin_3 * OpenAD_lin_7 + OpenAD_lin_1 *
     >  OpenAD_lin_6)
      OpenAD_acc_2 = (OpenAD_lin_2 * OpenAD_lin_7 + OpenAD_lin_0 *
     >  OpenAD_lin_6)
      OpenAD_acc_3 = (OpenAD_lin_2 * OpenAD_lin_5 + OpenAD_lin_0 *
     >  OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y2), __deriv__(X1))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y2), __deriv__(X2))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y2))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(Y1), __deriv__(X1))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y1), __deriv__(X2))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y1))
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
      __value__(T1) = __value__(X1)
      __value__(T2) = __value__(X2)
      OpenAD_lin_0 = (INT(1_w2f__i8) / __value__(T2))
      OpenAD_lin_1 = (-(__value__(T1) /(__value__(T2) * __value__(T2)))
     > )
      __value__(T3) = (__value__(T1) / __value__(T2))
      OpenAD_lin_2 = __value__(T2)
      OpenAD_lin_3 = __value__(T1)
      __value__(T4) = (__value__(T1) * __value__(T2))
      OpenAD_lin_4 = __value__(T4)
      OpenAD_lin_5 = __value__(T3)
      __value__(Y1) = (__value__(T3) * __value__(T4))
      OpenAD_lin_6 = (INT(1_w2f__i8) / __value__(T4))
      OpenAD_lin_7 = (-(__value__(T3) /(__value__(T4) * __value__(T4)))
     > )
      __value__(Y2) = (__value__(T3) / __value__(T4))
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_5 + OpenAD_lin_1 *
     >  OpenAD_lin_4)
      OpenAD_acc_1 = (OpenAD_lin_3 * OpenAD_lin_7 + OpenAD_lin_1 *
     >  OpenAD_lin_6)
      OpenAD_acc_2 = (OpenAD_lin_2 * OpenAD_lin_7 + OpenAD_lin_0 *
     >  OpenAD_lin_6)
      OpenAD_acc_3 = (OpenAD_lin_2 * OpenAD_lin_5 + OpenAD_lin_0 *
     >  OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y2), __deriv__(X1))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y2), __deriv__(X2))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y2))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(Y1), __deriv__(X1))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y1), __deriv__(X2))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y1))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
