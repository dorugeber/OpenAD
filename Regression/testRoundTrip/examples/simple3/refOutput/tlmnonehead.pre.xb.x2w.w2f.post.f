
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X1, X2, Y1, Y2)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X1
      type(active) :: X2
      type(active) :: Y1
      type(active) :: Y2
C
C     **** Local Variables and Functions ****
C
      type(active) :: V3
      type(active) :: V4
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X1)
C$OPENAD INDEPENDENT(X2)
C$OPENAD DEPENDENT(Y1)
C$OPENAD DEPENDENT(Y2)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = X2%v
      OpenAD_lin_1 = X1%v
      V3%v = (X1%v*X2%v)
      OpenAD_lin_2 = V3%v
      OpenAD_lin_3 = X1%v
      V4%v = (X1%v*V3%v)
      OpenAD_aux_0 = (X2%v*V3%v)
      OpenAD_lin_4 = OpenAD_aux_0
      OpenAD_lin_6 = V3%v
      OpenAD_lin_7 = X2%v
      OpenAD_lin_5 = V4%v
      Y1%v = (V4%v*OpenAD_aux_0)
      OpenAD_lin_8 = COS(V4%v)
      Y2%v = SIN(V4%v)
      OpenAD_acc_0 = (OpenAD_lin_6*OpenAD_lin_5)
      OpenAD_acc_1 = (OpenAD_lin_7*OpenAD_lin_5+OpenAD_lin_3*OpenAD_lin_
     +4)
      OpenAD_acc_2 = (OpenAD_lin_3*OpenAD_lin_8)
      OpenAD_acc_3 = (OpenAD_lin_2*OpenAD_lin_4)
      OpenAD_acc_4 = (OpenAD_lin_2*OpenAD_lin_8)
      CALL sax(OpenAD_lin_0,X1,V3)
      CALL saxpy(OpenAD_lin_1,X2,V3)
      CALL sax(OpenAD_acc_2,V3,Y2)
      CALL saxpy(OpenAD_acc_4,X1,Y2)
      CALL sax(OpenAD_acc_0,X2,Y1)
      CALL saxpy(OpenAD_acc_1,V3,Y1)
      CALL saxpy(OpenAD_acc_3,X1,Y1)
      END SUBROUTINE
