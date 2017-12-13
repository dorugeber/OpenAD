
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
      MODULE m
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) MX
C
C     **** Top Level Pragmas ****
C
      interface  FOO
        module procedure  FOO_II
        module procedure  FOO_I

      end interface 
      
      interface  OAD_S_FOO
        module procedure  OAD_S_FOO_II
        module procedure  OAD_S_FOO_I

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        Function FOO_I(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__8) X
        REAL(w2f__8) FOO_I
C
C       **** Statements ****
C
        __value__(MX) = (X + __value__(MX))
        FOO_I = __value__(MX)
        RETURN
        END FUNCTION

        Function FOO_II(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
        INTEGER(w2f__i4) FOO_II
C
C       **** Statements ****
C
        FOO_II = X + 1
        RETURN
        END FUNCTION

        SUBROUTINE OAD_S_FOO_I(X, FOO_I)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) X
        TYPE (OpenADTy_active) FOO_I
C
C       **** Local Variables and Functions ****
C
        TYPE (OpenADTy_active) OpenAD_prp_0
        TYPE (OpenADTy_active) OpenAD_prp_1
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        __value__(MX) = (__value__(X) + __value__(MX))
        __value__(FOO_I) = __value__(MX)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        __value__(MX) = (__value__(X) + __value__(MX))
        __value__(FOO_I) = __value__(MX)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FOO_I), __deriv__(MX))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FOO_I))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(MX), __deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(MX), __deriv__(OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(MX))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(MX))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
        CALL cp_arg_store_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(FOO_I))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
        CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        __value__(MX) = (__value__(X) + __value__(MX))
        __value__(FOO_I) = __value__(MX)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(FOO_I), __deriv__(MX))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(FOO_I))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(MX), __deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(MX), __deriv__(OpenAD_prp_0))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(MX))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_1), __deriv__(MX))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_1))
C       $OpenAD$ INLINE IncDeriv(subst,subst)
        CALL IncDeriv(__deriv__(OpenAD_prp_0), __deriv__(X))
C       $OpenAD$ INLINE ZeroDeriv(subst)
        CALL ZeroDeriv(__deriv__(OpenAD_prp_0))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
        CALL cp_arg_store_real_scalar_a_d(__deriv__(MX))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
        CALL cp_arg_store_real_scalar_a_d(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
        CALL cp_arg_store_real_scalar_a_d(__deriv__(FOO_I))
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
        CALL cp_arg_restore_real_scalar_a_d(__deriv__(FOO_I))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
        CALL cp_arg_restore_real_scalar_a_d(__deriv__(X))
C       $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
        CALL cp_arg_restore_real_scalar_a_d(__deriv__(MX))
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE

        SUBROUTINE OAD_S_FOO_II(X, FOO_II)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
        INTEGER(w2f__i4) FOO_II
        INTENT(OUT)  FOO_II
C
C       **** Statements ****
C
C       $OpenAD$ BEGIN REPLACEMENT 1
        FOO_II = (X + 1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 2
        FOO_II = (X + 1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 3
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 4
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(X)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 5
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 6
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(X)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 7
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 8
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(FOO_II)
C       $OpenAD$ INLINE cp_arg_store_integer_scalar(subst)
        CALL cp_arg_store_integer_scalar(X)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 9
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(X)
C       $OpenAD$ INLINE cp_arg_restore_integer_scalar(subst)
        CALL cp_arg_restore_integer_scalar(FOO_II)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 10
        FOO_II = (X + 1)
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 11
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 12
C       $OpenAD$ END REPLACEMENT
C       $OpenAD$ BEGIN REPLACEMENT 13
C       $OpenAD$ END REPLACEMENT
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      use m
      use oad_intrinsics
      use m
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      TYPE (OpenADTy_active) OAD_CTMP0
      TYPE (OpenADTy_active) OAD_CTMP1
      INTEGER(w2f__i4) OAD_CTMP2
      INTEGER(w2f__i4) OAD_CTMP3
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
      __value__(MX) = __value__(X(1))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
      OAD_CTMP3 = 2
      CALL OAD_S_FOO_II(OAD_CTMP3, OAD_CTMP2)
      I = OAD_CTMP2
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 2
C$OPENAD XXX Template ad_template.f
      __value__(MX) = __value__(X(1))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
      OAD_CTMP3 = 2
      CALL OAD_S_FOO_II(OAD_CTMP3, OAD_CTMP2)
      I = OAD_CTMP2
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 3
      CALL OAD_S_FOO_II(OAD_CTMP3, OAD_CTMP2)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(MX), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 4
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 5
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 6
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 7
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 8
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a(subst)
      CALL cp_arg_store_real_scalar_a(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 9
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a(subst)
      CALL cp_arg_restore_real_scalar_a(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 10
C$OPENAD XXX Template ad_template.f
      __value__(MX) = __value__(X(1))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
      OAD_CTMP3 = 2
      CALL OAD_S_FOO_II(OAD_CTMP3, OAD_CTMP2)
      I = OAD_CTMP2
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 11
      CALL OAD_S_FOO_II(OAD_CTMP3, OAD_CTMP2)
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(Y(1)))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
C     $OpenAD$ INLINE IncDeriv(subst,subst)
      CALL IncDeriv(__deriv__(MX), __deriv__(X(1)))
C     $OpenAD$ INLINE ZeroDeriv(subst)
      CALL ZeroDeriv(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 12
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(MX))
C     $OpenAD$ INLINE cp_arg_store_real_scalar_a_d(subst)
      CALL cp_arg_store_real_scalar_a_d(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
C     $OpenAD$ BEGIN REPLACEMENT 13
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(MX))
C     $OpenAD$ INLINE cp_arg_restore_real_scalar_a_d(subst)
      CALL cp_arg_restore_real_scalar_a_d(__deriv__(MX))
C     $OpenAD$ END REPLACEMENT
      END SUBROUTINE
