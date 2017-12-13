
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
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) A
      INTEGER(w2f__i4) B
      INTEGER(w2f__i4) I
      REAL(w2f__8) J
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A = 1
      B = 3
      DO I = A, B, 1
        J = 3.0D00
      END DO
      OpenAD_lin_0 = X(2)%v
      OpenAD_lin_1 = X(1)%v
      Y(1)%v = (X(1)%v*X(2)%v)
      CALL sax(OpenAD_lin_0,X(1),Y(1))
      CALL saxpy(OpenAD_lin_1,X(2),Y(1))
      END SUBROUTINE
