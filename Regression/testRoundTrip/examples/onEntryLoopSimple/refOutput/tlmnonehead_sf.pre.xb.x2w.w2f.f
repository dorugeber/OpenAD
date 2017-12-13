
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
      SUBROUTINE foo(K, A)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) A(1 : K)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      DO I = 1, K, 1
        __value__(A(INT(I))) = (__value__(A(I)) * 2.0D00)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(A(I)))
        CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(A(I)))
      END DO
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 2)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) K
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        K = 2
        CALL foo(K, __deriv__(X))
        K = (-19)
      END DO
      __value__(Y(1 : 2)) = __value__(X(1 : 2))
      CALL setderiv(__deriv__(Y(1 : 2)), __deriv__(X(1 : 2)))
      END SUBROUTINE
