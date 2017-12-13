C ***********************************************************
C Fortran file translated from WHIRL Fri Feb 24 21:36:06 2012
C ***********************************************************
C ***********************************************************

      PROGRAM oporder
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) S1
      REAL(w2f__8) S2
      REAL(w2f__8) V(1 : 3)
C
C     **** Statements ****
C
      V(1) = 5.0D00
      V(2) = 5.0D00
      V(3) = 1.0D+17
      S1 = V(3) +(V(1) + V(2))
      S2 = V(1) +(V(2) + V(3))
      IF((V(3) .eq. S2) .AND.(V(3) .LE. S1)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED', S1, S2
      ENDIF
      
      END PROGRAM
