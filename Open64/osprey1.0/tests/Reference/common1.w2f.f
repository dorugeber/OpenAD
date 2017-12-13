C ***********************************************************
C Fortran file translated from WHIRL Fri Oct 30 13:09:54 2009
C ***********************************************************
C ***********************************************************

      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ A, B, C
      REAL(w2f__4) A
      REAL(w2f__4) B
      REAL(w2f__4) C
C
C     **** Statements ****
C
      A = 1.0
      B = 2.0
      C = 3.0
      IF(B .eq. 2.0) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) B
      ENDIF
      
      END PROGRAM
