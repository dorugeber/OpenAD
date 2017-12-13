
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
      SUBROUTINE lagran(I, X, A, SP, LAG)
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
      INTEGER(w2f__i4) I
      REAL(w2f__8) X
      TYPE (OpenADTy_active) A(1 : 4)
      INTEGER(w2f__i4) SP
      TYPE (OpenADTy_active) LAG
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) DENOM
      INTEGER(w2f__i4) K
      TYPE (OpenADTy_active) NUMER
      REAL(w2f__8) OpenAD_Symbol_10
      REAL(w2f__8) OpenAD_Symbol_11
      REAL(w2f__8) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_Symbol_13
      INTEGER(w2f__i4) OpenAD_Symbol_6
      INTEGER(w2f__i4) OpenAD_Symbol_7
      REAL(w2f__8) OpenAD_Symbol_8
      REAL(w2f__8) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_2
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(NUMER) = 1.0D00
      __value__(DENOM) = 1.0D00
C$OPENAD XXX Simple loop
      DO K = 1, SP, 1
        IF(I .ne. K) THEN
          __value__(DENOM) = (__value__(DENOM) *(__value__(A(I)) -
     >  __value__(A(K))))
          __value__(NUMER) = (__value__(NUMER) *(X - __value__(A(K))))
        ENDIF
      END DO
      __value__(LAG) = (__value__(NUMER) / __value__(DENOM))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(NUMER) = 1.0D00
      __value__(DENOM) = 1.0D00
C$OPENAD XXX Simple loop
      DO K = 1, SP, 1
        IF(I .ne. K) THEN
          OpenAD_aux_0 = (__value__(A(I)) - __value__(A(K)))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_1 = __value__(DENOM)
          __value__(DENOM) = (__value__(DENOM) * OpenAD_aux_0)
          OpenAD_aux_1 = (X - __value__(A(K)))
          OpenAD_lin_2 = OpenAD_aux_1
          OpenAD_lin_3 = __value__(NUMER)
          __value__(NUMER) = (__value__(NUMER) * OpenAD_aux_1)
          OpenAD_acc_0 = (INT((-1_w2f__i8)) * OpenAD_lin_3)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
        ENDIF
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(SP)
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(I)
      OpenAD_lin_4 = (INT(1_w2f__i8) / __value__(DENOM))
      OpenAD_lin_5 = (-(__value__(NUMER) /(__value__(DENOM) * __value__
     > (DENOM))))
      __value__(LAG) = (__value__(NUMER) / __value__(DENOM))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_13)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(LAG), __deriv__(DENOM))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_13, __deriv__(LAG), __deriv__(NUMER))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(LAG))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(I)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(SP)
      K = 1 + 1 *((SP - 1) / 1)
      DO WHILE(K .GE. 1)
        IF(I .ne. K) THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_8)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_9)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_10)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_11)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_8, __deriv__(NUMER), __deriv__(A(K))
     > )
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_9, __deriv__(NUMER), __deriv__(
     > OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(NUMER))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_10, __deriv__(DENOM), __deriv__(
     > OpenAD_prp_2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_11, __deriv__(DENOM), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(DENOM))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_2), __deriv__(A(K)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(A(I)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(NUMER))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(DENOM))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
        K = K - 1
      END DO
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(DENOM))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(NUMER))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(X)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(SP)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(SP)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(X)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(LAG))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
      CALL cp_arg_store_real_scalar(X)
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(SP)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(A))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(A))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(SP)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
      CALL cp_arg_restore_real_scalar(X)
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(I)
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(LAG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      __value__(NUMER) = 1.0D00
      __value__(DENOM) = 1.0D00
C$OPENAD XXX Simple loop
      OpenAD_Symbol_3 = 0_w2f__i8
      DO K = 1, SP, 1
        IF(I .ne. K) THEN
          OpenAD_aux_0 = (__value__(A(I)) - __value__(A(K)))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_1 = __value__(DENOM)
          __value__(DENOM) = (__value__(DENOM) * OpenAD_aux_0)
          OpenAD_aux_1 = (X - __value__(A(K)))
          OpenAD_lin_2 = OpenAD_aux_1
          OpenAD_lin_3 = __value__(NUMER)
          __value__(NUMER) = (__value__(NUMER) * OpenAD_aux_1)
          OpenAD_acc_0 = (INT((-1_w2f__i8)) * OpenAD_lin_3)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_2)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(K)
          OpenAD_Symbol_4 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_4)
        ELSE
          OpenAD_Symbol_5 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_5)
        ENDIF
        OpenAD_Symbol_3 = (INT(OpenAD_Symbol_3) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
      OpenAD_lin_4 = (INT(1_w2f__i8) / __value__(DENOM))
      OpenAD_lin_5 = (-(__value__(NUMER) /(__value__(DENOM) * __value__
     > (DENOM))))
      __value__(LAG) = (__value__(NUMER) / __value__(DENOM))
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_13)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(LAG), __deriv__(DENOM))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_13, __deriv__(LAG), __deriv__(NUMER))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(LAG))
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_2)
        IF(OpenAD_Symbol_2 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_6)
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_7)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_8)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_9)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_10)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_11)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_8, __deriv__(NUMER), __deriv__(A(
     > OpenAD_Symbol_6)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_9, __deriv__(NUMER), __deriv__(
     > OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(NUMER))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_10, __deriv__(DENOM), __deriv__(
     > OpenAD_prp_2))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_11, __deriv__(DENOM), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(DENOM))
C         $OpenAD$ INLINE DecDeriv(subst,subst)
          CALL DecDeriv(__deriv__(OpenAD_prp_2), __deriv__(A(
     > OpenAD_Symbol_6)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_2), __deriv__(A(
     > OpenAD_Symbol_7)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_2))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(NUMER))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(DENOM))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(DENOM))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(NUMER))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(LAG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(LAG))
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
      TYPE (OpenADTy_active) X(1 : 4)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL lagran
      INTEGER(w2f__i4) OAD_CTMP0
      REAL(w2f__8) OAD_CTMP1
      INTEGER(w2f__i4) OAD_CTMP2
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
      OAD_CTMP0 = 2
      OAD_CTMP1 = 2.0D00
      OAD_CTMP2 = 4
      CALL lagran(OAD_CTMP0, OAD_CTMP1, __deriv__(X), OAD_CTMP2,
     >  __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OAD_CTMP0 = 2
      OAD_CTMP1 = 2.0D00
      OAD_CTMP2 = 4
      CALL lagran(OAD_CTMP0, OAD_CTMP1, __deriv__(X), OAD_CTMP2,
     >  __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL lagran(OAD_CTMP0, OAD_CTMP1, __deriv__(X), OAD_CTMP2,
     >  __deriv__(Y(1)))
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
      OAD_CTMP0 = 2
      OAD_CTMP1 = 2.0D00
      OAD_CTMP2 = 4
      CALL lagran(OAD_CTMP0, OAD_CTMP1, __deriv__(X), OAD_CTMP2,
     >  __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL lagran(OAD_CTMP0, OAD_CTMP1, __deriv__(X), OAD_CTMP2,
     >  __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
