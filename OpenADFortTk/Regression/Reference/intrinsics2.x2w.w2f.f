
      PROGRAM intrinsics2
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) X
      SAVE X
      REAL(w2f__4) Y
C
C     **** Initializers ****
C
      DATA X / 2.0 /
C
C     **** Statements ****
C
      Y = LOG10(DBLE(X))
      WRITE(*, *) X
      
      END PROGRAM
