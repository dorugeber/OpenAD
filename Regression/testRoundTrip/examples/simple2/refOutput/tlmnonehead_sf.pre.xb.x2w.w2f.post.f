
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
      type(active) :: T1
      type(active) :: T2
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
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
      T1%v = (X1%v*X2%v)
      OpenAD_aux_0 = SIN(T1%v)
      OpenAD_lin_2 = OpenAD_aux_0
      OpenAD_lin_4 = COS(T1%v)
      OpenAD_lin_3 = X1%v
      T2%v = (X1%v*OpenAD_aux_0)
      OpenAD_lin_5 = (-SIN(T2%v))
      Y1%v = COS(T2%v)
      OpenAD_lin_6 = T2%v
      OpenAD_lin_7 = X2%v
      Y2%v = (X2%v*T2%v)
      OpenAD_acc_0 = (OpenAD_lin_4*OpenAD_lin_3)
      OpenAD_acc_1 = (OpenAD_lin_0*OpenAD_acc_0)
      OpenAD_acc_2 = (OpenAD_lin_1*OpenAD_acc_0)
      CALL sax(OpenAD_lin_2,X1,T2)
      CALL saxpy(OpenAD_acc_1,X1,T2)
      CALL saxpy(OpenAD_acc_2,X2,T2)
      CALL sax(OpenAD_lin_5,T2,Y1)
      CALL sax(OpenAD_lin_6,X2,Y2)
      CALL saxpy(OpenAD_lin_7,T2,Y2)
      END SUBROUTINE
