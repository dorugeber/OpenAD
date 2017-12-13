      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      MODULE globals
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: AGLOBAL

      END MODULE

      SUBROUTINE head(X, Y)
      type (oadactive) AGLOBAL
      type (oadactive) :: X(1 : 2)
      type (oadactive) :: Y(1 : 1)
      type (oadactive) :: Z(1 :)
      END SUBROUTINE
