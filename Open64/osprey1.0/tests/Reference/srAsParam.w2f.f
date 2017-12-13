C ***********************************************************
C Fortran file translated from WHIRL Wed Nov 26 13:44:16 2008
C ***********************************************************
C ***********************************************************

      SUBROUTINE foo(I)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = I + 1
      RETURN
      END SUBROUTINE

      SUBROUTINE bar(K, S)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      
C
C     **** Statements ****
C
      CALL S(K)
      RETURN
      END SUBROUTINE

      PROGRAM srasparameter
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      EXTERNAL foo
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      I = 3
      CALL bar(I, foo)
      IF(I .eq. 4) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
