
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
      MODULE aglobalmodule
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      CHARACTER(3) GLOBALSTRING
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use aglobalmodule
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      INTENT(IN) X
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      CHARACTER(3) LOCALSTRING
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(GLOBALSTRING .EQ. 'yes') THEN
          OpenAD_lin_0 = X(I)%v
          OpenAD_lin_1 = X(I)%v
          Y(INT(I))%v = (Y(I)%v+X(I)%v*X(I)%v)
          CALL setderiv(OpenAD_prp_0,Y(I))
          CALL setderiv(Y(I),OpenAD_prp_0)
          CALL saxpy(OpenAD_lin_0,X(I),Y(I))
          CALL saxpy(OpenAD_lin_1,X(I),Y(I))
        ENDIF
        IF (GLOBALSTRING.EQ.'no') THEN
          OpenAD_aux_0 = (X(I)%v*X(I)%v)
          OpenAD_lin_2 = X(I)%v
          OpenAD_lin_3 = X(I)%v
          Y(INT(I))%v = (Y(I)%v+OpenAD_aux_0*2.0D00)
          OpenAD_acc_0 = (OpenAD_lin_2*2.0D00)
          OpenAD_acc_1 = (OpenAD_lin_3*2.0D00)
          CALL setderiv(OpenAD_prp_1,Y(I))
          CALL setderiv(Y(I),OpenAD_prp_1)
          CALL saxpy(OpenAD_acc_0,X(I),Y(I))
          CALL saxpy(OpenAD_acc_1,X(I),Y(I))
        ENDIF
      END DO
      GLOBALSTRING = 'either'
      LOCALSTRING = GLOBALSTRING
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use aglobalmodule
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
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
      Y(1)%v = 1.0
      Y(2)%v = 1.0
      CALL zero_deriv(Y(1))
      CALL zero_deriv(Y(2))
      GLOBALSTRING = 'yes'
      CALL foo(X,Y)
      GLOBALSTRING = 'both'
      END SUBROUTINE
