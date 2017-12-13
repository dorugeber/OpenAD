
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
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_3
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
      GO TO 4
4     CONTINUE
      OpenAD_lin_0 = __value__(X(1))
      OpenAD_lin_1 = __value__(X(1))
      __value__(X(1)) = (__value__(X(1)) * __value__(X(1)))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X(1)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(X(1)))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(X(1)))
      CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(X(1))
     > )
      GO TO 5
5     CONTINUE
      __value__(X(1)) = (__value__(X(1)) * 3.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(X(1)))
      CALL sax(3.0D00, __deriv__(OpenAD_prp_2), __deriv__(X(1)))
      GO TO 6
6     CONTINUE
      __value__(X(1)) = (__value__(X(1)) * 2.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_3), __deriv__(X(1)))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_3), __deriv__(X(1)))
      GO TO 7
7     CONTINUE
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      GO TO 8
8     CONTINUE
      GO TO 1
1     CONTINUE
      END SUBROUTINE
