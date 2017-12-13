
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
C$OPENAD XXX Template ad_template.f
      CALL foo(__deriv__(X), __deriv__(Y))
      END SUBROUTINE

      SUBROUTINE foo(FX, FY)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) FX(1 : 1)
      TYPE (OpenADTy_active) FY(1 : 1)
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) FVAR
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL BAR(__deriv__(FX), __deriv__(FY))
      CONTAINS

        SUBROUTINE BAR(BX, BY)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) BX(1 : 1)
        TYPE (OpenADTy_active) BY(1 : 1)
C
C       **** Statements ****
C
        __value__(FVAR) = __value__(BX(1))
        __value__(BY(1)) = __value__(FVAR)
        CALL setderiv(__deriv__(FVAR), __deriv__(BX(1)))
        CALL setderiv(__deriv__(BY(1)), __deriv__(FVAR))
        END SUBROUTINE
      END
