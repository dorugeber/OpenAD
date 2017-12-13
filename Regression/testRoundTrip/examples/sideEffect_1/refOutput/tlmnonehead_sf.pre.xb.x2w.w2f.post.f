
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
      MODULE globals
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: AGLOBAL
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = X(1)%v
      OpenAD_lin_1 = X(1)%v
      Y%v = (X(1)%v*X(1)%v)
      CALL sax(OpenAD_lin_0,X(1),Y)
      CALL saxpy(OpenAD_lin_1,X(1),Y)
      END SUBROUTINE

      SUBROUTINE bar(X)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_2 = AGLOBAL%v
      OpenAD_lin_3 = X%v
      X%v = (X%v*AGLOBAL%v)
      CALL setderiv(OpenAD_prp_0,X)
      CALL sax(OpenAD_lin_2,OpenAD_prp_0,X)
      CALL saxpy(OpenAD_lin_3,AGLOBAL,X)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
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
      AGLOBAL%v = 2.0D00
      CALL zero_deriv(AGLOBAL)
      CALL bar(X(1))
      CALL foo(X,AGLOBAL)
      Y(1)%v = AGLOBAL%v
      CALL setderiv(Y(1),AGLOBAL)
      END SUBROUTINE
