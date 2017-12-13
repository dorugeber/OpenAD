      SUBROUTINE head(X, Y)
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (oadactive) X(1 : 2)
      TYPE (oadactive) Y(1 : 2)
C
C     **** Local Variables and Functions ****
C
      INTEGER N
      SAVE N
      TYPE (oadactive) Z(1 : INT(SIZE(__value__(X))))
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
      __value__(Z(1 : INT(SIZE(X)))) = __value__(X(1 : 2))
      OpenAD_Symbol_5 = SIN(__value__(X(3)))
      OpenAD_Symbol_4 = COS(__value__(X(3)))
      CALL setderiv(__deriv__(Z(1 : INT(SIZE(X)))), __deriv__(X(1 : 2))
     > )
      __value__(Y(1 : 2)) = __value__(Z(1 : INT(SIZE(X))))
      CALL setderiv(__deriv__(Y(1 : 2)), __deriv__(Z(1 : INT(SIZE(X))))
     > )
      END SUBROUTINE
