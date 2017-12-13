
      SUBROUTINE foo(A, B, C, D)
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
      OPTIONAL B
      REAL(w2f__8) C
      REAL(w2f__8) D
      OPTIONAL D
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) t__1
C
C     **** Statements ****
C
      C = A
      t__1 = .TRUE.
      IF(.NOT. PRESENT(B)) THEN
        t__1 = .FALSE.
      ELSE
        IF(.NOT. PRESENT(D)) THEN
          t__1 = .FALSE.
        ENDIF
      ENDIF
      IF(t__1) THEN
        D = B
      ENDIF
      END SUBROUTINE

      PROGRAM optparm1
      use OAD_active
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) U
      REAL(w2f__8) V
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface
        SUBROUTINE foo(A, B, C, D)
        use OAD_active
        use w2f__types
        REAL(w2f__8) A
        REAL(w2f__8) B
        OPTIONAL B
        REAL(w2f__8) C
        REAL(w2f__8) D
        OPTIONAL D
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
      X = 2.0D00
      CALL foo(A=X,C=U)
      CALL foo(U,X,Y,V)
      WRITE(*,*) Y,V
      
      END PROGRAM
