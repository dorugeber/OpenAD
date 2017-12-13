
      PROGRAM arrays3
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      
      EQUIVALENCE(X, t__3)
      
      INTEGER(w2f__i4) t__3(1 : 6)
      SAVE t__3
      INTEGER(w2f__i4) X(1 : 2, 1 : 3)
      SAVE X
      INTEGER(w2f__i4) Y(1 : 2)
C
C     **** Initializers ****
C
      DATA t__3 / 6 * 2 /
C
C     **** Statements ****
C
      Y(1 : 2) = X(1 : 2, 2)
      WRITE(*, *) Y
      
      END PROGRAM
