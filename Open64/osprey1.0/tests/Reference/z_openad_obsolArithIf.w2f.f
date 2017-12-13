
      PROGRAM arithif
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = -3
      IF(I .LT. 0) GO TO 1
      IF(I .GT. 0) GO TO 3
      GO TO 2
3     CONTINUE
C     4     print*, 'failed'
      WRITE(*, *) 'failed'
      GO TO 4
2     CONTINUE
C     3     print*, 'failed'
      WRITE(*, *) 'failed'
      GO TO 4
1     CONTINUE
C     2     print*, 'OK'
      WRITE(*, *) 'OK'
4     CONTINUE
      I = 1
      
      END PROGRAM
