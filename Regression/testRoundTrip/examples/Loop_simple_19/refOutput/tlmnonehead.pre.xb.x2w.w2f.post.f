
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(A)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      A%v = (A%v*2.0D00)
      CALL setderiv(OpenAD_prp_0,A)
      CALL sax(2.0D00,OpenAD_prp_0,A)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
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
        K = I
        CALL foo(X(K))
        K = (-19)
      END DO
      Y(1:2)%v = X(1:2)%v
      CALL setderiv(Y(1:2),X(1:2))
      END SUBROUTINE
