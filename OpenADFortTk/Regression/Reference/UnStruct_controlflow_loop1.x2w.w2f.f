
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
        GO TO 6
      ELSE
        GO TO 5
      ENDIF
5     CONTINUE
      GO TO 13
6     CONTINUE
      X = (X +(-3.00000011920928955078D-01))
      GO TO 7
7     CONTINUE
      IF(X .LT. 0.0D00) THEN
        GO TO 12
      ELSE
        GO TO 8
      ENDIF
8     CONTINUE
      GO TO 9
9     CONTINUE
      GO TO 10
10    CONTINUE
      GO TO 11
11    CONTINUE
      GO TO 4
12    CONTINUE
      GO TO 1
      GO TO 13
13    CONTINUE
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
