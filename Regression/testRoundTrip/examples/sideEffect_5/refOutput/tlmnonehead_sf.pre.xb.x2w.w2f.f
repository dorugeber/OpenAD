
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
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) G
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(A, N)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) N
      TYPE (OpenADTy_active) A(1 : INT((N + G)))
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, (N + 1), 1
        __value__(A(INT(I))) = (__value__(A(I)) * 2.0D00)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(A(I)))
        CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(A(I)))
      END DO
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 3)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      G = 1
      I = 2
      CALL foo(__deriv__(X), 2)
      CALL foo(__deriv__(X), I)
      __value__(Y(1)) = 0
      CALL zero_deriv(__deriv__(Y(1)))
      DO I = 1, 3, 1
        __value__(Y(1)) = (__value__(X(I)) + __value__(Y(1)))
        CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
        CALL setderiv(__deriv__(Y(1)), __deriv__(X(I)))
        CALL inc_deriv(__deriv__(Y(1)), __deriv__(OpenAD_prp_1))
      END DO
      END SUBROUTINE
