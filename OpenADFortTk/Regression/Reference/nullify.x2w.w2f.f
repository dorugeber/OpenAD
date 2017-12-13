
      PROGRAM nullifytest
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) X
      POINTER X
C
C     **** Statements ****
C
      NULLIFY ( X)
      IF(.NOT. ASSOCIATED(X)) THEN
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
