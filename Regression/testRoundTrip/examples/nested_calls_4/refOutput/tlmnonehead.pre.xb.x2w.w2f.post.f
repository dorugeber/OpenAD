
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:4)
      type(active) :: Y(1:4)
C
C     **** Local Variables and Functions ****
C
      type(active) :: C
      type(active) :: D
      EXTERNAL foo
      REAL(w2f__8) P
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_tyc_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      P = 2.0D00
      CALL foo(X(1),X(2),C,D)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,P)
      CALL foo(X(1),OpenAD_tyc_0,C,D)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P,OpenAD_tyc_0)
      OpenAD_lin_0 = D%v
      OpenAD_lin_1 = C%v
      Y(3)%v = (C%v*D%v)
      CALL sax(OpenAD_lin_0,C,Y(3))
      CALL saxpy(OpenAD_lin_1,D,Y(3))
      Y(4)%v = (C%v+D%v)
      CALL setderiv(Y(4),C)
      CALL inc_deriv(Y(4),D)
      Y(1)%v = X(3)%v
      CALL setderiv(Y(1),X(3))
      Y(2)%v = X(4)%v
      CALL setderiv(Y(2),X(4))
      END SUBROUTINE

      SUBROUTINE foo(A, B, C, D)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
      type(active) :: B
      type(active) :: C
      type(active) :: D
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = (A%v*B%v)
      OpenAD_lin_3 = B%v
      OpenAD_lin_4 = A%v
      OpenAD_lin_2 = COS(OpenAD_aux_0)
      C%v = SIN(OpenAD_aux_0)
      OpenAD_aux_1 = (A%v+B%v)
      OpenAD_lin_5 = (-SIN(OpenAD_aux_1))
      D%v = COS(OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3*OpenAD_lin_2)
      OpenAD_acc_1 = (OpenAD_lin_4*OpenAD_lin_2)
      CALL setderiv(OpenAD_prp_0,A)
      CALL inc_deriv(OpenAD_prp_0,B)
      CALL sax(OpenAD_acc_0,A,C)
      CALL saxpy(OpenAD_acc_1,B,C)
      CALL sax(OpenAD_lin_5,OpenAD_prp_0,D)
      END SUBROUTINE
