
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
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
C$OPENAD XXX Template ad_template.f
      CALL FOO()
      CALL BAR()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      CALL FOO()
      CALL BAR()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL BAR()
      CALL FOO()
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
      CALL FOO()
      CALL BAR()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL BAR()
      CALL FOO()
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      CONTAINS

        SUBROUTINE FOO()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_Symbol_0
        REAL(w2f__8) OpenAD_Symbol_1
        REAL(w2f__8) OpenAD_lin_0
        REAL(w2f__8) OpenAD_lin_1
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        __value__(Y(1)) = (__value__(X(1)) * __value__(X(2)))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        OpenAD_lin_0 = __value__(X(2))
        OpenAD_lin_1 = __value__(X(1))
        __value__(Y(1)) = (__value__(X(1)) * __value__(X(2)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_0)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_1)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(1)), __deriv__(X(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(Y))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        OpenAD_lin_0 = __value__(X(2))
        OpenAD_lin_1 = __value__(X(1))
        __value__(Y(1)) = (__value__(X(1)) * __value__(X(2)))
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_lin_1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_0)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_1)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_0, __deriv__(Y(1)), __deriv__(X(2)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_1, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
        CALL cp_arg_store_real_vector_a_d(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
        CALL cp_arg_restore_real_vector_a_d(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE

        SUBROUTINE BAR()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_Symbol_2
        REAL(w2f__8) OpenAD_Symbol_3
        REAL(w2f__8) OpenAD_Symbol_4
        REAL(w2f__8) OpenAD_acc_0
        REAL(w2f__8) OpenAD_acc_1
        REAL(w2f__8) OpenAD_acc_2
        REAL(w2f__8) OpenAD_aux_0
        REAL(w2f__8) OpenAD_aux_1
        REAL(w2f__8) OpenAD_aux_2
        REAL(w2f__8) OpenAD_lin_2
        REAL(w2f__8) OpenAD_lin_3
        REAL(w2f__8) OpenAD_lin_4
        REAL(w2f__8) OpenAD_lin_5
        REAL(w2f__8) OpenAD_lin_6
        REAL(w2f__8) OpenAD_lin_7
        TYPE (OpenADTy_active) OpenAD_prp_0
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        __value__(Y(1)) = (COS(__value__(X(2))) * __value__(Y(1)) * SIN
     > (__value__(X(1))))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        OpenAD_aux_0 = COS(__value__(X(2)))
        OpenAD_aux_2 = SIN(__value__(X(1)))
        OpenAD_aux_1 = (__value__(Y(1)) * OpenAD_aux_2)
        OpenAD_lin_4 = (- SIN(__value__(X(2))))
        OpenAD_lin_2 = OpenAD_aux_1
        OpenAD_lin_5 = OpenAD_aux_2
        OpenAD_lin_7 = COS(__value__(X(1)))
        OpenAD_lin_6 = __value__(Y(1))
        OpenAD_lin_3 = OpenAD_aux_0
        __value__(Y(1)) = (OpenAD_aux_0 * OpenAD_aux_1)
        OpenAD_acc_0 = (OpenAD_lin_4 * OpenAD_lin_2)
        OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
        OpenAD_acc_2 = (OpenAD_lin_7 * OpenAD_lin_6 * OpenAD_lin_3)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_2)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_3)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_4)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_2, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y(1)), __deriv__(X(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(Y))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        OpenAD_aux_0 = COS(__value__(X(2)))
        OpenAD_aux_2 = SIN(__value__(X(1)))
        OpenAD_aux_1 = (__value__(Y(1)) * OpenAD_aux_2)
        OpenAD_lin_4 = (- SIN(__value__(X(2))))
        OpenAD_lin_2 = OpenAD_aux_1
        OpenAD_lin_5 = OpenAD_aux_2
        OpenAD_lin_7 = COS(__value__(X(1)))
        OpenAD_lin_6 = __value__(Y(1))
        OpenAD_lin_3 = OpenAD_aux_0
        __value__(Y(1)) = (OpenAD_aux_0 * OpenAD_aux_1)
        OpenAD_acc_0 = (OpenAD_lin_4 * OpenAD_lin_2)
        OpenAD_acc_1 = (OpenAD_lin_5 * OpenAD_lin_3)
        OpenAD_acc_2 = (OpenAD_lin_7 * OpenAD_lin_6 * OpenAD_lin_3)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_0)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_1)
C       $OpenAD$ INLINE push_s0(subst)
        CALL push_s0(OpenAD_acc_2)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_2)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_3)
C       $OpenAD$ INLINE pop_s0(subst)
        CALL pop_s0(OpenAD_Symbol_4)
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_2, __deriv__(Y(1)), __deriv__(X(1)))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_3, __deriv__(Y(1)), __deriv__(
     > OpenAD_prp_0))
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(OpenAD_Symbol_4, __deriv__(Y(1)), __deriv__(X(2)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y(1)))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(Y(1)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
        CALL cp_arg_store_real_vector_a_d(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
        CALL cp_arg_restore_real_vector_a_d(__deriv__(Y))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
      END
