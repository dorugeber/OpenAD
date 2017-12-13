
      PROGRAM stringlen
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      CHARACTER(10) S
C
C     **** Statements ****
C
      S = 'bla'
      A = (LEN(TRIM(S)) * 2 + 1.0D00)
      WRITE(*, *) A
      
      END PROGRAM
