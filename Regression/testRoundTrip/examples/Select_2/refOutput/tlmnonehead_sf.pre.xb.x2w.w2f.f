
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 3)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 3)
      INTENT(OUT)  Y
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
        IF ( select_expr_temp_0  .EQ.  1)  GO TO  19
        IF ( select_expr_temp_0  .EQ.  2)  GO TO  23
        GO TO 24
24      CONTINUE
        __value__(Y(INT(I))) = (__value__(X(I)) * 2.0D00)
        CALL sax(2.0D00, __deriv__(X(I)), __deriv__(Y(I)))
        GO TO 21
19      CONTINUE
        OpenAD_lin_0 = COS(__value__(X(I)))
        __value__(Y(INT(I))) = SIN(__value__(X(I)))
        CALL sax(OpenAD_lin_0, __deriv__(X(I)), __deriv__(Y(I)))
        GO TO 21
23      CONTINUE
        OpenAD_lin_1 = (- SIN(__value__(X(I))))
        __value__(Y(INT(I))) = COS(__value__(X(I)))
        CALL sax(OpenAD_lin_1, __deriv__(X(I)), __deriv__(Y(I)))
        GO TO 21
21      CONTINUE
      END DO
      END SUBROUTINE
