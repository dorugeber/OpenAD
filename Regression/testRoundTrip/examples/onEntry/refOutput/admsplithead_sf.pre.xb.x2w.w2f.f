
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
      SUBROUTINE foo(K, A)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_0
      INTEGER(w2f__i8) OpenAD_Symbol_1
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) A(1 : K)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) OpenAD_Symbol_12
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      DO I = 1, K, 1
        __value__(A(INT(I))) = (__value__(A(I)) * 2.0D00)
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = 1, K, 1
        __value__(A(INT(I))) = (__value__(A(I)) * 2.0D00)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_12)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(A(OpenAD_Symbol_12)), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(A(OpenAD_Symbol_12)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(A(
     > OpenAD_Symbol_12)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
      OpenAD_Symbol_5 = 0_w2f__i8
      DO I = 1, K, 1
        __value__(A(INT(I))) = (__value__(A(I)) * 2.0D00)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_3)
      OpenAD_Symbol_4 = 1
      DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_12)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(A(OpenAD_Symbol_12)), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(A(OpenAD_Symbol_12)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(A(
     > OpenAD_Symbol_12)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) K
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      DO I = 1, 2, 1
        K = 2
        CALL foo(K, __deriv__(X))
        K = (-19)
      END DO
      __value__(Y(1 : 2)) = __value__(X(1 : 2))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OpenAD_Symbol_8 = 0_w2f__i8
      DO I = 1, 2, 1
        K = 2
        CALL foo(K, __deriv__(X))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(K)
        K = (-19)
        OpenAD_Symbol_8 = (INT(OpenAD_Symbol_8) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_8)
      __value__(Y(1 : 2)) = __value__(X(1 : 2))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 2)), __deriv__(X(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = 1
      DO WHILE(INT(OpenAD_Symbol_7) .LE. INT(OpenAD_Symbol_6))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(K)
        CALL foo(K, __deriv__(X))
        OpenAD_Symbol_7 = INT(OpenAD_Symbol_7) + 1
      END DO
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
      OpenAD_Symbol_11 = 0_w2f__i8
      DO I = 1, 2, 1
        K = 2
        CALL foo(K, __deriv__(X))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(K)
        K = (-19)
        OpenAD_Symbol_11 = (INT(OpenAD_Symbol_11) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_11)
      __value__(Y(1 : 2)) = __value__(X(1 : 2))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 2)), __deriv__(X(1 : 2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 2)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_9)
      OpenAD_Symbol_10 = 1
      DO WHILE(INT(OpenAD_Symbol_10) .LE. INT(OpenAD_Symbol_9))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(K)
        CALL foo(K, __deriv__(X))
        OpenAD_Symbol_10 = INT(OpenAD_Symbol_10) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
