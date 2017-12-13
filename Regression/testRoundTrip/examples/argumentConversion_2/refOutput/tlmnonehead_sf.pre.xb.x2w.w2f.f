
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
      SUBROUTINE foo(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
      TYPE (OpenADTy_active) Y
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(Y) = __value__(X)
      CALL setderiv(__deriv__(Y), __deriv__(X))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__8) PY
      TYPE (OpenADTy_active) OpenAD_tyc_0
      TYPE (OpenADTy_active) OpenAD_tyc_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL foo(__deriv__(X), __deriv__(Y))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), 2.0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), PY)
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_1))
      END SUBROUTINE
