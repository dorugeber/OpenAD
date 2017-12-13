
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Top Level Pragmas ****
!
interface OAD_S_MIN
  module procedure OAD_S_MIN_D

end interface

!
!     **** Statements ****
!
CONTAINS

  SUBROUTINE OAD_S_MIN_D(A0, A1, R)
  use w2f__types
  IMPLICIT NONE
!
!       **** Parameters and Result ****
!
  type(active) :: A0
  INTENT(IN) A0
  type(active) :: A1
  INTENT(IN) A1
  type(active) :: R
  INTENT(OUT) R
!
!       **** Statements ****
!
  IF (A0%v.LT.A1%v) THEN
    R%v = A0%v
    CALL setderiv(R,A0)
  ELSE
    R%v = A1%v
    CALL setderiv(R,A1)
  ENDIF
  END SUBROUTINE
END

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
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: OAD_CTMP0
      type(active) :: OAD_CTMP1
      REAL(w2f__8) OAD_CTMP2
      REAL(w2f__8) T
      type(active) :: T1
      REAL(w2f__8) T2
      type(active) :: OpenAD_tyc_0
      type(active) :: OpenAD_tyc_1
      type(active) :: OpenAD_tyc_2
      type(active) :: OpenAD_tyc_3
      type(active) :: OpenAD_tyc_4
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T = 2.0D00
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,T)
      CALL OAD_S_MIN_D(OpenAD_tyc_0,X(1),OAD_CTMP0)
      T1%v = OAD_CTMP0%v
      CALL setderiv(T1,OAD_CTMP0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,T)
      CALL OAD_S_MIN_D(T1,OpenAD_tyc_1,OAD_CTMP1)
      Y(1)%v = OAD_CTMP1%v
      CALL setderiv(Y(1),OAD_CTMP1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_2,T)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_3,2.5D00)
      CALL OAD_S_MIN_D(OpenAD_tyc_2,OpenAD_tyc_3,OpenAD_tyc_4)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OAD_CTMP2,OpenAD_tyc_4)
      T2 = OAD_CTMP2
      END SUBROUTINE
