
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo()
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
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
      type(active) :: Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      type(active) :: T
      type(active) :: T1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T%v = (X(1)%v+X(2)%v)
      T1%v = (T%v*2.0D00)
      CALL setderiv(T,X(1))
      CALL inc_deriv(T,X(2))
      CALL sax(2.0D00,T,T1)
      CALL foo()
      Y%v = (T%v*2.0D00+T1%v*3.0D00)
      CALL sax(2.0D00,T,Y)
      CALL saxpy(3.0D00,T1,Y)
      END SUBROUTINE
