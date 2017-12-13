
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
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
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
      I = 1
      __value__(Y(1)) = 0.0
      __value__(Y(2)) = 0.0
      CALL zero_deriv(__deriv__(Y(1)))
      CALL zero_deriv(__deriv__(Y(2)))
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        J = 1
        IF(J .LT. 2) THEN
          OpenAD_lin_0 = __value__(X(J))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(J)))
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
          CALL setderiv(__deriv__(Y(I)), __deriv__(OpenAD_prp_0))
          CALL saxpy(OpenAD_lin_0, __deriv__(X(I)), __deriv__(Y(I)))
          CALL saxpy(OpenAD_lin_1, __deriv__(X(J)), __deriv__(Y(I)))
          J = (J + 1)
        ENDIF
      END DO
      END SUBROUTINE
