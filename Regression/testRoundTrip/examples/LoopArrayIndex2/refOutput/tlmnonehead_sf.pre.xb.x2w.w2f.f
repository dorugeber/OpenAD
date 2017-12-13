
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
      SUBROUTINE bar(K, A, P)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) A
      REAL(w2f__8) P
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      INTEGER(w2f__i4) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = (__value__(A) * P)
      OpenAD_lin_1 = P
      OpenAD_lin_0 = K
      __value__(A) = (K * OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_1 * OpenAD_lin_0)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(A))
      CALL sax(OpenAD_acc_0, __deriv__(OpenAD_prp_0), __deriv__(A))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      INTEGER(w2f__i4) J
      REAL(w2f__8) P(1 : 2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      P(1) = 3.0D00
      P(2) = 6.0D00
      DO J = 1, 2, 1
        CALL bar(J, __deriv__(X), P(J))
      END DO
      __value__(Y) = __value__(X)
      CALL setderiv(__deriv__(Y), __deriv__(X))
      END SUBROUTINE
