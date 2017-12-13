
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
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = __value__(X(2))
      OpenAD_lin_1 = __value__(X(1))
      __value__(Y) = (__value__(X(1)) * __value__(X(2)))
      CALL sax(OpenAD_lin_0, __deriv__(X(1)), __deriv__(Y))
      CALL saxpy(OpenAD_lin_1, __deriv__(X(2)), __deriv__(Y))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__8) PX(1 : 2)
      REAL(w2f__8) PY
      TYPE (OpenADTy_active) OpenAD_tyc_0(1 : 2)
      TYPE (OpenADTy_active) OpenAD_tyc_1
      TYPE (OpenADTy_active) OpenAD_tyc_2(1 : 2)
      TYPE (OpenADTy_active) OpenAD_tyc_3
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      PX(1) = 1.0
      PX(2) = 2.0
      CALL foo(__deriv__(X), __deriv__(Y))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), PX)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), PY)
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PX, __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_2), PX)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_3), PY)
      CALL foo(__deriv__(OpenAD_tyc_2), __deriv__(OpenAD_tyc_3))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PX, __deriv__(OpenAD_tyc_2))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_3))
      END SUBROUTINE
