
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__4) R
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X)
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) X
C
C     **** Statements ****
C
      R = X
      END SUBROUTINE

      SUBROUTINE bar(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) X
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
C
C     **** Statements ****
C
      CALL foo(X)
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      REAL(w2f__4) P_R
C
C     **** Statements ****
C
      P_R = 3.0
      R = 2.0
      CALL bar(P_R)
      WRITE(*, *) R
      
      END PROGRAM
