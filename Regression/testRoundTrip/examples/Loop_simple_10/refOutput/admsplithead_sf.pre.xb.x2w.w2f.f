
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
      SUBROUTINE foo(X, Y, A, J)
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
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
      INTEGER(w2f__i4) A(1 : 2, 1 : 2)
      INTEGER(w2f__i4) J
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_Symbol_30
      REAL(w2f__8) OpenAD_Symbol_31
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      INTEGER(w2f__i4) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = __value__(X(1))
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(A(I, J) .ne. 0) THEN
          __value__(Y(1)) = (A(I, J) * __value__(X(1)) * __value__(Y(1)
     > ))
        ELSE
          __value__(Y(1)) = 0.0
        ENDIF
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = __value__(X(1))
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(A(I, J) .ne. 0) THEN
          OpenAD_aux_0 = (__value__(X(1)) * __value__(Y(1)))
          OpenAD_lin_1 = __value__(Y(1))
          OpenAD_lin_2 = __value__(X(1))
          OpenAD_lin_0 = A(I, J)
          __value__(Y(1)) = (A(I, J) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_1 * OpenAD_lin_0)
          OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
        ELSE
          __value__(Y(1)) = 0.0
        ENDIF
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(A(I, J))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(J)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(J)
      I = 1 + 1 *((2 - 1) / 1)
      DO WHILE(I .GE. 1)
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(A(I, J))
        IF(A(I, J) .ne. 0) THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_30)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_31)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_30, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(1)), __deriv__(X(1))
     > )
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ELSE
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
        ENDIF
        I = I - 1
      END DO
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_matrix(subst)
      CALL cp_arg_store_integer_matrix(A)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_integer_matrix(subst)
      CALL cp_arg_restore_integer_matrix(A)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(J)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
      CALL cp_arg_store_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_integer_matrix(subst)
      CALL cp_arg_store_integer_matrix(A)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_integer_matrix(subst)
      CALL cp_arg_restore_integer_matrix(A)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
      CALL cp_arg_restore_integer_scalar(J)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = __value__(X(1))
C$OPENAD XXX Simple loop
      OpenAD_Symbol_3 = 0_w2f__i8
      DO I = 1, 2, 1
        IF(A(I, J) .ne. 0) THEN
          OpenAD_aux_0 = (__value__(X(1)) * __value__(Y(1)))
          OpenAD_lin_1 = __value__(Y(1))
          OpenAD_lin_2 = __value__(X(1))
          OpenAD_lin_0 = A(I, J)
          __value__(Y(1)) = (A(I, J) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_1 * OpenAD_lin_0)
          OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
          OpenAD_Symbol_4 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_4)
        ELSE
          __value__(Y(1)) = 0.0
          OpenAD_Symbol_5 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_5)
        ENDIF
        OpenAD_Symbol_3 = (INT(OpenAD_Symbol_3) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_3)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_2)
        IF(OpenAD_Symbol_2 .ne. 0) THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_30)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_31)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_30, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_31, __deriv__(Y(1)), __deriv__(X(1))
     > )
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ELSE
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(1)))
        ENDIF
        OpenAD_Symbol_1 = INT(OpenAD_Symbol_1) + 1
      END DO
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
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
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_12
      INTEGER(w2f__i8) OpenAD_Symbol_13
      INTEGER(w2f__i8) OpenAD_Symbol_14
      INTEGER(w2f__i8) OpenAD_Symbol_15
      INTEGER(w2f__i8) OpenAD_Symbol_16
      INTEGER(w2f__i8) OpenAD_Symbol_17
      INTEGER(w2f__i8) OpenAD_Symbol_18
      INTEGER(w2f__i8) OpenAD_Symbol_19
      INTEGER(w2f__i8) OpenAD_Symbol_20
      INTEGER(w2f__i8) OpenAD_Symbol_21
      INTEGER(w2f__i8) OpenAD_Symbol_22
      INTEGER(w2f__i8) OpenAD_Symbol_23
      INTEGER(w2f__i8) OpenAD_Symbol_24
      INTEGER(w2f__i8) OpenAD_Symbol_25
      INTEGER(w2f__i8) OpenAD_Symbol_26
      INTEGER(w2f__i8) OpenAD_Symbol_27
      INTEGER(w2f__i8) OpenAD_Symbol_28
      INTEGER(w2f__i8) OpenAD_Symbol_29
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) A(1 : 2, 1 : 2)
      EXTERNAL foo
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
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
C$OPENAD XXX Template ad_template.f
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          A(I, J) = (I + J)
        END DO
      END DO
      OAD_CTMP0 = 2
      CALL foo(__deriv__(X), __deriv__(Y), A, OAD_CTMP0)
      DO I = 1, 2, 1
        DO J = 1, 2, 1
          A(I, J) = 0
        END DO
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_14 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_15 = 0_w2f__i8
        DO J = 1, 2, 1
          A(I, J) = (I + J)
          OpenAD_Symbol_15 = (INT(OpenAD_Symbol_15) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_15)
        OpenAD_Symbol_14 = (INT(OpenAD_Symbol_14) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_14)
      OAD_CTMP0 = 2
      CALL foo(__deriv__(X), __deriv__(Y), A, OAD_CTMP0)
      OpenAD_Symbol_16 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_17 = 0_w2f__i8
        DO J = 1, 2, 1
          A(I, J) = 0
          OpenAD_Symbol_17 = (INT(OpenAD_Symbol_17) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_17)
        OpenAD_Symbol_16 = (INT(OpenAD_Symbol_16) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_16)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
      OpenAD_Symbol_7 = 1
      DO WHILE(INT(OpenAD_Symbol_7) .LE. INT(OpenAD_Symbol_6))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_8)
        OpenAD_Symbol_9 = 1
        DO WHILE(INT(OpenAD_Symbol_9) .LE. INT(OpenAD_Symbol_8))
          OpenAD_Symbol_9 = INT(OpenAD_Symbol_9) + 1
        END DO
        OpenAD_Symbol_7 = INT(OpenAD_Symbol_7) + 1
      END DO
      CALL foo(__deriv__(X), __deriv__(Y), A, OAD_CTMP0)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_10)
      OpenAD_Symbol_11 = 1
      DO WHILE(INT(OpenAD_Symbol_11) .LE. INT(OpenAD_Symbol_10))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_12)
        OpenAD_Symbol_13 = 1
        DO WHILE(INT(OpenAD_Symbol_13) .LE. INT(OpenAD_Symbol_12))
          OpenAD_Symbol_13 = INT(OpenAD_Symbol_13) + 1
        END DO
        OpenAD_Symbol_11 = INT(OpenAD_Symbol_11) + 1
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
C$OPENAD XXX Template ad_template.f
      OpenAD_Symbol_26 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_27 = 0_w2f__i8
        DO J = 1, 2, 1
          A(I, J) = (I + J)
          OpenAD_Symbol_27 = (INT(OpenAD_Symbol_27) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_27)
        OpenAD_Symbol_26 = (INT(OpenAD_Symbol_26) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_26)
      OAD_CTMP0 = 2
      CALL foo(__deriv__(X), __deriv__(Y), A, OAD_CTMP0)
      OpenAD_Symbol_28 = 0_w2f__i8
      DO I = 1, 2, 1
        OpenAD_Symbol_29 = 0_w2f__i8
        DO J = 1, 2, 1
          A(I, J) = 0
          OpenAD_Symbol_29 = (INT(OpenAD_Symbol_29) + INT(1_w2f__i8))
        END DO
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_29)
        OpenAD_Symbol_28 = (INT(OpenAD_Symbol_28) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_28)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_18)
      OpenAD_Symbol_19 = 1
      DO WHILE(INT(OpenAD_Symbol_19) .LE. INT(OpenAD_Symbol_18))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_20)
        OpenAD_Symbol_21 = 1
        DO WHILE(INT(OpenAD_Symbol_21) .LE. INT(OpenAD_Symbol_20))
          OpenAD_Symbol_21 = INT(OpenAD_Symbol_21) + 1
        END DO
        OpenAD_Symbol_19 = INT(OpenAD_Symbol_19) + 1
      END DO
      CALL foo(__deriv__(X), __deriv__(Y), A, OAD_CTMP0)
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_22)
      OpenAD_Symbol_23 = 1
      DO WHILE(INT(OpenAD_Symbol_23) .LE. INT(OpenAD_Symbol_22))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_24)
        OpenAD_Symbol_25 = 1
        DO WHILE(INT(OpenAD_Symbol_25) .LE. INT(OpenAD_Symbol_24))
          OpenAD_Symbol_25 = INT(OpenAD_Symbol_25) + 1
        END DO
        OpenAD_Symbol_23 = INT(OpenAD_Symbol_23) + 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
