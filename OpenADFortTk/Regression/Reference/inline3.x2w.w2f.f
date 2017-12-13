
      SUBROUTINE inline_swap(A, B)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) T
      REAL(w2f__8) T1
      REAL(w2f__8) T2
C
C     **** Statements ****
C
      T = A
      T1 = (A + B)
      T2 = SIN(A ** B)
      A = B
      B = T
      END SUBROUTINE

      PROGRAM inlinetest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL inline_swap
      REAL(w2f__8) PI
      SAVE PI
      REAL(w2f__8) T
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Initializers ****
C
      DATA PI / 3.14159250259399414062D00 /
C
C     **** Statements ****
C
      X = PI
      Y = (PI * 5.0D-01)
      T = PI
      WRITE(*, *) 'x =', X
      WRITE(*, *) 'y =', Y
      WRITE(*, *) '--------------------'
      CALL inline_swap(X, Y)
      WRITE(*, *) 'x =', X
      WRITE(*, *) 'y =', Y
      
      END PROGRAM
