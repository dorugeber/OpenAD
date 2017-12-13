
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
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = EXP(__value__(X) * 5.0D-01)
      OpenAD_lin_0 = OpenAD_aux_0
      __value__(Y(1 : 2)) = OpenAD_aux_0
      OpenAD_acc_0 = (5.0D-01 * OpenAD_lin_0)
      CALL sax(OpenAD_acc_0, __deriv__(X), __deriv__(Y(1 : 2)))
      END SUBROUTINE
