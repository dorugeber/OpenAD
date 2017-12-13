
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

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
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      type(active) :: GAMMA
      type(active) :: NU
      type(active) :: OMEGA
      type(active) :: T
      type(active) :: V
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      type(active) :: OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      NU%v = X(1)%v
      GAMMA%v = X(2)%v
      OMEGA%v = X(3)%v
      T%v = X(4)%v
      OpenAD_aux_0 = TAN(OMEGA%v*T%v)
      OpenAD_lin_1 = T%v
      OpenAD_lin_2 = OMEGA%v
      OpenAD_lin_0 = (OpenAD_aux_0*OpenAD_aux_0+INT(1_w2f__i8))
      V%v = OpenAD_aux_0
      OpenAD_aux_1 = (NU%v*V%v)
      OpenAD_aux_2 = (GAMMA%v-V%v)
      OpenAD_lin_5 = V%v
      OpenAD_lin_6 = NU%v
      OpenAD_lin_3 = (INT(1_w2f__i8)/OpenAD_aux_2)
      OpenAD_lin_4 = (-(OpenAD_aux_1/(OpenAD_aux_2*OpenAD_aux_2)))
      Y(1)%v = (OpenAD_aux_1/OpenAD_aux_2)
      OpenAD_acc_0 = (OpenAD_lin_6*OpenAD_lin_3+INT((-1_w2f__i8))*OpenAD
     +_lin_4)
      OpenAD_acc_1 = (OpenAD_lin_5*OpenAD_lin_3)
      OpenAD_acc_2 = (OpenAD_lin_1*OpenAD_lin_0*OpenAD_acc_0)
      OpenAD_acc_3 = (OpenAD_lin_2*OpenAD_lin_0*OpenAD_acc_0)
      CALL setderiv(GAMMA,X(2))
      CALL sax(OpenAD_acc_1,X(1),Y(1))
      CALL saxpy(OpenAD_acc_2,X(3),Y(1))
      CALL saxpy(OpenAD_acc_3,X(4),Y(1))
      CALL saxpy(OpenAD_lin_4,GAMMA,Y(1))
      OpenAD_lin_7 = GAMMA%v
      OpenAD_lin_8 = Y(1)%v
      Y(2)%v = (Y(1)%v*GAMMA%v)
      CALL setderiv(OpenAD_prp_0,Y(1))
      CALL sax(OpenAD_lin_7,OpenAD_prp_0,Y(2))
      CALL saxpy(OpenAD_lin_8,GAMMA,Y(2))
      END SUBROUTINE
