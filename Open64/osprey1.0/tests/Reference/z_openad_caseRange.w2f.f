
      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) N
      INTEGER(w2f__i4) X
      INTEGER(w2f__i4) select_expr_temp_0
C
C     **** Statements ****
C
      N = 3
      select_expr_temp_0 = N
      IF(.NOT.(N .GE. 1)) GO TO 5
      IF(.NOT.(N .LE. 5)) GO TO 5
      select_expr_temp_0 = 1
5     CONTINUE
      IF ( select_expr_temp_0  .EQ.  0)  GO TO  2
      IF ( select_expr_temp_0  .EQ.  1)  GO TO  3
      GO TO 4
2     CONTINUE
      X = 1
      GO TO 1
3     CONTINUE
      X = 2
      GO TO 1
4     CONTINUE
      X = 3
1     CONTINUE
      IF(X .eq. 2) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed', X
      ENDIF
      
      END PROGRAM
