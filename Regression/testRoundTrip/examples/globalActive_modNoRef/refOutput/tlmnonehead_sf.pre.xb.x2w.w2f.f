
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
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) X(1 : 1)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      MODULE m
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) G
C
C     **** Statements ****
C
      END MODULE

      MODULE m1
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (OpenADTy_active) G1
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE bar()
      use w2f__types
      use oad_intrinsics
      use m
      use m1
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
      use m
      use m1
      IMPLICIT NONE
C
C     **** Statements ****
C
      __value__(G1) = (__value__(G) * 2.0D00)
      CALL sax(2.0D00, __deriv__(G), __deriv__(G1))
      END SUBROUTINE

      SUBROUTINE foo1()
      use w2f__types
      use oad_intrinsics
      use m
      use m1
      IMPLICIT NONE
C
C     **** Statements ****
C
      __value__(G) = (__value__(G1) * 3.0D00)
      CALL sax(3.0D00, __deriv__(G1), __deriv__(G))
      END SUBROUTINE

      SUBROUTINE baz()
      use w2f__types
      use oad_intrinsics
      use m
      use m1
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL bar
C
C     **** Statements ****
C
      __value__(G) = __value__(X(1))
      CALL setderiv(__deriv__(G), __deriv__(X(1)))
      CALL bar()
      __value__(Y(1)) = __value__(G)
      CALL setderiv(__deriv__(Y(1)), __deriv__(G))
      END SUBROUTINE

      SUBROUTINE head()
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      use m
      use m1
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      EXTERNAL baz
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
      CALL baz()
      END SUBROUTINE
