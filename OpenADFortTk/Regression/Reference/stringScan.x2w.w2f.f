
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      CHARACTER(3) A
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      A = 'bla'
      I = SCAN(A, 'l')
      WRITE(*, *) I
      
      END PROGRAM
