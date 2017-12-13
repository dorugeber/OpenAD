
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
      SUBROUTINE foo(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
      type(active) :: Y
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Y%v = X%v
      CALL setderiv(Y,X)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
      type(active) :: Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__8) PY
      type(active) :: OpenAD_tyc_0
      type(active) :: OpenAD_tyc_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL foo(X,Y)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,2.0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,PY)
      CALL foo(OpenAD_tyc_0,OpenAD_tyc_1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY,OpenAD_tyc_1)
      END SUBROUTINE
