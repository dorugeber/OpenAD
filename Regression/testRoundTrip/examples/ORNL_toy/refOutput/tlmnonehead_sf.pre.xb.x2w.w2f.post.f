
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
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) P
      REAL(w2f__8) Q
      EXTERNAL sq
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      CALL sq(2,X,Y)
      P = 2.0D00
      Q = (P*P)
      END SUBROUTINE

      SUBROUTINE sq(N, U, V)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) N
      type(active) :: U(1:2)
      type(active) :: V(1:2)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Statements ****
C
      OpenAD_aux_0 = SQRT((U(1)%v**2)+(U(2)%v**2))
      OpenAD_lin_1 = (2*(U(1)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_2 = (2*(U(2)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_0 = (5.0D-01/OpenAD_aux_0)
      V(1)%v = OpenAD_aux_0
      OpenAD_acc_0 = (OpenAD_lin_2*OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_1*OpenAD_lin_0)
      CALL sax(OpenAD_acc_0,U(2),V(1))
      CALL saxpy(OpenAD_acc_1,U(1),V(1))
      OpenAD_aux_1 = (U(1)%v*2.0D00)
      OpenAD_lin_3 = (3*(U(1)%v**(3-INT(1_w2f__i8))))
      OpenAD_lin_4 = COS(U(2)%v)
      OpenAD_lin_5 = (INT(1_w2f__i8)/U(1)%v)
      OpenAD_lin_6 = (-(U(2)%v/(U(1)%v*U(1)%v)))
      OpenAD_lin_7 = (-SIN(U(1)%v))
      OpenAD_lin_8 = (3*(U(2)%v**(3-INT(1_w2f__i8))))
      OpenAD_lin_9 = OpenAD_aux_1
      OpenAD_lin_10 = U(2)%v
      V(2)%v = ((U(1)%v**3)+SIN(U(2)%v)+(U(2)%v/U(1)%v)-COS(U(1)%v)+(U(2
     +)%v**3)+U(2)%v*OpenAD_aux_1)
      OpenAD_acc_2 = (OpenAD_lin_7*INT((-1_w2f__i8)))
      OpenAD_acc_3 = (2.0D00*OpenAD_lin_10)
      CALL sax(OpenAD_lin_8,U(2),V(2))
      CALL saxpy(OpenAD_lin_3,U(1),V(2))
      CALL saxpy(OpenAD_acc_2,U(1),V(2))
      CALL saxpy(OpenAD_lin_4,U(2),V(2))
      CALL saxpy(OpenAD_lin_5,U(2),V(2))
      CALL saxpy(OpenAD_lin_6,U(1),V(2))
      CALL saxpy(OpenAD_lin_9,U(2),V(2))
      CALL saxpy(OpenAD_acc_3,U(1),V(2))
      END SUBROUTINE
