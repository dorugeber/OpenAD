
C$OPENAD XXX File_start [head.f]
      MODULE m
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: MX
C
C     **** Top Level Pragmas ****
C
      interface FOO
        module procedure FOO_II
        module procedure FOO_I

      end interface
      
      interface OAD_S_FOO
        module procedure OAD_S_FOO_II
        module procedure OAD_S_FOO_I

      end interface

C
C     **** Statements ****
C
      CONTAINS

        Function FOO_I(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        REAL(w2f__8) X
        REAL(w2f__8) FOO_I
C
C       **** Statements ****
C
        MX%v = (X+MX%v)
        FOO_I = MX%v
        RETURN
        END FUNCTION

        Function FOO_II(X)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
        INTEGER(w2f__i4) FOO_II
C
C       **** Statements ****
C
        FOO_II = X + 1
        RETURN
        END FUNCTION

        SUBROUTINE OAD_S_FOO_I(X, FOO_I)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        type(active) :: X
        type(active) :: FOO_I
        INTENT(OUT) FOO_I
C
C       **** Local Variables and Functions ****
C
        type(active) :: OpenAD_prp_0
        type(active) :: OpenAD_prp_1
C
C       **** Statements ****
C
        MX%v = (X%v+MX%v)
        FOO_I%v = MX%v
        CALL setderiv(OpenAD_prp_0,X)
        CALL setderiv(OpenAD_prp_1,MX)
        CALL setderiv(MX,OpenAD_prp_0)
        CALL inc_deriv(MX,OpenAD_prp_1)
        CALL setderiv(FOO_I,MX)
        END SUBROUTINE

        SUBROUTINE OAD_S_FOO_II(X, FOO_II)
        use w2f__types
        IMPLICIT NONE
C
C       **** Parameters and Result ****
C
        INTEGER(w2f__i4) X
        INTEGER(w2f__i4) FOO_II
        INTENT(OUT) FOO_II
C
C       **** Statements ****
C
        FOO_II = (X + 1)
        END SUBROUTINE
      END

      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use m
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      type(active) :: OAD_CTMP0
      type(active) :: OAD_CTMP1
      INTEGER(w2f__i4) OAD_CTMP2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      MX%v = X(1)%v
      CALL setderiv(MX,X(1))
      CALL OAD_S_FOO_I(X(1),OAD_CTMP1)
      CALL OAD_S_FOO_I(OAD_CTMP1,OAD_CTMP0)
      Y(1)%v = OAD_CTMP0%v
      CALL setderiv(Y(1),OAD_CTMP0)
      CALL OAD_S_FOO_II(2,OAD_CTMP2)
      I = OAD_CTMP2
      END SUBROUTINE
