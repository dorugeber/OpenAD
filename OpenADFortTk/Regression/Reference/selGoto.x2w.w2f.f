
      PROGRAM selgoto
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
      IF(I .LT. 3) THEN
        GO TO 13
      ELSE
        GO TO 5
      ENDIF
5     CONTINUE
      GO TO 6
6     CONTINUE
      GO TO 7
7     CONTINUE
      select_expr_temp_0 = I
      GO TO 8
8     CONTINUE
      IF(select_expr_temp_0 .ne. 0) THEN
        GO TO 10
      ELSE
        GO TO 9
      ENDIF
9     CONTINUE
      J = 1
      GO TO 11
10    CONTINUE
      J = 2
      GO TO 11
11    CONTINUE
      GO TO 12
12    CONTINUE
      GO TO 14
13    CONTINUE
      GO TO 14
14    CONTINUE
C     1 print *,j
      WRITE(*, *) J
      
      GO TO 15
15    CONTINUE
      GO TO 1
1     CONTINUE
      END PROGRAM
