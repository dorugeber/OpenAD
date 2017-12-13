
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
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE OAD_S_MAX_D(A0, A1, R)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        TYPE (OpenADTy_active) A0
        INTENT(IN)  A0
        REAL(w2f__8) A1
        INTENT(IN)  A1
        TYPE (OpenADTy_active) R
        INTENT(OUT)  R
C
C       **** Statements ****
C
        IF(__value__(A0) .GT. A1) THEN
          __value__(R) = __value__(A0)
          CALL setderiv(__deriv__(R), __deriv__(A0))
        ELSE
          __value__(R) = A1
          CALL zero_deriv(__deriv__(R))
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
      Function bar(X)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) X
      INTENT(IN)  X
      REAL(w2f__8) bar
C
C     **** Statements ****
C
      bar = MAX(X, 4.0D00)
      RETURN
      END FUNCTION

      SUBROUTINE oad_s_bar(X, BAR)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X
      INTENT(IN)  X
      TYPE (OpenADTy_active) BAR
      INTENT(OUT)  BAR
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OAD_CTMP0
C
C     **** Statements ****
C
      CALL OAD_S_MAX_D(__deriv__(X), 4.0D00, __deriv__(OAD_CTMP0))
      __value__(BAR) = __value__(OAD_CTMP0)
      CALL setderiv(__deriv__(BAR), __deriv__(OAD_CTMP0))
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
      TYPE (OpenADTy_active) OAD_CTMP0
      REAL(w2f__8) OAD_CTMP1
      EXTERNAL oad_s_bar
      REAL(w2f__8) T
      TYPE (OpenADTy_active) OpenAD_tyc_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL oad_s_bar(__deriv__(X(1)), __deriv__(OAD_CTMP0))
      __value__(Y(1)) = (__value__(OAD_CTMP0) * 2.0D00)
      CALL sax(2.0D00, __deriv__(OAD_CTMP0), __deriv__(Y(1)))
      CALL oad_s_bar(__deriv__(Y(1)), __deriv__(OpenAD_tyc_0))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OAD_CTMP1, __deriv__(OpenAD_tyc_0))
      T = OAD_CTMP1
      END SUBROUTINE
