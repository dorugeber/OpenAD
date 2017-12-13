
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:3)
      INTENT(IN) X
      type(active) :: Y(1:3)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) select_expr_temp_0
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
      DO I = 1, 3, 1
        select_expr_temp_0 = I
        IF ( select_expr_temp_0 .EQ. 1) GO TO 19
        IF ( select_expr_temp_0 .EQ. 2) GO TO 23
        GO TO 24
 24     CONTINUE
        Y(INT(I))%v = (X(I)%v*2.0D00)
        CALL sax(2.0D00,X(I),Y(I))
        GO TO 21
 19     CONTINUE
        OpenAD_lin_0 = COS(X(I)%v)
        Y(INT(I))%v = SIN(X(I)%v)
        CALL sax(OpenAD_lin_0,X(I),Y(I))
        GO TO 21
 23     CONTINUE
        OpenAD_lin_1 = (-SIN(X(I)%v))
        Y(INT(I))%v = COS(X(I)%v)
        CALL sax(OpenAD_lin_1,X(I),Y(I))
        GO TO 21
 21     CONTINUE
      END DO
      END SUBROUTINE
