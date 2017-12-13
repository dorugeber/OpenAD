
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
      SUBROUTINE foo(M1, M2)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) M1
      INTENT(IN) M1
      type(active) :: M2
C
C     **** Local Variables and Functions ****
C
      type(active) :: OAD_CTMP0
      type(active) :: OpenAD_tyc_0
C
C     **** Statements ****
C
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,M1)
      CALL OAD_S_MIN_D(OpenAD_tyc_0,M2,OAD_CTMP0)
      M2%v = OAD_CTMP0%v
      CALL setderiv(M2,OAD_CTMP0)
      END SUBROUTINE

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
      EXTERNAL foo
      type(active) :: OAD_CTMP0
      type(active) :: OAD_CTMP1
      REAL(w2f__8) P
      REAL(w2f__8) T
      type(active) :: T1
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
      T = 2.0D00
      P = SQRT(3.0D00)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,T)
      CALL OAD_S_MIN_D(OpenAD_tyc_1,X(1),OAD_CTMP0)
      T1%v = OAD_CTMP0%v
      CALL setderiv(T1,OAD_CTMP0)
      CALL OAD_S_MIN_D(X(1),T1,OAD_CTMP1)
      Y(1)%v = OAD_CTMP1%v
      CALL setderiv(Y(1),OAD_CTMP1)
      CALL foo(P,T1)
      END SUBROUTINE
