
      PROGRAM nestedproc
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) N
      SAVE N
C
C     **** Initializers ****
C
      DATA N / 7 /
C
C     **** Statements ****
C
      CALL FOO(N)
      
      CONTAINS

        SUBROUTINE FOO(N)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) N
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) BOGUS
C
C       **** Statements ****
C
        IF(N .eq. 1) THEN
          BOGUS = 2
        ELSE
          BOGUS = 3
        ENDIF
        END SUBROUTINE
      END
