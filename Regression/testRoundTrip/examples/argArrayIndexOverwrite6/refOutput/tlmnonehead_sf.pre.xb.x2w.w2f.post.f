
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
      SUBROUTINE foo(A)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A%v = (A%v*2.0D00)
      CALL setderiv(OpenAD_prp_0,A)
      CALL sax(2.0D00,OpenAD_prp_0,A)
      END SUBROUTINE

      SUBROUTINE bar(A, K)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) OpenAD_lin_0
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      K = (K * 2)
      OpenAD_lin_0 = K
      A%v = (K*A%v)
      CALL setderiv(OpenAD_prp_1,A)
      CALL sax(OpenAD_lin_0,OpenAD_prp_1,A)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      EXTERNAL foo
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
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
      CALL foo(X(I))
      CALL bar(X(I+1),I)
      OpenAD_lin_1 = X(2)%v
      OpenAD_lin_2 = X(1)%v
      Y%v = (X(1)%v*X(2)%v)
      CALL sax(OpenAD_lin_1,X(1),Y)
      CALL saxpy(OpenAD_lin_2,X(2),Y)
      END SUBROUTINE
