      MODULE globals
      use OAD_active
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: AGLOBAL
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
      use OAD_active
      use globals
      IMPLICIT NONE
C
C     **** Global Variables & Derived Type Definitions ****
C
      REAL OpenAD_Symbol_0
      REAL OpenAD_Symbol_1
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
      CALL sax(OpenAD_Symbol_0,X(1),AGLOBAL)
      CALL saxpy(OpenAD_Symbol_1,X(2),AGLOBAL)
      CALL sax(OpenAD_Symbol_0,X(1),Y(1))
      CALL saxpy(OpenAD_Symbol_1,X(2),Y(1))
      END SUBROUTINE
