C ***********************************************************
C Fortran file translated from WHIRL Tue Oct 12 14:26:06 2010
C ***********************************************************
C ***********************************************************

      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
C
C       **** Local Variables and Functions ****
C
        EXTERNAL FOO
C
C       **** Statements ****
C
        X = I * X
        RETURN
        END SUBROUTINE

        SUBROUTINE FOO(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
C
C       **** Statements ****
C
        I = 2
        CALL BAR(X)
        RETURN
        END SUBROUTINE
      END

      PROGRAM p
      use w2f__types
      use m ,only: FOO
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      SAVE I
C
C     **** Initializers ****
C
      DATA I / 2 /
C
C     **** Statements ****
C
      CALL FOO(I)
      IF(I .eq. 4) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) I
      ENDIF
      
      END PROGRAM
