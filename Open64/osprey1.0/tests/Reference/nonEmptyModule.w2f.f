C ***********************************************************
C Fortran file translated from WHIRL Tue Jun 23 12:35:19 2009
C ***********************************************************
C ***********************************************************

      MODULE nem
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      END MODULE

      MODULE anem
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
      use anem
      IMPLICIT NONE
C
C     **** Statements ****
C
      AJ = AI
      RETURN
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      use nem
      use anem ,only: AI , AK
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
