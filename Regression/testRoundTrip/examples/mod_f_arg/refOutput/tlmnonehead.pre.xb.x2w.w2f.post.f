
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
      type(active) :: OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      X(1)%v = (X(1)%v*1.0D+01)
      CALL setderiv(OpenAD_prp_0,X(1))
      CALL sax(1.0D+01,OpenAD_prp_0,X(1))
      Y(1)%v = MOD(X(1)%v,2.0D00)
      CALL setderiv(Y(1),X(1))
      END SUBROUTINE
