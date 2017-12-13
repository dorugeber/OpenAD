
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
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) T1
      TYPE (OpenADTy_active) T2
      TYPE (OpenADTy_active) T3
      TYPE (OpenADTy_active) T4
      TYPE (OpenADTy_active) T5
      TYPE (OpenADTy_active) T6
      TYPE (OpenADTy_active) T7
      TYPE (OpenADTy_active) Y1
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(T1) = __value__(X(1))
      __value__(T2) = (__value__(T1) + 2.0D00)
      __value__(T3) = (__value__(T2) * 5.0D-01)
      __value__(T4) = (__value__(T3) * 2.0D00)
      __value__(T5) = (__value__(T4) + 4.0D00)
      __value__(T6) = (__value__(T5) + 3.0D00)
      __value__(T7) = (__value__(T6) +(-9.0D00))
      OpenAD_aux_0 = EXP(__value__(T7))
      OpenAD_lin_0 = OpenAD_aux_0
      __value__(Y1) = OpenAD_aux_0
      __value__(Y(1)) = __value__(Y1)
      OpenAD_acc_0 = (1.0D00 * OpenAD_lin_0)
      CALL sax(OpenAD_acc_0, __deriv__(X(1)), __deriv__(Y(1)))
      END SUBROUTINE
