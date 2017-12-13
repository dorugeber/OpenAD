
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
      SUBROUTINE foo(A)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A(1:1,1:1,1:1,1:1,1:1,1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A(1,1,1,1,1,1)%v = (A(1,1,1,1,1,1)%v*2.0D00)
      CALL setderiv(OpenAD_prp_0,A(1,1,1,1,1,1))
      CALL sax(2.0D00,OpenAD_prp_0,A(1,1,1,1,1,1))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      type(active) :: Z(1:1,1:1,1:1,1:1,1:1,1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Z(1,1,1,1,1,1)%v = X(1)%v
      CALL setderiv(Z(1,1,1,1,1,1),X(1))
      CALL foo(Z)
      Y(1)%v = Z(1,1,1,1,1,1)%v
      CALL setderiv(Y(1),Z(1,1,1,1,1,1))
      END SUBROUTINE
