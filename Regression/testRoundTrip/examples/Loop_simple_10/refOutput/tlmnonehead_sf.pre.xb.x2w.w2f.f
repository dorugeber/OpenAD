
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
      SUBROUTINE foo(X, Y, A, J)
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
      INTEGER(w2f__i4) A(1 : 2, 1 : 2)
      INTEGER(w2f__i4) J
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      INTEGER(w2f__i4) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = __value__(X(1))
      CALL setderiv(__deriv__(Y(1)), __deriv__(X(1)))
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(A(I, J) .ne. 0) THEN
          OpenAD_aux_0 = (__value__(X(1)) * __value__(Y(1)))
          OpenAD_lin_1 = __value__(Y(1))
          OpenAD_lin_2 = __value__(X(1))
          OpenAD_lin_0 = A(I, J)
          __value__(Y(1)) = (A(I, J) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_1 * OpenAD_lin_0)
          OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
          CALL sax(OpenAD_acc_0, __deriv__(X(1)), __deriv__(Y(1)))
          CALL saxpy(OpenAD_acc_1, __deriv__(OpenAD_prp_0), __deriv__(Y
     > (1)))
        ELSE
          __value__(Y(1)) = 0.0
          CALL zero_deriv(__deriv__(Y(1)))
        ENDIF
      END DO
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
      INTEGER(w2f__i4) A(1 : 2, 1 : 2)
      EXTERNAL foo
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          A(I, J) = (I + J)
        END DO
      END DO
      CALL foo(__deriv__(X), __deriv__(Y), A, 2)
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          A(I, J) = 0
        END DO
      END DO
      END SUBROUTINE
