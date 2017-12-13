
      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      INTENT(IN)  X
      REAL(w2f__8) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) T1
      REAL(w2f__8) T2
C
C     **** Statements ****
C
      T1 = (X * Y)
      T2 = (Y + X * T1)
      Y = (T1 + T2)
      END SUBROUTINE

      PROGRAM scalars1
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
      EXTERNAL head
C
C     **** Statements ****
C
      A = 1.29999995231628417969D00
      B = 2.5D00
      CALL head(A, B)
      WRITE(*, *) B
      
      END PROGRAM
