
C$OPENAD XXX File_start [head.f]
      SUBROUTINE bar()
      use OAD_active
      use w2f__types
      use oad_intrinsics
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

      SUBROUTINE foo()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      type(active) :: G
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      G%v = (G%v*2.0D00)
      CALL setderiv(OpenAD_prp_0,G)
      CALL sax(2.0D00,OpenAD_prp_0,G)
      END SUBROUTINE

      SUBROUTINE foo1()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      type(active) :: G
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
      G%v = (G%v*3.0D00)
      CALL setderiv(OpenAD_prp_1,G)
      CALL sax(3.0D00,OpenAD_prp_1,G)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      type(active) :: G
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      G%v = X(1)%v
      CALL setderiv(G,X(1))
      CALL bar()
      Y(1)%v = G%v
      CALL setderiv(Y(1),G)
      END SUBROUTINE
