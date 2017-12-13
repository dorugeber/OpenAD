
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
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
      CALL foo(X,Y)
      END SUBROUTINE

      SUBROUTINE foo(FX, FY)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: FX(1:1)
      type(active) :: FY(1:1)
C
C     **** Local Variables and Functions ****
C
      type(active) :: FVAR
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL BAR(FX,FY)
      CONTAINS

        SUBROUTINE BAR(BX, BY)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        type(active) :: BX(1:1)
        type(active) :: BY(1:1)
C
C       **** Statements ****
C
        FVAR%v = BX(1)%v
        BY(1)%v = FVAR%v
        CALL setderiv(FVAR,BX(1))
        CALL setderiv(BY(1),FVAR)
        END SUBROUTINE
      END
