
      MODULE empty
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      MODULE a
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) AI
      INTEGER(w2f__i4) AJ
      INTEGER(w2f__i4) AK
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo()
      use w2f__types
      use a
      IMPLICIT NONE
C
C     **** Statements ****
C
      AJ = AI
      END SUBROUTINE

      PROGRAM multrename
      use w2f__types
      use empty
      use a ,only: AI , AK
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) AJ
      EXTERNAL foo
C
C     **** Statements ****
C
      AJ = 1
      AI = 2
      CALL foo()
      IF(AJ .eq. 1) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
