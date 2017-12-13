
      SUBROUTINE foo(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) X
C
C     **** Statements ****
C
      X = X + 1
      END SUBROUTINE

      SUBROUTINE bar(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) X(1 : 1)
C
C     **** Statements ****
C
      X(1) = (X(1) + 1)
      END SUBROUTINE

      PROGRAM arrays3
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      EXTERNAL foo
      INTEGER(w2f__i4) X(1 : 2)
      INTEGER(w2f__i4) Y(1 : 2)
C
C     **** Statements ****
C
      X(1) = 1
      Y(2) = 2
      CALL foo(X(1))
      CALL bar(Y(2 : 2))
      IF((X(1) .eq. 2) .AND.(Y(2) .eq. 3)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
