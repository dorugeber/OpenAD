
      PROGRAM p
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
      X(1) = 5.0D-01
      X(2) = 1.0D00
      Y(1 : 2) = EXP(X * 5.0D-01)
      WRITE(*, *) Y
      
      END PROGRAM
