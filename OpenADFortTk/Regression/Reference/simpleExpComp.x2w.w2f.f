
      PROGRAM arrays
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.00000000000000011102D-01
      Y = EXP(X * 5.0D-01)
      WRITE(*, *) Y
      
      END PROGRAM
