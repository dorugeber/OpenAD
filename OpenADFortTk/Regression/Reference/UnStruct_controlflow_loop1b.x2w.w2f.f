
      SUBROUTINE myloop(X)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) J
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
        GO TO 12
      ENDIF
5     CONTINUE
      X = (X +(-3.00000011920928955078D-01))
      GO TO 6
6     CONTINUE
      J = 1
      GO TO 16
7     CONTINUE
      J = J + 1
16    CONTINUE
      IF(J .LE. 3) THEN
        GO TO 8
      ELSE
        GO TO 14
      ENDIF
8     CONTINUE
      IF(X .LT. 0.0D00) THEN
        GO TO 12
      ELSE
        GO TO 9
      ENDIF
9     CONTINUE
      GO TO 10
10    CONTINUE
      X = (X + 1.00000000000000002082D-02)
      GO TO 11
11    CONTINUE
      GO TO 7
12    CONTINUE
      GO TO 13
13    CONTINUE
      GO TO 1
14    CONTINUE
      GO TO 15
15    CONTINUE
      GO TO 4
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
