
      PROGRAM doup
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      SAVE J
      INTEGER(w2f__i4) K
      SAVE K
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) S
      INTEGER(w2f__i4) U
C
C     **** Initializers ****
C
      DATA J / 1 /
      DATA K / 1 /
C
C     **** Statements ****
C
      L = 1
      U = 3
      S = 1
      DO I = L, U, S
        J = I + J
      END DO
      L = 3
      U = 1
      S = -1
      DO I = L, U, S
        K = I + K
      END DO
      IF((J .eq. 7) .AND.(K .eq. 7)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
