
      SUBROUTINE compute(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = (X / Y)
      Y = (Y * 7.88999974727630615234D-01 +(-2.0D00))
      END SUBROUTINE

      SUBROUTINE myloop(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL compute
C
C     **** Statements ****
C
2     CONTINUE
      GO TO 3
3     CONTINUE
      GO TO 4
4     CONTINUE
      IF(X .GT. 0.0D00) THEN
        GO TO 5
      ELSE
        GO TO 10
      ENDIF
5     CONTINUE
      CALL compute(X, Y)
      WRITE(*, *) 'x =', X
      WRITE(*, *) 'y =', Y
      GO TO 6
6     CONTINUE
      IF(Y .LT. 0.0D00) THEN
        GO TO 10
      ELSE
        GO TO 7
      ENDIF
7     CONTINUE
      GO TO 8
8     CONTINUE
      GO TO 9
9     CONTINUE
      GO TO 4
10    CONTINUE
      GO TO 11
11    CONTINUE
      GO TO 1
1     CONTINUE
      END SUBROUTINE

      PROGRAM controlflow
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL myloop
      REAL(w2f__8) X
      REAL(w2f__8) Y
C
C     **** Statements ****
C
      X = 2.0D00
      Y = 8.0D00
      CALL myloop(X, Y)
      
      END PROGRAM
