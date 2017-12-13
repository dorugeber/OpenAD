
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
      INTEGER(w2f__i4) OpenAD_aux_0
      INTEGER(w2f__i4) OpenAD_lin_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = FLOAT(6)
      OpenAD_lin_0 = OpenAD_aux_0
      Y(1)%v = (OpenAD_aux_0*X(1)%v)
      CALL sax(OpenAD_lin_0,X(1),Y(1))
      END SUBROUTINE
