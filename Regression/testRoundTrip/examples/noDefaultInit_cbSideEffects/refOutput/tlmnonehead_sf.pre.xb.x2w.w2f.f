
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
      SUBROUTINE bar()
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      EXTERNAL foo1
C
C     **** Statements ****
C
      CALL foo()
      CALL foo1()
      END SUBROUTINE

      SUBROUTINE foo()
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      TYPE (OpenADTy_active) G
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
      __value__(G) = (__value__(G) * 2.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(G))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_0), __deriv__(G))
      END SUBROUTINE

      SUBROUTINE foo1()
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      TYPE (OpenADTy_active) G
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) OpenAD_prp_1
C
C     **** Statements ****
C
      __value__(G) = (__value__(G) * 3.0D00)
      CALL setderiv(__deriv__(OpenAD_prp_1), __deriv__(G))
      CALL sax(3.0D00, __deriv__(OpenAD_prp_1), __deriv__(G))
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      SAVE /cb/
      COMMON /cb/ G
      TYPE (OpenADTy_active) G
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      __value__(G) = __value__(X(1))
      CALL setderiv(__deriv__(G), __deriv__(X(1)))
      CALL bar()
      __value__(Y(1)) = __value__(G)
      CALL setderiv(__deriv__(Y(1)), __deriv__(G))
      END SUBROUTINE
