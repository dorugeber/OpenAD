
      PROGRAM allocatedarray
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X(:)
      ALLOCATABLE X
C
C     **** Statements ****
C
      ALLOCATE(X(2))
      IF(ALLOCATED(X)) THEN
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
