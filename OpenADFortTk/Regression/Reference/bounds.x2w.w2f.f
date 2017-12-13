
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) L(1 : 1)
      REAL(w2f__8) X(1 : 3)
C
C     **** Statements ****
C
      DO I = LBOUND(X, 1), UBOUND(X, 1), 1
        X(INT(I)) = (I * 5.0D-01)
      END DO
      L(1 : 1) = LBOUND(X)
      WRITE(*, *) X, L
      
      END PROGRAM
