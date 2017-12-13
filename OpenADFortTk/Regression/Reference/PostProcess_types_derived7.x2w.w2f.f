
      MODULE m
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE T
        type(active) :: X
        type(active),POINTER :: Y(:)
      END TYPE

C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X, Y)
      use OAD_active
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) X
      REAL(w2f__4) Y
C
C     **** Local Variables and Functions ****
C
      type(T) :: AT(:)
      ALLOCATABLE AT
C
C     **** Statements ****
C
      ALLOCATE(AT(1))
      AT(1)%X%v = X
      ALLOCATE(AT(1)%Y(1))
      AT(1)%Y(:)%v = 0.0
      Y = AT(1)%Y(1)%v
      END SUBROUTINE

      PROGRAM p
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__4) X
      REAL(w2f__4) Y
C
C     **** Statements ****
C
      X = 2.0
      CALL foo(X,Y)
      WRITE(*,*) Y
      
      END PROGRAM
