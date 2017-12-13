C ***********************************************************
C Fortran file translated from WHIRL Tue Sep  9 13:00:36 2008
C ***********************************************************
C ***********************************************************

      PROGRAM main
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) A
      TARGET A
      REAL(w2f__4) B
      TARGET B
      REAL(w2f__4) P
      POINTER P
      REAL(w2f__4) Q
      POINTER Q
C
C     **** Statements ****
C
      NULLIFY ( P , Q)
      IF(ASSOCIATED(P)) THEN
        WRITE(*, *) 'failed'
      ENDIF
      A = 3.0
      P => A
      IF(.NOT. ASSOCIATED(P, A)) THEN
        WRITE(*, *) 'failed'
      ENDIF
      Q => B
      NULLIFY ( Q)
      IF(ASSOCIATED(Q)) THEN
        WRITE(*, *) 'failed'
      ELSE
        WRITE(*, *) 'OK'
      ENDIF
      
      END PROGRAM
