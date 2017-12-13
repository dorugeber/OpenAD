
      MODULE parmmod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) C
      PARAMETER ( C = 3)
C
C     **** Statements ****
C
      END MODULE

      PROGRAM useparm
      use w2f__types
      use parmmod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = 2
      I = (I * 3)
      IF(I .eq. 6) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
