      MODULE all_globals_mod
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

      MODULE globals
      use OAD_active
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      type(active) :: AGLOBAL

      END MODULE

      SUBROUTINE head(X, Y)
      use OAD_active
      type(active) :: AGLOBAL
      type(active) :: X(1:2)
      type(active) :: Y(1:1)
      type(active) :: Z(1:)
      END SUBROUTINE
