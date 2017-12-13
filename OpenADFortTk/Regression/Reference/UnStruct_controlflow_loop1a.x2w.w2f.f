
      SUBROUTINE myloop(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
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
      X = (X +(-3.00000011920928955078D-01))
      GO TO 6
6     CONTINUE
      IF(X .LT. 0.0D00) THEN
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
C
C     **** Statements ****
C
      X = 2.0D00
      CALL myloop(X)
      WRITE(*, *) X
      
      END PROGRAM
