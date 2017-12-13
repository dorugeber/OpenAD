C ***********************************************************
C Fortran file translated from WHIRL Tue Jul 28 11:37:30 2009
C ***********************************************************
C ***********************************************************

      PROGRAM sumvar
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) V(1 : 3)
      REAL(w2f__8) W(1 : 3)
      SAVE W
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(W(tmp0), tmp0 = 1, 3, 1) / 1.0D00, 2.0D00, 3.0D00 /
C
C     **** Statements ****
C
      V(1) = 1.0D00
      V(2) = 2.0D00
      V(3) = 3.0D00
      IF(V(3) .eq. W(3)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
