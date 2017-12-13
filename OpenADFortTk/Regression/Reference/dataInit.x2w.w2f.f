
      SUBROUTINE foo(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__4) X
      REAL(w2f__4) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) LOCAL
      SAVE LOCAL
      REAL(w2f__4) OTHER
C
C     **** Initializers ****
C
      DATA LOCAL / 1.0 /
C
C     **** Statements ****
C
      OTHER = 1.0
      X = LOCAL
      Y = OTHER
      LOCAL = (DBLE(LOCAL) * DBLE(2.0))
      OTHER = (DBLE(OTHER) * DBLE(2.0))
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ERRORS
      EXTERNAL foo
      REAL(w2f__4) X
      REAL(w2f__4) Y
C
C     **** Statements ****
C
      ERRORS = 0
      CALL foo(X, Y)
      IF(DBLE(X) .ne. DBLE(1.0)) THEN
        WRITE(*, *) X, Y
        ERRORS = (ERRORS + 1)
      ENDIF
      CALL foo(X, Y)
      IF((DBLE(X) .ne. DBLE(1.0)) .AND.(DBLE(X) .ne. DBLE(2.0))) THEN
        WRITE(*, *) X, Y
        ERRORS = (ERRORS + 1)
      ENDIF
      IF(ERRORS .eq. 0) THEN
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
