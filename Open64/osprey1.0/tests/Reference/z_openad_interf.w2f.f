
      PROGRAM mytest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) DP
      PARAMETER ( DP = 8)
      REAL(w2f__8) POUT_DB
      REAL(w2f__8) P_DB
C
C     **** Statements ****
C
      interface 
        SUBROUTINE sub1(P, POUT)
        use w2f__types
        REAL(w2f__8) P
        INTENT(in)  P
        REAL(w2f__8) POUT
        INTENT(out)  POUT
        END SUBROUTINE

      end interface 
      
      P_DB = 1.23456791043281555176D-01
      CALL sub1(P_DB, POUT_DB)
      IF(POUT_DB .eq. P_DB) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM

      SUBROUTINE sub1(P, POUT)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) P
      INTENT(IN)  P
      REAL(w2f__8) POUT
      INTENT(OUT)  POUT
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) DP
      PARAMETER ( DP = 8)
C
C     **** Statements ****
C
      POUT = P
      END SUBROUTINE
