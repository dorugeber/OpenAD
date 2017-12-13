
      SUBROUTINE foo()
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      REAL(w2f__8) G
C
C     **** Statements ****
C
      G = (G * 2.0D00)
      END SUBROUTINE

      SUBROUTINE foo1()
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      REAL(w2f__8) G
C
C     **** Statements ****
C
      G = (G * 2.0D00)
      END SUBROUTINE

      SUBROUTINE bar()
      use w2f__types
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      EXTERNAL foo1
C
C     **** Statements ****
C
      CALL foo()
      CALL foo1()
      END SUBROUTINE

      PROGRAM p
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      REAL(w2f__8) G
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
C
C     **** Statements ****
C
      G = 1.0D00
      CALL bar()
      WRITE(*, *) G
      
      END PROGRAM
