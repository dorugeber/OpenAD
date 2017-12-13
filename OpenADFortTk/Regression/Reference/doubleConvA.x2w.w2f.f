
      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) D(1 : 2)
      INTEGER(w2f__i4) I(1 : 2)
      SAVE I
C
C     **** Temporary Variables ****
C
      INTEGER(w2f__i8) tmp0
C
C     **** Initializers ****
C
      DATA(I(tmp0), tmp0 = 1, 2, 1) / 1, 2 /
C
C     **** Statements ****
C
      D(1 : 2) = DBLE(I)
      WRITE(*, *) D
      
      END PROGRAM
