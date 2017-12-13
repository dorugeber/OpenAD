
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
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) C
      TYPE (OpenADTy_active) D
      EXTERNAL foo
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
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
      __value__(Y(1)) = __value__(X(3))
      __value__(Y(2)) = __value__(X(4))
      CALL foo(__deriv__(X(1)), __deriv__(X(2)), __deriv__(C),
     >  __deriv__(D))
      __value__(Y(3)) = (__value__(C) * __value__(D))
      __value__(Y(4)) = (__value__(C) + __value__(D))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = __value__(X(3))
      __value__(Y(2)) = __value__(X(4))
      CALL foo(__deriv__(X(1)), __deriv__(X(2)), __deriv__(C),
     >  __deriv__(D))
      OpenAD_lin_0 = __value__(D)
      OpenAD_lin_1 = __value__(C)
      __value__(Y(3)) = (__value__(C) * __value__(D))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_1)
      __value__(Y(4)) = (__value__(C) + __value__(D))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(4)), __deriv__(D))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(4)), __deriv__(C))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(4)))
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(3)), __deriv__(D))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y(3)), __deriv__(C))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(3)))
      CALL foo(__deriv__(X(1)), __deriv__(X(2)), __deriv__(C),
     >  __deriv__(D))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(2)), __deriv__(X(4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(3)))
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
      __value__(Y(1)) = __value__(X(3))
      __value__(Y(2)) = __value__(X(4))
      CALL foo(__deriv__(X(1)), __deriv__(X(2)), __deriv__(C),
     >  __deriv__(D))
      OpenAD_lin_0 = __value__(D)
      OpenAD_lin_1 = __value__(C)
      __value__(Y(3)) = (__value__(C) * __value__(D))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_1)
      __value__(Y(4)) = (__value__(C) + __value__(D))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(4)), __deriv__(D))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(4)), __deriv__(C))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(4)))
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(3)), __deriv__(D))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y(3)), __deriv__(C))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(3)))
      CALL foo(__deriv__(X(1)), __deriv__(X(2)), __deriv__(C),
     >  __deriv__(D))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(2)), __deriv__(X(4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(2)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(3)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE foo(A, B, C, D)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) A
      TYPE (OpenADTy_active) B
      TYPE (OpenADTy_active) C
      TYPE (OpenADTy_active) D
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(C) = SIN(__value__(A) * __value__(B))
      __value__(D) = COS(__value__(A) + __value__(B))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = (__value__(A) * __value__(B))
      OpenAD_lin_3 = __value__(B)
      OpenAD_lin_4 = __value__(A)
      OpenAD_lin_2 = COS(OpenAD_aux_0)
      __value__(C) = SIN(OpenAD_aux_0)
      OpenAD_aux_1 = (__value__(A) + __value__(B))
      OpenAD_lin_5 = (- SIN(OpenAD_aux_1))
      __value__(D) = COS(OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_2)
      OpenAD_acc_1 = (OpenAD_lin_4 * OpenAD_lin_2)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_4)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(D), __deriv__(OpenAD_prp_0)
     > )
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(D))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(C), __deriv__(B))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(C), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(C))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(B))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(B))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(B))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(C))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(D))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(B))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(B))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(D))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(C))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = (__value__(A) * __value__(B))
      OpenAD_lin_3 = __value__(B)
      OpenAD_lin_4 = __value__(A)
      OpenAD_lin_2 = COS(OpenAD_aux_0)
      __value__(C) = SIN(OpenAD_aux_0)
      OpenAD_aux_1 = (__value__(A) + __value__(B))
      OpenAD_lin_5 = (- SIN(OpenAD_aux_1))
      __value__(D) = COS(OpenAD_aux_1)
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_2)
      OpenAD_acc_1 = (OpenAD_lin_4 * OpenAD_lin_2)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_4)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(D), __deriv__(OpenAD_prp_0)
     > )
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(D))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(C), __deriv__(B))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(C), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(C))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(B))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(C))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(D))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(D))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(C))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
