
      PROGRAM sumvar
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) S
      REAL(w2f__4) V(1 : 3)
C
C     **** Statements ****
C
      V(1) = 1.0
      V(2) = 2.0
      V(3) = 3.0
      S = SUM(DBLE(V))
      IF(S .eq. 6.0D00) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
