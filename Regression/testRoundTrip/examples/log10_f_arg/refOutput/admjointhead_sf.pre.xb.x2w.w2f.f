
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
      use oad_intrinsics
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
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_lin_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = LOG10(__value__(X(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = (INT(1_w2f__i8) /(__value__(X(1)) * LOG(1.0D+01)))
      __value__(Y(1)) = LOG10(__value__(X(1)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = (INT(1_w2f__i8) /(__value__(X(1)) * LOG(1.0D+01)))
      __value__(Y(1)) = LOG10(__value__(X(1)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
