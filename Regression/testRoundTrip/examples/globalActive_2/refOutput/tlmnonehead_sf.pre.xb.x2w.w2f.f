
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
      MODULE globals
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) AGLOBAL
      REAL(w2f__8) APASSIVEGLOBAL
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      APASSIVEGLOBAL = 2.0D00
      OpenAD_lin_0 = __value__(X(2))
      OpenAD_lin_1 = __value__(X(1))
      __value__(AGLOBAL) = (__value__(X(1)) * __value__(X(2)))
      OpenAD_lin_2 = APASSIVEGLOBAL
      __value__(Y(1)) = (__value__(AGLOBAL) * APASSIVEGLOBAL)
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(AGLOBAL))
      CALL saxpy(OpenAD_lin_1, __deriv__(X(2)), __deriv__(AGLOBAL))
      CALL sax(OpenAD_lin_2, __deriv__(AGLOBAL), __deriv__(Y(1)))
      END SUBROUTINE
