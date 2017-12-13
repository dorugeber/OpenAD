
      PROGRAM print1
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) P
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
      P = 1.0D00
      X%v = 2.0D00
      Y%v = X%v
      WRITE(*,*) X%v,Y%v
      IF (Y%v.eq.2.0D00) THEN
        WRITE(*,*) 'OK'
      ENDIF
      
      END PROGRAM
