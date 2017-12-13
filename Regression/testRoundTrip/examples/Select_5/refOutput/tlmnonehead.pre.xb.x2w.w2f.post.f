
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
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) select_expr_temp_0
      INTEGER(w2f__i4) select_expr_temp_1
      INTEGER(w2f__i4) OpenAD_lin_0
      INTEGER(w2f__i4) OpenAD_lin_1
      INTEGER(w2f__i4) OpenAD_lin_2
      INTEGER(w2f__i4) OpenAD_lin_3
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
 2    CONTINUE
      GO TO 3
 3    CONTINUE
C$OPENAD XXX Template ad_template.f
      I = 2
      GO TO 4
 4    CONTINUE
      IF (X(1)%v.LT.1.0D00) THEN
        GO TO 6
      ELSE
        GO TO 5
      ENDIF
 5    CONTINUE
      GO TO 11
 6    CONTINUE
      select_expr_temp_0 = I
      GO TO 7
 7    CONTINUE
      IF (select_expr_temp_0.ne.0) THEN
        GO TO 18
      ELSE
        GO TO 8
      ENDIF
 8    CONTINUE
      OpenAD_lin_1 = I
      Y(1)%v = (I*X(1)%v)
      CALL sax(OpenAD_lin_1,X(1),Y(1))
      GO TO 9
 9    CONTINUE
      GO TO 10
 10   CONTINUE
      GO TO 11
 11   CONTINUE
      GO TO 12
 12   CONTINUE
      Y(1)%v = (Y(1)%v*4.0D00)
      CALL setderiv(OpenAD_prp_0,Y(1))
      CALL sax(4.0D00,OpenAD_prp_0,Y(1))
      select_expr_temp_1 = I
      GO TO 13
 13   CONTINUE
      IF (select_expr_temp_1.ne.0) THEN
        GO TO 17
      ELSE
        GO TO 14
      ENDIF
 14   CONTINUE
      OpenAD_lin_3 = I
      Y(1)%v = (I*Y(1)%v)
      CALL setderiv(OpenAD_prp_2,Y(1))
      CALL sax(OpenAD_lin_3,OpenAD_prp_2,Y(1))
      GO TO 15
 15   CONTINUE
      GO TO 16
 16   CONTINUE
      GO TO 19
 17   CONTINUE
      OpenAD_lin_2 = I
      Y(1)%v = (I*Y(1)%v)
      CALL setderiv(OpenAD_prp_1,Y(1))
      CALL sax(OpenAD_lin_2,OpenAD_prp_1,Y(1))
      RETURN
      GO TO 19
 18   CONTINUE
      OpenAD_lin_0 = I
      Y(1)%v = (I*X(1)%v)
      CALL sax(OpenAD_lin_0,X(1),Y(1))
      GO TO 1
      GO TO 19
 19   CONTINUE
      GO TO 1
 1    CONTINUE
      END SUBROUTINE
