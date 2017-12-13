
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
      use oad_intrinsics
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
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(Y) = (__value__(X) * 2.0D00)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y) = (__value__(X) * 2.0D00)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(2.0D00, __deriv__(Y), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      __value__(Y) = (__value__(X) * 2.0D00)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(2.0D00, __deriv__(Y), __deriv__(X))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
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
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) L
      REAL(w2f__8) P(1 : 2)
      REAL(w2f__8) Q(1 : 2)
      TYPE (OpenADTy_active) OpenAD_tyc_0
      TYPE (OpenADTy_active) OpenAD_tyc_1
      TYPE (OpenADTy_active) OpenAD_tyc_2
      TYPE (OpenADTy_active) OpenAD_tyc_3
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
      K = 1
      CALL foo(__deriv__(X(K)), __deriv__(Y))
      P(1) = 1.0
      L = 1
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P(K))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), Q(L))
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P(K), __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(Q(L), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      K = 1
      CALL foo(__deriv__(X(K)), __deriv__(Y))
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(K)
      P(1) = 1.0
      L = 1
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P(K))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), Q(L))
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P(K), __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(Q(L), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(K)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(L)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(L)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(K)
      CALL foo(__deriv__(OpenAD_tyc_2), __deriv__(OpenAD_tyc_3))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(K)
      CALL foo(__deriv__(X(K)), __deriv__(Y))
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
      K = 1
      CALL foo(__deriv__(X(K)), __deriv__(Y))
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(K)
      P(1) = 1.0
      L = 1
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P(K))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), Q(L))
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P(K), __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(Q(L), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(K)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(L)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(L)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(K)
      CALL foo(__deriv__(OpenAD_tyc_2), __deriv__(OpenAD_tyc_3))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(K)
      CALL foo(__deriv__(X(K)), __deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
