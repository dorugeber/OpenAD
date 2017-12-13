C ***********************************************************
C Fortran file translated from WHIRL Fri Oct 31 13:36:31 2008
C ***********************************************************
C ***********************************************************

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
      IF(Y(1) .GT. 0.0) THEN
        DO I = 1, 3, 1
          Y(1) = (X(1) * Y(1))
        END DO
      ELSE
        Y(1) = (Y(1) / X(1))
      ENDIF
      RETURN
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
      LOGICAL(w2f__i4) t__1
C
C     **** Statements ****
C
      X(1) = 2
      CALL head(X, Y)
      t__1 = Y(1) .eq.(X(1) ** 4)
      IF(t__1) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
