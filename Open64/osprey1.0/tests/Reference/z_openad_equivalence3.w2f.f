
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      
      EQUIVALENCE(V1, F1)
      
      REAL(w2f__4) F1
      
      EQUIVALENCE(V2, F2)
      
      REAL(w2f__4) F2
      REAL(w2f__4) V1
      REAL(w2f__4) V2
C
C     **** Statements ****
C
      V1 = 1.0
      V2 = 2.0
      IF(F2 .GT. 1.5) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) F2
      ENDIF
      
      END PROGRAM
