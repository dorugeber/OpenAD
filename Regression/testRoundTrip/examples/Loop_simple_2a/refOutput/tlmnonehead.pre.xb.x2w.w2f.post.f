
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
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) K
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
      DO I = 1, 3, 1
        Y(INT(I))%v = X(I)%v
        CALL setderiv(Y(I),X(I))
      END DO
C$OPENAD XXX Simple loop
      DO I = 1,3,1
        DO J = 1,3,1
          DO K = 1,3,1
            OpenAD_lin_0 = Y(I)%v
            OpenAD_lin_1 = X(J)%v
            Y(INT(I))%v = (X(J)%v*Y(I)%v)
            CALL setderiv(OpenAD_prp_0,Y(I))
            CALL sax(OpenAD_lin_0,X(J),Y(I))
            CALL saxpy(OpenAD_lin_1,OpenAD_prp_0,Y(I))
          END DO
        END DO
      END DO
      END SUBROUTINE
