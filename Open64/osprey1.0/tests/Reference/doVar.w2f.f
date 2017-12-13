C ***********************************************************
C Fortran file translated from WHIRL Fri Dec 12 14:51:51 2008
C ***********************************************************
C ***********************************************************

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
      INTEGER(w2f__i4) t__1
      INTEGER(w2f__i4) t__2
      INTEGER(w2f__i4) t__3
      INTEGER(w2f__i4) t__4
      INTEGER(w2f__i4) t__5
      INTEGER(w2f__i4) t__6
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
      t__1 = L
      t__2 = U
      t__3 = S
      DO I = t__1, t__2, t__3
        J = I + J
      END DO
      L = 3
      U = 1
      S = -1
      t__4 = L
      t__5 = U
      t__6 = S
      DO I = t__4, t__5, t__6
        K = I + K
      END DO
      IF((J .eq. 7) .AND.(K .eq. 7)) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'failed'
      ENDIF
      
      END PROGRAM
