
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
      TYPE (OpenADTy_active) X(1 : 1)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 1)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) select_expr_temp_0
      INTEGER(w2f__i4) OpenAD_lin_0
      INTEGER(w2f__i4) OpenAD_lin_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
2     CONTINUE
      GO TO 3
3     CONTINUE
C$OPENAD XXX Template ad_template.f
      I = 2
      GO TO 4
4     CONTINUE
      IF(__value__(X(1)) .LT. 1.0D00) THEN
        GO TO 8
      ELSE
        GO TO 5
      ENDIF
5     CONTINUE
      GO TO 6
6     CONTINUE
      GO TO 7
7     CONTINUE
      __value__(Y(1)) = (__value__(X(1)) * 3.0D00)
      CALL sax(3.0D00, __deriv__(X(1)), __deriv__(Y(1)))
      GO TO 12
8     CONTINUE
      select_expr_temp_0 = I
      GO TO 9
9     CONTINUE
      IF(select_expr_temp_0 .ne. 0) THEN
        GO TO 11
      ELSE
        GO TO 10
      ENDIF
10    CONTINUE
      OpenAD_lin_1 = I
      __value__(Y(1)) = (I * __value__(X(1)))
      CALL sax(OpenAD_lin_1, __deriv__(X(1)), __deriv__(Y(1)))
      GO TO 1
      GO TO 12
11    CONTINUE
      OpenAD_lin_0 = I
      __value__(Y(1)) = (I * __value__(X(1)))
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(Y(1)))
      GO TO 1
      GO TO 12
12    CONTINUE
      GO TO 1
1     CONTINUE
      END SUBROUTINE
