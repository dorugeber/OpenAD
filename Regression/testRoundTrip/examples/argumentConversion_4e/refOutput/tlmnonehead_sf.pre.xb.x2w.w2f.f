
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
      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL barext
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) T(1 : 2, 1 : 3)
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
        __value__(T(INT(I), 2)) = __value__(X(I))
        CALL setderiv(__deriv__(T(I, 2)), __deriv__(X(I)))
      END DO
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0, __deriv__(T))
      CALL barext(OpenAD_tyc_0(1, 2))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(T), OpenAD_tyc_0)
      __value__(Y(1)) = __value__(T(1, 2))
      CALL setderiv(__deriv__(Y(1)), __deriv__(T(1, 2)))
      END SUBROUTINE
