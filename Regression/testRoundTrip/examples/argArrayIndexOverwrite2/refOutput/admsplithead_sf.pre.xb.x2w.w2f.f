
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(A, K)
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_Symbol_2
      TYPE (OpenADTy_active) OpenAD_Symbol_5
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) A
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_6
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      K = (K * 2)
      __value__(A) = (K * __value__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      K = (K * 2)
      OpenAD_Symbol_2 = (K * __value__(A))
      OpenAD_Symbol_1 = K
      __value__(A) = OpenAD_Symbol_2
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_6)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_6, __deriv__(A), __deriv__(
     > OpenAD_Symbol_5))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(A))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_5), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_5))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      K = (K * 2)
      OpenAD_Symbol_2 = (K * __value__(A))
      OpenAD_Symbol_1 = K
      __value__(A) = OpenAD_Symbol_2
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_6)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_6, __deriv__(A), __deriv__(
     > OpenAD_Symbol_5))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(A))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_5), __deriv__(A))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_5))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
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
      REAL(w2f__8) OpenAD_Symbol_7
      REAL(w2f__8) OpenAD_Symbol_8
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
      CALL foo(__deriv__(X(K)), K)
      K = 1
      CALL foo(__deriv__(X(K)), K)
      __value__(Y) = (__value__(X(1)) * __value__(X(2)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      K = 1
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(K)
      K = 1
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(K)
      __value__(Y) = (__value__(X(1)) * __value__(X(2)))
      OpenAD_Symbol_3 = __value__(X(2))
      OpenAD_Symbol_4 = __value__(X(1))
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_3)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_4)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_7)
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_8)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(Y), __deriv__(X(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(Y), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y))
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(K)
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(K)
      CALL foo(__deriv__(X(K)), K)
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
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(K)
      K = 1
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(K)
      __value__(Y) = (__value__(X(1)) * __value__(X(2)))
      OpenAD_Symbol_3 = __value__(X(2))
      OpenAD_Symbol_4 = __value__(X(1))
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_3)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_4)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_7)
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_8)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(Y), __deriv__(X(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(Y), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y))
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(K)
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(K)
      CALL foo(__deriv__(X(K)), K)
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
