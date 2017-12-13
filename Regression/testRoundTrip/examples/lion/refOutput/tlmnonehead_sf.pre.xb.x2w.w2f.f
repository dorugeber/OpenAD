
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
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) T1
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) Y1
      TYPE (OpenADTy_active) Y2
      TYPE (OpenADTy_active) Y3
      TYPE (OpenADTy_active) Y4
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = (INT(1_w2f__i8) / __value__(X(2)))
      OpenAD_lin_1 = (-(__value__(X(1)) /(__value__(X(2)) * __value__(X
     > (2)))))
      __value__(T1) = (__value__(X(1)) / __value__(X(2)))
      OpenAD_lin_2 = (- SIN(__value__(T1)))
      __value__(T2) = COS(__value__(T1))
      OpenAD_aux_0 = EXP(__value__(T2))
      OpenAD_lin_3 = OpenAD_aux_0
      __value__(Y1) = OpenAD_aux_0
      __value__(Y2) = (__value__(T2) * 3.14000010490417480469D00)
      OpenAD_lin_4 = COS(__value__(T2))
      __value__(Y3) = SIN(__value__(T2))
      OpenAD_lin_5 = __value__(T2)
      OpenAD_lin_6 = __value__(T1)
      __value__(Y4) = (__value__(T1) * __value__(T2))
      __value__(Y(1)) = __value__(Y1)
      OpenAD_acc_0 = (OpenAD_lin_5 + OpenAD_lin_2 * OpenAD_lin_6)
      OpenAD_acc_1 = (OpenAD_lin_2 * 3.14000010490417480469D00)
      OpenAD_acc_2 = (OpenAD_lin_2 * OpenAD_lin_4)
      OpenAD_acc_3 = (OpenAD_lin_2 * OpenAD_lin_3)
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(T1))
      CALL saxpy(OpenAD_lin_1, __deriv__(X(2)), __deriv__(T1))
      CALL sax(OpenAD_acc_1, __deriv__(T1), __deriv__(Y2))
      CALL sax(OpenAD_acc_2, __deriv__(T1), __deriv__(Y3))
      CALL sax(OpenAD_acc_0, __deriv__(T1), __deriv__(Y4))
      CALL sax(OpenAD_acc_3, __deriv__(T1), __deriv__(Y(1)))
      __value__(Y(2)) = __value__(Y2)
      CALL setderiv(__deriv__(Y(2)), __deriv__(Y2))
      __value__(Y(3)) = __value__(Y3)
      CALL setderiv(__deriv__(Y(3)), __deriv__(Y3))
      __value__(Y(4)) = __value__(Y4)
      CALL setderiv(__deriv__(Y(4)), __deriv__(Y4))
      END SUBROUTINE
