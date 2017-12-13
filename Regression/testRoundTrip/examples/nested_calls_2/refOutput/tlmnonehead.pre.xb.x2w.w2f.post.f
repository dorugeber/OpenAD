
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
      EXTERNAL foo
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL foo(X(1),X(2),Y(1))
      CALL foo(X(1),X(2),Y(1))
      OpenAD_aux_0 = (X(1)%v*X(2)%v)
      OpenAD_lin_1 = X(2)%v
      OpenAD_lin_2 = X(1)%v
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      Y(2)%v = SIN(OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_1*OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_2*OpenAD_lin_0)
      CALL sax(OpenAD_acc_0,X(1),Y(2))
      CALL saxpy(OpenAD_acc_1,X(2),Y(2))
      OpenAD_lin_3 = COS(X(3)%v)
      Y(3)%v = SIN(X(3)%v)
      CALL sax(OpenAD_lin_3,X(3),Y(3))
      OpenAD_lin_4 = (-SIN(X(4)%v))
      Y(4)%v = COS(X(4)%v)
      CALL sax(OpenAD_lin_4,X(4),Y(4))
      END SUBROUTINE

      SUBROUTINE foo(A, B, C)
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
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL bar(B)
      OpenAD_lin_5 = A%v
      OpenAD_lin_6 = A%v
      C%v = (B%v+A%v*A%v)
      CALL setderiv(C,B)
      CALL saxpy(OpenAD_lin_5,A,C)
      CALL saxpy(OpenAD_lin_6,A,C)
      END SUBROUTINE

      SUBROUTINE bar(A)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_1 = (A%v*A%v)
      OpenAD_lin_8 = A%v
      OpenAD_lin_9 = A%v
      OpenAD_lin_7 = (-SIN(OpenAD_aux_1))
      A%v = COS(OpenAD_aux_1)
      OpenAD_acc_2 = (OpenAD_lin_8*OpenAD_lin_7)
      OpenAD_acc_3 = (OpenAD_lin_9*OpenAD_lin_7)
      CALL setderiv(OpenAD_prp_0,A)
      CALL setderiv(OpenAD_prp_1,A)
      CALL sax(OpenAD_acc_2,OpenAD_prp_0,A)
      CALL saxpy(OpenAD_acc_3,OpenAD_prp_1,A)
      END SUBROUTINE
