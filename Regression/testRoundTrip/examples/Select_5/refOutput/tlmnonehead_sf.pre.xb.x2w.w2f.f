
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
      INTEGER(w2f__i4) select_expr_temp_1
      INTEGER(w2f__i4) OpenAD_lin_0
      INTEGER(w2f__i4) OpenAD_lin_1
      INTEGER(w2f__i4) OpenAD_lin_2
      INTEGER(w2f__i4) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
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
        GO TO 6
      ELSE
        GO TO 5
      ENDIF
5     CONTINUE
      GO TO 11
6     CONTINUE
      select_expr_temp_0 = I
      GO TO 7
7     CONTINUE
      IF(select_expr_temp_0 .ne. 0) THEN
        GO TO 18
      ELSE
        GO TO 8
      ENDIF
8     CONTINUE
      OpenAD_lin_1 = I
      __value__(Y(1)) = (I * __value__(X(1)))
      CALL sax(OpenAD_lin_1, __deriv__(X(1)), __deriv__(Y(1)))
      GO TO 9
9     CONTINUE
      GO TO 10
10    CONTINUE
      GO TO 11
11    CONTINUE
      GO TO 12
12    CONTINUE
      __value__(Y(1)) = (__value__(Y(1)) * 4.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL sax(4.0D00, __deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      select_expr_temp_1 = I
      GO TO 13
13    CONTINUE
      IF(select_expr_temp_1 .ne. 0) THEN
        GO TO 17
      ELSE
        GO TO 14
      ENDIF
14    CONTINUE
      OpenAD_lin_3 = I
      __value__(Y(1)) = (I * __value__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(Y(1)))
      CALL sax(OpenAD_lin_3, __deriv__(OpenAD_prp_2), __deriv__(Y(1)))
      GO TO 15
15    CONTINUE
      GO TO 16
16    CONTINUE
      GO TO 19
17    CONTINUE
      OpenAD_lin_2 = I
      __value__(Y(1)) = (I * __value__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      CALL sax(OpenAD_lin_2, __deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      RETURN
      GO TO 19
18    CONTINUE
      OpenAD_lin_0 = I
      __value__(Y(1)) = (I * __value__(X(1)))
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(Y(1)))
      GO TO 1
      GO TO 19
19    CONTINUE
      GO TO 1
1     CONTINUE
      END SUBROUTINE
