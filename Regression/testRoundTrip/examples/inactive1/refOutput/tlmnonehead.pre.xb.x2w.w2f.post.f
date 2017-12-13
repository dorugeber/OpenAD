
C$OPENAD XXX File_start [head.f]
      MODULE m
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface I
        module procedure P2
        module procedure P1

      end interface

C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE P1(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__8) X
C
C       **** Statements ****
C
        END SUBROUTINE

        SUBROUTINE P2(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
C
C       **** Statements ****
C
        END SUBROUTINE
      END

      SUBROUTINE foo(X)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) THEINT
      INTEGER(w2f__i4) OpenAD_lin_0
      type(active) :: OpenAD_prp_0
      REAL(w2f__8) OpenAD_tyc_0
      REAL(w2f__8) OpenAD_tyc_1
      REAL(w2f__8) OpenAD_tyc_2
C
C     **** Statements ****
C
      THEINT = 2
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,X)
      CALL P1(OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(X,OpenAD_tyc_0)
      OpenAD_lin_0 = THEINT
      X%v = (THEINT*X%v)
      CALL setderiv(OpenAD_prp_0,X)
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,X)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,X)
      CALL P1(OpenAD_tyc_1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(X,OpenAD_tyc_1)
      CALL P2(THEINT)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_2,X)
      CALL P1(OpenAD_tyc_2)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(X,OpenAD_tyc_2)
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
      type(active) :: OpenAD_prp_1(1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      X(1:1)%v = (X(1:1)%v*2.0D00)
      CALL setderiv(OpenAD_prp_1,X(1:1))
      CALL sax(2.0D00,OpenAD_prp_1,X(1:1))
      CALL foo(X)
      Y(1:1)%v = X(1:1)%v
      CALL setderiv(Y(1:1),X(1:1))
      END SUBROUTINE
