
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL FOO()
      CALL BAR()
      CONTAINS

        SUBROUTINE FOO()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_lin_0
        REAL(w2f__8) OpenAD_lin_1
C
C       **** Statements ****
C
        OpenAD_lin_0 = X(2)%v
        OpenAD_lin_1 = X(1)%v
        Y(1)%v = (X(1)%v*X(2)%v)
        CALL sax(OpenAD_lin_0,X(1),Y(1))
        CALL saxpy(OpenAD_lin_1,X(2),Y(1))
        END SUBROUTINE

        SUBROUTINE BAR()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_acc_0
        REAL(w2f__8) OpenAD_acc_1
        REAL(w2f__8) OpenAD_acc_2
        REAL(w2f__8) OpenAD_aux_0
        REAL(w2f__8) OpenAD_aux_1
        REAL(w2f__8) OpenAD_aux_2
        REAL(w2f__8) OpenAD_lin_2
        REAL(w2f__8) OpenAD_lin_3
        REAL(w2f__8) OpenAD_lin_4
        REAL(w2f__8) OpenAD_lin_5
        REAL(w2f__8) OpenAD_lin_6
        REAL(w2f__8) OpenAD_lin_7
        type(active) :: OpenAD_prp_0
C
C       **** Statements ****
C
        OpenAD_aux_0 = COS(X(2)%v)
        OpenAD_aux_2 = SIN(X(1)%v)
        OpenAD_aux_1 = (Y(1)%v*OpenAD_aux_2)
        OpenAD_lin_4 = (-SIN(X(2)%v))
        OpenAD_lin_2 = OpenAD_aux_1
        OpenAD_lin_5 = OpenAD_aux_2
        OpenAD_lin_7 = COS(X(1)%v)
        OpenAD_lin_6 = Y(1)%v
        OpenAD_lin_3 = OpenAD_aux_0
        Y(1)%v = (OpenAD_aux_0*OpenAD_aux_1)
        OpenAD_acc_0 = (OpenAD_lin_4*OpenAD_lin_2)
        OpenAD_acc_1 = (OpenAD_lin_5*OpenAD_lin_3)
        OpenAD_acc_2 = (OpenAD_lin_7*OpenAD_lin_6*OpenAD_lin_3)
        CALL setderiv(OpenAD_prp_0,Y(1))
        CALL sax(OpenAD_acc_0,X(2),Y(1))
        CALL saxpy(OpenAD_acc_1,OpenAD_prp_0,Y(1))
        CALL saxpy(OpenAD_acc_2,X(1),Y(1))
        END SUBROUTINE
      END
