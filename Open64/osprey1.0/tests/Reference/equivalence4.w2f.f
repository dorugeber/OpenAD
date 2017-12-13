C ***********************************************************
C Fortran file translated from WHIRL Fri Oct 30 17:26:31 2009
C ***********************************************************
C ***********************************************************

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
      REAL(w2f__4) X
C
C     **** Statements ****
C
      V1 = 1.0
      V2 = 2.0
      X = 3.0
      IF((F2 .GT. 1.5) .AND.(F1 .LT. 1.5)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) F1, F2, X
      ENDIF
      
      END PROGRAM
