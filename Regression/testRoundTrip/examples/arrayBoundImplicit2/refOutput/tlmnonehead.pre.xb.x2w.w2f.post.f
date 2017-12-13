
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
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      type(active) :: P(:)
      ALLOCATABLE P
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      ALLOCATE(P(3))
      P(:2)%v = X(1:2)%v
      CALL setderiv(P(:2),X(1:2))
      Y(1:2)%v = P(:2)%v
      CALL setderiv(Y(1:2),P(:2))
      END SUBROUTINE
