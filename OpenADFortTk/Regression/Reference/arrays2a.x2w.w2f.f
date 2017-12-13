
      SUBROUTINE compute2()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) A(1 : 20)
      REAL(w2f__4) B(1 : 20)
      REAL(w2f__4) C(1 : 20)
      REAL(w2f__4) X
C
C     **** Statements ****
C
      X = 5.0
      A(1 : 20) = X
      B(1 : 20) = 9
      C(1 : 20) = 68
      A(1 : 20) = ((DBLE(A(1 : 20)) / DBLE(3.0999999046)) + DBLE(B(1 :
     >  20)) * SQRT(C))
      WRITE(*, *) 'a =', A
      END SUBROUTINE

      PROGRAM f77_f90_arrays
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL compute2
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.0D00
      Y = 8.0D00
      CALL compute2()
      
      END PROGRAM
