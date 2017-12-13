      SUBROUTINE head(X, Y)
      use OAD_active
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X(1:2)
      type(active) :: Y(1:2)
C
C     **** Local Variables and Functions ****
C
      INTEGER N
      SAVE N
      type(active) :: Z(1:INT(SIZE(X)))
      double precision :: openad_symbol_4, openad_symbol_5
C
C     **** Initializers ****
C
      DATA N / 2 /
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      Z(1:INT(SIZE(X)))%v = X(1:2)%v
      OpenAD_Symbol_5 = SIN(X(3)%v)
      OpenAD_Symbol_4 = COS(X(3)%v)
      CALL setderiv(Z(1:INT(SIZE(X))),X(1:2))
      Y(1:2)%v = Z(1:INT(SIZE(X)))%v
      CALL setderiv(Y(1:2),Z(1:INT(SIZE(X))))
      END SUBROUTINE
