
      PROGRAM arraypointershape
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      type(active) :: BTN(:,:)
      
      POINTER BTN
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
      ALLOCATE(BTN(1, 1))
      X%v = 1.0D00
      BTN(1,1)%v = X%v
      Y%v = BTN(1,1)%v
      WRITE(*,*) Y%v
      WRITE(*,*) 'OK'
      
      END PROGRAM
