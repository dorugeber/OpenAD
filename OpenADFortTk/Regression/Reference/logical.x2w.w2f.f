
      PROGRAM logical
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) A
      LOGICAL(w2f__i4) B
      LOGICAL(w2f__i4) C
C
C     **** Statements ****
C
      A = .TRUE.
      B = .FALSE.
      C = .FALSE.
      IF(B .AND.(A .OR. C)) THEN
        WRITE(*, *) A
      ELSE
        WRITE(*, *) B
      ENDIF
      
      END PROGRAM
