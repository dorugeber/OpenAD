
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ S, T
      type(active) :: S
      type(active) :: T
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = COS(S%v)
      T%v = SIN(S%v)
      CALL sax(OpenAD_lin_0,S,T)
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
      COMMON /cb/ S, T
      type(active) :: S
      type(active) :: T
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      S%v = X(1)%v
      CALL setderiv(S,X(1))
      CALL foo()
      Y(1)%v = T%v
      CALL setderiv(Y(1),T)
      END SUBROUTINE
