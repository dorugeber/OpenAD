
      SUBROUTINE ex(A, B)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) A
      REAL(w2f__4) B
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) C
      REAL(w2f__4) D
      REAL(w2f__4) E
C
C     **** Statements ****
C
      C = (DBLE(A) * DBLE(B))
      E = C
      IF(DBLE(C) .GT. DBLE(0.0)) THEN
        D = (DBLE(A) * DBLE(C))
        C = (DBLE(D) * DBLE(E))
        C = (DBLE(B) + DBLE(C))
        B = C
      ELSE
        E = C
        D = (DBLE(A) * DBLE(C))
        B = E
      ENDIF
      END SUBROUTINE

      PROGRAM controlflow_if2
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) A
      REAL(w2f__4) B
      EXTERNAL ex
C
C     **** Statements ****
C
      A = 2.0
      B = 3.2000000477
      CALL ex(A, B)
      WRITE(*, *) A, B
      
      END PROGRAM
