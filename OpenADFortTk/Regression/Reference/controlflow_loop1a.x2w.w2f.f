
      SUBROUTINE sub1()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) Y
      SAVE Y
C
C     **** Initializers ****
C
      DATA Y / 8.0D00 /
C
C     **** Statements ****
C
      DO I = 1, 10, 2
        Y = (I +(-2))
      END DO
      END SUBROUTINE

      SUBROUTINE sub2()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) Y
      SAVE Y
C
C     **** Initializers ****
C
      DATA Y / 8.0D00 /
C
C     **** Statements ****
C
      DO WHILE(I .LT. 10)
        Y = (I +(-2))
        I = (I + 2)
      END DO
      END SUBROUTINE

      PROGRAM controlflow
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL sub1
      EXTERNAL sub2
C
C     **** Statements ****
C
      CALL sub1()
      CALL sub2()
      
      END PROGRAM
