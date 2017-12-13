
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
      X = X / Y
      Y = (Y * 7.88999974727630615234D-01) +(-2.0D00)
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
      DO WHILE(X .GT. 0.0D00)
        CALL compute(X, Y)
        IF(Y .LT. 0.0D00) GO TO 2
      END DO
2     CONTINUE
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
      IF((Y .GT.(-8.99999976158142089844D-01)) .AND.(Y .LT.(
     > -8.89999985694885253906D-01))) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
