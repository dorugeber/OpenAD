
!$OPENAD XXX File_start [OAD_intrinsics.f90]
MODULE oad_intrinsics
use OAD_active
use w2f__types
IMPLICIT NONE
SAVE
!
!     **** Statements ****
!
END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      MODULE m
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: G
C
C     **** Statements ****
C
      END MODULE

      MODULE m1
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: G1
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE bar()
      use OAD_active
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
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m
      use m1
      IMPLICIT NONE
C
C     **** Statements ****
C
      G1%v = (G%v*2.0D00)
      CALL sax(2.0D00,G,G1)
      END SUBROUTINE

      SUBROUTINE foo1()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m
      use m1
      IMPLICIT NONE
C
C     **** Statements ****
C
      G%v = (G1%v*3.0D00)
      CALL sax(3.0D00,G1,G)
      END SUBROUTINE

      SUBROUTINE baz()
      use OAD_active
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
      G%v = X(1)%v
      CALL setderiv(G,X(1))
      CALL bar()
      Y(1)%v = G%v
      CALL setderiv(Y(1),G)
      END SUBROUTINE

      SUBROUTINE head()
      use OAD_active
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
