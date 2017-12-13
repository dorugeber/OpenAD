
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
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
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
      OpenAD_lin_0 = X(2)%v
      OpenAD_lin_1 = X(1)%v
      AGLOBAL%v = (X(1)%v*X(2)%v)
      Y%v = AGLOBAL%v
      CALL sax(OpenAD_lin_0,X(1),AGLOBAL)
      CALL saxpy(OpenAD_lin_1,X(2),AGLOBAL)
      CALL setderiv(Y,AGLOBAL)
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
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      EXTERNAL foo
      type(active) :: P
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL foo(X,Y(1))
      CALL bar(P)
      Y(2)%v = P%v
      CALL setderiv(Y(2),P)
      END SUBROUTINE

      SUBROUTINE bar(AP)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: AP
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_2
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_2 = (3*(AGLOBAL%v**(3-INT(1_w2f__i8))))
      AP%v = (AGLOBAL%v**3)
      CALL sax(OpenAD_lin_2,AGLOBAL,AP)
      END SUBROUTINE
