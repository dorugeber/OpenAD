
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
      SUBROUTINE opt(REQARG, OPTARG, OUTARG)
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
      INTEGER(w2f__i8) OpenAD_Symbol_10
      INTEGER(w2f__i8) OpenAD_Symbol_11
      INTEGER(w2f__i8) OpenAD_Symbol_2
      INTEGER(w2f__i8) OpenAD_Symbol_3
      INTEGER(w2f__i8) OpenAD_Symbol_4
      INTEGER(w2f__i8) OpenAD_Symbol_5
      INTEGER(w2f__i8) OpenAD_Symbol_6
      INTEGER(w2f__i8) OpenAD_Symbol_7
      INTEGER(w2f__i8) OpenAD_Symbol_8
      INTEGER(w2f__i8) OpenAD_Symbol_9
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) REQARG
      TYPE (OpenADTy_active) OPTARG
      OPTIONAL  OPTARG
      TYPE (OpenADTy_active) OUTARG
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_Symbol_13
      REAL(w2f__8) OpenAD_Symbol_14
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      IF(PRESENT(__value__(OPTARG))) THEN
        IF(__value__(OPTARG) .LE. 2.0D00) THEN
          __value__(OUTARG) = (__value__(REQARG) * __value__(OPTARG) *
     >  __value__(OUTARG))
        ENDIF
      ELSE
        __value__(OUTARG) = __value__(REQARG)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IF(PRESENT(__value__(OPTARG))) THEN
        IF(__value__(OPTARG) .LE. 2.0D00) THEN
          OpenAD_aux_0 = (__value__(OPTARG) * __value__(OUTARG))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_2 = __value__(OUTARG)
          OpenAD_lin_3 = __value__(OPTARG)
          OpenAD_lin_1 = __value__(REQARG)
          __value__(OUTARG) = (__value__(REQARG) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_3 * OpenAD_lin_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
          OpenAD_Symbol_2 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_2)
        ELSE
          OpenAD_Symbol_3 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_3)
        ENDIF
        OpenAD_Symbol_5 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_5)
      ELSE
        __value__(OUTARG) = __value__(REQARG)
        OpenAD_Symbol_4 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_4)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      IF(OpenAD_Symbol_0 .ne. 0) THEN
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_1)
        IF(OpenAD_Symbol_1 .ne. 0) THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_12)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_13)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_14)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_12, __deriv__(OUTARG), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_13, __deriv__(OUTARG), __deriv__(
     > OPTARG))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_14, __deriv__(OUTARG), __deriv__(
     > REQARG))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OUTARG))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(OUTARG))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
      ELSE
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OUTARG), __deriv__(REQARG))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OUTARG))
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(REQARG))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(OPTARG))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(OPTARG))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(REQARG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(REQARG))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(OPTARG))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(OPTARG))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(REQARG))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(OUTARG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      IF(PRESENT(__value__(OPTARG))) THEN
        IF(__value__(OPTARG) .LE. 2.0D00) THEN
          OpenAD_aux_0 = (__value__(OPTARG) * __value__(OUTARG))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_2 = __value__(OUTARG)
          OpenAD_lin_3 = __value__(OPTARG)
          OpenAD_lin_1 = __value__(REQARG)
          __value__(OUTARG) = (__value__(REQARG) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_3 * OpenAD_lin_1)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_lin_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_0)
C         $OpenAD$ INLINE push_s0(subst)
          CALL push_s0(OpenAD_acc_1)
          OpenAD_Symbol_8 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_8)
        ELSE
          OpenAD_Symbol_9 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_9)
        ENDIF
        OpenAD_Symbol_11 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_11)
      ELSE
        __value__(OUTARG) = __value__(REQARG)
        OpenAD_Symbol_10 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_10)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_6)
      IF(OpenAD_Symbol_6 .ne. 0) THEN
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_7)
        IF(OpenAD_Symbol_7 .ne. 0) THEN
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_12)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_13)
C         $OpenAD$ INLINE pop_s0(subst)
          CALL pop_s0(OpenAD_Symbol_14)
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_12, __deriv__(OUTARG), __deriv__(
     > OpenAD_prp_0))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_13, __deriv__(OUTARG), __deriv__(
     > OPTARG))
C         $OpenAD$ INLINE Saxpy(subst,subst,subst)
          CALL Saxpy(OpenAD_Symbol_14, __deriv__(OUTARG), __deriv__(
     > REQARG))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OUTARG))
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(OUTARG))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
        ENDIF
      ELSE
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OUTARG), __deriv__(REQARG))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OUTARG))
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(OUTARG))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(OUTARG))
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
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE opt(REQARG, OPTARG, OUTARG)
        use w2f__types
        TYPE (OpenADTy_active) REQARG
        INTENT(in)  REQARG
        TYPE (OpenADTy_active) OPTARG
        OPTIONAL  OPTARG
        INTENT(in)  OPTARG
        TYPE (OpenADTy_active) OUTARG
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      __value__(Y(1)) = (__value__(X(1)) * 2.0D00)
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      __value__(Y(1)) = (__value__(X(1)) * 2.0D00)
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(2.0D00, __deriv__(Y(1)), __deriv__(X(1)))
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
      __value__(Y(1)) = (__value__(X(1)) * 2.0D00)
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(2.0D00, __deriv__(Y(1)), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
