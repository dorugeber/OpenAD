
C$OPENAD XXX File_start [head.f]
      SUBROUTINE bar(X, Y, K)
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
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, K, 1
        Y(INT(I))%v = X(I)%v
        CALL setderiv(Y(I),X(I))
      END DO
      END SUBROUTINE

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
      EXTERNAL bar
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL bar(X,Y,3)
      END SUBROUTINE
