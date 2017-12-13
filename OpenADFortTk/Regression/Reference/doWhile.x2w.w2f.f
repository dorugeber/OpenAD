
      PROGRAM dw
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = 3
      DO WHILE(I .GT. 0)
        I = (I +(-1))
      END DO
      WRITE(*, *) I
      
      END PROGRAM
