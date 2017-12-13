
      SUBROUTINE foo(A, X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      CHARACTER(*) A
      REAL(w2f__8) X
C
C     **** Statements ****
C
      IF(A(1 : LEN(A)) .EQ. 'two') THEN
        X = (X * 2.0D00)
      ENDIF
      IF(A(1 : LEN(A)) .EQ. 'three') THEN
        X = (X * 3.0D00)
      ENDIF
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X(1 : 1)
      REAL(w2f__8) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      CHARACTER(10) A
      EXTERNAL foo
C
C     **** Statements ****
C
      A = 'two'
      CALL foo(A, X)
      A = 'three'
      CALL foo(A, X)
      A = 'four'
      Y(1) = X(1)
      END SUBROUTINE

      PROGRAM sideeffect_6
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X(1 : 1)
      REAL(w2f__8) Y(1 : 1)
C
C     **** Statements ****
C
      X(1) = 2.4000000954
      WRITE(*, *) Y
      
      END PROGRAM
