
      PROGRAM exs
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
      J = 0
      GO TO 4
4     CONTINUE
      I = 1
      GO TO 12
5     CONTINUE
      I = I + 1
12    CONTINUE
      IF(I .LE. 4) THEN
        GO TO 6
      ELSE
        GO TO 10
      ENDIF
6     CONTINUE
      J = (J + 1)
      GO TO 7
7     CONTINUE
      IF(I .LT. 3) THEN
        GO TO 10
      ELSE
        GO TO 8
      ENDIF
8     CONTINUE
      GO TO 9
9     CONTINUE
      GO TO 5
10    CONTINUE
      WRITE(*, *) J
      
      GO TO 11
11    CONTINUE
      GO TO 1
1     CONTINUE
      END PROGRAM
