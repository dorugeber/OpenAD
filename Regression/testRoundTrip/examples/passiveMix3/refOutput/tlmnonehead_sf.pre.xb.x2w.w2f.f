
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
      SUBROUTINE foo(M1, M2)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) M1
      INTENT(IN)  M1
      TYPE (OpenADTy_active) M2
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OAD_CTMP0
      TYPE (OpenADTy_active) OpenAD_tyc_0
C
C     **** Statements ****
C
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_0), M1)
      CALL OAD_S_MIN_D(__deriv__(OpenAD_tyc_0), __deriv__(M2),
     >  __deriv__(OAD_CTMP0))
      __value__(M2) = __value__(OAD_CTMP0)
      CALL setderiv(__deriv__(M2), __deriv__(OAD_CTMP0))
      END SUBROUTINE

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
      TYPE (OpenADTy_active) OAD_CTMP0
      TYPE (OpenADTy_active) OAD_CTMP1
      REAL(w2f__8) P
      REAL(w2f__8) T
      TYPE (OpenADTy_active) T1
      TYPE (OpenADTy_active) OpenAD_tyc_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T = 2.0D00
      P = SQRT(3.0D00)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(__deriv__(OpenAD_tyc_1), T)
      CALL OAD_S_MIN_D(__deriv__(OpenAD_tyc_1), __deriv__(X(1)),
     >  __deriv__(OAD_CTMP0))
      __value__(T1) = __value__(OAD_CTMP0)
      CALL setderiv(__deriv__(T1), __deriv__(OAD_CTMP0))
      CALL OAD_S_MIN_D(__deriv__(X(1)), __deriv__(T1), __deriv__(
     > OAD_CTMP1))
      __value__(Y(1)) = __value__(OAD_CTMP1)
      CALL setderiv(__deriv__(Y(1)), __deriv__(OAD_CTMP1))
      CALL foo(P, __deriv__(T1))
      END SUBROUTINE
