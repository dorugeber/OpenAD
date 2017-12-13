
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
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(Y) = (__value__(X(2)) * __value__(X(4)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = __value__(X(4))
      OpenAD_lin_1 = __value__(X(2))
      __value__(Y) = (__value__(X(2)) * __value__(X(4)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y), __deriv__(X(4)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y), __deriv__(X(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = __value__(X(4))
      OpenAD_lin_1 = __value__(X(2))
      __value__(Y) = (__value__(X(2)) * __value__(X(4)))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y), __deriv__(X(4)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y), __deriv__(X(2)))
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
      REAL(w2f__8) P(1 : 2, 1 : 2)
      REAL(w2f__8) PY
      TYPE (OpenADTy_active) T(1 : 2, 1 : 2)
      TYPE (OpenADTy_active) OpenAD_tyc_0(1 : 2, 1 : 2)
      TYPE (OpenADTy_active) OpenAD_tyc_1
      TYPE (OpenADTy_active) OpenAD_tyc_2(1 : 2, 1 : 2)
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
      __value__(T(2, 1)) = __value__(X(1))
      __value__(T(2, 2)) = __value__(X(2))
      CALL foo(__deriv__(T), __deriv__(Y))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), PY)
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P, __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_1))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(T(2, 1)) = __value__(X(1))
      __value__(T(2, 2)) = __value__(X(2))
      CALL foo(__deriv__(T), __deriv__(Y))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), PY)
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P, __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_1))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL foo(__deriv__(OpenAD_tyc_2), __deriv__(OpenAD_tyc_3))
      CALL foo(__deriv__(T), __deriv__(Y))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(T(2, 2)), __deriv__(X(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2, 2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(T(2, 1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2, 1)))
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
      __value__(T(2, 1)) = __value__(X(1))
      __value__(T(2, 2)) = __value__(X(2))
      CALL foo(__deriv__(T), __deriv__(Y))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), P)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), PY)
      CALL foo(__deriv__(OpenAD_tyc_0), __deriv__(OpenAD_tyc_1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P, __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(PY, __deriv__(OpenAD_tyc_1))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL foo(__deriv__(OpenAD_tyc_2), __deriv__(OpenAD_tyc_3))
      CALL foo(__deriv__(T), __deriv__(Y))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(T(2, 2)), __deriv__(X(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2, 2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(T(2, 1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(T(2, 1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
