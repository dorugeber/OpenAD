
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
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, 2, 1
        IF(I .eq. 1) THEN
          OpenAD_lin_0 = X(I)%v
          OpenAD_lin_1 = X(I)%v
          Y(INT(I))%v = (X(I)%v*X(I)%v)
          CALL sax(OpenAD_lin_0,X(I),Y(I))
          CALL saxpy(OpenAD_lin_1,X(I),Y(I))
        ELSE
          Y(INT(I))%v = (X(I)%v+X(I)%v)
          CALL setderiv(Y(I),X(I))
          CALL inc_deriv(Y(I),X(I))
        ENDIF
        X(INT(I))%v = 0.0
        CALL zero_deriv(X(INT(I)))
      END DO
      END SUBROUTINE
