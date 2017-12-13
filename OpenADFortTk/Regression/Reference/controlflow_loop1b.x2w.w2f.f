
      SUBROUTINE sub1()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      REAL(w2f__8) Y
      SAVE Y
C
C     **** Initializers ****
C
      DATA Y / 8.0D00 /
C
C     **** Statements ****
C
      DO I = 1, 10, 1
        DO J = 1, 10, 2
          Y = (I + J)
        END DO
        Y = (Y +(-2.0D00))
      END DO
      END SUBROUTINE

      PROGRAM controlflow
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL sub1
C
C     **** Statements ****
C
      CALL sub1()
      
      END PROGRAM
