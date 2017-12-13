      MODULE globals
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      TYPE (oadactive) AGLOBAL
C
C     **** Statements ****
C
      END MODULE

      SUBROUTINE head(X, Y)
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
      TYPE (oadactive) X(1 : 2)
      TYPE (oadactive) Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      read(*,*) __value__(X(1)), __value__(AGLOBAL)
      write(*,*) __value__(X(1)), __value__(AGLOBAL)
      END SUBROUTINE
