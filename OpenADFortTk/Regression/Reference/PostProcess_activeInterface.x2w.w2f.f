
      SUBROUTINE foo(X)
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
C
C     **** Statements ****
C
      X%v = (X%v*2.0D00)
      END SUBROUTINE

      PROGRAM activeinterface
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface
        SUBROUTINE foo(X)
        use OAD_active
        use w2f__types
        type(active) :: X
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
      X(1)%v = 2.0
      X(2)%v = 3.0
      CALL foo(X(1))
      Y(1:2)%v = X(1:2)%v
      IF (Y(1)%v.eq.4.0D00) THEN
        WRITE(*,*) 'OK'
      ENDIF
      
      END PROGRAM
