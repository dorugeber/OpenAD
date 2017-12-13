
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
      type(active) :: X(1:)
      type(active) :: Y(1:)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL barext
      REAL(w2f__8) OpenAD_tyc_0(:)
      ALLOCATABLE OpenAD_tyc_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C     $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
      CALL oad_AllocateMatching(OpenAD_tyc_0,X)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,X)
      CALL barext(OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_ShapeTest(subst,subst)
      CALL oad_ShapeTest(OpenAD_tyc_0,X)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(X,OpenAD_tyc_0)
      Y(1)%v = (X(1)%v+X(2)%v)
      CALL setderiv(Y(1),X(1))
      CALL inc_deriv(Y(1),X(2))
      Y(2)%v = (-(X(1)%v+X(2)%v))
      CALL set_neg_deriv(Y(2),X(1))
      CALL dec_deriv(Y(2),X(2))
      END SUBROUTINE
