
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
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
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
      IF (X(1)%v.LE.X(2)%v) THEN
        IF (X(1)%v.eq.X(2)%v) THEN
          OpenAD_lin_0 = X(1)%v
          OpenAD_lin_1 = X(1)%v
          Y(1)%v = (X(1)%v*X(1)%v)
          CALL sax(OpenAD_lin_0,X(1),Y(1))
          CALL saxpy(OpenAD_lin_1,X(1),Y(1))
        ELSE
          Y(1)%v = (X(2)%v-X(1)%v)
          CALL setderiv(Y(1),X(2))
          CALL dec_deriv(Y(1),X(1))
        ENDIF
      ELSE
        Y(1)%v = (X(1)%v-X(2)%v)
        CALL setderiv(Y(1),X(1))
        CALL dec_deriv(Y(1),X(2))
      ENDIF
      Y(2)%v = Y(1)%v
      CALL setderiv(OpenAD_prp_0,Y(1))
      CALL setderiv(Y(2),OpenAD_prp_0)
      END SUBROUTINE
