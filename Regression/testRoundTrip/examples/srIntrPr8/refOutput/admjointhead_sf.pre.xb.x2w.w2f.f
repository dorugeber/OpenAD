
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  OAD_S_MAX
        module procedure  OAD_S_MAX_D

      end interface 
      
      interface  OAD_S_MIN
        module procedure  OAD_S_MIN_D

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE OAD_S_MAX_D(A0, A1, R)
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
        REAL(w2f__8) A0
        REAL(w2f__8) A1
        REAL(w2f__8) R
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        IF(A0 .GT. A1) THEN
          R = A0
        ELSE
          R = A1
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        IF(A0 .GT. A1) THEN
          R = A0
          OpenAD_Symbol_1 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_1)
        ELSE
          R = A1
          OpenAD_Symbol_2 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_2)
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_0)
        IF(OpenAD_Symbol_0 .ne. 0) THEN
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
        CALL cp_arg_store_real_scalar(A0)
C       $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
        CALL cp_arg_store_real_scalar(A1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
        CALL cp_arg_restore_real_scalar(A1)
C       $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
        CALL cp_arg_restore_real_scalar(A0)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
        CALL cp_arg_store_real_scalar(R)
C       $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
        CALL cp_arg_store_real_scalar(A0)
C       $OpenAD$ INLINE cp_arg_store_real_scalar(subst)
        CALL cp_arg_store_real_scalar(A1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
        CALL cp_arg_restore_real_scalar(A1)
C       $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
        CALL cp_arg_restore_real_scalar(A0)
C       $OpenAD$ INLINE cp_arg_restore_real_scalar(subst)
        CALL cp_arg_restore_real_scalar(R)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        IF(A0 .GT. A1) THEN
          R = A0
          OpenAD_Symbol_4 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_4)
        ELSE
          R = A1
          OpenAD_Symbol_5 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_5)
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_3)
        IF(OpenAD_Symbol_3 .ne. 0) THEN
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE

        SUBROUTINE OAD_S_MIN_D(A0, A1, R)
        use w2f__types
        IMPLICIT NONE
C
C       **** Global Variables & Derived Type Definitions ****
C
        INTEGER(w2f__i8) OpenAD_Symbol_10
        INTEGER(w2f__i8) OpenAD_Symbol_11
        INTEGER(w2f__i8) OpenAD_Symbol_6
        INTEGER(w2f__i8) OpenAD_Symbol_7
        INTEGER(w2f__i8) OpenAD_Symbol_8
        INTEGER(w2f__i8) OpenAD_Symbol_9
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) A0
        TYPE (OpenADTy_active) A1
        TYPE (OpenADTy_active) R
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        IF(__value__(A0) .LT. __value__(A1)) THEN
          __value__(R) = __value__(A0)
        ELSE
          __value__(R) = __value__(A1)
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        IF(__value__(A0) .LT. __value__(A1)) THEN
          __value__(R) = __value__(A0)
          OpenAD_Symbol_7 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_7)
        ELSE
          __value__(R) = __value__(A1)
          OpenAD_Symbol_8 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_8)
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_6)
        IF(OpenAD_Symbol_6 .ne. 0) THEN
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(R), __deriv__(A0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(R))
        ELSE
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(R), __deriv__(A1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(R))
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(A0))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(A1))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(A1))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(A0))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(R))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(A0))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(A1))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(A1))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(A0))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(R))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        IF(__value__(A0) .LT. __value__(A1)) THEN
          __value__(R) = __value__(A0)
          OpenAD_Symbol_10 = 1_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_10)
        ELSE
          __value__(R) = __value__(A1)
          OpenAD_Symbol_11 = 0_w2f__i8
C         $OpenAD$ INLINE push_i_s0(subst)
          CALL push_i_s0(OpenAD_Symbol_11)
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_9)
        IF(OpenAD_Symbol_9 .ne. 0) THEN
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(R), __deriv__(A0))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(R))
        ELSE
C         $OpenAD$ INLINE IncDeriv(subst,subst)
          CALL IncDeriv(__deriv__(R), __deriv__(A1))
C         $OpenAD$ INLINE ZeroDeriv(subst)
          CALL ZeroDeriv(__deriv__(R))
        ENDIF
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
        CALL cp_arg_store_real_scalar_a_d(__deriv__(R))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
        CALL cp_arg_restore_real_scalar_a_d(__deriv__(R))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
      END

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
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OAD_CTMP0
      REAL(w2f__8) OAD_CTMP1
      REAL(w2f__8) P1
      REAL(w2f__8) P2
      REAL(w2f__8) OpenAD_Symbol_12
      REAL(w2f__8) OpenAD_lin_0
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
      P1 = 2.39999999999999991118D00
      P2 = (-3.0D+01)
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
      CALL OAD_S_MAX_D(P1, P2, OAD_CTMP1)
      __value__(Y(1)) = (__value__(OAD_CTMP0) * OAD_CTMP1)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      P1 = 2.39999999999999991118D00
      P2 = (-3.0D+01)
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
      CALL OAD_S_MAX_D(P1, P2, OAD_CTMP1)
      OpenAD_lin_0 = OAD_CTMP1
      __value__(Y(1)) = (__value__(OAD_CTMP0) * OAD_CTMP1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(Y(1)), __deriv__(OAD_CTMP0
     > ))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_MAX_D(P1, P2, OAD_CTMP1)
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
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
      P1 = 2.39999999999999991118D00
      P2 = (-3.0D+01)
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
      CALL OAD_S_MAX_D(P1, P2, OAD_CTMP1)
      OpenAD_lin_0 = OAD_CTMP1
      __value__(Y(1)) = (__value__(OAD_CTMP0) * OAD_CTMP1)
C     $OpenAD$ INLINE push_s0(subst)
      CALL push_s0(OpenAD_lin_0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE pop_s0(subst)
      CALL pop_s0(OpenAD_Symbol_12)
C     $OpenAD$ INLINE Saxpy(subst,subst,subst)
      CALL Saxpy(OpenAD_Symbol_12, __deriv__(Y(1)), __deriv__(OAD_CTMP0
     > ))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_MAX_D(P1, P2, OAD_CTMP1)
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
