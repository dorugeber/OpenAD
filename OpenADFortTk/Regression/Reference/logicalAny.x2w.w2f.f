
      PROGRAM logicalany
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) A(1 : 3)
      LOGICAL(w2f__i4) B(1 : 2)
C
C     **** Statements ****
C
      A(1) = .TRUE.
      A(2) = .FALSE.
      A(3) = .FALSE.
      B(1) = .FALSE.
      B(2) = .FALSE.
      IF(ANY(A) .AND.(.NOT. ANY(B))) THEN
        WRITE(*, *) A, B
      ELSE
        WRITE(*, *) 'not ok'
      ENDIF
      
      END PROGRAM
