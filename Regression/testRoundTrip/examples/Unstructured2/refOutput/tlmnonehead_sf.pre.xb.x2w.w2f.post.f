
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
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) II
      INTEGER(w2f__i4) J
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
 2    CONTINUE
      GO TO 3
 3    CONTINUE
      GO TO 4
 4    CONTINUE
      J = 1
      GO TO 18
 5    CONTINUE
      J = J + 1
 18   CONTINUE
      IF(J .LE. 5) THEN
        GO TO 6
      ELSE
        GO TO 16
      ENDIF
 6    CONTINUE
      GO TO 7
 7    CONTINUE
      II = 1
      GO TO 19
 8    CONTINUE
      II = II + 1
 19   CONTINUE
      IF(II .LE. 5) THEN
        GO TO 9
      ELSE
        GO TO 10
      ENDIF
 9    CONTINUE
      GO TO 10
 10   CONTINUE
      GO TO 11
 11   CONTINUE
      II = 1
      GO TO 20
 12   CONTINUE
      II = II + 1
 20   CONTINUE
      IF(II .LE. 5) THEN
        GO TO 13
      ELSE
        GO TO 15
      ENDIF
 13   CONTINUE
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      GO TO 14
 14   CONTINUE
      GO TO 12
 15   CONTINUE
      GO TO 5
 16   CONTINUE
      GO TO 17
 17   CONTINUE
      GO TO 1
 1    CONTINUE
      END SUBROUTINE
