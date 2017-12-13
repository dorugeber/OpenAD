
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
      SUBROUTINE foo(X, Y, L, U, S)
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
      TYPE (OpenADTy_active) X(1 : 5)
      TYPE (OpenADTy_active) Y
      INTEGER(w2f__i4) L
      INTEGER(w2f__i4) U
      INTEGER(w2f__i4) S
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
      REAL(w2f__8) OpenAD_Symbol_5
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = L, U, S
        __value__(Y) = (__value__(X(I)) * __value__(Y))
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = L, U, S
        OpenAD_lin_0 = __value__(Y)
        OpenAD_lin_1 = __value__(X(I))
        __value__(Y) = (__value__(X(I)) * __value__(Y))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(L)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(U)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(S)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(S)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(U)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(L)
      I = L + S * ABS((L - U) / S)
      DO WHILE(((S .GT. 0.0D00) .AND.(I .GE. L)) .OR.((S .LT. 0.0D00)
     >  .AND.(I .LE. L)))
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_4)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_5)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y), __deriv__(X(I)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        I = I - S
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(L)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(U)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(S)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(S)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(U)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(L)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(L)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(U)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(S)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(S)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(U)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(L)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      OpenAD_Symbol_2 = 0_w2f__i8
      DO I = L, U, S
        OpenAD_lin_0 = __value__(Y)
        OpenAD_lin_1 = __value__(X(I))
        __value__(Y) = (__value__(X(I)) * __value__(Y))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
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
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_4)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_5)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_5, __deriv__(Y), __deriv__(X(
     > OpenAD_Symbol_3)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
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
      TYPE (OpenADTy_active) X(1 : 5)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) OAD_CTMP0
      INTEGER(w2f__i4) OAD_CTMP1
      INTEGER(w2f__i4) OAD_CTMP10
      INTEGER(w2f__i4) OAD_CTMP11
      INTEGER(w2f__i4) OAD_CTMP2
      INTEGER(w2f__i4) OAD_CTMP3
      INTEGER(w2f__i4) OAD_CTMP4
      INTEGER(w2f__i4) OAD_CTMP5
      INTEGER(w2f__i4) OAD_CTMP6
      INTEGER(w2f__i4) OAD_CTMP7
      INTEGER(w2f__i4) OAD_CTMP8
      INTEGER(w2f__i4) OAD_CTMP9
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
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
      OAD_CTMP0 = 1
      OAD_CTMP1 = 3
      OAD_CTMP2 = 1
      CALL foo(__deriv__(X), __deriv__(Y(1)), OAD_CTMP0, OAD_CTMP1,
     >  OAD_CTMP2)
      OAD_CTMP3 = 3
      OAD_CTMP4 = 1
      OAD_CTMP5 = (-1)
      CALL foo(__deriv__(X), __deriv__(Y(2)), OAD_CTMP3, OAD_CTMP4,
     >  OAD_CTMP5)
      OAD_CTMP6 = 1
      OAD_CTMP7 = 5
      OAD_CTMP8 = 2
      CALL foo(__deriv__(X), __deriv__(Y(3)), OAD_CTMP6, OAD_CTMP7,
     >  OAD_CTMP8)
      OAD_CTMP9 = 5
      OAD_CTMP10 = 1
      OAD_CTMP11 = (-2)
      CALL foo(__deriv__(X), __deriv__(Y(4)), OAD_CTMP9, OAD_CTMP10,
     >  OAD_CTMP11)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
      OAD_CTMP0 = 1
      OAD_CTMP1 = 3
      OAD_CTMP2 = 1
      CALL foo(__deriv__(X), __deriv__(Y(1)), OAD_CTMP0, OAD_CTMP1,
     >  OAD_CTMP2)
      OAD_CTMP3 = 3
      OAD_CTMP4 = 1
      OAD_CTMP5 = (-1)
      CALL foo(__deriv__(X), __deriv__(Y(2)), OAD_CTMP3, OAD_CTMP4,
     >  OAD_CTMP5)
      OAD_CTMP6 = 1
      OAD_CTMP7 = 5
      OAD_CTMP8 = 2
      CALL foo(__deriv__(X), __deriv__(Y(3)), OAD_CTMP6, OAD_CTMP7,
     >  OAD_CTMP8)
      OAD_CTMP9 = 5
      OAD_CTMP10 = 1
      OAD_CTMP11 = (-2)
      CALL foo(__deriv__(X), __deriv__(Y(4)), OAD_CTMP9, OAD_CTMP10,
     >  OAD_CTMP11)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL foo(__deriv__(X), __deriv__(Y(4)), OAD_CTMP9, OAD_CTMP10,
     >  OAD_CTMP11)
      CALL foo(__deriv__(X), __deriv__(Y(3)), OAD_CTMP6, OAD_CTMP7,
     >  OAD_CTMP8)
      CALL foo(__deriv__(X), __deriv__(Y(2)), OAD_CTMP3, OAD_CTMP4,
     >  OAD_CTMP5)
      CALL foo(__deriv__(X), __deriv__(Y(1)), OAD_CTMP0, OAD_CTMP1,
     >  OAD_CTMP2)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 4)))
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
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
      OAD_CTMP0 = 1
      OAD_CTMP1 = 3
      OAD_CTMP2 = 1
      CALL foo(__deriv__(X), __deriv__(Y(1)), OAD_CTMP0, OAD_CTMP1,
     >  OAD_CTMP2)
      OAD_CTMP3 = 3
      OAD_CTMP4 = 1
      OAD_CTMP5 = (-1)
      CALL foo(__deriv__(X), __deriv__(Y(2)), OAD_CTMP3, OAD_CTMP4,
     >  OAD_CTMP5)
      OAD_CTMP6 = 1
      OAD_CTMP7 = 5
      OAD_CTMP8 = 2
      CALL foo(__deriv__(X), __deriv__(Y(3)), OAD_CTMP6, OAD_CTMP7,
     >  OAD_CTMP8)
      OAD_CTMP9 = 5
      OAD_CTMP10 = 1
      OAD_CTMP11 = (-2)
      CALL foo(__deriv__(X), __deriv__(Y(4)), OAD_CTMP9, OAD_CTMP10,
     >  OAD_CTMP11)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL foo(__deriv__(X), __deriv__(Y(4)), OAD_CTMP9, OAD_CTMP10,
     >  OAD_CTMP11)
      CALL foo(__deriv__(X), __deriv__(Y(3)), OAD_CTMP6, OAD_CTMP7,
     >  OAD_CTMP8)
      CALL foo(__deriv__(X), __deriv__(Y(2)), OAD_CTMP3, OAD_CTMP4,
     >  OAD_CTMP5)
      CALL foo(__deriv__(X), __deriv__(Y(1)), OAD_CTMP0, OAD_CTMP1,
     >  OAD_CTMP2)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 4)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
