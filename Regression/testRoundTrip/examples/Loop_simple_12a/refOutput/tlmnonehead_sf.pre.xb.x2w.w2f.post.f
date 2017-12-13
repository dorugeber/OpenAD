
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
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
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
      Y(1)%v = 0.0
      Y(2)%v = 0.0
      CALL zero_deriv(Y(1))
      CALL zero_deriv(Y(2))
C$OPENAD XXX Simple loop
      DO I = 1,2,1
        J = 1
        IF (J.LT.2) THEN
          OpenAD_lin_0 = X(J)%v
          OpenAD_lin_1 = X(I)%v
          Y(INT(I))%v = (Y(I)%v+X(I)%v*X(J)%v)
          CALL setderiv(OpenAD_prp_0,Y(I))
          CALL setderiv(Y(I),OpenAD_prp_0)
          CALL saxpy(OpenAD_lin_0,X(I),Y(I))
          CALL saxpy(OpenAD_lin_1,X(J),Y(I))
          J = (J+1)
        ENDIF
      END DO
      END SUBROUTINE
