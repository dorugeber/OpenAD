
      RECURSIVE Function fact(N)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) N
      INTENT(IN)  N
      REAL(w2f__4) fact
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) select_expr_temp_0
C
C     **** Statements ****
C
      select_expr_temp_0 = N
      IF(.NOT.(N .GE. 1)) GO TO 5
      select_expr_temp_0 = 1
5     CONTINUE
      IF ( select_expr_temp_0  .EQ.  0)  GO TO  2
      IF ( select_expr_temp_0  .EQ.  1)  GO TO  3
      GO TO 4
2     CONTINUE
      fact = 1.0
      GO TO 1
3     CONTINUE
      fact = REAL(N) * fact((N +(-1)))
      GO TO 1
4     CONTINUE
      fact = 0.0
1     CONTINUE
      RETURN
      END FUNCTION

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL fact
      REAL(w2f__4) fact
C
C     **** Statements ****
C
      IF(fact((6)) .eq. 7.2E+02) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
