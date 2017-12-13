
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_lin_0 = X(2)%v
      OpenAD_lin_1 = X(1)%v
      Y(1)%v = (X(1)%v*X(2)%v)
      CALL sax(OpenAD_lin_0,X(1),Y(1))
      CALL saxpy(OpenAD_lin_1,X(2),Y(1))
      END SUBROUTINE

      SUBROUTINE head()
      use OAD_active
      use w2f__types
      use oad_intrinsics
      use all_globals_mod
      IMPLICIT NONE
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
      CALL foo()
      END SUBROUTINE
