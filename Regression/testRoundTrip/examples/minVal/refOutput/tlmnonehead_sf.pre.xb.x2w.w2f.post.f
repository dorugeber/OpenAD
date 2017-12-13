
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Top Level Pragmas ****
!
interface OAD_S_MAXVAL
  module procedure OAD_S_MAXVAL_D_1

end interface

!
!     **** Statements ****
!
CONTAINS

  SUBROUTINE OAD_S_MAXVAL_D_1(A, R)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  type(active) :: A(1:)
  INTENT(IN) A
  type(active) :: R
  INTENT(OUT) R
!
!       **** Local Variables and Functions ****
!
  INTEGER(w2f__i4) I(1 : 1)
!
!       **** Statements ****
!
  I(1:1) = MAXLOC(A%v)
  R%v = A(I(1))%v
  CALL setderiv(R,A(I(1)))
  END SUBROUTINE
END

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
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: OAD_CTMP0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL OAD_S_MAXVAL_D_1(X,OAD_CTMP0)
      Y(1)%v = OAD_CTMP0%v
      CALL setderiv(Y(1),OAD_CTMP0)
      END SUBROUTINE
