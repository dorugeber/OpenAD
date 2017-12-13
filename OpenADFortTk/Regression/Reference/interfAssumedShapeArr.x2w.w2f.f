
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface 
        SUBROUTINE foo(I)
        use w2f__types
        INTEGER(w2f__i4) I(1 : 1)
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(I)
      use w2f__types
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) I(1 : *)
C
C     **** Statements ****
C
      WRITE(*, *) I(2)
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      use m
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I(1 : 2)
C
C     **** Statements ****
C
      I(1) = 1
      I(2) = 2
      CALL foo(I)
      
      END PROGRAM
