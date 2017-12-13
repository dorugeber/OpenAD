C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:37:59 2008
C ***********************************************************
C ***********************************************************

      PROGRAM intent_gotcha
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  MYTYPE
        INTEGER(w2f__i4) X
        REAL(w2f__4) Y
      END TYPE
      
C
C     **** Local Variables and Functions ****
C
      TYPE (MYTYPE) A
C
C     **** Statements ****
C
      A%X = 1
      A%Y = 2.0
      CALL ASSIGN(A)
      IF(A%X .eq. 2_w2f__i8) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      CONTAINS

        SUBROUTINE ASSIGN(THIS)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (MYTYPE) THIS
        INTENT(OUT)  THIS
C
C       **** Statements ****
C
        THIS%X = 2
        THIS%Y = 2.0
        RETURN
        END SUBROUTINE
      END
