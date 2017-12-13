
      SUBROUTINE foo(A)
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ CBVAR
      type(active) :: CBVAR(1:2)
C
C     **** Parameters and Result ****
C
      type(active) :: A
C
C     **** Statements ****
C
      CBVAR(1)%v = (CBVAR(1)%v*A%v)
      A%v = CBVAR(1)%v
      END SUBROUTINE

      PROGRAM cb1
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ CBVAR
      type(active) :: CBVAR(1:2)
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
      X%v = 2.0D00
      CBVAR(1)%v = X%v
      CALL foo(X)
      Y%v = (CBVAR(1)%v*X%v)
      CALL foo(Y)
      WRITE(*,*) Y%v
      
      END PROGRAM
