C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:38:04 2008
C ***********************************************************
C ***********************************************************

      MODULE mymod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) MYINT
C
C     **** Statements ****
C
      CONTAINS

        Function FUNAND(ARG1, ARG2)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        LOGICAL(w2f__i4) ARG1
        INTENT(IN)  ARG1
        LOGICAL(w2f__i4) ARG2
        INTENT(IN)  ARG2
        LOGICAL(w2f__i4) FUNAND
C
C       **** Statements ****
C
        FUNAND = ARG1 .AND. ARG2
        RETURN
        END FUNCTION
      END

      PROGRAM main
      use w2f__types
      use mymod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) X1
      LOGICAL(w2f__i4) X2
C
C     **** Statements ****
C
      X1 = .TRUE.
      X2 = FUNAND(X1, X1)
      IF(X2) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
