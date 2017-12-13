
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
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: LOCALX(1:1)
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
      LOCALX(1)%v = (X(1)%v*2.0D00)
      CALL sax(2.0D00,X(1),LOCALX(1))
      OpenAD_lin_0 = LOCALX(1)%v
      OpenAD_lin_1 = LOCALX(1)%v
      Y(1)%v = (LOCALX(1)%v*LOCALX(1)%v)
      CALL sax(OpenAD_lin_0,LOCALX(1),Y(1))
      CALL saxpy(OpenAD_lin_1,LOCALX(1),Y(1))
      END SUBROUTINE
