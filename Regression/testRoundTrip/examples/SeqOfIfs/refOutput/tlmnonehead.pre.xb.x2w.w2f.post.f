
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
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
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
      IF (X(1)%v.LE.X(2)%v) THEN
        Y(1)%v = (X(2)%v-X(1)%v)
        CALL setderiv(Y(1),X(2))
        CALL dec_deriv(Y(1),X(1))
      ELSE
        Y(1)%v = (X(1)%v-X(2)%v)
        CALL setderiv(Y(1),X(1))
        CALL dec_deriv(Y(1),X(2))
      ENDIF
      IF (Y(1)%v.eq.0.0D00) THEN
        Y(2)%v = X(1)%v
        CALL setderiv(Y(2),X(1))
      ELSE
        Y(2)%v = Y(1)%v
        CALL setderiv(OpenAD_prp_0,Y(1))
        CALL setderiv(Y(2),OpenAD_prp_0)
      ENDIF
      END SUBROUTINE
