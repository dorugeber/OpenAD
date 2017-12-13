C ***********************************************************
C Fortran file translated from WHIRL Tue Jul  7 02:54:41 2009
C ***********************************************************
C ***********************************************************

      MODULE a
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) AI
C
C     **** Statements ****
C
C$OPENAD XXX P1end
      END MODULE

      MODULE b
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) BI
C
C     **** Statements ****
C
C$OPENAD XXX P2a p2aend
      END MODULE

      PROGRAM p
      use w2f__types
      use a
      use b
      IMPLICIT NONE
C
C     **** Statements ****
C
C$OPENAD XXX P3 p3end
      WRITE(*, *) 'OK'
      
      END PROGRAM
