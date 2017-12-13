
      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) X(1 : 1)
      INTENT(IN)  X
      INTEGER(w2f__i4) Y(1 : 1)
      INTENT(OUT)  Y
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      Y(1) = X(1)
      IF(REAL(Y(1)) .GT. 0.0) THEN
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
      INTEGER(w2f__i4) X(1 : 1)
      INTEGER(w2f__i4) Y(1 : 1)
C
C     **** Statements ****
C
      X(1) = 2
      CALL head(X, Y)
      IF(Y(1) .eq.(X(1) ** 4)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
