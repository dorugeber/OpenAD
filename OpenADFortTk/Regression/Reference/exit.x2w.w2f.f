
      PROGRAM ex
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
C
C     **** Statements ****
C
2     CONTINUE
      GO TO 3
3     CONTINUE
      I = 1
      J = 0
      GO TO 4
4     CONTINUE
      I = 1
      GO TO 14
5     CONTINUE
      I = I + 1
14    CONTINUE
      IF(I .LE. 4) THEN
        GO TO 6
      ELSE
        GO TO 12
      ENDIF
6     CONTINUE
      IF(I .LT. 3) THEN
        GO TO 11
      ELSE
        GO TO 7
      ENDIF
7     CONTINUE
      GO TO 8
8     CONTINUE
      GO TO 9
9     CONTINUE
      J = (J + 1)
      GO TO 10
10    CONTINUE
      GO TO 5
11    CONTINUE
      GO TO 12
12    CONTINUE
      WRITE(*, *) J
      
      GO TO 13
13    CONTINUE
      GO TO 1
1     CONTINUE
      END PROGRAM
