
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
      MODULE aglobalmodule
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      CHARACTER(3) GLOBALSTRING
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X, Y)
      use w2f__types
      use oad_intrinsics
      use aglobalmodule
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      INTENT(IN)  X
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      CHARACTER(3) LOCALSTRING
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(GLOBALSTRING .EQ. 'yes') THEN
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(I)))
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
          CALL setderiv(__deriv__(Y(I)), __deriv__(OpenAD_prp_0))
          CALL saxpy(OpenAD_lin_0, __deriv__(X(I)), __deriv__(Y(I)))
          CALL saxpy(OpenAD_lin_1, __deriv__(X(I)), __deriv__(Y(I)))
        ENDIF
        IF(GLOBALSTRING .EQ. 'no') THEN
          OpenAD_aux_0 = (__value__(X(I)) * __value__(X(I)))
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + OpenAD_aux_0 *
     >  2.0D00)
          OpenAD_acc_0 = (OpenAD_lin_2 * 2.0D00)
          OpenAD_acc_1 = (OpenAD_lin_3 * 2.0D00)
          CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(Y(I)))
          CALL setderiv(__deriv__(Y(I)), __deriv__(OpenAD_prp_1))
          CALL saxpy(OpenAD_acc_0, __deriv__(X(I)), __deriv__(Y(I)))
          CALL saxpy(OpenAD_acc_1, __deriv__(X(I)), __deriv__(Y(I)))
        ENDIF
      END DO
      GLOBALSTRING = 'either'
      LOCALSTRING = GLOBALSTRING
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use aglobalmodule
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
      EXTERNAL foo
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      CALL zero_deriv(__deriv__(Y(1)))
      CALL zero_deriv(__deriv__(Y(2)))
      GLOBALSTRING = 'yes'
      CALL foo(__deriv__(X), __deriv__(Y))
      GLOBALSTRING = 'both'
      END SUBROUTINE
