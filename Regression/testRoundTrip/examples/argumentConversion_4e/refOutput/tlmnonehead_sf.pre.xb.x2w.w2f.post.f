
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
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL barext
      INTEGER(w2f__i4) I
      type(active) :: T(1:2,1:3)
      REAL(w2f__8) OpenAD_tyc_0(1 : 2, 1 : 3)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      DO I = 1, 2, 1
        T(INT(I),2)%v = X(I)%v
        CALL setderiv(T(I,2),X(I))
      END DO
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,T)
      CALL barext(OpenAD_tyc_0(1,2))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(T,OpenAD_tyc_0)
      Y(1)%v = T(1,2)%v
      CALL setderiv(Y(1),T(1,2))
      END SUBROUTINE
