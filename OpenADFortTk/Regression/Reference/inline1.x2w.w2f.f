
      SUBROUTINE inline_useless(A, B)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
C
C     **** Statements ****
C
      A = COS(A)
      B = SIN(B)
      END SUBROUTINE

      PROGRAM inlinetest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL inline_useless
      REAL(w2f__8) PI
      SAVE PI
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
      CALL inline_useless(X, Y)
      
      END PROGRAM
