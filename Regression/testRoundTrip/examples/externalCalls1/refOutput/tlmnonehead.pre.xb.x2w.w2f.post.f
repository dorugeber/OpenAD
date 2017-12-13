
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(X, Y)
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
      EXTERNAL barext
      type(active) :: T
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_tyc_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T%v = X(1)%v
      CALL setderiv(T,X(1))
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,T)
      CALL barext(OpenAD_tyc_0)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(T,OpenAD_tyc_0)
      OpenAD_lin_0 = T%v
      OpenAD_lin_1 = X(2)%v
      Y%v = (X(2)%v*T%v)
      CALL sax(OpenAD_lin_0,X(2),Y)
      CALL saxpy(OpenAD_lin_1,T,Y)
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
      type(active) :: Y(1:1)
C
C     **** Local Variables and Functions ****
C
      EXTERNAL barext
      EXTERNAL foo
      REAL(w2f__8) T
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL barext(T)
      CALL foo(X,Y)
      END SUBROUTINE
