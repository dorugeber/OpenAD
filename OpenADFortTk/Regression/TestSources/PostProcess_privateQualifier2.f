C ***********************************************************
C Fortran file translated from WHIRL Wed Sep 16 07:24:38 2009
C ***********************************************************
C ***********************************************************

      MODULE m
      IMPLICIT NONE
      SAVE
C
C     **** Global Variables & Derived Type Definitions ****
C
      INTEGER NC
      PRIVATE NC
      INTEGER NT
      PRIVATE NT
      REAL X
      PRIVATE X
      REAL Y
      PRIVATE Y
C
C     **** Initializers ****
C
      DATA NC / 20 /
      DATA NT / 12 /
C
C     **** Statements ****
C
      interface  FOO
        module procedure  FOO_I

      end interface 
      
      CONTAINS

        SUBROUTINE FOO_I
        IMPLICIT NONE
C
C       **** Statements ****
C
        X = 1.0
        Y = 1.0
        IF(((NC * NT) .eq. 240) .AND.((X * Y) .eq. 1.0)) THEN
          WRITE(*, *) 'OK'
        ELSE
          WRITE(*, *)(NC * NT), (X * Y)
        ENDIF
        RETURN
        END SUBROUTINE
      END

      PROGRAM privateq
      use m
      IMPLICIT NONE
C
C     **** Statements ****
C
      CALL FOO()
      
      END PROGRAM
