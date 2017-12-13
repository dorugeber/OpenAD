
C$OPENAD XXX File_start [head.f]
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
      type(active) :: T1
      type(active) :: T2
      type(active) :: Y1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T1%v = (X(1)%v+X(1)%v)
      T2%v = (T1%v*2.0D00)
      Y1%v = T2%v
      Y(1)%v = Y1%v
      CALL setderiv(T1,X(1))
      CALL inc_deriv(T1,X(1))
      CALL sax(2.0D00,T1,Y(1))
      END SUBROUTINE
