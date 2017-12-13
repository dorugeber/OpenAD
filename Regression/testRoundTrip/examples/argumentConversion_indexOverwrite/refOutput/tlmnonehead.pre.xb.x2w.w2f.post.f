
C$OPENAD XXX File_start [head.f]
      MODULE globals
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(P, Q)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: P
      INTENT(IN) P
      type(active) :: Q
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      OpenAD_lin_0 = Q%v
      OpenAD_lin_1 = P%v
      Q%v = (P%v*Q%v)
      I = (I+1)
      CALL setderiv(OpenAD_prp_0,Q)
      CALL sax(OpenAD_lin_0,P,Q)
      CALL saxpy(OpenAD_lin_1,OpenAD_prp_0,Q)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      type(active) :: V(1:2)
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      I = 1
      Y(1)%v = 1.0D00
      V(1)%v = 9.085830301E-03
      V(2)%v = (-2.9846420512E-04)
      CALL zero_deriv(Y(1))
      CALL zero_deriv(V(1))
      CALL zero_deriv(V(2))
      CALL foo(X(1),Y(1))
      CALL foo(X(I),V(I))
      OpenAD_lin_2 = V(2)%v
      OpenAD_lin_3 = Y(1)%v
      Y(1)%v = (Y(1)%v*V(2)%v)
      CALL setderiv(OpenAD_prp_1,Y(1))
      CALL setderiv(OpenAD_prp_2,V(2))
      CALL sax(OpenAD_lin_2,OpenAD_prp_1,Y(1))
      CALL saxpy(OpenAD_lin_3,OpenAD_prp_2,Y(1))
      END SUBROUTINE
