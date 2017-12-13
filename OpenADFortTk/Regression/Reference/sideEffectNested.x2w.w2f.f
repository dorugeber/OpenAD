
      SUBROUTINE foo(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) X
C
C     **** Statements ****
C
      CALL BAR()
      CONTAINS

        SUBROUTINE BAR()
        use w2f__types
        IMPLICIT NONE
C
C       **** Statements ****
C
        X = (DBLE(X) * DBLE(2.0))
        END SUBROUTINE
      END

      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__4) R
C
C     **** Statements ****
C
      R = 3.0
      CALL foo(R)
      WRITE(*, *) R
      
      END PROGRAM
