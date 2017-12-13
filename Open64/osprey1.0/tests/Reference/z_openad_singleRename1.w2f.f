
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
C     **** Statements ****
C
      interface  BFOO
        module procedure  FOO

      end interface 
      
      CONTAINS

        SUBROUTINE FOO
        use w2f__types
        IMPLICIT NONE
C
C       **** Statements ****
C
        S = S + 1
        END SUBROUTINE
      END

      PROGRAM multrename
      use w2f__types
      use a
      use b
      IMPLICIT NONE
C
C     **** Statements ****
C
      LONGNAME = 1
      CALL FOO()
      IF(LONGNAME .eq. 2) THEN
        WRITE(*, *) 'OK'
      ELSE
        WRITE(*, *) 'FAILED'
      ENDIF
      
      END PROGRAM
