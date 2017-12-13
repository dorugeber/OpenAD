
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(M1, M2)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      REAL(w2f__8) M1
      INTENT(IN) M1
      type(active) :: M2
C
C     **** Local Variables and Functions ****
C
      type(active) :: OAD_CTMP0
      type(active) :: OpenAD_tyc_0
C
C     **** Statements ****
C
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_0,M1)
      CALL OAD_S_MIN_D(OpenAD_tyc_0,M2,OAD_CTMP0)
      M2%v = OAD_CTMP0%v
      CALL setderiv(M2,OAD_CTMP0)
      END SUBROUTINE

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
      type(active) :: OAD_CTMP0
      type(active) :: OAD_CTMP1
      REAL(w2f__8) P
      REAL(w2f__8) T
      type(active) :: T1
      type(active) :: OpenAD_tyc_1
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      T = 2.0D00
      P = SQRT(3.0D00)
C     $OpenAD$ INLINE oad_convert(subst,subst)
      CALL oad_convert(OpenAD_tyc_1,T)
      CALL OAD_S_MIN_D(OpenAD_tyc_1,X(1),OAD_CTMP0)
      T1%v = OAD_CTMP0%v
      CALL setderiv(T1,OAD_CTMP0)
      CALL OAD_S_MIN_D(X(1),T1,OAD_CTMP1)
      Y(1)%v = OAD_CTMP1%v
      CALL setderiv(Y(1),OAD_CTMP1)
      CALL foo(P,T1)
      END SUBROUTINE
