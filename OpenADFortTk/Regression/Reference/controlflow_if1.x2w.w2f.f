
      PROGRAM controlflow_if
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
      X = 2.0D00
      IF(X .GE. 0.0D00) THEN
        Y = 5.0D00
      ELSE
        Y = 3.0D00
      ENDIF
      
      END PROGRAM
