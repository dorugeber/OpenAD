C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:38:13 2008
C ***********************************************************
C ***********************************************************

      SUBROUTINE opt(REQARG, OPTARG)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) REQARG
      INTENT(OUT)  REQARG
      LOGICAL(w2f__i4) OPTARG
      OPTIONAL  OPTARG
      INTENT(IN)  OPTARG
C
C     **** Statements ****
C
      IF(PRESENT(OPTARG)) THEN
        IF(OPTARG) THEN
          REQARG = 1
        ELSE
          REQARG = 2
        ENDIF
      ELSE
        REQARG = 3
      ENDIF
      RETURN
      END SUBROUTINE

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) RET1
      INTEGER(w2f__i4) RET2
      INTEGER(w2f__i4) RET3
C
C     **** Statements ****
C
      interface 
        SUBROUTINE opt(REQARG, OPTARG)
        use w2f__types
        INTEGER(w2f__i4) REQARG
        INTENT(out)  REQARG
        LOGICAL(w2f__i4) OPTARG
        OPTIONAL  OPTARG
        INTENT(in)  OPTARG
        END SUBROUTINE

      end interface 
      
      CALL opt(RET1, (.TRUE.))
      CALL opt(RET2, (.FALSE.))
      CALL opt(RET3)
      IF((RET3 .eq. 3) .AND.((RET1 .eq. 1) .AND.(RET2 .eq. 2))) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
