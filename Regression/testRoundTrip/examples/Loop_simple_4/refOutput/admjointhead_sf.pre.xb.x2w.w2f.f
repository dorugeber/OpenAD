
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
      SUBROUTINE foo(X, Y, K)
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
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      INTEGER(w2f__i4) OpenAD_Symbol_3
      INTEGER(w2f__i4) OpenAD_Symbol_4
      REAL(w2f__8) OpenAD_Symbol_5
      REAL(w2f__8) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      J = K
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        __value__(Y(INT(J))) = (__value__(Y(J)) + __value__(X(I)) *
     >  __value__(X(I)))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      J = K
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        OpenAD_lin_0 = __value__(X(I))
        OpenAD_lin_1 = __value__(X(I))
        __value__(Y(INT(J))) = (__value__(Y(J)) + __value__(X(I)) *
     >  __value__(X(I)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(J)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(J)
      I = 1 + 1 *((2 - 1) / 1)
      DO WHILE(I .GE. 1)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_5)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_6)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y(J)), __deriv__(X(I)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_6, __deriv__(Y(J)), __deriv__(X(I)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(J)), __deriv__(OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(J)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(J)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        I = I - 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(K)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      J = K
C$OPENAD XXX Simple loop
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_lin_0 = __value__(X(I))
        OpenAD_lin_1 = __value__(X(I))
        __value__(Y(INT(J))) = (__value__(Y(J)) + __value__(X(I)) *
     >  __value__(X(I)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(J)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
        OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_2)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_3)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_4)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_5)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_6)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y(OpenAD_Symbol_4)),
     >  __deriv__(X(OpenAD_Symbol_3)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_6, __deriv__(Y(OpenAD_Symbol_4)),
     >  __deriv__(X(OpenAD_Symbol_3)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_4)), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_4)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(
     > OpenAD_Symbol_4)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(Y))
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
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) OAD_CTMP0
      INTEGER(w2f__i4) OAD_CTMP1
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
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      OAD_CTMP0 = 1
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP0)
      OAD_CTMP1 = 2
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      OAD_CTMP0 = 1
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP0)
      OAD_CTMP1 = 2
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP1)
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP0)
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(2)))
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
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      OAD_CTMP0 = 1
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP0)
      OAD_CTMP1 = 2
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP1)
      CALL foo(__deriv__(X), __deriv__(Y), OAD_CTMP0)
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
