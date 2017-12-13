
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
      type(active) :: Y
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Y%v = (X%v*2.0D00)
      CALL sax(2.0D00,X,Y)
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
      type(active) :: Y
C
C     **** Local Variables and Functions ****
C
      EXTERNAL foo
      INTEGER(w2f__i4) K
      INTEGER(w2f__i4) L
      REAL(w2f__8) P(1 : 2)
      type(active) :: Q(1:2)
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
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
      K = 1
      CALL foo(X(K),Y)
      Q(1)%v = Y%v
      CALL setderiv(OpenAD_prp_0,Y)
      CALL setderiv(Q(1),OpenAD_prp_0)
      Y%v = Q(1)%v
      P(1) = 1.0
      L = 1
      CALL setderiv(OpenAD_prp_1,Q(1))
      CALL setderiv(Y,OpenAD_prp_1)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,P(K))
      CALL foo(OpenAD_tyc_0,Q(L))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(P(K),OpenAD_tyc_0)
      END SUBROUTINE
