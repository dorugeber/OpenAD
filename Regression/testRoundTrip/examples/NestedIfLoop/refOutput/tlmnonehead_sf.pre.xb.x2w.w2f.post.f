
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
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF (X(1)%v.LT.X(2)%v) THEN
        DO I = 1,2,1
          OpenAD_lin_0 = X(I)%v
          OpenAD_lin_1 = X(I)%v
          Y(INT(I))%v = (X(I)%v*X(I)%v)
          CALL sax(OpenAD_lin_0,X(I),Y(I))
          CALL saxpy(OpenAD_lin_1,X(I),Y(I))
        END DO
      ELSE
        DO I = 1,2,1
          OpenAD_lin_2 = X(I)%v
          OpenAD_lin_3 = X(I)%v
          Y(INT(I))%v = (X(I)%v*X(I)%v)
          CALL sax(OpenAD_lin_2,X(I),Y(I))
          CALL saxpy(OpenAD_lin_3,X(I),Y(I))
        END DO
      ENDIF
      END SUBROUTINE
