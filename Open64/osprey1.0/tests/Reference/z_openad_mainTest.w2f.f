
      PROGRAM myprogram
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) MAIN
      SAVE MAIN
C
C     **** Initializers ****
C
      DATA MAIN / 0 /
C
C     **** Statements ****
C
      IF(MAIN .eq. 0) THEN
C       if (main == 0) Print *, "OK"
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
