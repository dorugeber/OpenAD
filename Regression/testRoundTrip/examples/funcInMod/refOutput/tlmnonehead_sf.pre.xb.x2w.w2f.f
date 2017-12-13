
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
        INTENT(OUT)  FOO_I
C
C       **** Local Variables and Functions ****
C
        TYPE (OpenADTy_active) OpenAD_prp_0
        TYPE (OpenADTy_active) OpenAD_prp_1
C
C       **** Statements ****
C
        __value__(MX) = (__value__(X) + __value__(MX))
        __value__(FOO_I) = __value__(MX)
        CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(X))
        CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(MX))
        CALL setderiv(__deriv__(MX), __deriv__(OpenAD_prp_0))
        CALL inc_deriv(__deriv__(MX), __deriv__(OpenAD_prp_1))
        CALL setderiv(__deriv__(FOO_I), __deriv__(MX))
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
        FOO_II = (X + 1)
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use w2f__types
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
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      __value__(MX) = __value__(X(1))
      CALL setderiv(__deriv__(MX), __deriv__(X(1)))
      CALL OAD_S_FOO_I(__deriv__(X(1)), __deriv__(OAD_CTMP1))
      CALL OAD_S_FOO_I(__deriv__(OAD_CTMP1), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
      CALL setderiv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
      CALL OAD_S_FOO_II(2, OAD_CTMP2)
      I = OAD_CTMP2
      END SUBROUTINE
