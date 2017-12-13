C ***********************************************************
C Fortran file translated from WHIRL Thu Jul 23 16:28:25 2009
C ***********************************************************
C ***********************************************************

      MODULE m
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

      PROGRAM p
      use w2f__types
      use m ,only:
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = 1
      WRITE(*, *) 'OK'
      
      END PROGRAM
