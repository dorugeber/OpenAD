
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
      type(active) :: T1
      type(active) :: T2
      type(active) :: Y1
      type(active) :: Y2
      type(active) :: Y3
      type(active) :: Y4
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T1%v = (X(1)%v+X(2)%v)
      T2%v = (-T1%v)
      Y1%v = T2%v
      Y2%v = (3.14000010490417480469D00-T2%v)
      Y3%v = T2%v
      Y4%v = (T1%v+T2%v)
      Y(1)%v = Y1%v
      CALL setderiv(T1,X(1))
      CALL inc_deriv(T1,X(2))
      CALL setderiv(Y2,T1)
      CALL set_neg_deriv(Y3,T1)
      CALL set_neg_deriv(Y(1),T1)
      Y(2)%v = Y2%v
      CALL setderiv(Y(2),Y2)
      Y(3)%v = Y3%v
      CALL setderiv(Y(3),Y3)
      Y(4)%v = Y4%v
      CALL setderiv(Y(4),Y4)
      END SUBROUTINE
