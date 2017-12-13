
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      CHARACTER(3) SARRAY(1 : 3)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) N
      PARAMETER ( N = 3)
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(S, R)
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      CHARACTER(*) S
      LOGICAL(w2f__i4) R
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      DO I = 1, 3, 1
        IF(SARRAY(I) .EQ. S(1 : LEN(S))) THEN
          R = .TRUE.
        ENDIF
      END DO
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      LOGICAL(w2f__i4) L1
      LOGICAL(w2f__i4) L2
C
C     **** Statements ****
C
      SARRAY(1) = 'bla'
      SARRAY(2) = 'blo'
      SARRAY(3) = 'blu'
      CALL foo('ble', L1)
      CALL foo('blo', L2)
      IF(L2 .AND.(.NOT. L1)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'BAD'
      ENDIF
      
      END PROGRAM
