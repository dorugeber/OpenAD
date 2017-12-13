
      PROGRAM scalartest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) IX
      INTEGER(w2f__i4) JJJ
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 6.0D00
      Y = 2.29999995231628417969D00
      IX = 2
      JJJ = 3
      X = (X * X + 3.14149999618530273438D00 - Y)
      IX = (JJJ + 1)
      WRITE(*, *) 'x(1) = ', X, 'ix(4) = ', IX
      
      END PROGRAM
