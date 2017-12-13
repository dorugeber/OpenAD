
      PROGRAM arrays
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X(1 : 2)
      REAL(w2f__8) Y(1 : 2)
C
C     **** Statements ****
C
      X(1) = 2.00000000000000011102D-01
      X(2) = 2.99999999999999988898D-01
      Y(1 : 2) = EXP(X(1 : 2) * 5.0D-01)
      WRITE(*, *) Y
      
      END PROGRAM
