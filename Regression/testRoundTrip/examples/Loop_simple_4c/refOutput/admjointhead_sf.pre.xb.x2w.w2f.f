
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
      MODULE aglobalmodule
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      CHARACTER(3) GLOBALSTRING
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(X, Y)
      use w2f__types
      use oad_intrinsics
      use aglobalmodule
      use oad_intrinsics
      use aglobalmodule
      use oad_intrinsics
      use aglobalmodule
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
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      CHARACTER(3) LOCALSTRING
      REAL(w2f__8) OpenAD_Symbol_10
      REAL(w2f__8) OpenAD_Symbol_11
      INTEGER(w2f__i4) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_Symbol_13
      REAL(w2f__8) OpenAD_Symbol_14
      INTEGER(w2f__i4) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(GLOBALSTRING .EQ. 'yes') THEN
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(I)))
        ENDIF
        IF(GLOBALSTRING .EQ. 'no') THEN
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(I)) * 2.0D00)
        ENDIF
      END DO
      GLOBALSTRING = 'either'
      LOCALSTRING = GLOBALSTRING
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        IF(GLOBALSTRING .EQ. 'yes') THEN
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
        ENDIF
        IF(GLOBALSTRING .EQ. 'no') THEN
          OpenAD_aux_0 = (__value__(X(I)) * __value__(X(I)))
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + OpenAD_aux_0 *
     >  2.0D00)
          OpenAD_acc_0 = (OpenAD_lin_2 * 2.0D00)
          OpenAD_acc_1 = (OpenAD_lin_3 * 2.0D00)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
        ENDIF
      END DO
C     $OpenAD$ INLINE push_s(subst)
      CALL push_s(GLOBALSTRING)
      GLOBALSTRING = 'either'
      LOCALSTRING = GLOBALSTRING
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s(subst)
      CALL pop_s(GLOBALSTRING)
      I = 1 + 1 *((2 - 1) / 1)
      DO WHILE(I .GE. 1)
        IF(GLOBALSTRING .EQ. 'no') THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_13)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_14)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_13, __deriv__(Y(I)), __deriv__(X(I))
     > )
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_14, __deriv__(Y(I)), __deriv__(X(I))
     > )
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(I)), __deriv__(OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(I)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(I)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        ENDIF
        IF(GLOBALSTRING .EQ. 'yes') THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_10)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_11)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_10, __deriv__(Y(I)), __deriv__(X(I))
     > )
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_11, __deriv__(Y(I)), __deriv__(X(I))
     > )
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(I)), __deriv__(OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(I)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(I)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
        I = I - 1
      END DO
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_string_scalar(subst)
      CALL cp_arg_store_string_scalar(GLOBALSTRING)
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
C     $OpenAD$ INLINE cp_arg_restore_string_scalar(subst)
      CALL cp_arg_restore_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_string_scalar(subst)
      CALL cp_arg_store_string_scalar(GLOBALSTRING)
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_string_scalar(subst)
      CALL cp_arg_store_string_scalar(GLOBALSTRING)
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
C     $OpenAD$ INLINE cp_arg_restore_string_scalar(subst)
      CALL cp_arg_restore_string_scalar(GLOBALSTRING)
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_restore_string_scalar(subst)
      CALL cp_arg_restore_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      OpenAD_Symbol_4 = 0_w2f__i8
      DO I = 1, 2, 1
        IF(GLOBALSTRING .EQ. 'yes') THEN
          OpenAD_lin_0 = __value__(X(I))
          OpenAD_lin_1 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + __value__(X(I)) *
     >  __value__(X(I)))
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_1)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_5 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_5)
        ELSE
          OpenAD_Symbol_6 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_6)
        ENDIF
        IF(GLOBALSTRING .EQ. 'no') THEN
          OpenAD_aux_0 = (__value__(X(I)) * __value__(X(I)))
          OpenAD_lin_2 = __value__(X(I))
          OpenAD_lin_3 = __value__(X(I))
          __value__(Y(INT(I))) = (__value__(Y(I)) + OpenAD_aux_0 *
     >  2.0D00)
          OpenAD_acc_0 = (OpenAD_lin_2 * 2.0D00)
          OpenAD_acc_1 = (OpenAD_lin_3 * 2.0D00)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(I)
          OpenAD_Symbol_7 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_7)
        ELSE
          OpenAD_Symbol_8 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_8)
        ENDIF
        OpenAD_Symbol_4 = (INT(OpenAD_Symbol_4) + INT(1_w2f__i8))
      END DO
C     $OpenAD$ INLINE push_i_s0(subst)
      CALL push_i_s0(OpenAD_Symbol_4)
      GLOBALSTRING = 'either'
      LOCALSTRING = GLOBALSTRING
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      OpenAD_Symbol_1 = 1
      DO WHILE(INT(OpenAD_Symbol_1) .LE. INT(OpenAD_Symbol_0))
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_2)
        IF(OpenAD_Symbol_2 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_12)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_13)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_14)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_13, __deriv__(Y(OpenAD_Symbol_12)),
     >  __deriv__(X(OpenAD_Symbol_12)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_14, __deriv__(Y(OpenAD_Symbol_12)),
     >  __deriv__(X(OpenAD_Symbol_12)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_12)), __deriv__(
     > OpenAD_prp_1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_12)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(Y(
     > OpenAD_Symbol_12)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
        ENDIF
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_3)
        IF(OpenAD_Symbol_3 .ne. 0) THEN
C         $OpenAD$ INLINE pop_i_s0(subst)
          CALL pop_i_s0(OpenAD_Symbol_9)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_10)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_11)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_10, __deriv__(Y(OpenAD_Symbol_9)),
     >  __deriv__(X(OpenAD_Symbol_9)))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_11, __deriv__(Y(OpenAD_Symbol_9)),
     >  __deriv__(X(OpenAD_Symbol_9)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(Y(OpenAD_Symbol_9)), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(Y(OpenAD_Symbol_9)))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(
     > OpenAD_Symbol_9)))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
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
      use aglobalmodule
      use oad_intrinsics
      use aglobalmodule
      use oad_intrinsics
      use aglobalmodule
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
      GLOBALSTRING = 'yes'
      CALL foo(__deriv__(X), __deriv__(Y))
      GLOBALSTRING = 'both'
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      GLOBALSTRING = 'yes'
      CALL foo(__deriv__(X), __deriv__(Y))
      GLOBALSTRING = 'both'
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL foo(__deriv__(X), __deriv__(Y))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_string_scalar(subst)
      CALL cp_arg_store_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_string_scalar(subst)
      CALL cp_arg_restore_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_string_scalar(subst)
      CALL cp_arg_store_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_string_scalar(subst)
      CALL cp_arg_restore_string_scalar(GLOBALSTRING)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      __value__(Y(1)) = 1.0
      __value__(Y(2)) = 1.0
      GLOBALSTRING = 'yes'
      CALL foo(__deriv__(X), __deriv__(Y))
      GLOBALSTRING = 'both'
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL foo(__deriv__(X), __deriv__(Y))
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
