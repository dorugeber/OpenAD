
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  OAD_S_MIN
        module procedure  OAD_S_MIN_D

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE OAD_S_MIN_D(A0, A1, R)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) A0
        INTENT(IN)  A0
        TYPE (OpenADTy_active) A1
        INTENT(IN)  A1
        TYPE (OpenADTy_active) R
        INTENT(OUT)  R
C
C       **** Statements ****
C
        IF(__value__(A0) .LT. __value__(A1)) THEN
          __value__(R) = __value__(A0)
          CALL setderiv(__deriv__(R), __deriv__(A0))
        ELSE
          __value__(R) = __value__(A1)
          CALL setderiv(__deriv__(R), __deriv__(A1))
        ENDIF
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
C$OPENAD XXX Template ad_template.f
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(X(2)), __deriv__(
     > OAD_CTMP0))
      __value__(Y(1)) = __value__(OAD_CTMP0)
      CALL setderiv(__deriv__(Y(1)), __deriv__(OAD_CTMP0))
      END SUBROUTINE
