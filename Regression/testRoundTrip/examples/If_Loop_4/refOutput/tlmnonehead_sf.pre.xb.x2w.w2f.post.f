
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
      type(active) :: X(1:10)
      INTENT(IN) X
      type(active) :: Y(1:10)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) PI
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      PI = 3.14149999618530273438D00
      DO I = 1, 9, 1
        IF(I .GT. 5) THEN
          OpenAD_aux_0 = SIN(X(I)%v)
          OpenAD_lin_1 = COS(X(I)%v)
          OpenAD_lin_0 = PI
          Y(INT(I))%v = (PI*OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_1*OpenAD_lin_0)
          CALL sax(OpenAD_acc_0,X(I),Y(I))
        ELSE
          OpenAD_lin_2 = (-SIN(X(I)%v))
          Y(INT(I))%v = (PI+COS(X(I)%v))
          CALL sax(OpenAD_lin_2,X(I),Y(I))
        ENDIF
      END DO
      OpenAD_lin_3 = Y(9)%v
      OpenAD_lin_4 = Y(1)%v
      Y(10)%v = (Y(1)%v*Y(9)%v)
      CALL setderiv(OpenAD_prp_0,Y(1))
      CALL setderiv(OpenAD_prp_1,Y(9))
      CALL sax(OpenAD_lin_3,OpenAD_prp_0,Y(10))
      CALL saxpy(OpenAD_lin_4,OpenAD_prp_1,Y(10))
      END SUBROUTINE
