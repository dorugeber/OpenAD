C ***********************************************************
C Fortran file translated from WHIRL Tue Jun 23 12:38:45 2009
C ***********************************************************
C ***********************************************************

      MODULE aemp
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) AI
      INTEGER(w2f__i4) AJ
      INTEGER(w2f__i4) AK
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo()
      use w2f__types
      use aemp
      IMPLICIT NONE
C
C     **** Statements ****
C
      AJ = AI
      RETURN
      END SUBROUTINE

      MODULE emp
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      PROGRAM p
      use w2f__types
      use aemp ,only: AI , AK
      use emp
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) AJ
      EXTERNAL foo
C
C     **** Statements ****
C
      AJ = 1
      AI = 2
      CALL foo()
      IF(AJ .eq. 1) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
