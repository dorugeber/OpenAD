
      PROGRAM se
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) select_expr_temp_0
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
      GO TO 18
5     CONTINUE
      I = I + 1
18    CONTINUE
      IF(I .LE. 4) THEN
        GO TO 6
      ELSE
        GO TO 11
      ENDIF
6     CONTINUE
      IF(I .LT. 3) THEN
        GO TO 10
      ELSE
        GO TO 7
      ENDIF
7     CONTINUE
      GO TO 8
8     CONTINUE
      GO TO 9
9     CONTINUE
      GO TO 5
10    CONTINUE
      GO TO 11
11    CONTINUE
      select_expr_temp_0 = I
      GO TO 12
12    CONTINUE
      IF(select_expr_temp_0 .ne. 0) THEN
        GO TO 14
      ELSE
        GO TO 13
      ENDIF
13    CONTINUE
      J = 1
      GO TO 15
14    CONTINUE
      J = 2
      GO TO 15
15    CONTINUE
      GO TO 16
16    CONTINUE
C     1 print *,j
      WRITE(*, *) J
      
      GO TO 17
17    CONTINUE
      GO TO 1
1     CONTINUE
      END PROGRAM
