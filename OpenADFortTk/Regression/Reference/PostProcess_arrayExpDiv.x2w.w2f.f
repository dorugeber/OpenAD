
      PROGRAM p
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
C
C     **** Statements ****
C
      X(1)%v = 5.0D-01
      X(2)%v = 1.0D00
      Y(1:2)%v = EXP(X%v*5.0D-01)
      WRITE(*,*) Y%v
      
      END PROGRAM
