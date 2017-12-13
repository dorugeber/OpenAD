
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
      REAL(w2f__8) DEG2RAD
      PARAMETER ( DEG2RAD = 1.74532925199432954744D-02)
      REAL(w2f__8) PI
      PARAMETER ( PI = 3.141592653589793116D00)
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
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
      OpenAD_aux_0 = (X(1)%v*1.74532925199432954744D-02)
      OpenAD_lin_0 = (-SIN(OpenAD_aux_0))
      Y(1)%v = COS(OpenAD_aux_0)
      OpenAD_acc_0 = (1.74532925199432954744D-02*OpenAD_lin_0)
      CALL sax(OpenAD_acc_0,X(1),Y(1))
      END SUBROUTINE
