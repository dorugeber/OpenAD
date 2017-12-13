
      SUBROUTINE foo(X1, X2, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X1
      REAL(w2f__8) X2
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      Y = (SIN(X1) * COS(X2))
      END SUBROUTINE

      PROGRAM scalars3
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
      REAL(w2f__8) C
      EXTERNAL foo
C
C     **** Statements ****
C
      A = 3.5D00
      B = 2.40000009536743164062D00
      CALL foo(A, B, C)
      WRITE(*, *) C
      
      END PROGRAM
