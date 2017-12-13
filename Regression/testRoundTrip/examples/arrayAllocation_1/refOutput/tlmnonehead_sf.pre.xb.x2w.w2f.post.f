
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
      type(active) :: Z(1:INT(SIZE(X)))
      REAL(w2f__8) OpenAD_acc_0(1 : 2)
      REAL(w2f__8) OpenAD_acc_1(1 : 2)
      REAL(w2f__8) OpenAD_acc_2(1 : 2)
      REAL(w2f__8) OpenAD_aux_0(1 : 2)
      REAL(w2f__8) OpenAD_aux_1(1 : 2)
      REAL(w2f__8) OpenAD_lin_0(1 : 2)
      REAL(w2f__8) OpenAD_lin_1(:)
      ALLOCATABLE OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2(1 : 2)
      REAL(w2f__8) OpenAD_lin_3(1 : 2)
      REAL(w2f__8) OpenAD_lin_4(1 : 2)
      REAL(w2f__8) OpenAD_lin_5(:)
      ALLOCATABLE OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6(1 : 2)
      type(active) :: OpenAD_prp_0(1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Z(1:INT(SIZE(X)))%v = X(1:2)%v
      CALL setderiv(Z(1:INT(SIZE(X))),X(1:2))
      OpenAD_aux_0 = COS(X%v)
      OpenAD_lin_0 = OpenAD_aux_0
      OpenAD_lin_2 = (-SIN(X%v))
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_1,Z(1:INT(SIZE(X))))
      OpenAD_lin_1 = Z(1:INT(SIZE(X)))%v
      Y(1:2)%v = (Z(1:INT(SIZE(X)))%v*OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_2*OpenAD_lin_1)
      CALL sax(OpenAD_lin_0,Z(1:INT(SIZE(X))),Y(1:2))
      CALL saxpy(OpenAD_acc_0,X,Y(1:2))
      OpenAD_aux_1 = (X(1:2)%v*Z(1:INT(SIZE(X)))%v)
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_lin_5,Z(1:INT(SIZE(X))))
      OpenAD_lin_5 = Z(1:INT(SIZE(X)))%v
      OpenAD_lin_6 = X(1:2)%v
      OpenAD_lin_3 = (INT(1_w2f__i8)/Y(1:2)%v)
      OpenAD_lin_4 = (-(OpenAD_aux_1/(Y(1:2)%v*Y(1:2)%v)))
      Y(1:2)%v = (OpenAD_aux_1/Y(1:2)%v)
      OpenAD_acc_1 = (OpenAD_lin_5*OpenAD_lin_3)
      OpenAD_acc_2 = (OpenAD_lin_6*OpenAD_lin_3)
      CALL setderiv(OpenAD_prp_0,Y(1:2))
      CALL sax(OpenAD_lin_4,OpenAD_prp_0,Y(1:2))
      CALL saxpy(OpenAD_acc_1,X(1:2),Y(1:2))
      CALL saxpy(OpenAD_acc_2,Z(1:INT(SIZE(X))),Y(1:2))
      END SUBROUTINE
