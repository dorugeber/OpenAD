
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
      type(active) :: Y(1:1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      CALL FOO()
      CONTAINS

        SUBROUTINE FOO()
        use w2f__types
        IMPLICIT NONE
C
C       **** Local Variables and Functions ****
C
        REAL(w2f__8) OpenAD_lin_0
        REAL(w2f__8) OpenAD_lin_1
C
C       **** Statements ****
C
        OpenAD_lin_0 = X(2)%v
        OpenAD_lin_1 = X(1)%v
        Y(1)%v = (X(1)%v*X(2)%v)
        CALL sax(OpenAD_lin_0,X(1),Y(1))
        CALL saxpy(OpenAD_lin_1,X(2),Y(1))
        END SUBROUTINE
      END
