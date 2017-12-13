
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:3)
      INTENT(IN) X
      type(active) :: Y(1:3)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF (X(1)%v.LT.4.0D00) THEN
C$OPENAD XXX Simple loop
        DO I = 1,3,1
          Y(INT(I))%v = X(I)%v
          CALL setderiv(Y(I),X(I))
        END DO
      ELSE
C$OPENAD XXX Simple loop
        DO I = 1,3,1
          Y(INT(I))%v = (X(I)%v*2.0D00)
          CALL sax(2.0D00,X(I),Y(I))
        END DO
      ENDIF
      END SUBROUTINE
