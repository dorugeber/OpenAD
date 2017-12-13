
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
      __value__(Y(1)) = 2.0
      CALL zero_deriv(__deriv__(Y(1)))
      IF(__value__(X(1)) .GT. 0.0D00) THEN
        __value__(Y(1)) = __value__(X(1))
        CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
      ENDIF
      OpenAD_lin_0 = __value__(Y(1))
      OpenAD_lin_1 = __value__(Y(1))
      __value__(Y(1)) = (__value__(Y(1)) * __value__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(1)))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(Y(1))
     > )
      END SUBROUTINE
