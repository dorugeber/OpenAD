
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
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(A, B, C, D)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: A
      type(active) :: B
      OPTIONAL B
      type(active) :: C
      type(active) :: D
      OPTIONAL D
C
C     **** Local Variables and Functions ****
C
      LOGICAL(w2f__i4) t__1
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
      C%v = (A%v*2.0D00)
      CALL setderiv(OpenAD_prp_0,A)
      CALL sax(2.0D00,OpenAD_prp_0,C)
      t__1 = .TRUE.
      IF (.not. PRESENT(B)) THEN
        t__1 = .FALSE.
      ELSE
        IF (.not. PRESENT(D)) THEN
          t__1 = .FALSE.
        ENDIF
      ENDIF
      IF(t__1) THEN
        D%v = (B%v*3.0D00)
        CALL setderiv(OpenAD_prp_1,B)
        CALL sax(3.0D00,OpenAD_prp_1,D)
      ENDIF
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:3)
      type(active) :: Y(1:3)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface
        SUBROUTINE foo(A, B, C, D)
        use OAD_active
        use w2f__types
        type(active) :: A
        type(active) :: B
        OPTIONAL B
        type(active) :: C
        type(active) :: D
        OPTIONAL D
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
      CALL foo(A=X(1),C=Y(1))
      CALL foo(X(2),X(3),Y(2),Y(3))
      END SUBROUTINE
