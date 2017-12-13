
C$OPENAD XXX File_start [head.f]
      MODULE globals
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: GX
      type(active) :: GY
      contains
      subroutine mod_globals_init()
      	use OAD_active
      	GX%d = 0
      	GY%d = 0
      end subroutine
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE bar(BARX, BARY)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) BARX
      REAL(w2f__8) BARY
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) T
C
C     **** Statements ****
C
      T = BARX
      BARX = BARY
      BARY = T
      END SUBROUTINE

      SUBROUTINE foo()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
      REAL(w2f__8) OpenAD_tyc_0
      REAL(w2f__8) OpenAD_tyc_1
C
C     **** Statements ****
C
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,GX)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,GY)
      CALL bar(OpenAD_tyc_0,OpenAD_tyc_1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(GX,OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(GY,OpenAD_tyc_1)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      GX%v = X(1)%v
      GY%v = GX%v
      Y(1)%v = GY%v
      CALL setderiv(GX,X(1))
      CALL setderiv(GY,GX)
      CALL setderiv(Y(1),GY)
      END SUBROUTINE
      subroutine OAD_globalVar_init()
      	use globals
      	call mod_globals_init()
      end subroutine
