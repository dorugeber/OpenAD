
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
      REAL(w2f__8) OAD_CTMP1
      REAL(w2f__8) P1
      REAL(w2f__8) P2
      REAL(w2f__8) OpenAD_lin_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      P1 = 2.39999999999999991118D00
      P2 = (-3.0D+01)
      CALL OAD_S_MIN_D(X(1),X(2),OAD_CTMP0)
      CALL OAD_S_MAX_D(P1,P2,OAD_CTMP1)
      OpenAD_lin_0 = OAD_CTMP1
      Y(1)%v = (OAD_CTMP0%v*OAD_CTMP1)
      CALL sax(OpenAD_lin_0,OAD_CTMP0,Y(1))
      END SUBROUTINE
