
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
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      I = 1
      __value__(Y(1)) = 2.0
      CALL zero_deriv(__deriv__(Y(1)))
      select_expr_temp_0 = I
      IF ( select_expr_temp_0  .EQ.  1)  GO TO  4
      IF ( select_expr_temp_0  .EQ.  2)  GO TO  8
      GO TO 9
9     CONTINUE
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      GO TO 6
4     CONTINUE
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      GO TO 6
8     CONTINUE
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      GO TO 6
6     CONTINUE
      OpenAD_lin_0 = __value__(Y(1))
      OpenAD_lin_1 = __value__(Y(1))
      __value__(Y(1)) = (__value__(Y(1)) * __value__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(Y(1))
     > )
      END SUBROUTINE
