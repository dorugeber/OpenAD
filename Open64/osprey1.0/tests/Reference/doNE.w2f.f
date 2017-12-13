C ***********************************************************
C Fortran file translated from WHIRL Fri Dec 12 14:32:29 2008
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
      LOGICAL(w2f__i4) t__1
C
C     **** Initializers ****
C
      DATA J / 1 /
C
C     **** Statements ****
C
      I = 3
      t__1 = I .ne. 0
      DO WHILE(t__1)
        J = I + J
        I = I +(-1)
        t__1 = I .ne. 0
      END DO
      IF(J .eq. 7) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed j is', J
      ENDIF
      
      END PROGRAM
