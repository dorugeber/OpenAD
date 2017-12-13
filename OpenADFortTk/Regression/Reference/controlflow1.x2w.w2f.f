
      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X(1 : 1)
      INTENT(IN)  X
      REAL(w2f__8) Y(1 : 1)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      Y(1) = X(1)
      IF(Y(1) .GT. 0.0D00) THEN
        DO I = 1, 3, 1
          Y(1) = (X(1) * Y(1))
        END DO
      ELSE
        Y(1) = (Y(1) / X(1))
      ENDIF
      END SUBROUTINE

      PROGRAM controlflow1
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL head
      REAL(w2f__8) X(1 : 1)
      REAL(w2f__8) Y(1 : 1)
C
C     **** Statements ****
C
      X(1) = 2.29999999999999982236D00
      CALL head(X, Y)
      WRITE(*, *) Y(1)
      
      END PROGRAM
