
      SUBROUTINE foo(A)
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ CBVAR
      type(active) :: CBVAR
C
C     **** Parameters and Result ****
C
      type(active) :: A
C
C     **** Statements ****
C
      CBVAR%v = (A%v*CBVAR%v)
      A%v = CBVAR%v
      END SUBROUTINE

      PROGRAM programcb
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ CBVAR
      type(active) :: CBVAR
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      type(active) :: X
      type(active) :: Y
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      CBVAR%v = 1.39999999999999991118D00
      X%v = 2.0D00
      CALL foo(X)
      Y%v = X%v
      CALL foo(Y)
      WRITE(*,*) Y%v
      
      END PROGRAM
