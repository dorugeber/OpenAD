
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
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:4)
C
C     **** Local Variables and Functions ****
C
      type(active) :: A(1:2,1:2)
      type(active) :: B(1:4)
      INTEGER(w2f__i4) t__1(1 : 1)
      SAVE t__1
C
C     **** Initializers ****
C
      DATA t__1 / 4 /
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A(1:2,1:2)%v = 0.0
      A(1,1)%v = X(1)%v
      CALL zero_deriv(A(1:2,1:2))
      CALL setderiv(A(1,1),X(1))
      A(1,2)%v = X(2)%v
      CALL setderiv(A(1,2),X(2))
      B(1:4)%v = RESHAPE(A%v,t__1)
      B(1:4)%d = RESHAPE(A%d,t__1)
      Y(1:4)%v = B(1:4)%v
      CALL setderiv(Y(1:4),B(1:4))
      END SUBROUTINE
