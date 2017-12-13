
      SUBROUTINE getlabel(J)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) J
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      SAVE I
C
C     **** Initializers ****
C
      DATA I / 5 /
C
C     **** Statements ****
C
      I = (I +(-1))
      J = I
      END SUBROUTINE

      PROGRAM cg
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL getlabel
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) V
      SAVE V
      INTEGER(w2f__i4) select_expr_temp_0
C
C     **** Initializers ****
C
      DATA V / 1 /
C
C     **** Statements ****
C
2     CONTINUE
      GO TO 3
3     CONTINUE
      GO TO 4
4     CONTINUE
      CALL getlabel(I)
      WRITE(*, *) I
      select_expr_temp_0 = I
      GO TO 5
5     CONTINUE
      IF ( select_expr_temp_0  .EQ.  2)  GO TO  9
      IF ( select_expr_temp_0  .EQ.  3)  GO TO  8
      IF ( select_expr_temp_0  .EQ.  4)  GO TO  7
      GO TO 6
6     CONTINUE
      STOP 'problem'
      GO TO 11
7     CONTINUE
      V = (V * 2)
      GO TO 4
8     CONTINUE
      V = (V +(-1))
      GO TO 4
9     CONTINUE
      GO TO 10
10    CONTINUE
C     2 print *,v
      WRITE(*, *) V
      
      GO TO 11
11    CONTINUE
      GO TO 1
1     CONTINUE
      END PROGRAM
