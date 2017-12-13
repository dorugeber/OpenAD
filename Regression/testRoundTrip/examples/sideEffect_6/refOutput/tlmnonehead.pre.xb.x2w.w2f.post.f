
C$OPENAD XXX File_start [head.f]
      SUBROUTINE foo(A, X)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      CHARACTER(*) A
      type(active) :: X
C
C     **** Local Variables and Functions ****
C
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF(A(1 : LEN(A)) .EQ. 'two') THEN
        X%v = (X%v*2.0D00)
        CALL setderiv(OpenAD_prp_0,X)
        CALL sax(2.0D00,OpenAD_prp_0,X)
      ENDIF
      IF (A(1:LEN(A)).EQ.'three') THEN
        X%v = (X%v*3.0D00)
        CALL setderiv(OpenAD_prp_1,X)
        CALL sax(3.0D00,OpenAD_prp_1,X)
      ENDIF
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
      CHARACTER(10) A
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
      A = 'two'
      CALL foo(A,X)
      A = 'three'
      CALL foo(A,X)
      A = 'four'
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      END SUBROUTINE
