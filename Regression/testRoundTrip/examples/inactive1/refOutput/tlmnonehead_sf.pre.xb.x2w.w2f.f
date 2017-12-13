
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
      MODULE m
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  I
        module procedure  P2
        module procedure  P1

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
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) THEINT
      INTEGER(w2f__i4) OpenAD_lin_0
      TYPE (OpenADTy_active) OpenAD_prp_0
      REAL(w2f__8) OpenAD_tyc_0
      REAL(w2f__8) OpenAD_tyc_1
      REAL(w2f__8) OpenAD_tyc_2
C
C     **** Statements ****
C
      THEINT = 2
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
      CALL P1(OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
      OpenAD_lin_0 = THEINT
      __value__(X) = (THEINT * __value__(X))
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(X))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1, __deriv__(X))
      CALL P1(OpenAD_tyc_1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(X), OpenAD_tyc_1)
      CALL P2(THEINT)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_2, __deriv__(X))
      CALL P1(OpenAD_tyc_2)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(X), OpenAD_tyc_2)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      TYPE (OpenADTy_active) OpenAD_prp_1(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      __value__(X(1 : 1)) = (__value__(X(1 : 1)) * 2.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(X(1 : 1)))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_1), __deriv__(X(1 : 1)))
      CALL foo(__deriv__(X))
      __value__(Y(1 : 1)) = __value__(X(1 : 1))
      CALL setderiv(__deriv__(Y(1 : 1)), __deriv__(X(1 : 1)))
      END SUBROUTINE
