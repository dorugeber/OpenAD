
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      MODULE globals
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) I
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(P, Q)
      use w2f__types
      use globals
      use globals
      use globals
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL(w2f__8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
      TYPE (OpenADTy_active) OpenAD_Symbol_8
      TYPE (OpenADTy_active) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) P
      TYPE (OpenADTy_active) Q
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_Symbol_13
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      __value__(Q) = (__value__(P) * __value__(Q))
      I = I + 1
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OpenAD_Symbol_4 = (__value__(P) * __value__(Q))
      OpenAD_Symbol_2 = __value__(Q)
      OpenAD_Symbol_3 = __value__(P)
      __value__(Q) = OpenAD_Symbol_4
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_2)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_3)
      I = I + 1
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_12)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(Q), __deriv__(
     > OpenAD_Symbol_9))
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_13)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_13, __deriv__(Q), __deriv__(
     > OpenAD_Symbol_8))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Q))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_9), __deriv__(Q))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_9))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_8), __deriv__(P))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_8))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Q))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Q))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Q))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Q))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Q))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Q))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(P))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
      OpenAD_Symbol_4 = (__value__(P) * __value__(Q))
      OpenAD_Symbol_2 = __value__(Q)
      OpenAD_Symbol_3 = __value__(P)
      __value__(Q) = OpenAD_Symbol_4
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_2)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_3)
      I = I + 1
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_12)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(Q), __deriv__(
     > OpenAD_Symbol_9))
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_13)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_13, __deriv__(Q), __deriv__(
     > OpenAD_Symbol_8))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Q))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_9), __deriv__(Q))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_9))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_8), __deriv__(P))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_8))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use globals
      use globals
      use globals
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) OpenAD_Symbol_0
      TYPE (OpenADTy_active) OpenAD_Symbol_10
      TYPE (OpenADTy_active) OpenAD_Symbol_11
      TYPE (OpenADTy_active) OpenAD_Symbol_16
      REAL(w2f__8) OpenAD_Symbol_5
      REAL(w2f__8) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_Symbol_7
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      REAL(w2f__8) V(1 : 2)
      INTEGER(w2f__i8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_Symbol_14
      REAL(w2f__8) OpenAD_Symbol_15
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
      I = 1
      V(1) = 9.085830301E-03
      V(2) = (-2.9846420512E-04)
      CALL foo(__deriv__(Y(1)), __deriv__(X(1)))
      OpenAD_Symbol_1 = I
C     $OpenAD$ INLINE convert_p2a_scalar(subst,subst)
      CALL convert_p2a_scalar(__deriv__(OpenAD_Symbol_0), V(I))
      CALL foo(__deriv__(X(I)), __deriv__(OpenAD_Symbol_0))
C     $OpenAD$ INLINE convert_a2p_scalar(subst,subst)
      CALL convert_a2p_scalar(V(INT(OpenAD_Symbol_1)), __deriv__(
     > OpenAD_Symbol_0))
      __value__(Y(1)) = (__value__(X(2)) * __value__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      I = 1
      V(1) = 9.085830301E-03
      V(2) = (-2.9846420512E-04)
      CALL foo(__deriv__(Y(1)), __deriv__(X(1)))
      OpenAD_Symbol_1 = I
C     $OpenAD$ INLINE convert_p2a_scalar(subst,subst)
      CALL convert_p2a_scalar(__deriv__(OpenAD_Symbol_0), V(I))
      CALL foo(__deriv__(X(I)), __deriv__(OpenAD_Symbol_0))
C     $OpenAD$ INLINE convert_a2p_scalar(subst,subst)
      CALL convert_a2p_scalar(V(INT(OpenAD_Symbol_1)), __deriv__(
     > OpenAD_Symbol_0))
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(OpenAD_Symbol_1)
      OpenAD_Symbol_7 = (__value__(X(2)) * __value__(Y(1)))
      OpenAD_Symbol_5 = __value__(Y(1))
      OpenAD_Symbol_6 = __value__(X(2))
      __value__(Y(1)) = OpenAD_Symbol_7
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_5)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_6)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_14)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_14, __deriv__(Y(1)), __deriv__(
     > OpenAD_Symbol_11))
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_15)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_15, __deriv__(Y(1)), __deriv__(
     > OpenAD_Symbol_10))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_11), __deriv__(Y(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_11))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_10), __deriv__(X(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_10))
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(I)
      CALL foo(__deriv__(X(I)), __deriv__(OpenAD_Symbol_16))
      CALL foo(__deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      I = 1
      V(1) = 9.085830301E-03
      V(2) = (-2.9846420512E-04)
      CALL foo(__deriv__(Y(1)), __deriv__(X(1)))
      OpenAD_Symbol_1 = I
C     $OpenAD$ INLINE convert_p2a_scalar(subst,subst)
      CALL convert_p2a_scalar(__deriv__(OpenAD_Symbol_0), V(I))
      CALL foo(__deriv__(X(I)), __deriv__(OpenAD_Symbol_0))
C     $OpenAD$ INLINE convert_a2p_scalar(subst,subst)
      CALL convert_a2p_scalar(V(INT(OpenAD_Symbol_1)), __deriv__(
     > OpenAD_Symbol_0))
C     $OpenAD$ INLINE push_i(subst)
      CALL push_i(OpenAD_Symbol_1)
      OpenAD_Symbol_7 = (__value__(X(2)) * __value__(Y(1)))
      OpenAD_Symbol_5 = __value__(Y(1))
      OpenAD_Symbol_6 = __value__(X(2))
      __value__(Y(1)) = OpenAD_Symbol_7
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_5)
C     $OpenAD$ INLINE push(subst)
      CALL push(OpenAD_Symbol_6)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_14)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_14, __deriv__(Y(1)), __deriv__(
     > OpenAD_Symbol_11))
C     $OpenAD$ INLINE Pop(subst)
      CALL Pop(OpenAD_Symbol_15)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_15, __deriv__(Y(1)), __deriv__(
     > OpenAD_Symbol_10))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_11), __deriv__(Y(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_11))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(OpenAD_Symbol_10), __deriv__(X(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(OpenAD_Symbol_10))
C     $OpenAD$ INLINE pop_i(subst)
      CALL pop_i(I)
      CALL foo(__deriv__(X(I)), __deriv__(OpenAD_Symbol_16))
      CALL foo(__deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
