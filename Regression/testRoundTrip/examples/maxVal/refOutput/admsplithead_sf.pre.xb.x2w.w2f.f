
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  OAD_S_MAXVAL
        module procedure  OAD_S_MAXVAL_D_1

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE OAD_S_MAXVAL_D_1(A, R)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) A(1 :)
        TYPE (OpenADTy_active) R
C
C       **** Local Variables and Functions ****
C
        INTEGER(w2f__i4) I(1 : 1)
        INTEGER(w2f__i4) OpenAD_Symbol_0
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        I(1 : 1) = MAXLOC(__value__(A))
        __value__(R) = __value__(A(I(1)))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        I(1 : 1) = MAXLOC(__value__(A))
        __value__(R) = __value__(A(I(1)))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I(1))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_0)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(R), __deriv__(A(OpenAD_Symbol_0)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(R))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(A))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(A))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(R))
C       $OpenAD$ INLINE cp_arg_store_real_vector_a(subst)
        CALL cp_arg_store_real_vector_a(__deriv__(A))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_vector_a(subst)
        CALL cp_arg_restore_real_vector_a(__deriv__(A))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(R))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        I(1 : 1) = MAXLOC(__value__(A))
        __value__(R) = __value__(A(I(1)))
C       $OpenAD$ INLINE push_i_s0(subst)
        CALL push_i_s0(I(1))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE pop_i_s0(subst)
        CALL pop_i_s0(OpenAD_Symbol_0)
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(R), __deriv__(A(OpenAD_Symbol_0)))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(R))
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
      CALL OAD_S_MAXVAL_D_1(__deriv__(X), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      CALL OAD_S_MAXVAL_D_1(__deriv__(X), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_MAXVAL_D_1(__deriv__(X), __deriv__(OAD_CTMP0))
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
      CALL OAD_S_MAXVAL_D_1(__deriv__(X), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_MAXVAL_D_1(__deriv__(X), __deriv__(OAD_CTMP0))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
