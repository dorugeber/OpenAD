
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
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Y(1)%v = 2.0
      CALL zero_deriv(Y(1))
      IF (X(1)%v.GT.0.0D00) THEN
        Y(1)%v = X(1)%v
        CALL setderiv(Y(1),X(1))
        IF (X(1)%v.GT.1.0D00) THEN
          CALL setderiv(OpenAD_prp_0,Y(1))
          CALL setderiv(Y(1),OpenAD_prp_0)
        ELSE
          IF (X(1)%v.GT.0.0D00) THEN
            Y(1)%v = X(1)%v
            CALL setderiv(Y(1),X(1))
          ENDIF
        ENDIF
      ENDIF
      OpenAD_lin_0 = Y(1)%v
      OpenAD_lin_1 = Y(1)%v
      Y(1)%v = (Y(1)%v*Y(1)%v)
      CALL setderiv(OpenAD_prp_1,Y(1))
      CALL setderiv(OpenAD_prp_2,Y(1))
      CALL sax(OpenAD_lin_0,OpenAD_prp_1,Y(1))
      CALL saxpy(OpenAD_lin_1,OpenAD_prp_2,Y(1))
      END SUBROUTINE
