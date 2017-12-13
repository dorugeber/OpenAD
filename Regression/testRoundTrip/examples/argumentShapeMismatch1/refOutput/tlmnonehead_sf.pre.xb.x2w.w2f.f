
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      MODULE m1
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
        TYPE (OpenADTy_active) X(1 : N)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        INTEGER(w2f__i4) OAD_CTMP0
        TYPE (OpenADTy_active) OpenAD_prp_0
        REAL(w2f__8) OpenAD_tyc_0(:)
        ALLOCATABLE OpenAD_tyc_0
C
C       **** Statements ****
C
        DO I = 1, N, 1
          __value__(X(INT(I))) = (__value__(X(I)) * 2.0D00)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X(I)))
          CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(X(I)))
        END DO
        I = 2
        OAD_CTMP0 = (SIZE(X) - I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
        CALL FOO(OpenAD_tyc_0(I), OAD_CTMP0)
C       $OpenAD$ INLINE oad_ShapeTest(subst,subst)
        CALL oad_ShapeTest(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use m2
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      CALL BAR(__deriv__(X), 4)
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
      CALL setderiv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
      END SUBROUTINE
