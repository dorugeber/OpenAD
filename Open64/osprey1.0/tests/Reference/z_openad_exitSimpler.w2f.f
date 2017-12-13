
      SUBROUTINE foo(J)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) J
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      J = 0
      DO I = 1, 4, 1
        J = J + 1
        IF(I .LT. 3) GO TO 2
      END DO
2     CONTINUE
      END SUBROUTINE

      PROGRAM exs
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) J
C
C     **** Statements ****
C
      J = 0
      CALL foo(J)
      IF(J .eq. 1) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) J
      ENDIF
      
      END PROGRAM
