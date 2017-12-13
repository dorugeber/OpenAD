C ***********************************************************
C Fortran file translated from WHIRL Fri Dec 12 14:32:38 2008
C ***********************************************************
C ***********************************************************

      PROGRAM doup
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      SAVE J
C
C     **** Initializers ****
C
      DATA J / 1 /
C
C     **** Statements ****
C
      DO I = 1, 3, 1
        J = I + J
      END DO
      IF(J .eq. 7) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
