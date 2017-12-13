
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ CB1, CB2
      REAL(w2f__4) CB1
      REAL(w2f__4) F1
      REAL(w2f__4) CB2
      REAL(w2f__4) F2
      EQUIVALENCE(CB1, F1)
      EQUIVALENCE(CB2, F2)
C
C     **** Statements ****
C
      CB1 = 1.0
      CB2 = 2.0
      IF(F2 .GT. 1.5) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) F2
      ENDIF
      
      END PROGRAM
