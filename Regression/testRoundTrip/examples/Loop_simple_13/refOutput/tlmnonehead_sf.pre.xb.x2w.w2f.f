
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
      TYPE (OpenADTy_active) S
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
        __value__(S) = __value__(X(I))
        CALL setderiv(__deriv__(S), __deriv__(X(I)))
        IF(__value__(S) .GT. 0.0D00) THEN
          __value__(S) = 0.0D00
          CALL zero_deriv(__deriv__(S))
        ENDIF
        __value__(Y(INT(I))) = (__value__(X(I)) + __value__(S))
        CALL setderiv(__deriv__(Y(I)), __deriv__(X(I)))
        CALL inc_deriv(__deriv__(Y(I)), __deriv__(S))
      END DO
      END SUBROUTINE
