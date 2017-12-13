
      MODULE single
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__4) E
      REAL(w2f__4) PI
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) FLOAT
      PARAMETER ( FLOAT = 4)
C
C     **** Initializers ****
C
      DATA E / 2.7182817459 /
      DATA PI / 3.141592741 /
C
C     **** Statements ****
C
      END MODULE

      MODULE double
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) E
      REAL(w2f__8) PI
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) FLOAT
      PARAMETER ( FLOAT = 8)
C
C     **** Initializers ****
C
      DATA E / 2.7182818284590450908D00 /
      DATA PI / 3.141592653589793116D00 /
C
C     **** Statements ****
C
      END MODULE

      PROGRAM constants
      use w2f__types
      use single
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) ESLEN
      CHARACTER(60) E_SINGLE
      EXTERNAL more_precision
      INTEGER(w2f__i4) PISLEN
      CHARACTER(60) PI_SINGLE
C
C     **** Statements ****
C
      WRITE(PI_SINGLE, *) PI
      WRITE(E_SINGLE, *) E
      PISLEN = LEN_TRIM(ADJUSTL(PI_SINGLE))
      ESLEN = LEN_TRIM(ADJUSTL(E_SINGLE))
      IF((ESLEN .GE. 7) .AND.(PISLEN .GE. 7)) THEN
        CALL more_precision()
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM

      SUBROUTINE more_precision()
      use w2f__types
      use double
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) EDLEN
      CHARACTER(60) E_DOUBLE
      INTEGER(w2f__i4) PIDLEN
      CHARACTER(60) PI_DOUBLE
C
C     **** Statements ****
C
      WRITE(PI_DOUBLE, *) PI
      WRITE(E_DOUBLE, *) E
      PIDLEN = LEN_TRIM(ADJUSTL(PI_DOUBLE))
      EDLEN = LEN_TRIM(ADJUSTL(E_DOUBLE))
      IF((EDLEN .GE. 16) .AND.(PIDLEN .GE. 16)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      END SUBROUTINE
