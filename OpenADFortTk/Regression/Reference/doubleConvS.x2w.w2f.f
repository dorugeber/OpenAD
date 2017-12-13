
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) D
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = 2
      D = DBLE(I)
      WRITE(*, *) D
      
      END PROGRAM
