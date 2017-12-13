
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
      I = 3
      DO WHILE(I .ne. 0)
        J = I + J
        I = I +(-1)
      END DO
      IF(J .eq. 7) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed j is', J
      ENDIF
      
      END PROGRAM
