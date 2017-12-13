
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
      INTEGER(w2f__i4) I
      REAL(w2f__8) T1
      REAL(w2f__8) T2
C
C     **** Statements ****
C
      I = 1
      T1 = (Y * SIN(X))
      T2 = (I + X * T1)
      Y = (T1 + T2)
      END SUBROUTINE

      PROGRAM simple1
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL head
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 1.0D+01
      Y = 1.0D+02
      CALL head(X, Y)
      WRITE(*, *) Y
      
      END PROGRAM
