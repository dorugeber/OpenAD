
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
      MODULE m1
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO(X, N)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) N
        REAL(w2f__8) X(1 : N)
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
      END

      MODULE m2
      use w2f__types
      use oad_intrinsics
      use m1
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR(X, N)
        use w2f__types
        IMPLICIT NONE
C
C       **** Global Variables & Derived Type Definitions ****
C
        INTEGER(w2f__i8) OpenAD_Symbol_0
        INTEGER(w2f__i8) OpenAD_Symbol_1
        INTEGER(w2f__i8) OpenAD_Symbol_2
        INTEGER(w2f__i8) OpenAD_Symbol_3
        INTEGER(w2f__i8) OpenAD_Symbol_4
        INTEGER(w2f__i8) OpenAD_Symbol_5
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) N
        TYPE (OpenADTy_active) X(1 : N)
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I
        INTEGER(w2f__i4) OAD_CTMP0
        INTEGER(w2f__i4) OpenAD_Symbol_6
        TYPE (OpenADTy_active) OpenAD_prp_0
        REAL(w2f__8) OpenAD_tyc_0(:)
        ALLOCATABLE OpenAD_tyc_0
        REAL(w2f__8) OpenAD_tyc_1(:)
        ALLOCATABLE OpenAD_tyc_1
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        DO I = 1, N, 1
          __value__(X(INT(I))) = (__value__(X(I)) * 2.0D00)
        END DO
        I = 2
        OAD_CTMP0 = (SIZE(X) - I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
        CALL FOO(OpenAD_tyc_0(I), OAD_CTMP0)
C       $OpenAD$ INLINE oad_ShapeTest(subst,subst)
        CALL oad_ShapeTest(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        OpenAD_Symbol_2 = 0_w2f__i8
        DO I = 1, N, 1
          __value__(X(INT(I))) = (__value__(X(I)) * 2.0D00)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_2 = (INT(OpenAD_Symbol_2) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_2)
        I = 2
        OAD_CTMP0 = (SIZE(X) - I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
        CALL FOO(OpenAD_tyc_0(I), OAD_CTMP0)
C       $OpenAD$ INLINE oad_ShapeTest(subst,subst)
        CALL oad_ShapeTest(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OAD_CTMP0)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OAD_CTMP0)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_1, __deriv__(X))
        CALL FOO(OpenAD_tyc_1(I), OAD_CTMP0)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_0)
        OpenAD_Symbol_1 = 1
        DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_6)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(X(OpenAD_Symbol_6)), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(X(OpenAD_Symbol_6)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(X(
     > OpenAD_Symbol_6)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
          OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
        END DO
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(N)
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(N)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(N)
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        OpenAD_Symbol_5 = 0_w2f__i8
        DO I = 1, N, 1
          __value__(X(INT(I))) = (__value__(X(I)) * 2.0D00)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_5 = (INT(OpenAD_Symbol_5) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_5)
        I = 2
        OAD_CTMP0 = (SIZE(X) - I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(OpenAD_tyc_0, __deriv__(X))
        CALL FOO(OpenAD_tyc_0(I), OAD_CTMP0)
C       $OpenAD$ INLINE oad_ShapeTest(subst,subst)
        CALL oad_ShapeTest(OpenAD_tyc_0, __deriv__(X))
C       $OpenAD$ INLINE oad_convert(subst,subst)
        CALL oad_convert(__deriv__(X), OpenAD_tyc_0)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I)
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OAD_CTMP0)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OAD_CTMP0)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(I)
C       $OpenAD$ INLINE oad_AllocateMatching(subst,subst)
        CALL oad_AllocateMatching(OpenAD_tyc_1, __deriv__(X))
        CALL FOO(OpenAD_tyc_1(I), OAD_CTMP0)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_3)
        OpenAD_Symbol_4 = 1
        DO WHILE(INT(OpenAD_Symbol_4) .LE. INT(OpenAD_Symbol_3))
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_6)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(2.0D00, __deriv__(X(OpenAD_Symbol_6)), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(X(OpenAD_Symbol_6)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(X(
     > OpenAD_Symbol_6)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
          OpenAD_Symbol_4 = INT(OpenAD_Symbol_4) + 1
        END DO
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
        CALL cp_arg_store_real_vector_a_d(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
        CALL cp_arg_restore_real_vector_a_d(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use m2
      use oad_intrinsics
      use m2
      use oad_intrinsics
      use m2
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 4)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) OAD_CTMP0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      OAD_CTMP0 = 4
      CALL BAR(__deriv__(X), OAD_CTMP0)
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OAD_CTMP0 = 4
      CALL BAR(__deriv__(X), OAD_CTMP0)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OAD_CTMP0)
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 4)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OAD_CTMP0)
      CALL BAR(__deriv__(X), OAD_CTMP0)
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
      OAD_CTMP0 = 4
      CALL BAR(__deriv__(X), OAD_CTMP0)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OAD_CTMP0)
      __value__(Y(1 : 4)) = __value__(X(1 : 4))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1 : 4)), __deriv__(X(1 : 4)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1 : 4)))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OAD_CTMP0)
      CALL BAR(__deriv__(X), OAD_CTMP0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
