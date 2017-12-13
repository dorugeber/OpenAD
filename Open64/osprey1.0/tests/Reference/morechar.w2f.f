C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:38:09 2008
C ***********************************************************
C ***********************************************************

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) MYINT
      INTEGER(w2f__i4) MYKIND
      PARAMETER ( MYKIND = 1)
      CHARACTER(10) ORIGSTR
      CHARACTER(10) STR1
      SAVE STR1
C
C     **** Initializers ****
C
      DATA STR1 / '1234567890' /
C
C     **** Statements ****
C
      ORIGSTR = STR1
      STR1(1_w2f__i8 : 1) = 'A'
      MYINT = IACHAR(STR1(1_w2f__i8 : 1))
      STR1(2_w2f__i8 : 1_w2f__i8 + 1) = ACHAR(MYINT + 1)
      STR1(3_w2f__i8 : 2_w2f__i8 + 1) = CHAR(MYINT + 2, 1)
      MYINT = ICHAR(STR1(3_w2f__i8 : 2_w2f__i8 + 1))
      IF((MYINT .eq. 67) .AND. LGE(STR1, ORIGSTR)) THEN
        IF(LLT(ORIGSTR, STR1)) THEN
          WRITE(*, *) 'OK'
        ELSE
          WRITE(*, *) 'failed'
        ENDIF
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
