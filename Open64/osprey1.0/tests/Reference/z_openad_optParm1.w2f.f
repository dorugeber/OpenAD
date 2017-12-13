
      SUBROUTINE foo(A, B, C, D)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) A
      REAL(w2f__8) B
      OPTIONAL  B
      REAL(w2f__8) C
      REAL(w2f__8) D
      OPTIONAL  D
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) t__1
C
C     **** Statements ****
C
      C = A * 2.0D00
      t__1 = .TRUE.
      IF(.NOT. PRESENT(B)) THEN
        t__1 = .FALSE.
      ELSE
        IF(.NOT. PRESENT(D)) THEN
          t__1 = .FALSE.
        ENDIF
      ENDIF
      IF(t__1) THEN
        D = B * 3.0D00
      ENDIF
      END SUBROUTINE

      PROGRAM optparm1
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
C     **** Statements ****
C
      interface 
        SUBROUTINE foo(A, B, C, D)
        use w2f__types
        REAL(w2f__8) A
        REAL(w2f__8) B
        OPTIONAL  B
        REAL(w2f__8) C
        REAL(w2f__8) D
        OPTIONAL  D
        END SUBROUTINE

      end interface 
      
      X = 2.0D00
      CALL foo(A = X, C = U)
      CALL foo(U, X, Y, V)
      IF((V .eq.(X * 3.0D00)) .AND.(Y .eq.(X * 4.0D00))) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) Y, V, 'failed'
      ENDIF
      
      END PROGRAM
