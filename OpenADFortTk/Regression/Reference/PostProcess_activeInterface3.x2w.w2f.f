
      MODULE moda
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface
        SUBROUTINE foo(X, Y, Z, I)
        use OAD_active
        use w2f__types
        type(active) :: X(1:1)
        REAL(w2f__8) Y(1 : 1)
        REAL(w2f__8) Z(1 : 1)
        INTEGER(w2f__i4) I
        INTENT(in) I
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(A, B, C, I)
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) I
      type(active) :: A(1:I)
      REAL(w2f__8) B(1 : I)
      REAL(w2f__8) C(1 : INT((I + 1)))
C
C     **** Statements ****
C
      A(1)%v = (A(1)%v*2.0D00)
      END SUBROUTINE

      PROGRAM activeinterface2
      use OAD_active
      use w2f__types
      use moda
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) B(1 : 2)
      REAL(w2f__8) C(1 : 3)
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      X(1)%v = 2.0
      X(2)%v = 3.0
      CALL foo(X,B,C,2)
      Y(1:2)%v = X(1:2)%v
      IF (Y(1)%v.eq.4.0D00) THEN
        WRITE(*,*) 'OK'
      ENDIF
      
      END PROGRAM
