
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:12)
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_3 = (X(8)%v+X(7)%v+X(5)%v+X(6)%v)
      OpenAD_aux_1 = (X(1)%v*OpenAD_aux_3+X(2)%v*X(4)%v)
      OpenAD_aux_4 = (X(12)%v+X(11)%v+X(9)%v+X(10)%v)
      OpenAD_aux_2 = (X(2)%v+X(1)%v*OpenAD_aux_4)
      OpenAD_aux_0 = (OpenAD_aux_1/OpenAD_aux_2)
      OpenAD_lin_0 = OpenAD_aux_0
      OpenAD_lin_4 = OpenAD_aux_3
      OpenAD_lin_5 = X(1)%v
      OpenAD_lin_6 = X(4)%v
      OpenAD_lin_7 = X(2)%v
      OpenAD_lin_2 = (INT(1_w2f__i8)/OpenAD_aux_2)
      OpenAD_lin_8 = OpenAD_aux_4
      OpenAD_lin_9 = X(1)%v
      OpenAD_lin_3 = (-(OpenAD_aux_1/(OpenAD_aux_2*OpenAD_aux_2)))
      OpenAD_lin_1 = X(3)%v
      Y(1)%v = (X(3)%v*OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_2*OpenAD_lin_1)
      OpenAD_acc_1 = (OpenAD_lin_3*OpenAD_lin_1)
      OpenAD_acc_2 = (OpenAD_lin_8*OpenAD_acc_1)
      OpenAD_acc_3 = (OpenAD_lin_9*OpenAD_acc_1)
      OpenAD_acc_4 = (OpenAD_lin_4*OpenAD_acc_0)
      OpenAD_acc_5 = (OpenAD_lin_5*OpenAD_acc_0)
      OpenAD_acc_6 = (OpenAD_lin_6*OpenAD_acc_0)
      OpenAD_acc_7 = (OpenAD_lin_7*OpenAD_acc_0)
      CALL setderiv(OpenAD_prp_0,X(8))
      CALL inc_deriv(OpenAD_prp_0,X(7))
      CALL inc_deriv(OpenAD_prp_0,X(5))
      CALL inc_deriv(OpenAD_prp_0,X(6))
      CALL setderiv(OpenAD_prp_1,X(12))
      CALL inc_deriv(OpenAD_prp_1,X(11))
      CALL inc_deriv(OpenAD_prp_1,X(9))
      CALL inc_deriv(OpenAD_prp_1,X(10))
      CALL sax(OpenAD_lin_0,X(3),Y(1))
      CALL saxpy(OpenAD_acc_2,X(1),Y(1))
      CALL saxpy(OpenAD_acc_3,OpenAD_prp_1,Y(1))
      CALL saxpy(OpenAD_acc_1,X(2),Y(1))
      CALL saxpy(OpenAD_acc_4,X(1),Y(1))
      CALL saxpy(OpenAD_acc_5,OpenAD_prp_0,Y(1))
      CALL saxpy(OpenAD_acc_6,X(2),Y(1))
      CALL saxpy(OpenAD_acc_7,X(4),Y(1))
      END SUBROUTINE
