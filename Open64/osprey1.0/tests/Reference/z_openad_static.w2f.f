
      PROGRAM myprog
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE  TEST
        INTEGER(w2f__i4) LIST(1 : 200)
      END TYPE
      
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) N
      PARAMETER ( N = 200)
      TYPE (TEST) ROWS(1 : 200)
C
C     **** Statements ****
C
      DO I = 1, 200, 1
        DO J = 1, 200, 1
          ROWS(I)%LIST(J) = 123
        END DO
      END DO
      IF(ROWS(10)%LIST(15) .eq. 123) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
