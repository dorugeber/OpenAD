
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
      MODULE mytypemodule
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE MYTYPE
      SEQUENCE
        type(active) :: FIELD1
        type(active) :: FIELD2
      END TYPE

C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use mytypemodule
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      type(MYTYPE) :: TYPED_X
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
      TYPED_X%FIELD1%v = X(1)%v
      CALL setderiv(TYPED_X%FIELD1,X(1))
      TYPED_X%FIELD2%v = X(2)%v
      CALL setderiv(TYPED_X%FIELD2,X(2))
      OpenAD_lin_0 = TYPED_X%FIELD2%v
      OpenAD_lin_1 = TYPED_X%FIELD1%v
      Y(1)%v = (TYPED_X%FIELD1%v*TYPED_X%FIELD2%v)
      CALL sax(OpenAD_lin_0,TYPED_X%FIELD1,Y(1))
      CALL saxpy(OpenAD_lin_1,TYPED_X%FIELD2,Y(1))
      END SUBROUTINE
