
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
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) select_expr_temp_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
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
      I = 1
      Y(1)%v = 2.0
      CALL zero_deriv(Y(1))
      select_expr_temp_0 = I
      IF (select_expr_temp_0.EQ.1) go to 4
      IF (select_expr_temp_0.EQ.2) go to 8
      GO TO 9
 9    CONTINUE
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      GO TO 6
 4    CONTINUE
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      GO TO 6
 8    CONTINUE
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      GO TO 6
 6    CONTINUE
      OpenAD_lin_0 = Y(1)%v
      OpenAD_lin_1 = Y(1)%v
      Y(1)%v = (Y(1)%v*Y(1)%v)
      CALL setderiv(OpenAD_prp_0,Y(1))
      CALL setderiv(OpenAD_prp_1,Y(1))
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,Y(1))
      CALL saxpy(OpenAD_lin_1,OpenAD_prp_1,Y(1))
      END SUBROUTINE
