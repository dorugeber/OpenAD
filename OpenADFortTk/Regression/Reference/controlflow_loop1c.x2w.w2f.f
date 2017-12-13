
      SUBROUTINE sub1()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) ID
      REAL(w2f__8) Y
      SAVE Y
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) doloop_count
      INTEGER(w2f__i8) doloop_var
C
C     **** Initializers ****
C
      DATA Y / 8.0D00 /
C
C     **** Statements ****
C
      ID = 1.0D00
      doloop_count = INT(1.0D+01 - ID + 1.0D00)
      doloop_var = 0
      DO WHILE(INT(doloop_count) .GT. INT(doloop_var))
        Y = (ID +(-2.0D00))
        ID = (ID + 1.0D00)
        doloop_var = INT(doloop_var) + 1
      END DO
      END SUBROUTINE

      PROGRAM controlflow
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL sub1
C
C     **** Statements ****
C
      CALL sub1()
      
      END PROGRAM
