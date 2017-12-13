
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(X, Y, K)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTEGER(w2f__i4) K
C
C     **** Local Variables and Functions ****
C
      INTEGER(w2f__i4) I
      INTEGER(w2f__i4) J
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
C$OPENAD XXX Simple loop
      DO I = 1, 2, 1
        J = K
        OpenAD_lin_0 = X(I)%v
        OpenAD_lin_1 = X(I)%v
        Y(INT(J))%v = (Y(J)%v+X(I)%v*X(I)%v)
        CALL setderiv(OpenAD_prp_0,Y(J))
        CALL setderiv(Y(J),OpenAD_prp_0)
        CALL saxpy(OpenAD_lin_0,X(I),Y(J))
        CALL saxpy(OpenAD_lin_1,X(I),Y(J))
      END DO
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
      INTENT(IN) X
      type(active) :: Y(1:2)
      INTENT(OUT) Y
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
      Y(1)%v = 1.0
      Y(2)%v = 1.0
      CALL zero_deriv(Y(1))
      CALL zero_deriv(Y(2))
      CALL foo(X,Y,1)
      CALL foo(X,Y,2)
      END SUBROUTINE
