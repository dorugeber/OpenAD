
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
      SUBROUTINE head(X1, X2, Y1, Y2)
      use w2f__types
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
      TYPE (OpenADTy_active) V3
      TYPE (OpenADTy_active) V4
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
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
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = __value__(X2)
      OpenAD_lin_1 = __value__(X1)
      __value__(V3) = (__value__(X1) * __value__(X2))
      OpenAD_lin_2 = __value__(V3)
      OpenAD_lin_3 = __value__(X1)
      __value__(V4) = (__value__(X1) * __value__(V3))
      OpenAD_aux_0 = (__value__(X2) * __value__(V3))
      OpenAD_lin_4 = OpenAD_aux_0
      OpenAD_lin_6 = __value__(V3)
      OpenAD_lin_7 = __value__(X2)
      OpenAD_lin_5 = __value__(V4)
      __value__(Y1) = (__value__(V4) * OpenAD_aux_0)
      OpenAD_lin_8 = COS(__value__(V4))
      __value__(Y2) = SIN(__value__(V4))
      OpenAD_acc_0 = (OpenAD_lin_6 * OpenAD_lin_5)
      OpenAD_acc_1 = (OpenAD_lin_7 * OpenAD_lin_5 + OpenAD_lin_3 *
     >  OpenAD_lin_4)
      OpenAD_acc_2 = (OpenAD_lin_3 * OpenAD_lin_8)
      OpenAD_acc_3 = (OpenAD_lin_2 * OpenAD_lin_4)
      OpenAD_acc_4 = (OpenAD_lin_2 * OpenAD_lin_8)
      CALL sax(OpenAD_lin_0, __deriv__(X1), __deriv__(V3))
      CALL saxpy(OpenAD_lin_1, __deriv__(X2), __deriv__(V3))
      CALL sax(OpenAD_acc_2, __deriv__(V3), __deriv__(Y2))
      CALL saxpy(OpenAD_acc_4, __deriv__(X1), __deriv__(Y2))
      CALL sax(OpenAD_acc_0, __deriv__(X2), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_1, __deriv__(V3), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_3, __deriv__(X1), __deriv__(Y1))
      END SUBROUTINE
