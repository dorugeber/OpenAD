
C$OPENAD XXX File_start [head.f]
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
C     **** Local Variables and Functions ****
C
      type(active) :: T(1:1,1:2)
      REAL(w2f__8) OpenAD_lin_0(1 : 1, 1 : 2)
      REAL(w2f__8) OpenAD_lin_1(1 : 1, 1 : 2)
      type(active) :: OpenAD_prp_0(1:1,1:2)
      type(active) :: OpenAD_prp_1(1:1,1:2)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T(1,1:2)%v = X(1:2)%v
      CALL setderiv(T(1,1:2),X(1:2))
      OpenAD_lin_0 = T(1:1,1:2)%v
      OpenAD_lin_1 = T(1:1,1:2)%v
      T(1:1,1:2)%v = (T(1:1,1:2)%v*T(1:1,1:2)%v)
      CALL setderiv(OpenAD_prp_0,T(1:1,1:2))
      CALL setderiv(OpenAD_prp_1,T(1:1,1:2))
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,T(1:1,1:2))
      CALL saxpy(OpenAD_lin_1,OpenAD_prp_1,T(1:1,1:2))
      Y(1:2)%v = T(1,1:2)%v
      CALL setderiv(Y(1:2),T(1,1:2))
      END SUBROUTINE
