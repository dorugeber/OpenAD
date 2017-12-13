
C$OPENAD XXX File_start [head.f]
      MODULE globals
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: AGLOBALACTIVE
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE foo(P)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use globals
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: P
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
      OpenAD_lin_0 = AGLOBALACTIVE%v
      OpenAD_lin_1 = P%v
      AGLOBALACTIVE%v = (P%v*AGLOBALACTIVE%v)
      CALL setderiv(OpenAD_prp_0,AGLOBALACTIVE)
      CALL sax(OpenAD_lin_0,P,AGLOBALACTIVE)
      CALL saxpy(OpenAD_lin_1,OpenAD_prp_0,AGLOBALACTIVE)
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
      type(active) :: X(1:1)
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) ANINACTIVE
      EXTERNAL foo
      REAL(w2f__8) OpenAD_lin_2
      type(active) :: OpenAD_tyc_0
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      ANINACTIVE = 4.445600128173828125D+01
      AGLOBALACTIVE%v = 1.61803400516510009766D00
      CALL zero_deriv(AGLOBALACTIVE)
      CALL foo(X(1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,ANINACTIVE)
      CALL foo(OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(ANINACTIVE,OpenAD_tyc_0)
      OpenAD_lin_2 = COS(AGLOBALACTIVE%v)
      Y(1)%v = SIN(AGLOBALACTIVE%v)
      CALL sax(OpenAD_lin_2,AGLOBALACTIVE,Y(1))
      END SUBROUTINE
