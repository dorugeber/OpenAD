
      PROGRAM intmaxval
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I(1 : 3)
      INTEGER(w2f__i4) MAXI
C
C     **** Statements ****
C
      I(1) = 1
      I(2) = 3
      I(3) = 2
      MAXI = MAXVAL(I)
      WRITE(*, *) MAXI
      
      END PROGRAM
