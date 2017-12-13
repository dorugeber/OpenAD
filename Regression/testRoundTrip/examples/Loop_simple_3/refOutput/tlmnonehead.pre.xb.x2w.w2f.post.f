
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X, Y)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:1)
      INTENT(IN) X
      type(active) :: Y(1:1)
      INTENT(OUT) Y
C
C     **** Local Variables and Functions ****
C
      CHARACTER(10) ASTRING
      INTEGER(w2f__i4) I
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_2
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Y(1)%v = X(1)%v
      CALL setderiv(Y(1),X(1))
      ASTRING = 'blah'
C$OPENAD XXX Simple loop
      DO I = 1,3,1
        IF (ASTRING.EQ.'blah') THEN
          OpenAD_lin_0 = Y(1)%v
          OpenAD_lin_1 = X(1)%v
          Y(1)%v = (X(1)%v*Y(1)%v)
          CALL setderiv(OpenAD_prp_0,Y(1))
          CALL sax(OpenAD_lin_0,X(1),Y(1))
          CALL saxpy(OpenAD_lin_1,OpenAD_prp_0,Y(1))
        ENDIF
        IF (ASTRING.EQ.'bloh') THEN
          Y(1)%v = (Y(1)%v-X(1)%v)
          CALL setderiv(OpenAD_prp_1,Y(1))
          CALL setderiv(Y(1),OpenAD_prp_1)
          CALL dec_deriv(Y(1),X(1))
        ENDIF
      END DO
      CALL setderiv(OpenAD_prp_2,Y(1))
      CALL setderiv(Y(1),OpenAD_prp_2)
      END SUBROUTINE
