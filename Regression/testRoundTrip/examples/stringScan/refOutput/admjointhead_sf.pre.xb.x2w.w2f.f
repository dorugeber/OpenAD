
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
      SUBROUTINE foo(X, Y, A)
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
      TYPE (OpenADTy_active) X
      TYPE (OpenADTy_active) Y
      CHARACTER(3) A
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
      IF(SCAN(A, 'a') .eq. 3) THEN
        __value__(Y) = __value__(X)
      ELSE
        __value__(Y) = (__value__(X) * 2.0D00)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      IF(SCAN(A, 'a') .eq. 3) THEN
        __value__(Y) = __value__(X)
        OpenAD_Symbol_1 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_1)
      ELSE
        __value__(Y) = (__value__(X) * 2.0D00)
        OpenAD_Symbol_2 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_2)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_0)
      IF(OpenAD_Symbol_0 .ne. 0) THEN
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
      ELSE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(Y), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(Y))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(X))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(Y))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      IF(SCAN(A, 'a') .eq. 3) THEN
        __value__(Y) = __value__(X)
        OpenAD_Symbol_4 = 1_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_4)
      ELSE
        __value__(Y) = (__value__(X) * 2.0D00)
        OpenAD_Symbol_5 = 0_w2f__i8
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(OpenAD_Symbol_5)
      ENDIF
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_i_s0(subst)
      CALL pop_i_s0(OpenAD_Symbol_3)
      IF(OpenAD_Symbol_3 .ne. 0) THEN
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(Y), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
      ELSE
C       $OpenAD$ INLINE Saxpy(subst,subst,subst)
        CALL Saxpy(2.0D00, __deriv__(Y), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(Y))
      ENDIF
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
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
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
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)), 'bla')
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)), 'bla')
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)), 'bla')
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
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)), 'bla')
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL foo(__deriv__(X(1)), __deriv__(Y(1)), 'bla')
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
