
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
      TYPE (OpenADTy_active) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) OAD_CTMP0
      REAL(w2f__8) P
      REAL(w2f__8) Q
      EXTERNAL sq
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      OAD_CTMP0 = 2
      CALL sq(OAD_CTMP0, __deriv__(X), __deriv__(Y))
      P = 2.0D00
      Q = (P * P)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OAD_CTMP0 = 2
      CALL sq(OAD_CTMP0, __deriv__(X), __deriv__(Y))
      P = 2.0D00
      Q = (P * P)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL sq(OAD_CTMP0, __deriv__(X), __deriv__(Y))
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
      OAD_CTMP0 = 2
      CALL sq(OAD_CTMP0, __deriv__(X), __deriv__(Y))
      P = 2.0D00
      Q = (P * P)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL sq(OAD_CTMP0, __deriv__(X), __deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE

      SUBROUTINE sq(N, U, V)
      use w2f__types
      use oad_intrinsics
      use oad_intrinsics
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      INTEGER(w2f__i4) N
      TYPE (OpenADTy_active) U(1 : 2)
      TYPE (OpenADTy_active) V(1 : 2)
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_Symbol_0
      REAL(w2f__8) OpenAD_Symbol_1
      REAL(w2f__8) OpenAD_Symbol_2
      REAL(w2f__8) OpenAD_Symbol_3
      REAL(w2f__8) OpenAD_Symbol_4
      REAL(w2f__8) OpenAD_Symbol_5
      REAL(w2f__8) OpenAD_Symbol_6
      REAL(w2f__8) OpenAD_Symbol_7
      REAL(w2f__8) OpenAD_Symbol_8
      REAL(w2f__8) OpenAD_Symbol_9
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_9
C
C     **** Statements ****
C
C     $OpenAD$ BEGIN REPLACEMENT 1
      __value__(V(1)) = SQRT((__value__(U(1)) ** 2) +(__value__(U(2))
     >  ** 2))
      __value__(V(2)) = ((__value__(U(1)) ** 3) + SIN(__value__(U(2)))
     >  +(__value__(U(2)) / __value__(U(1))) - COS(__value__(U(1))) +(
     > __value__(U(2)) ** 3) + __value__(U(2)) * __value__(U(1)) *
     >  2.0D00)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
      OpenAD_aux_0 = SQRT((__value__(U(1)) ** 2) +(__value__(U(2)) ** 2
     > ))
      OpenAD_lin_1 = (2 *(__value__(U(1)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_2 = (2 *(__value__(U(2)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_0 = (5.0D-01 / OpenAD_aux_0)
      __value__(V(1)) = OpenAD_aux_0
      OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_1 * OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
      OpenAD_aux_1 = (__value__(U(1)) * 2.0D00)
      OpenAD_lin_3 = (3 *(__value__(U(1)) **(3 - INT(1_w2f__i8))))
      OpenAD_lin_4 = COS(__value__(U(2)))
      OpenAD_lin_5 = (INT(1_w2f__i8) / __value__(U(1)))
      OpenAD_lin_6 = (-(__value__(U(2)) /(__value__(U(1)) * __value__(U
     > (1)))))
      OpenAD_lin_7 = (- SIN(__value__(U(1))))
      OpenAD_lin_8 = (3 *(__value__(U(2)) **(3 - INT(1_w2f__i8))))
      OpenAD_lin_9 = OpenAD_aux_1
      OpenAD_lin_10 = __value__(U(2))
      __value__(V(2)) = ((__value__(U(1)) ** 3) + SIN(__value__(U(2)))
     >  +(__value__(U(2)) / __value__(U(1))) - COS(__value__(U(1))) +(
     > __value__(U(2)) ** 3) + __value__(U(2)) * OpenAD_aux_1)
      OpenAD_acc_2 = (OpenAD_lin_7 * INT((-1_w2f__i8)))
      OpenAD_acc_3 = (2.0D00 * OpenAD_lin_10)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_8)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_2)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_6)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_9)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_3)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_4)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_5)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_6)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_7)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_5, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_6, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(V(2)))
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_8)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(V(1)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_9, __deriv__(V(1)), __deriv__(U(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(V(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(U))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(U))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(V))
C     $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
      CALL cp_arg_store_real_vector_a(__deriv__(U))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(U))
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
      CALL cp_arg_restore_real_vector_a(__deriv__(V))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
      OpenAD_aux_0 = SQRT((__value__(U(1)) ** 2) +(__value__(U(2)) ** 2
     > ))
      OpenAD_lin_1 = (2 *(__value__(U(1)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_2 = (2 *(__value__(U(2)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_0 = (5.0D-01 / OpenAD_aux_0)
      __value__(V(1)) = OpenAD_aux_0
      OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_1 * OpenAD_lin_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_0)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_1)
      OpenAD_aux_1 = (__value__(U(1)) * 2.0D00)
      OpenAD_lin_3 = (3 *(__value__(U(1)) **(3 - INT(1_w2f__i8))))
      OpenAD_lin_4 = COS(__value__(U(2)))
      OpenAD_lin_5 = (INT(1_w2f__i8) / __value__(U(1)))
      OpenAD_lin_6 = (-(__value__(U(2)) /(__value__(U(1)) * __value__(U
     > (1)))))
      OpenAD_lin_7 = (- SIN(__value__(U(1))))
      OpenAD_lin_8 = (3 *(__value__(U(2)) **(3 - INT(1_w2f__i8))))
      OpenAD_lin_9 = OpenAD_aux_1
      OpenAD_lin_10 = __value__(U(2))
      __value__(V(2)) = ((__value__(U(1)) ** 3) + SIN(__value__(U(2)))
     >  +(__value__(U(2)) / __value__(U(1))) - COS(__value__(U(1))) +(
     > __value__(U(2)) ** 3) + __value__(U(2)) * OpenAD_aux_1)
      OpenAD_acc_2 = (OpenAD_lin_7 * INT((-1_w2f__i8)))
      OpenAD_acc_3 = (2.0D00 * OpenAD_lin_10)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_8)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_3)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_2)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_4)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_5)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_6)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_9)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_acc_3)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_0)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_1)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_2)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_3)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_4)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_5)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_6)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_7)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_0, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_1, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_2, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_3, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_4, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_5, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_6, __deriv__(V(2)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_7, __deriv__(V(2)), __deriv__(U(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(V(2)))
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_8)
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_9)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_8, __deriv__(V(1)), __deriv__(U(1)))
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_9, __deriv__(V(1)), __deriv__(U(2)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(V(1)))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_vector_a_d(subst)
      CALL cp_arg_store_real_vector_a_d(__deriv__(V))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_vector_a_d(subst)
      CALL cp_arg_restore_real_vector_a_d(__deriv__(V))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
