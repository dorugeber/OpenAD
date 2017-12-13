
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
      SUBROUTINE foo(X, T)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
      INTENT(IN)  X
      TYPE (OpenADTy_active) T
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      OpenAD_lin_0 = __value__(T)
      OpenAD_lin_1 = __value__(X)
      __value__(T) = (__value__(X) * __value__(T))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(T))
      CALL sax(OpenAD_lin_0, __deriv__(X), __deriv__(T))
      CALL saxpy(OpenAD_lin_1, __deriv__(OpenAD_prp_0), __deriv__(T))
      END SUBROUTINE

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
      EXTERNAL foo
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) T
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
      __value__(T) = __value__(X(I))
      CALL setderiv(__deriv__(T), __deriv__(X(I)))
      CALL foo(__deriv__(X(I)), __deriv__(T))
      __value__(Y(INT(I))) = __value__(T)
      CALL setderiv(__deriv__(Y(I)), __deriv__(T))
      END SUBROUTINE
