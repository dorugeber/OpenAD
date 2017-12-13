
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(X, Y, K)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X
      type(active) :: Y
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) OpenAD_lin_0
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = K
      Y%v = (K*X%v)
      K = (K*2)
      CALL setderiv(OpenAD_prp_0,X)
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,Y)
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
      CALL foo(X(K),X(K+1),K)
      CALL foo(X(K),Y,K)
      END SUBROUTINE
