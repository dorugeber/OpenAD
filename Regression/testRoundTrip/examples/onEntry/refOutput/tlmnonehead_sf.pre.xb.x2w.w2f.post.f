
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(K, A)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      type(active) :: A(1:K)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      DO I = 1, K, 1
        A(INT(I))%v = (A(I)%v*2.0D00)
        CALL setderiv(OpenAD_prp_0,A(I))
        CALL sax(2.0D00,OpenAD_prp_0,A(I))
      END DO
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
      DO I = 1, 2, 1
        K = 2
        CALL foo(K,X)
        K = (-19)
      END DO
      Y(1:2)%v = X(1:2)%v
      CALL setderiv(Y(1:2),X(1:2))
      END SUBROUTINE
