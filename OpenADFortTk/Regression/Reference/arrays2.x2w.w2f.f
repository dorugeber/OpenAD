
      SUBROUTINE compute1()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) A(1 : 20)
      REAL(w2f__4) B(1 : 20)
      REAL(w2f__4) C(1 : 20)
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) TI(1 : 5)
      REAL(w2f__4) TR(1 : 5)
      REAL(w2f__4) X
C
C     **** Statements ****
C
      X = 5.0
      TR(INT(IABS(1))) = X
      TR(2) = 9
      TR(3) = 68
      DO I = 1, 20, 1
        TI(1) = I
        A(TI(1)) = TR(1)
        B(TI(1)) = TR(2)
        C(TI(1)) = TR(3)
      END DO
      DO I = 1, 20, 1
        A(INT(I)) = ((DBLE(A(I)) / DBLE(3.0999999046)) + DBLE(B(I)) *
     >  SQRT(DBLE(C(I))))
        WRITE(*, *) 'a(', I, ')=', A(I)
      END DO
      END SUBROUTINE

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
      EXTERNAL compute1
      EXTERNAL compute2
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.0D00
      Y = 8.0D00
      CALL compute1()
      CALL compute2()
      
      END PROGRAM
