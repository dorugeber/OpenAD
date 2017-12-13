
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
      INTEGER(w2f__i4) A(1 : 4)
      EXTERNAL foo
      INTEGER(w2f__i4) I
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
      A(1) = 2
      A(2) = 1
      A(3) = 2
      A(4) = 3
      I = 1
      CALL foo(X(A(A(A(A(A(A(A(A(A(A(A(4)))))))))))))
      OpenAD_lin_0 = X(2)%v
      OpenAD_lin_1 = X(1)%v
      Y%v = (X(1)%v*X(2)%v)
      CALL sax(OpenAD_lin_0,X(1),Y)
      CALL saxpy(OpenAD_lin_1,X(2),Y)
      END SUBROUTINE
