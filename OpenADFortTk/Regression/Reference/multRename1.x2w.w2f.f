
      MODULE a
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER(w2f__i4) LONGNAME
C
C     **** Statements ****
C
      END MODULE

      MODULE b
      use w2f__types
      use a ,only: S => LONGNAME
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  BFOO
        module procedure  FOO

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE FOO
        use w2f__types
        IMPLICIT NONE
C
C       **** Statements ****
C
        S = (S + 1)
        END SUBROUTINE
      END

      MODULE c
      use w2f__types
      use a ,only: S => LONGNAME
      IMPLICIT NONE
      SAVE
C
C     **** Top Level Pragmas ****
C
      interface  CBAR
        module procedure  BAR

      end interface 
      
C
C     **** Statements ****
C
      CONTAINS

        SUBROUTINE BAR
        use w2f__types
        use b
        IMPLICIT NONE
C
C       **** Statements ****
C
        S = (S + 1)
        CALL FOO()
        END SUBROUTINE
      END

      PROGRAM multrename
      use w2f__types
      use a
      use c
      IMPLICIT NONE
C
C     **** Statements ****
C
      LONGNAME = 1
      CALL BAR()
      WRITE(*, *) LONGNAME
      
      END PROGRAM
