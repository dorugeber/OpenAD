
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
      type(active) :: T
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = X(2)%v
      OpenAD_lin_1 = X(1)%v
      T%v = (X(1)%v*X(2)%v)
      OpenAD_lin_2 = COS(T%v)
      Y(1)%v = SIN(T%v)
      CALL sax(OpenAD_lin_0,X(1),T)
      CALL saxpy(OpenAD_lin_1,X(2),T)
      CALL sax(OpenAD_lin_2,T,Y(1))
      OpenAD_lin_3 = (-SIN(T%v))
      Y(2)%v = COS(T%v)
      CALL sax(OpenAD_lin_3,T,Y(2))
      END SUBROUTINE
