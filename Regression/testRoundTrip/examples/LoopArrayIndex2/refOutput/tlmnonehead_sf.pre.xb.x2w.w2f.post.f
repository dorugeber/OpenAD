
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE bar(K, A, P)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      type(active) :: A
      REAL(w2f__8) P
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      INTEGER(w2f__i4) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = (A%v*P)
      OpenAD_lin_1 = P
      OpenAD_lin_0 = K
      A%v = (K*OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_1*OpenAD_lin_0)
      CALL setderiv(OpenAD_prp_0,A)
      CALL sax(OpenAD_acc_0,OpenAD_prp_0,A)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
      type(active) :: Y
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
        CALL bar(J,X,P(J))
      END DO
      Y%v = X%v
      CALL setderiv(Y,X)
      END SUBROUTINE
