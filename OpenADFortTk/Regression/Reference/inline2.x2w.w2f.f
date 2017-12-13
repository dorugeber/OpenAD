
      SUBROUTINE inline_useless(A, B)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
      INTENT(IN)  B
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) LOCAL
C
C     **** Statements ****
C
      A = COS(A)
      LOCAL = SIN(B)
      END SUBROUTINE

      PROGRAM inlinetest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL inline_useless
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.0D00
      Y = 8.0D00
      CALL inline_useless(X, Y)
      
      END PROGRAM
