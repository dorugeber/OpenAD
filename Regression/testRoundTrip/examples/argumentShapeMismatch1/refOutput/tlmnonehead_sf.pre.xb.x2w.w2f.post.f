
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
      MODULE m1
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO(X, N)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) N
        REAL(w2f__8) X(1 : N)
C
C       **** Statements ****
C
        END SUBROUTINE
      END

      MODULE m2
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m1
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR(X, N)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) N
        type(active) :: X(1:N)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        INTEGER(w2f__i4) OAD_CTMP0
        type(active) :: OpenAD_prp_0
        REAL(w2f__8) OpenAD_tyc_0(:)
        ALLOCATABLE OpenAD_tyc_0
C
C       **** Statements ****
C
        DO I = 1, N, 1
          X(INT(I))%v = (X(I)%v*2.0D00)
          CALL setderiv(OpenAD_prp_0,X(I))
          CALL sax(2.0D00,OpenAD_prp_0,X(I))
        END DO
        I = 2
        OAD_CTMP0 = (SIZE(X)-I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_0,X)
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(OpenAD_tyc_0,X)
        CALL FOO(OpenAD_tyc_0(I),OAD_CTMP0)
C       $OpenAD$ INLINE oad_ShapeTest(subst,subst)
        CALL oad_ShapeTest(OpenAD_tyc_0,X)
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(X,OpenAD_tyc_0)
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m2
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:4)
      type(active) :: Y(1:4)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      CALL BAR(X,4)
      Y(1:4)%v = X(1:4)%v
      CALL setderiv(Y(1:4),X(1:4))
      END SUBROUTINE
