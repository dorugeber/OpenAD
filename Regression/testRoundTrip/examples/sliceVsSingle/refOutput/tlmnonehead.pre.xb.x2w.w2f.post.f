
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(X)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      X%v = (X%v+1.0D00)
      CALL setderiv(OpenAD_prp_0,X)
      CALL setderiv(X,OpenAD_prp_0)
      END SUBROUTINE

      SUBROUTINE bar(X)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
      X(1)%v = (X(1)%v+1.0D00)
      CALL setderiv(OpenAD_prp_1,X(1))
      CALL setderiv(X(1),OpenAD_prp_1)
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface
        SUBROUTINE foo(X)
        use OAD_active
        use w2f__types
        type(active) :: X
        END SUBROUTINE

      end interface
      
      interface
        SUBROUTINE bar(X)
        use OAD_active
        use w2f__types
        type(active) :: X(1:1)
        END SUBROUTINE

      end interface

C
C     **** Statements ****
C
      CALL foo(X(1))
      Y(1:2)%v = X(1:2)%v
      CALL setderiv(Y(1:2),X(1:2))
      CALL bar(Y(2:2))
      END SUBROUTINE
