
      PROGRAM rtype
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) S
C
C     **** Statements ****
C
      S = 1.0D00
      S = SIN(S)
      WRITE(*, *) S
      
      END PROGRAM
