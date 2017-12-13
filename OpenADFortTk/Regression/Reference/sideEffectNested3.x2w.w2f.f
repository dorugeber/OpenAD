
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) L_X
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        EXTERNAL FOO
C
C       **** Statements ****
C
        L_X = (DBLE(L_X) * DBLE(2.0))
        END SUBROUTINE

        SUBROUTINE FOO(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__4) X
C
C       **** Statements ****
C
        L_X = X
        CALL BAR()
        X = L_X
        END SUBROUTINE
      END

      PROGRAM p
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) R
C
C     **** Statements ****
C
      R = 3.0
      CALL FOO(R)
      WRITE(*, *) R
      
      END PROGRAM
