
      SUBROUTINE foo(N, RES)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      CHARACTER(*) N
      INTEGER(w2f__i4) RES
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) S
C
C     **** Statements ****
C
      S = 1
      IF(N(S + 1 : (S + 1 - 1) + 3) .EQ. 'bla') THEN
        RES = 1
      ELSE
        RES = 5
      ENDIF
      END SUBROUTINE

      PROGRAM functiontest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) N
      CHARACTER(10) S
C
C     **** Statements ****
C
      S = 'ablab'
      N = 2
      CALL foo(S, N)
      IF(N .eq. 1) THEN
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
