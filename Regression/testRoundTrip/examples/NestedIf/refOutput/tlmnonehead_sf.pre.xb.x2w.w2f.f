
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
      TYPE (OpenADTy_active) X(1 : 2)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 2)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF(__value__(X(1)) .LE. __value__(X(2))) THEN
        IF(__value__(X(1)) .eq. __value__(X(2))) THEN
          OpenAD_lin_0 = __value__(X(1))
          OpenAD_lin_1 = __value__(X(1))
          __value__(Y(1)) = (__value__(X(1)) * __value__(X(1)))
          CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(Y(1)))
          CALL saxpy(OpenAD_lin_1, __deriv__(X(1)), __deriv__(Y(1)))
        ELSE
          __value__(Y(1)) = (__value__(X(2)) - __value__(X(1)))
          CALL setderiv(__deriv__(Y(1)), __deriv__(X(2)))
          CALL dec_deriv(__deriv__(Y(1)), __deriv__(X(1)))
        ENDIF
      ELSE
        __value__(Y(1)) = (__value__(X(1)) - __value__(X(2)))
        CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
        CALL dec_deriv(__deriv__(Y(1)), __deriv__(X(2)))
      ENDIF
      __value__(Y(2)) = __value__(Y(1))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
      CALL setderiv(__deriv__(Y(2)), __deriv__(OpenAD_prp_0))
      END SUBROUTINE
