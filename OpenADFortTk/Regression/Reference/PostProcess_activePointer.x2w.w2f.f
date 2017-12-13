
      PROGRAM arraypointershape
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(active) :: BTN
      POINTER BTN
      type(active) :: X
      TARGET X
      type(active) :: Y
      TARGET Y
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      BTN => X
      X%v = 1.0D00
      Y%v = BTN%v
      WRITE(*,*) Y%v
      WRITE(*,*) 'OK'
      
      END PROGRAM
