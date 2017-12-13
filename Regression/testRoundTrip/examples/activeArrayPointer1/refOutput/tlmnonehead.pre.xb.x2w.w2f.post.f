
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use w2f__types
      use OAD_active
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      TARGET X
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) P(:)
      POINTER P
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      P => X(1:2)%v
      Y(1:2)%v = (P(:)+X(1:2)%v)
      CALL setderiv(Y(1:2),X(1:2))
      END SUBROUTINE
