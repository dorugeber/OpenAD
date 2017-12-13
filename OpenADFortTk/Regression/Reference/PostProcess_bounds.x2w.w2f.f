
      PROGRAM bounds
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      type(active) :: P(:)
      
      POINTER P
      type(active) :: X(1:3)
      TARGET X
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(X)
C
C     **** Statements ****
C
      P => X(1 : 3)
      DO I = LBOUND(P, 1), UBOUND(P, 1), 1
        P(INT(I))%v = X(I)%v
        X(INT(I))%v = (P(I)%v+I*5.0D-01)
      END DO
      WRITE(*,*) X%v
      
      END PROGRAM
